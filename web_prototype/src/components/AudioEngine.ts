export interface Track {
    file: File;
    name: string;
    url: string;
}

export class AudioEngine {
    private context!: AudioContext;
    private source: MediaElementAudioSourceNode | null = null;
    private audioElement: HTMLAudioElement;

    // Playlist
    private playlist: Track[] = [];
    private currentIndex: number = -1;

    // Nodes
    private preampNode!: GainNode;
    private eqBands: BiquadFilterNode[] = [];
    private delayNode!: DelayNode;
    private delayFeedback!: GainNode;
    private reverbNode!: ConvolverNode;
    private reverbMix!: GainNode;
    private dryNode!: GainNode;

    // Spatial Nodes
    private pannerNode!: StereoPannerNode;
    private widthNode!: GainNode; // Side signal gain
    private midNode!: GainNode;   // Mid signal gain



    private compressorNode!: DynamicsCompressorNode;
    private analyser!: AnalyserNode;

    // Callbacks
    private onTrackChange: ((track: Track) => void) | null = null;


    private initialized: boolean = false;

    constructor() {
        this.audioElement = new Audio();
        this.audioElement.crossOrigin = "anonymous";
        this.audioElement.onended = () => this.next();
    }

    async init() {
        if (this.initialized) return;

        this.context = new (window.AudioContext || (window as any).webkitAudioContext)();

        // Initialize Nodes
        this.preampNode = this.context.createGain();
        this.delayNode = this.context.createDelay(5.0);
        this.delayFeedback = this.context.createGain();
        this.reverbNode = this.context.createConvolver();
        this.reverbMix = this.context.createGain();
        this.dryNode = this.context.createGain();

        // Spatial Setup
        this.widthNode = this.context.createGain();
        this.midNode = this.context.createGain();
        this.pannerNode = this.context.createStereoPanner();

        this.compressorNode = this.context.createDynamicsCompressor();
        this.analyser = this.context.createAnalyser();

        // Defaults
        this.analyser.fftSize = 2048;
        this.delayNode.delayTime.value = 0;
        this.delayFeedback.gain.value = 0;
        this.reverbMix.gain.value = 0;
        this.dryNode.gain.value = 1;
        this.widthNode.gain.value = 1;
        this.midNode.gain.value = 1;

        // EQ Setup
        const frequencies = [31, 62, 125, 250, 500, 1000, 2000, 4000, 8000, 16000];
        frequencies.forEach(f => {
            const filter = this.context.createBiquadFilter();
            filter.type = 'peaking';
            filter.frequency.value = f;
            filter.Q.value = 1.4;
            filter.gain.value = 0;
            this.eqBands.push(filter);
        });

        this.generateImpulseResponse();
        this.initialized = true;
    }

    private generateImpulseResponse() {
        const rate = this.context.sampleRate;
        const length = rate * 2.0;
        const decay = 2.0;
        const impulse = this.context.createBuffer(2, length, rate);
        const left = impulse.getChannelData(0);
        const right = impulse.getChannelData(1);

        for (let i = 0; i < length; i++) {
            left[i] = (Math.random() * 2 - 1) * Math.pow(1 - i / length, decay);
            right[i] = (Math.random() * 2 - 1) * Math.pow(1 - i / length, decay);
        }
        this.reverbNode.buffer = impulse;
    }

    // Playlist Management
    addToPlaylist(files: FileList) {
        for (let i = 0; i < files.length; i++) {
            const file = files[i];
            this.playlist.push({
                file,
                name: file.name,
                url: URL.createObjectURL(file)
            });
        }
        if (this.currentIndex === -1 && this.playlist.length > 0) {
            this.loadTrack(0);
        }
    }

    async loadTrack(index: number) {
        if (!this.initialized) await this.init();
        if (index < 0 || index >= this.playlist.length) return;

        this.currentIndex = index;
        const track = this.playlist[index];

        if (this.context.state === 'suspended') await this.context.resume();

        this.audioElement.src = track.url;

        if (!this.source) {
            this.source = this.context.createMediaElementSource(this.audioElement);
            this.connectGraph();
        }

        if (this.onTrackChange) this.onTrackChange(track);
        this.play();
    }

    next() {
        if (this.currentIndex < this.playlist.length - 1) {
            this.loadTrack(this.currentIndex + 1);
        }
    }

    prev() {
        if (this.currentIndex > 0) {
            this.loadTrack(this.currentIndex - 1);
        }
    }

    getPlaylist() { return this.playlist; }
    getCurrentIndex() { return this.currentIndex; }

    private connectGraph() {
        if (!this.source) return;

        let current: AudioNode = this.source;
        current.connect(this.preampNode);
        current = this.preampNode;

        this.eqBands.forEach(band => {
            current.connect(band);
            current = band;
        });

        const effectsInput = current;

        // Parallel Effects
        effectsInput.connect(this.dryNode);

        effectsInput.connect(this.delayNode);
        this.delayNode.connect(this.delayFeedback);
        this.delayFeedback.connect(this.delayNode);
        this.delayNode.connect(this.dryNode);

        effectsInput.connect(this.reverbNode);
        this.reverbNode.connect(this.reverbMix);

        // Merge to Spatial Processor
        const mix = this.context.createGain();
        this.dryNode.connect(mix);
        this.reverbMix.connect(mix);

        // Spatial Processing (Simple Width)
        mix.connect(this.pannerNode);
        this.pannerNode.connect(this.compressorNode);
        this.compressorNode.connect(this.analyser);
        this.analyser.connect(this.context.destination);
    }

    async resumeContext() {
        if (this.initialized && this.context.state === 'suspended') {
            await this.context.resume();
        }
    }

    // Controls
    async play() {
        if (!this.initialized) await this.init();
        try {
            await this.resumeContext();
            return this.audioElement.play();
        } catch (e) {
            console.error("Playback failed:", e);
        }
    }
    pause() { this.audioElement.pause(); }
    stop() { this.audioElement.pause(); this.audioElement.currentTime = 0; }
    seek(time: number) { this.audioElement.currentTime = time; }

    setVolume(val: number) { if (this.initialized) this.preampNode.gain.value = val; }
    setPan(val: number) { if (this.initialized) this.pannerNode.pan.value = val; }
    setEqBand(index: number, gain: number) { if (this.initialized && this.eqBands[index]) this.eqBands[index].gain.value = gain; }
    setDelayTime(time: number) { if (this.initialized) this.delayNode.delayTime.value = time; }
    setDelayFeedback(val: number) { if (this.initialized) this.delayFeedback.gain.value = val; }
    setReverbMix(val: number) {
        if (this.initialized) {
            this.reverbMix.gain.value = val;
            this.dryNode.gain.value = 1 - (val * 0.5);
        }
    }

    // Spatial Controls
    setStereoWidth(_val: number) {
        // Placeholder
    }

    // Getters
    getCurrentTime() { return this.audioElement.currentTime; }
    getDuration() { return this.audioElement.duration; }
    getAnalyserData(array: Uint8Array) {
        if (this.initialized && this.analyser) {
            this.analyser.getByteFrequencyData(array as any);
        } else {
            array.fill(0);
        }
    }
    onTimeUpdate(cb: () => void) { this.audioElement.ontimeupdate = cb; }
    onEnded(cb: () => void) { this.audioElement.onended = cb; }
    setOnTrackChange(cb: (track: Track) => void) { this.onTrackChange = cb; }

    // Presets
    getEqState() {
        if (!this.initialized) return [];
        return this.eqBands.map(b => b.gain.value);
    }
    setEqState(gains: number[]) {
        gains.forEach((g, i) => this.setEqBand(i, g));
    }
}

export const audioEngine = new AudioEngine();
