import React, { useState, useEffect } from 'react';
import { audioEngine } from './AudioEngine';

export const Controls = () => {
    const [isPlaying, setIsPlaying] = useState(false);
    const [currentTime, setCurrentTime] = useState(0);
    const [duration, setDuration] = useState(0);
    const [volume, setVolume] = useState(1);

    useEffect(() => {
        const updateTime = () => {
            setCurrentTime(audioEngine.getCurrentTime());
            setDuration(audioEngine.getDuration() || 0);
        };

        audioEngine.onTimeUpdate(updateTime);
        audioEngine.onEnded(() => setIsPlaying(false));

        const interval = setInterval(updateTime, 500);
        return () => clearInterval(interval);
    }, []);

    const togglePlay = async () => {
        if (isPlaying) {
            audioEngine.pause();
        } else {
            await audioEngine.play();
        }
        setIsPlaying(!isPlaying);
    };

    const handleStop = () => {
        audioEngine.stop();
        setIsPlaying(false);
        setCurrentTime(0);
    };

    const handleSeek = (e: React.ChangeEvent<HTMLInputElement>) => {
        const time = parseFloat(e.target.value);
        audioEngine.seek(time);
        setCurrentTime(time);
    };

    const handleVolume = (e: React.ChangeEvent<HTMLInputElement>) => {
        const vol = parseFloat(e.target.value);
        audioEngine.setVolume(vol);
        setVolume(vol);
    };

    const formatTime = (time: number) => {
        const minutes = Math.floor(time / 60);
        const seconds = Math.floor(time % 60);
        return `${minutes}:${seconds.toString().padStart(2, '0')}`;
    };

    const btnStyle: React.CSSProperties = {
        background: 'linear-gradient(180deg, #444, #222)',
        border: '1px solid #000',
        borderTopColor: '#666',
        borderLeftColor: '#666',
        color: '#ccc',
        padding: '10px 20px',
        fontFamily: 'Share Tech Mono',
        fontSize: '1rem',
        cursor: 'pointer',
        boxShadow: '2px 2px 5px rgba(0,0,0,0.5)',
        borderRadius: '2px',
        textTransform: 'uppercase',
        letterSpacing: '1px',
    };

    return (
        <div style={{ width: '100%', padding: '20px 0', display: 'flex', flexDirection: 'column', gap: '15px' }}>
            {/* LCD Time Display */}
            <div style={{
                background: '#112211',
                border: '2px solid #444',
                borderBottomColor: '#666',
                borderRightColor: '#666',
                padding: '5px 10px',
                display: 'flex',
                justifyContent: 'space-between',
                fontFamily: 'VT323',
                fontSize: '1.5rem',
                color: '#33ff33',
                textShadow: '0 0 5px #33ff33',
                boxShadow: 'inset 0 0 10px #000'
            }}>
                <span>TRACK TIME</span>
                <span>{formatTime(currentTime)} / {formatTime(duration)}</span>
            </div>

            {/* Seek Bar */}
            <input
                type="range"
                min="0"
                max={duration}
                value={currentTime}
                onChange={handleSeek}
                style={{
                    width: '100%',
                    accentColor: '#ff3333',
                    height: '10px',
                    background: '#111',
                    border: '1px solid #444'
                }}
            />

            {/* Buttons Row */}
            <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', background: '#222', padding: '10px', borderRadius: '4px', border: '1px solid #000', boxShadow: 'inset 0 0 10px #000' }}>
                <div style={{ display: 'flex', gap: '10px' }}>
                    <button onClick={() => audioEngine.prev()} style={btnStyle}>PREV</button>
                    <button onClick={togglePlay} style={btnStyle}>
                        {isPlaying ? 'PAUSE' : 'PLAY'}
                    </button>
                    <button onClick={handleStop} style={btnStyle}>STOP</button>
                    <button onClick={() => audioEngine.next()} style={btnStyle}>NEXT</button>
                </div>

                {/* Volume */}
                <div style={{ display: 'flex', alignItems: 'center', gap: '10px' }}>
                    <span style={{ fontSize: '0.8rem', color: '#888' }}>MAIN VOL</span>
                    <input
                        type="range"
                        min="0"
                        max="1"
                        step="0.01"
                        value={volume}
                        onChange={handleVolume}
                        style={{ width: '100px', accentColor: '#33ff33' }}
                    />
                </div>
            </div>
        </div>
    );
};
