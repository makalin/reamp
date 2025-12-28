
import { audioEngine } from './AudioEngine';
import { RackUnit } from './RackUnit';

const Knob = ({ label, onChange, min = 0, max = 1, step = 0.01, defaultValue = 0 }: any) => (
    <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', width: '60px' }}>
        <div style={{
            position: 'relative', width: '40px', height: '40px',
            borderRadius: '50%', background: '#222',
            border: '2px solid #111', boxShadow: '0 2px 5px rgba(0,0,0,0.5)'
        }}>
            <input
                type="range"
                min={min} max={max} step={step} defaultValue={defaultValue}
                onChange={onChange}
                style={{
                    position: 'absolute', width: '100%', height: '100%', opacity: 0, cursor: 'pointer'
                }}
                title={label}
            />
            <div style={{
                position: 'absolute', top: '5px', left: '50%', width: '2px', height: '15px',
                background: '#fff', transform: 'translateX(-50%)', pointerEvents: 'none'
            }} />
        </div>
        <span style={{ fontSize: '0.7rem', marginTop: '5px', color: '#aaa' }}>{label}</span>
    </div>
);

export const EffectsUnit = () => {
    return (
        <RackUnit title="FX-PRO MULTI-EFFECTS">
            <div style={{ display: 'flex', justifyContent: 'space-around', padding: '10px 0' }}>

                {/* Reverb Section */}
                <div style={{ display: 'flex', gap: '10px', borderRight: '1px solid #444', paddingRight: '20px' }}>
                    <div style={{ fontSize: '0.8rem', color: '#666', writingMode: 'vertical-lr', transform: 'rotate(180deg)' }}>REVERB</div>
                    <Knob label="MIX" onChange={(e: any) => audioEngine.setReverbMix(parseFloat(e.target.value))} />
                </div>

                {/* Delay Section */}
                <div style={{ display: 'flex', gap: '10px', borderRight: '1px solid #444', paddingRight: '20px' }}>
                    <div style={{ fontSize: '0.8rem', color: '#666', writingMode: 'vertical-lr', transform: 'rotate(180deg)' }}>DELAY</div>
                    <Knob label="TIME" max={1.0} onChange={(e: any) => audioEngine.setDelayTime(parseFloat(e.target.value))} />
                    <Knob label="FDBK" onChange={(e: any) => audioEngine.setDelayFeedback(parseFloat(e.target.value))} />
                </div>

                {/* Master Section */}
                <div style={{ display: 'flex', gap: '10px' }}>
                    <div style={{ fontSize: '0.8rem', color: '#666', writingMode: 'vertical-lr', transform: 'rotate(180deg)' }}>MASTER</div>
                    <Knob label="PAN" min={-1} max={1} onChange={(e: any) => audioEngine.setPan(parseFloat(e.target.value))} />
                    <Knob label="VOL" max={2} defaultValue={1} onChange={(e: any) => audioEngine.setVolume(parseFloat(e.target.value))} />
                </div>

            </div>
        </RackUnit>
    );
};
