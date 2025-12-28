
import { audioEngine } from './AudioEngine';
import { RackUnit } from './RackUnit';

export const Equalizer = () => {
    const bands = [31, 62, 125, 250, 500, '1k', '2k', '4k', '8k', '16k'];

    const handleBandChange = (index: number, val: string) => {
        audioEngine.setEqBand(index, parseFloat(val));
    };

    return (
        <RackUnit title="EQ-10 GRAPHIC EQUALIZER">
            <div style={{ display: 'flex', justifyContent: 'space-between', height: '150px', padding: '0 20px' }}>
                {bands.map((freq, i) => (
                    <div key={i} style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', width: '30px' }}>
                        <input
                            type="range"
                            min="-12"
                            max="12"
                            defaultValue="0"
                            step="0.1"
                            onChange={(e) => handleBandChange(i, e.target.value)}
                            style={{
                                writingMode: 'vertical-lr',
                                direction: 'rtl',
                                width: '20px',
                                flex: 1,
                                accentColor: '#33ff33',
                                cursor: 'pointer',
                                background: '#111',
                                border: '1px solid #333'
                            } as any}
                        />
                        <span style={{ fontSize: '0.7rem', marginTop: '10px', color: '#888', fontFamily: 'Share Tech Mono' }}>
                            {freq}
                        </span>
                    </div>
                ))}
            </div>
        </RackUnit>
    );
};
