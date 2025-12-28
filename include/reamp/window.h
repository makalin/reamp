#pragma once

#include <string>
#include <memory>

namespace reamp {

class Window {
public:
    Window();
    ~Window();

    bool create(const std::string& title, int width, int height);
    void destroy();

    void poll_events();
    bool should_close() const { return should_close_; }
    void swap_buffers();

    int get_width() const { return width_; }
    int get_height() const { return height_; }

    // Input
    bool is_key_pressed(int key_code) const;
    bool is_mouse_button_pressed(int button) const;
    void get_mouse_position(int& x, int& y) const;

private:
    void* sdl_window_;
    void* gl_context_;
    int width_;
    int height_;
    bool should_close_;
};

} // namespace reamp

