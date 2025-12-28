import React, { useState } from 'react';
import { Visualizer } from './Visualizer';
import { Controls } from './Controls';
import { Equalizer } from './Equalizer';
import { EffectsUnit } from './EffectsUnit';
import { PlaylistUnit } from './PlaylistUnit';
import { SpatialUnit } from './SpatialUnit';
import { PresetUnit } from './PresetUnit';
import { RackUnit } from './RackUnit';
import { audioEngine } from './AudioEngine';

export const PlayerUI = () => {
    const [fileName, setFileName] = useState<string>('SYSTEM READY');

    const handleDrop = async (e: React.DragEvent) => {
        e.preventDefault();
        e.stopPropagation();

        const files = e.dataTransfer.files;
        if (files.length > 0) {
            audioEngine.addToPlaylist(files);
            setFileName(`LOADED ${files.length} TRACKS`);
        }
    };

    const handleDragOver = (e: React.DragEvent) => {
        e.preventDefault();
        e.stopPropagation();
    };

    return (
        <div
            onDrop={handleDrop}
            onDragOver={handleDragOver}
            style={{
                width: '650px',
                height: '90vh',
                background: '#151515',
                border: '10px solid #111',
                borderRight: '10px solid #222', // Depth effect
                borderRadius: '4px',
                boxShadow: '0 0 50px rgba(0,0,0,0.9)',
                display: 'flex',
                flexDirection: 'column'
            }}
        >
            {/* Header */}
            <div style={{
                padding: '15px', background: '#111', borderBottom: '2px solid #000',
                display: 'flex', justifyContent: 'space-between', alignItems: 'center'
            }}>
                <div style={{ fontFamily: 'Share Tech Mono', fontSize: '1.5rem', color: '#fff', letterSpacing: '2px' }}>
                    ReAmp <span style={{ color: '#ff3333' }}>ULTIMATE</span>
                </div>
                <div className="lcd-text" style={{ fontSize: '1rem', color: '#33ff33' }}>
                    {fileName}
                </div>
            </div>

            {/* Scrollable Rack Area */}
            <div style={{
                flex: 1,
                overflowY: 'auto',
                padding: '20px',
                background: 'repeating-linear-gradient(0deg, #1a1a1a, #1a1a1a 10px, #151515 10px, #151515 20px)' // Rack rails bg
            }}>

                <RackUnit title="MASTER TRANSPORT">
                    <Visualizer />
                    <Controls />
                </RackUnit>

                <PlaylistUnit />

                <Equalizer />

                <PresetUnit />

                <SpatialUnit />

                <EffectsUnit />

                {/* Footer Vent */}
                <div style={{ height: '50px', background: '#111', borderTop: '1px solid #333', marginTop: '20px' }} />
            </div>
        </div>
    );
};
