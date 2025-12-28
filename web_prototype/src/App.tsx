import { PlayerUI } from './components/PlayerUI';

function App() {
    return (
        <div style={{
            display: 'flex',
            justifyContent: 'center',
            alignItems: 'center',
            height: '100vh',
            width: '100vw',
            background: 'radial-gradient(circle at 50% 50%, #1a1a2e 0%, #0f0f13 100%)'
        }}>
            <PlayerUI />
        </div>
    )
}

export default App
