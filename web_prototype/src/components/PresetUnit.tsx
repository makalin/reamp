
import { audioEngine } from './AudioEngine';
import { RackUnit } from './RackUnit';

export const PresetUnit = () => {
    const presets: Record<string, number[]> = {
        'FLAT': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'ROCK': [4, 3, 2, 0, -1, -1, 0, 2, 3, 4],
        'BASS': [6, 5, 3, 0, 0, 0, 0, 0, 0, 0],
        'VOCAL': [-2, -2, -1, 0, 3, 4, 3, 1, 0, 0],
        'JAZZ': [2, 2, 0, 2, 2, 2, 0, 2, 3, 4]
    };

    const loadPreset = (name: string) => {
        const gains = presets[name];
        if (gains) audioEngine.setEqState(gains);
    };

    return (
        <RackUnit title="PRESET MANAGER // MEMORY">
            <div style={{ display: 'flex', gap: '10px', flexWrap: 'wrap' }}>
                {Object.keys(presets).map(name => (
                    <button
                        key={name}
                        onClick={() => loadPreset(name)}
                        style={{
                            background: '#222',
                            border: '1px solid #444',
                            color: '#aaa',
                            padding: '5px 15px',
                            fontFamily: 'Share Tech Mono',
                            cursor: 'pointer',
                            fontSize: '0.8rem'
                        }}
                    >
                        {name}
                    </button>
                ))}
                <button style={{ background: '#422', border: '1px solid #633', color: '#f88', padding: '5px 15px', fontFamily: 'Share Tech Mono', cursor: 'pointer' }}>
                    SAVE USER 1
                </button>
            </div>
        </RackUnit>
    );
};
