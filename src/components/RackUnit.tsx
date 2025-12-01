import React from 'react';

interface RackUnitProps {
    title: string;
    height?: string;
    children: React.ReactNode;
    power?: boolean;
}

export const RackUnit: React.FC<RackUnitProps> = ({ title, height = 'auto', children, power = true }) => {
    return (
        <div style={{
            background: '#2b2b2b',
            border: '2px solid #111',
            borderTopColor: '#444',
            borderLeftColor: '#444',
            borderRadius: '4px',
            position: 'relative',
            marginBottom: '10px',
            boxShadow: '0 4px 10px rgba(0,0,0,0.5)'
        }}>
            {/* Rack Ears */}
            <div style={{
                position: 'absolute', left: 0, top: 0, bottom: 0, width: '30px',
                background: '#222', borderRight: '1px solid #000',
                display: 'flex', flexDirection: 'column', justifyContent: 'space-between', padding: '10px 0'
            }}>
                <div className="screw" style={{ margin: '0 auto' }} />
                <div className="screw" style={{ margin: '0 auto' }} />
            </div>
            <div style={{
                position: 'absolute', right: 0, top: 0, bottom: 0, width: '30px',
                background: '#222', borderLeft: '1px solid #000',
                display: 'flex', flexDirection: 'column', justifyContent: 'space-between', padding: '10px 0'
            }}>
                <div className="screw" style={{ margin: '0 auto' }} />
                <div className="screw" style={{ margin: '0 auto' }} />
            </div>

            {/* Content Area */}
            <div style={{ margin: '0 30px', padding: '15px', minHeight: height }}>
                <div style={{
                    display: 'flex', justifyContent: 'space-between', alignItems: 'center',
                    borderBottom: '1px solid #444', paddingBottom: '5px', marginBottom: '15px'
                }}>
                    <span style={{
                        fontFamily: 'Share Tech Mono', fontSize: '0.9rem', color: '#aaa', letterSpacing: '1px'
                    }}>
                        {title}
                    </span>
                    {power && <div style={{ width: '8px', height: '8px', borderRadius: '50%', background: '#ff3333', boxShadow: '0 0 5px #ff3333' }} />}
                </div>
                {children}
            </div>
        </div>
    );
};
