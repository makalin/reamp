#include "reamp/window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

namespace reamp {

Window::Window()
    : sdl_window_(nullptr)
    , gl_context_(nullptr)
    , width_(0)
    , height_(0)
    , should_close_(false)
{
}

Window::~Window() {
    destroy();
}

bool Window::create(const std::string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Try OpenGL 2.1 first (most compatible)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Make context current
    if (SDL_GL_MakeCurrent(window, context) != 0) {
        std::cerr << "SDL_GL_MakeCurrent failed: " << SDL_GetError() << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    sdl_window_ = window;
    gl_context_ = context;
    width_ = width;
    height_ = height;
    should_close_ = false;

    return true;
}

void Window::destroy() {
    if (gl_context_) {
        SDL_GL_DeleteContext(static_cast<SDL_GLContext>(gl_context_));
        gl_context_ = nullptr;
    }

    if (sdl_window_) {
        SDL_DestroyWindow(static_cast<SDL_Window*>(sdl_window_));
        sdl_window_ = nullptr;
    }

    SDL_Quit();
}

void Window::poll_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            should_close_ = true;
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                width_ = event.window.data1;
                height_ = event.window.data2;
            }
        }
    }
}

void Window::swap_buffers() {
    if (sdl_window_) {
        SDL_GL_SwapWindow(static_cast<SDL_Window*>(sdl_window_));
    }
}

bool Window::is_key_pressed(int key_code) const {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    return state[key_code] != 0;
}

bool Window::is_mouse_button_pressed(int button) const {
    Uint32 buttons = SDL_GetMouseState(nullptr, nullptr);
    return (buttons & SDL_BUTTON(button)) != 0;
}

void Window::get_mouse_position(int& x, int& y) const {
    SDL_GetMouseState(&x, &y);
}

} // namespace reamp

