
import { audioEngine } from './AudioEngine';
import { RackUnit } from './RackUnit';

const Knob = ({ label, onChange }: any) => (
    <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', width: '60px' }}>
        <div style={{
            width: '40px', height: '40px', borderRadius: '50%', background: '#222',
            border: '2px solid #111', boxShadow: '0 2px 5px rgba(0,0,0,0.5)',
            display: 'flex', justifyContent: 'center', alignItems: 'center'
        }}>
            <input
                type="range" min="0" max="2" step="0.1" defaultValue="1"
                onChange={onChange}
                style={{ opacity: 0, width: '100%', height: '100%', cursor: 'pointer', position: 'absolute' }}
            />
            <div style={{ width: '2px', height: '15px', background: '#33ff33' }} />
        </div>
        <span style={{ fontSize: '0.7rem', marginTop: '5px', color: '#aaa' }}>{label}</span>
    </div>
);

export const SpatialUnit = () => {
    return (
        <RackUnit title="SPATIALIZER // 3D PROCESSOR">
            <div style={{ display: 'flex', justifyContent: 'space-around', alignItems: 'center', height: '100px' }}>

                {/* Radar Visual (Fake) */}
                <div style={{
                    width: '80px', height: '80px', borderRadius: '50%',
                    background: 'radial-gradient(circle, #112211 0%, #000 100%)',
                    border: '1px solid #33ff33', position: 'relative',
                    boxShadow: '0 0 10px #33ff33'
                }}>
                    <div style={{
                        position: 'absolute', top: '50%', left: '50%', width: '4px', height: '4px',
                        background: '#fff', borderRadius: '50%', transform: 'translate(-50%, -50%)'
                    }} />
                    <div style={{
                        position: 'absolute', top: '50%', left: '50%', width: '100%', height: '1px',
                        background: 'rgba(51, 255, 51, 0.3)', transform: 'translate(-50%, -50%) rotate(45deg)'
                    }} />
                    <div style={{
                        position: 'absolute', top: '50%', left: '50%', width: '100%', height: '1px',
                        background: 'rgba(51, 255, 51, 0.3)', transform: 'translate(-50%, -50%) rotate(-45deg)'
                    }} />
                </div>

                <div style={{ display: 'flex', gap: '20px' }}>
                    <Knob label="WIDTH" onChange={(e: any) => audioEngine.setStereoWidth(parseFloat(e.target.value))} />
                    <Knob label="DEPTH" onChange={(e: any) => audioEngine.setReverbMix(parseFloat(e.target.value) * 0.5)} />
                    <Knob label="ROTATE" onChange={(e: any) => audioEngine.setPan(parseFloat(e.target.value) - 1)} />
                </div>

            </div>
        </RackUnit>
    );
};
