import { useState } from 'react';
import { audioEngine } from './AudioEngine';

export const PowerOverlay = () => {
    const [visible, setVisible] = useState(true);

    const handlePowerOn = async () => {
        try {
            await audioEngine.init();
            await audioEngine.resumeContext();
            setVisible(false);
        } catch (e) {
            console.error("Failed to resume context:", e);
        }
    };

    if (!visible) return null;

    return (
        <div style={{
            position: 'absolute', top: 0, left: 0, right: 0, bottom: 0,
            background: 'rgba(0,0,0,0.9)',
            zIndex: 1000,
            display: 'flex',
            justifyContent: 'center',
            alignItems: 'center',
            flexDirection: 'column'
        }}>
            <button
                onClick={handlePowerOn}
                style={{
                    width: '100px', height: '100px',
                    borderRadius: '50%',
                    border: '4px solid #ff3333',
                    background: '#220000',
                    color: '#ff3333',
                    fontSize: '1.5rem',
                    cursor: 'pointer',
                    boxShadow: '0 0 50px #ff3333',
                    fontFamily: 'Share Tech Mono',
                    textTransform: 'uppercase',
                    animation: 'pulse 2s infinite'
                }}
            >
                POWER
            </button>
            <div style={{ marginTop: '20px', color: '#aaa', fontFamily: 'Share Tech Mono' }}>
                CLICK TO INITIALIZE SYSTEM
            </div>
            <style>{`
        @keyframes pulse {
          0% { box-shadow: 0 0 20px #ff3333; }
          50% { box-shadow: 0 0 50px #ff3333; }
          100% { box-shadow: 0 0 20px #ff3333; }
        }
      `}</style>
        </div>
    );
};
