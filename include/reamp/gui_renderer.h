#pragma once

#include <string>

namespace reamp {

class GuiRenderer {
public:
    GuiRenderer();
    ~GuiRenderer();

    bool initialize(int width, int height);
    void shutdown();
    void resize(int width, int height);
    int get_width() const { return width_; }
    int get_height() const { return height_; }

    void begin_frame();
    void end_frame();

    // Drawing functions
    void draw_text(float x, float y, const std::string& text, float scale = 1.0f);
    void draw_rect(float x, float y, float w, float h, float r, float g, float b, float a = 1.0f);
    void draw_button(float x, float y, float w, float h, const std::string& label, bool& clicked);
    void draw_progress_bar(float x, float y, float w, float h, float progress);
    void draw_spectrum(float x, float y, float w, float h, const float* data, int bands);

private:
    int width_;
    int height_;
    bool initialized_;
    
    // Simple text rendering (will use bitmap font later)
    void setup_ortho();
};

} // namespace reamp

