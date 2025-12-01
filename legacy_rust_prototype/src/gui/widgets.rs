use font8x8::{BASIC_FONTS, UnicodeFonts};

pub trait Widget {
    fn draw(&self, buffer: &mut [u32], width: usize, height: usize, debug: bool);
    fn handle_mouse(&mut self, x: usize, y: usize, down: bool) -> bool; // Returns true if handled/clicked
    fn is_hovered(&self, x: usize, y: usize) -> bool;
}

// Helper for drawing text
fn draw_text(buffer: &mut [u32], width: usize, x: usize, y: usize, text: &str, color: u32) {
    let mut curr_x = x;
    for c in text.chars() {
        if let Some(glyph) = BASIC_FONTS.get(c) {
            for (row_idx, row) in glyph.iter().enumerate() {
                for col_idx in 0..8 {
                    if (row >> col_idx) & 1 == 1 {
                        let pixel_x = curr_x + col_idx;
                        let pixel_y = y + row_idx;
                        let idx = pixel_y * width + pixel_x;
                        if idx < buffer.len() {
                            buffer[idx] = color;
                        }
                    }
                }
            }
        }
        curr_x += 8;
    }
}

pub struct Button {
    pub x: usize,
    pub y: usize,
    pub w: usize,
    pub h: usize,
    pub label: String,
    pub pressed: bool,
}

impl Button {
    pub fn new(x: usize, y: usize, w: usize, h: usize, label: &str) -> Self {
        Self {
            x, y, w, h,
            label: label.to_string(),
            pressed: false,
        }
    }
}

impl Widget for Button {
    fn draw(&self, buffer: &mut [u32], width: usize, _height: usize, debug: bool) {
        // Draw label
        // Center text
        let text_width = self.label.len() * 8;
        let text_x = self.x + (self.w.saturating_sub(text_width)) / 2;
        let text_y = self.y + (self.h.saturating_sub(8)) / 2;
        
        let color = if self.pressed { 0x00FF00 } else { 0xFFFFFF };
        draw_text(buffer, width, text_x, text_y, &self.label, color);

        if debug {
            // Draw red outline
            let color = if self.pressed { 0x00FF00 } else { 0xFF0000 };
            for i in 0..self.w {
                let top = self.y * width + (self.x + i);
                let bottom = (self.y + self.h - 1) * width + (self.x + i);
                if top < buffer.len() { buffer[top] = color; }
                if bottom < buffer.len() { buffer[bottom] = color; }
            }
            for i in 0..self.h {
                let left = (self.y + i) * width + self.x;
                let right = (self.y + i) * width + (self.x + self.w - 1);
                if left < buffer.len() { buffer[left] = color; }
                if right < buffer.len() { buffer[right] = color; }
            }
        }
    }

    fn is_hovered(&self, x: usize, y: usize) -> bool {
        x >= self.x && x < self.x + self.w && y >= self.y && y < self.y + self.h
    }

    fn handle_mouse(&mut self, x: usize, y: usize, down: bool) -> bool {
        let hovered = self.is_hovered(x, y);
        if down && hovered {
            self.pressed = true;
            return false; // Still holding
        } else if !down && self.pressed && hovered {
            self.pressed = false;
            return true; // Clicked!
        } else if !down {
            self.pressed = false;
        }
        false
    }
}

pub struct Slider {
    pub x: usize,
    pub y: usize,
    pub w: usize,
    pub h: usize,
    pub value: f32, // 0.0 to 1.0
    pub vertical: bool,
    pub dragging: bool,
}

impl Slider {
    pub fn new(x: usize, y: usize, w: usize, h: usize, initial: f32, vertical: bool) -> Self {
        Self {
            x, y, w, h,
            value: initial,
            vertical,
            dragging: false,
        }
    }
}

impl Widget for Slider {
    fn draw(&self, buffer: &mut [u32], width: usize, _height: usize, debug: bool) {
        if debug {
            // Draw blue outline
            let color = 0x0000FF;
            for i in 0..self.w {
                let top = self.y * width + (self.x + i);
                let bottom = (self.y + self.h - 1) * width + (self.x + i);
                if top < buffer.len() { buffer[top] = color; }
                if bottom < buffer.len() { buffer[bottom] = color; }
            }
            for i in 0..self.h {
                let left = (self.y + i) * width + self.x;
                let right = (self.y + i) * width + (self.x + self.w - 1);
                if left < buffer.len() { buffer[left] = color; }
                if right < buffer.len() { buffer[right] = color; }
            }
            
            // Draw handle position
            let handle_color = 0xFFFF00;
            if self.vertical {
                let handle_y = self.y + self.h - (self.value * self.h as f32) as usize;
                let handle_y = handle_y.clamp(self.y, self.y + self.h - 1);
                for i in 0..self.w {
                    let idx = handle_y * width + (self.x + i);
                    if idx < buffer.len() { buffer[idx] = handle_color; }
                }
            } else {
                let handle_x = self.x + (self.value * self.w as f32) as usize;
                let handle_x = handle_x.clamp(self.x, self.x + self.w - 1);
                for i in 0..self.h {
                    let idx = (self.y + i) * width + handle_x;
                    if idx < buffer.len() { buffer[idx] = handle_color; }
                }
            }
        }
    }

    fn is_hovered(&self, x: usize, y: usize) -> bool {
        x >= self.x && x < self.x + self.w && y >= self.y && y < self.y + self.h
    }

    fn handle_mouse(&mut self, x: usize, y: usize, down: bool) -> bool {
        if down {
            if self.is_hovered(x, y) || self.dragging {
                self.dragging = true;
                // Update value
                if self.vertical {
                    let rel_y = (self.y + self.h).saturating_sub(y) as f32;
                    self.value = (rel_y / self.h as f32).clamp(0.0, 1.0);
                } else {
                    let rel_x = x.saturating_sub(self.x) as f32;
                    self.value = (rel_x / self.w as f32).clamp(0.0, 1.0);
                }
                return true; // Value changed
            }
        } else {
            self.dragging = false;
        }
        false
    }
}

pub struct Playlist {
    pub x: usize,
    pub y: usize,
    pub w: usize,
    pub h: usize,
    pub files: Vec<String>,
    pub selected: Option<usize>,
    pub scroll_offset: usize,
}

impl Playlist {
    pub fn new(x: usize, y: usize, w: usize, h: usize) -> Self {
        // Load files from current directory
        let mut files = Vec::new();
        if let Ok(entries) = std::fs::read_dir(".") {
            for entry in entries.flatten() {
                if let Ok(path) = entry.path().into_os_string().into_string() {
                    if path.ends_with(".mp3") || path.ends_with(".wav") || path.ends_with(".flac") {
                        files.push(path);
                    }
                }
            }
        }
        files.sort();
        
        Self {
            x, y, w, h,
            files,
            selected: None,
            scroll_offset: 0,
        }
    }
    
    pub fn get_selected_file(&self) -> Option<String> {
        self.selected.map(|idx| self.files[idx].clone())
    }
}

impl Widget for Playlist {
    fn draw(&self, buffer: &mut [u32], width: usize, _height: usize, debug: bool) {
        // Draw background
        let bg_color = 0x101010;
        for i in 0..self.h {
            for j in 0..self.w {
                let idx = (self.y + i) * width + (self.x + j);
                if idx < buffer.len() { buffer[idx] = bg_color; }
            }
        }
        
        // Draw items
        let item_height = 20;
        let max_items = self.h / item_height;
        
        for (i, file) in self.files.iter().skip(self.scroll_offset).take(max_items).enumerate() {
            let item_y = self.y + i * item_height;
            
            // Highlight selected
            let color = if Some(i + self.scroll_offset) == self.selected { 0x00FF00 } else { 0xAAAAAA };
            
            // Draw text
            // Truncate file name if too long
            let max_chars = self.w / 8;
            let display_text = if file.len() > max_chars {
                &file[..max_chars]
            } else {
                file
            };
            
            draw_text(buffer, width, self.x + 5, item_y + 6, display_text, color);
        }
        
        if debug {
            // Outline
            let color = 0xFF00FF;
             for i in 0..self.w {
                let top = self.y * width + (self.x + i);
                let bottom = (self.y + self.h - 1) * width + (self.x + i);
                if top < buffer.len() { buffer[top] = color; }
                if bottom < buffer.len() { buffer[bottom] = color; }
            }
            for i in 0..self.h {
                let left = (self.y + i) * width + self.x;
                let right = (self.y + i) * width + (self.x + self.w - 1);
                if left < buffer.len() { buffer[left] = color; }
                if right < buffer.len() { buffer[right] = color; }
            }
        }
    }

    fn is_hovered(&self, x: usize, y: usize) -> bool {
        x >= self.x && x < self.x + self.w && y >= self.y && y < self.y + self.h
    }

    fn handle_mouse(&mut self, x: usize, y: usize, down: bool) -> bool {
        if down && self.is_hovered(x, y) {
            let rel_y = y - self.y;
            let item_idx = rel_y / 20;
            let actual_idx = item_idx + self.scroll_offset;
            
            if actual_idx < self.files.len() {
                if self.selected != Some(actual_idx) {
                    self.selected = Some(actual_idx);
                    return true; // Selection changed
                }
            }
        }
        false
    }
}
