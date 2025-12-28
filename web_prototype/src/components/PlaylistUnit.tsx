import React, { useState, useEffect } from 'react';
import { audioEngine, Track } from './AudioEngine';
import { RackUnit } from './RackUnit';

export const PlaylistUnit = () => {
    const [playlist, setPlaylist] = useState<Track[]>([]);
    const [currentIndex, setCurrentIndex] = useState(-1);

    useEffect(() => {
        const update = () => {
            setPlaylist([...audioEngine.getPlaylist()]);
            setCurrentIndex(audioEngine.getCurrentIndex());
        };

        // Poll for updates (simple way to sync without complex state management lib)
        const interval = setInterval(update, 500);
        return () => clearInterval(interval);
    }, []);

    const handlePlay = (index: number) => {
        audioEngine.loadTrack(index);
    };

    return (
        <RackUnit title="PLAYLIST DECK // QUEUE" height="200px">
            <div style={{
                height: '180px',
                overflowY: 'auto',
                background: '#111',
                border: '1px solid #333',
                padding: '5px'
            }}>
                {playlist.length === 0 ? (
                    <div style={{ color: '#444', textAlign: 'center', marginTop: '20px' }}>
                        DRAG FILES TO LOAD QUEUE
                    </div>
                ) : (
                    playlist.map((track, i) => (
                        <div
                            key={i}
                            onClick={() => handlePlay(i)}
                            style={{
                                padding: '5px 10px',
                                borderBottom: '1px solid #222',
                                cursor: 'pointer',
                                background: i === currentIndex ? '#224422' : 'transparent',
                                color: i === currentIndex ? '#33ff33' : '#aaa',
                                fontFamily: 'Share Tech Mono',
                                fontSize: '0.9rem',
                                display: 'flex',
                                justifyContent: 'space-between'
                            }}
                        >
                            <span>{i + 1}. {track.name}</span>
                            {i === currentIndex && <span>PLAYING</span>}
                        </div>
                    ))
                )}
            </div>
            <div style={{ display: 'flex', gap: '10px', marginTop: '10px' }}>
                <button onClick={() => audioEngine.prev()} style={btnStyle}>PREV</button>
                <button onClick={() => audioEngine.next()} style={btnStyle}>NEXT</button>
            </div>
        </RackUnit>
    );
};

const btnStyle: React.CSSProperties = {
    background: '#333', color: '#ccc', border: '1px solid #000',
    padding: '5px 15px', cursor: 'pointer', fontFamily: 'Share Tech Mono'
};
