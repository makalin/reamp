mod widgets;
use widgets::{Widget, Button, Slider, Playlist};
use minifb::{Key, MouseButton, MouseMode, Window, WindowOptions};
use image::GenericImageView;
use rfd::FileDialog;

const WIDTH: usize = 1024; // Enforce skin size
const HEIGHT: usize = 512;

pub enum GuiEvent {
    None,
    Play,
    Pause,
    Stop,
    LoadFile(String),
    Volume(f32),
    Seek(f32),
    Eq(f32, f32), // Bass, Treble
}

pub struct Gui {
    window: Window,
    buffer: Vec<u32>,
    skin_buffer: Vec<u32>,
    // Visualizer state
    bars: Vec<f32>,
    // Widgets
    play_btn: Button,
    pause_btn: Button,
    stop_btn: Button,
    load_btn: Button,
    vol_slider: Slider,
    seek_slider: Slider,
    bass_slider: Slider,
    treble_slider: Slider,
    playlist: Playlist,
    // Debug
    debug_mode: bool,
}

impl Gui {
    pub fn new(title: &str) -> Self {
        let mut window = Window::new(
            title,
            WIDTH,
            HEIGHT,
            WindowOptions::default(),
        )
        .unwrap_or_else(|e| {
            panic!("{}", e);
        });

        window.limit_update_rate(Some(std::time::Duration::from_micros(16600)));

        let buffer = vec![0; WIDTH * HEIGHT];
        
        // Load Skin
        let mut skin_buffer = vec![0; WIDTH * HEIGHT];
        let skin_path = "assets/skin.png";
        println!("Attempting to load skin from: {}", skin_path);
        match image::open(skin_path) {
            Ok(img) => {
                println!("Skin loaded successfully. Dimensions: {:?}", img.dimensions());
                let img = img.resize_exact(WIDTH as u32, HEIGHT as u32, image::imageops::FilterType::Nearest);
                for (x, y, pixel) in img.pixels() {
                    let r = pixel[0] as u32;
                    let g = pixel[1] as u32;
                    let b = pixel[2] as u32;
                    let color = (r << 16) | (g << 8) | b;
                    let index = (y as usize) * WIDTH + (x as usize);
                    if index < skin_buffer.len() {
                        skin_buffer[index] = color;
                    }
                }
            }
            Err(e) => {
                eprintln!("Failed to load skin.png: {}", e);
                // Fallback to grey
                for i in skin_buffer.iter_mut() {
                    *i = 0x202020;
                }
            }
        }

        // Initialize Widgets (Approximate positions based on skin)
        // Play: Green Triangle (x: 20-70, y: 150-200) -> Let's refine
        // Pause: Yellow Bars (x: 80-130)
        // Stop: Red Square (x: 140-190)
        // Eject: x: 500-550
        // Volume: x: 560-580, y: 50-250
        
        // Seek Bar: Let's put it at the bottom or top of controls?
        // Let's assume a seek bar area x: 20, y: 220, w: 500, h: 20
        
        Self { 
            window, 
            buffer, 
            skin_buffer,
            bars: vec![0.0; 20],
            play_btn: Button::new(20, 150, 50, 50, "Play"),
            pause_btn: Button::new(80, 150, 50, 50, "Pause"),
            stop_btn: Button::new(140, 150, 50, 50, "Stop"),
            load_btn: Button::new(500, 150, 50, 50, "Load"),
            vol_slider: Slider::new(560, 50, 20, 200, 1.0, true),
            seek_slider: Slider::new(20, 220, 500, 20, 0.0, false),
            bass_slider: Slider::new(600, 50, 20, 100, 0.5, true), // Example pos
            treble_slider: Slider::new(630, 50, 20, 100, 0.5, true), // Example pos
            playlist: Playlist::new(700, 50, 300, 400), // Right side playlist
            debug_mode: false,
        }
    }

    pub fn update(&mut self, spectrum_data: &[f32]) -> GuiEvent {
        // Toggle Debug
        if self.window.is_key_pressed(Key::F1, minifb::KeyRepeat::No) {
            self.debug_mode = !self.debug_mode;
            println!("Debug Mode: {}", self.debug_mode);
        }

        // 1. Draw Skin Background
        self.buffer.copy_from_slice(&self.skin_buffer);

        // 2. Draw LCD Area (Visualizer)
        for (i, &val) in spectrum_data.iter().enumerate() {
            if i < self.bars.len() {
                if val > self.bars[i] {
                    self.bars[i] = val;
                } else {
                    self.bars[i] *= 0.90;
                }
            }
        }

        for i in 0..20 {
            let bar_x = 30 + i * 25;
            let val = self.bars[i] * 1000.0;
            let height = (val as usize).min(80);
            
            for y in (120 - height)..120 {
                for x in bar_x..(bar_x + 20) {
                    let index = y * WIDTH + x;
                    if index < self.buffer.len() {
                        let color = if y < 60 { 0xFF0000 } else { 0x00FF00 };
                        self.buffer[index] = color;
                    }
                }
            }
        }

        // 3. Draw Widgets (Debug Overlay)
        self.play_btn.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);
        self.pause_btn.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);
        self.stop_btn.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);
        self.load_btn.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);
        self.vol_slider.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);
        self.seek_slider.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);
        self.bass_slider.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);
        self.treble_slider.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);
        self.playlist.draw(&mut self.buffer, WIDTH, HEIGHT, self.debug_mode);

        // 4. Handle Input
        let mut event = GuiEvent::None;
        let mouse_down = self.window.get_mouse_down(MouseButton::Left);
        let (mx, my) = self.window.get_mouse_pos(MouseMode::Discard).map(|(x, y)| (x as usize, y as usize)).unwrap_or((0, 0));

        if self.play_btn.handle_mouse(mx, my, mouse_down) { event = GuiEvent::Play; }
        if self.pause_btn.handle_mouse(mx, my, mouse_down) { event = GuiEvent::Pause; }
        if self.stop_btn.handle_mouse(mx, my, mouse_down) { event = GuiEvent::Stop; }
        
        // Load button logic (trigger on release)
        if self.load_btn.handle_mouse(mx, my, mouse_down) {
             // Open File Dialog
             // Note: This blocks the UI thread, which is not ideal but acceptable for now.
             if let Some(path) = FileDialog::new().add_filter("Audio", &["mp3", "wav", "flac", "ogg"]).pick_file() {
                 if let Some(path_str) = path.to_str() {
                     event = GuiEvent::LoadFile(path_str.to_string());
                 }
             }
        }
        
        // Sliders return true if value changed while dragging
        if self.vol_slider.handle_mouse(mx, my, mouse_down) {
            event = GuiEvent::Volume(self.vol_slider.value);
        }
        if self.seek_slider.handle_mouse(mx, my, mouse_down) {
            event = GuiEvent::Seek(self.seek_slider.value);
        }
        
        let mut eq_changed = false;
        if self.bass_slider.handle_mouse(mx, my, mouse_down) { eq_changed = true; }
        if self.treble_slider.handle_mouse(mx, my, mouse_down) { eq_changed = true; }
        
        if eq_changed {
            event = GuiEvent::Eq(self.bass_slider.value, self.treble_slider.value);
        }
        
        if self.playlist.handle_mouse(mx, my, mouse_down) {
            if let Some(file) = self.playlist.get_selected_file() {
                event = GuiEvent::LoadFile(file);
            }
        }

        // Keyboard Shortcuts
        if self.window.is_key_pressed(Key::O, minifb::KeyRepeat::No) {
             if let Some(path) = FileDialog::new().add_filter("Audio", &["mp3", "wav", "flac", "ogg"]).pick_file() {
                 if let Some(path_str) = path.to_str() {
                     event = GuiEvent::LoadFile(path_str.to_string());
                 }
             }
        }

        self.window
            .update_with_buffer(&self.buffer, WIDTH, HEIGHT)
            .unwrap();

        event
    }

    pub fn is_open(&self) -> bool {
        self.window.is_open() && !self.window.is_key_down(Key::Escape)
    }
}
