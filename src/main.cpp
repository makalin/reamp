#include "reamp/audio_engine.h"
#include "reamp/window.h"
#include "reamp/gui_renderer.h"
#include "reamp/player.h"
#include "reamp/bitmap.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_keyboard.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

std::string format_time(double seconds) {
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << mins << ":" 
        << std::setfill('0') << std::setw(2) << secs;
    return oss.str();
}

int main(int argc, char* argv[]) {
    std::cout << "ReAmp ⚡ - Native Music Player\n";
    std::cout << "No Electron. No Bloat. Just Sound.\n\n";

    // Initialize window
    reamp::Window window;
    if (!window.create("ReAmp Pro", 800, 600)) {
        std::cerr << "Failed to create window\n";
        return 1;
    }

    // Initialize GUI renderer
    reamp::GuiRenderer gui;
    if (!gui.initialize(window.get_width(), window.get_height())) {
        std::cerr << "Failed to initialize GUI\n";
        window.destroy();
        return 1;
    }

    // Initialize bitmap renderer
    reamp::BitmapRenderer bitmap_renderer;
    if (!bitmap_renderer.initialize()) {
        std::cerr << "Warning: Bitmap renderer initialization failed\n";
    }

    // Try to load skin bitmap (optional)
    reamp::Bitmap skin_background;
    bool has_skin = false;
    
    // Try loading from common locations
    const char* skin_paths[] = {
        "skins/default/main.bmp",
        "assets/skin.bmp",
        "assets/skin.png",
        "skins/main.bmp"
    };
    
    for (const char* path : skin_paths) {
        if (skin_background.load(path)) {
            std::cout << "Loaded skin: " << path << std::endl;
            has_skin = true;
            break;
        }
    }
    
    if (!has_skin) {
        std::cout << "No skin bitmap found. Using default rendering.\n";
        std::cout << "Create a bitmap at skins/default/main.bmp to use custom design.\n";
    }

    // Initialize player
    reamp::Player player;
    if (!player.initialize()) {
        std::cerr << "Failed to initialize player\n";
        gui.shutdown();
        window.destroy();
        return 1;
    }

    std::cout << "Audio engine initialized\n";
    std::cout << "Window created: " << window.get_width() 
              << "x" << window.get_height() << "\n";
    std::cout << "\nControls:\n";
    std::cout << "  SPACE - Play/Pause\n";
    std::cout << "  S - Stop\n";
    std::cout << "  O - Open file\n";
    std::cout << "  +/- - Volume up/down\n";
    std::cout << "  ESC - Quit\n\n";

    // Try to load file from command line
    if (argc > 1) {
        std::string filepath = argv[1];
        std::cout << "Loading: " << filepath << "\n";
        if (player.load_file(filepath)) {
            std::cout << "File loaded successfully\n";
            player.play();
        } else {
            std::cout << "Failed to load file\n";
        }
    }

    // Main loop
    auto last_time = std::chrono::steady_clock::now();
    const double target_fps = 60.0;
    const double frame_time = 1.0 / target_fps;
    
    bool play_clicked = false;
    bool pause_clicked = false;
    bool stop_clicked = false;

    while (!window.should_close()) {
        auto current_time = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration<double>(
            current_time - last_time).count();

        if (delta < frame_time) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(frame_time - delta));
            continue;
        }

        last_time = current_time;

        // Poll events
        window.poll_events();
        
        // Check if window should close
        if (window.should_close()) {
            break;
        }

        // Handle keyboard input
        const Uint8* keystate = SDL_GetKeyboardState(nullptr);
        if (keystate[SDL_SCANCODE_ESCAPE]) {
            break;
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
            if (player.is_playing()) {
                player.pause();
            } else {
                player.play();
            }
            SDL_Delay(200); // Debounce
        }
        if (keystate[SDL_SCANCODE_S]) {
            player.stop();
            SDL_Delay(200);
        }
        if (keystate[SDL_SCANCODE_EQUALS] || keystate[SDL_SCANCODE_KP_PLUS]) {
            float vol = player.get_volume();
            player.set_volume(std::min(1.0f, vol + 0.1f));
            SDL_Delay(100);
        }
        if (keystate[SDL_SCANCODE_MINUS] || keystate[SDL_SCANCODE_KP_MINUS]) {
            float vol = player.get_volume();
            player.set_volume(std::max(0.0f, vol - 0.1f));
            SDL_Delay(100);
        }

        // Handle window resize
        static int last_width = window.get_width();
        static int last_height = window.get_height();
        if (window.get_width() != last_width || window.get_height() != last_height) {
            gui.resize(window.get_width(), window.get_height());
            last_width = window.get_width();
            last_height = window.get_height();
        }

        // Render GUI
        gui.begin_frame();

        // Draw skin background if available
        if (has_skin && skin_background.is_loaded()) {
            // Draw the skin bitmap as background (scaled to window size)
            bitmap_renderer.draw_bitmap_scaled(skin_background, 
                                               0, 0, 
                                               window.get_width(), 
                                               window.get_height());
        } else {
            // Default rendering - draw main panel
            float panel_x = 50.0f;
            float panel_y = 50.0f;
            float panel_w = static_cast<float>(window.get_width()) - 100.0f;
            float panel_h = static_cast<float>(window.get_height()) - 100.0f;

            // Background - make it more visible
            gui.draw_rect(panel_x, panel_y, panel_w, panel_h, 0.2f, 0.2f, 0.25f, 1.0f);
        }
        
        // Draw UI elements on top (these will overlay the skin)
        float panel_x = 50.0f;
        float panel_y = 50.0f;
        float panel_w = static_cast<float>(window.get_width()) - 100.0f;
        float panel_h = static_cast<float>(window.get_height()) - 100.0f;

        // Title
        gui.draw_text(panel_x + 20, panel_y + 20, "ReAmp Pro", 1.5f);

        // Status
        std::string status = "Stopped";
        if (player.is_playing()) {
            status = "Playing";
        } else if (player.is_paused()) {
            status = "Paused";
        }
        gui.draw_text(panel_x + 20, panel_y + 60, "Status: " + status);

        // Time display
        double pos = player.get_position();
        double dur = player.get_duration();
        std::string time_str = format_time(pos) + " / " + format_time(dur);
        gui.draw_text(panel_x + 20, panel_y + 90, time_str);

        // Progress bar
        float progress = dur > 0.0 ? static_cast<float>(pos / dur) : 0.0f;
        gui.draw_progress_bar(panel_x + 20, panel_y + 120, panel_w - 40, 20, progress);

        // Controls
        float btn_y = panel_y + 160;
        float btn_w = 80.0f;
        float btn_h = 40.0f;
        float btn_spacing = 20.0f;

        gui.draw_button(panel_x + 20, btn_y, btn_w, btn_h, "Play", play_clicked);
        if (play_clicked && !player.is_playing()) {
            player.play();
        }

        gui.draw_button(panel_x + 20 + btn_w + btn_spacing, btn_y, btn_w, btn_h, "Pause", pause_clicked);
        if (pause_clicked && player.is_playing()) {
            player.pause();
        }

        gui.draw_button(panel_x + 20 + (btn_w + btn_spacing) * 2, btn_y, btn_w, btn_h, "Stop", stop_clicked);
        if (stop_clicked) {
            player.stop();
        }

        // Volume display
        float vol = player.get_volume();
        std::ostringstream vol_oss;
        vol_oss << "Volume: " << std::fixed << std::setprecision(0) << (vol * 100) << "%";
        gui.draw_text(panel_x + 20, btn_y + btn_h + 20, vol_oss.str());

        // Spectrum visualization (placeholder)
        std::vector<float> spectrum = player.get_spectrum();
        if (!spectrum.empty()) {
            gui.draw_spectrum(panel_x + 20, btn_y + btn_h + 60, panel_w - 40, 100, 
                            spectrum.data(), static_cast<int>(spectrum.size()));
        }

        gui.end_frame();

        // Swap buffers
        window.swap_buffers();
    }

    // Cleanup
    skin_background.unload();
    bitmap_renderer.shutdown();
    player.shutdown();
    gui.shutdown();
    window.destroy();

    std::cout << "ReAmp shutdown complete\n";
    return 0;
}

