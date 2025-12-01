mod audio;
mod gui;

use audio::AudioPlayer;
use gui::{Gui, GuiEvent};

fn main() {
    println!("ReAmp - Initializing...");

    // Initialize Audio Engine
    let player = AudioPlayer::new();
    println!("Audio Engine: OK");

    // Initialize GUI
    let mut gui = Gui::new("ReAmp v0.1 - Pro");
    println!("GUI Engine: OK");

    // Main Loop
    while gui.is_open() {
        // Get spectrum data from player
        let spectrum = {
            let data = player.spectrum_data.lock().unwrap();
            data.clone()
        };

        let event = gui.update(&spectrum);

        match event {
            GuiEvent::Play => {
                println!("Play clicked");
                if player.is_playing() {
                    player.resume();
                } else {
                    // For demo purposes, try to play a local file if it exists
                    player.play_file("test.mp3");
                }
            }
            GuiEvent::Pause => {
                println!("Pause clicked");
                player.pause();
            }
            GuiEvent::Stop => {
                println!("Stop clicked");
                player.stop();
            }
            GuiEvent::LoadFile(path) => {
                println!("Loading file: {}", path);
                player.play_file(&path);
            }
            GuiEvent::Volume(vol) => {
                println!("Volume set to: {}", vol);
                player.set_volume(vol);
            }
            GuiEvent::Seek(pct) => {
                println!("Seeking to: {:.2}%", pct * 100.0);
                player.seek(pct);
            }
            GuiEvent::Eq(bass, treble) => {
                println!("EQ: Bass={:.2}, Treble={:.2}", bass, treble);
                let mut state = player.eq_state.lock().unwrap();
                state.bass = bass * 2.0; // Scale to 0-2.0
                state.treble = treble * 2.0;
            }
            GuiEvent::None => {}
        }
    }

    println!("ReAmp - Exiting...");
}
