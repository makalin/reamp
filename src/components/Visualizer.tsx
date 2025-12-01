import { useEffect, useRef } from 'react';
import { audioEngine } from './AudioEngine';

export const Visualizer = () => {
    const canvasRef = useRef<HTMLCanvasElement>(null);

    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) return;

        const ctx = canvas.getContext('2d');
        if (!ctx) return;

        const bufferLength = 32; // Fewer bars for blocky retro look
        const dataArray = new Uint8Array(bufferLength);

        let animationId: number;

        const renderFrame = () => {
            animationId = requestAnimationFrame(renderFrame);

            audioEngine.getAnalyserData(dataArray);

            const width = canvas.width;
            const height = canvas.height;
            const barWidth = (width / bufferLength) - 4; // Spacing

            ctx.fillStyle = '#112211'; // LCD Background
            ctx.fillRect(0, 0, width, height);

            // Draw Grid
            ctx.strokeStyle = '#1a331a';
            ctx.lineWidth = 1;
            ctx.beginPath();
            for (let y = 0; y < height; y += 10) {
                ctx.moveTo(0, y);
                ctx.lineTo(width, y);
            }
            ctx.stroke();

            let x = 2;

            for (let i = 0; i < bufferLength; i++) {
                const barHeight = (dataArray[i] / 255) * height;
                const segments = Math.floor(barHeight / 6); // 6px segments

                for (let j = 0; j < segments; j++) {
                    const y = height - (j * 6);

                    // Color based on height (Green -> Yellow -> Red)
                    if (j > 20) ctx.fillStyle = '#ff3333'; // Red
                    else if (j > 15) ctx.fillStyle = '#ffff33'; // Yellow
                    else ctx.fillStyle = '#33ff33'; // Green

                    ctx.fillRect(x, y - 4, barWidth, 4);
                }

                x += barWidth + 4;
            }
        };

        renderFrame();

        return () => {
            cancelAnimationFrame(animationId);
        };
    }, []);

    return (
        <div style={{
            padding: '10px',
            background: '#000',
            border: '2px solid #444',
            borderBottomColor: '#666',
            borderRightColor: '#666',
            borderRadius: '4px',
            boxShadow: 'inset 0 0 10px #000'
        }}>
            <canvas
                ref={canvasRef}
                width={600}
                height={150}
                style={{
                    width: '100%',
                    height: '150px',
                    display: 'block',
                    imageRendering: 'pixelated'
                }}
            />
        </div>
    );
};
