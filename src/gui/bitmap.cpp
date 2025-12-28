#include "reamp/bitmap.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <cstring>

#ifdef REAMP_USE_SDL2_IMAGE
#include <SDL2/SDL_image.h>
#endif

namespace reamp {

Bitmap::Bitmap()
    : width_(0)
    , height_(0)
    , loaded_(false)
{
}

Bitmap::~Bitmap() {
    unload();
}

bool Bitmap::load(const std::string& filepath) {
    unload();

    SDL_Surface* surface = nullptr;

#ifdef REAMP_USE_SDL2_IMAGE
    // Try SDL_image first (supports PNG, JPG, etc.)
    surface = IMG_Load(filepath.c_str());
#endif

    // Fallback to SDL_LoadBMP for .bmp files (built into SDL2)
    if (!surface) {
        surface = SDL_LoadBMP(filepath.c_str());
    }

    if (!surface) {
        std::cerr << "Failed to load image: " << filepath 
                  << " - " << SDL_GetError() << std::endl;
        std::cerr << "Note: SDL2 supports BMP natively. For PNG/JPG, install SDL2_image and rebuild with -DUSE_SDL2_IMAGE=ON" << std::endl;
        return false;
    }

    width_ = surface->w;
    height_ = surface->h;

    // Convert to RGBA format
    SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if (!converted) {
        SDL_FreeSurface(surface);
        return false;
    }

    // Copy pixel data
    pixels_.resize(width_ * height_ * 4);
    memcpy(pixels_.data(), converted->pixels, pixels_.size());

    SDL_FreeSurface(converted);
    SDL_FreeSurface(surface);

    loaded_ = true;
    return true;
}

void Bitmap::unload() {
    pixels_.clear();
    width_ = 0;
    height_ = 0;
    loaded_ = false;
}

BitmapRenderer::BitmapRenderer()
    : initialized_(false)
{
}

BitmapRenderer::~BitmapRenderer() {
    shutdown();
}

bool BitmapRenderer::initialize() {
#ifdef REAMP_USE_SDL2_IMAGE
    // Initialize SDL_image
    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }
#endif

    initialized_ = true;
    return true;
}

void BitmapRenderer::shutdown() {
    if (initialized_) {
#ifdef REAMP_USE_SDL2_IMAGE
        IMG_Quit();
#endif
        initialized_ = false;
    }
}

void BitmapRenderer::draw_bitmap(const Bitmap& bitmap, int x, int y) {
    if (!bitmap.is_loaded()) return;
    draw_bitmap_scaled(bitmap, x, y, bitmap.get_width(), bitmap.get_height());
}

void BitmapRenderer::draw_bitmap_region(const Bitmap& bitmap,
                                       int src_x, int src_y, int src_w, int src_h,
                                       int dst_x, int dst_y) {
    if (!bitmap.is_loaded()) return;
    
    // Create temporary bitmap for the region
    // For now, just draw the full bitmap scaled
    // TODO: Implement proper region rendering
    draw_bitmap_scaled(bitmap, dst_x, dst_y, src_w, src_h);
}

void BitmapRenderer::draw_bitmap_scaled(const Bitmap& bitmap, int x, int y, int w, int h) {
    if (!bitmap.is_loaded() || !initialized_) return;

    // Use OpenGL to render the bitmap
    // Create texture from pixel data
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.get_width(), bitmap.get_height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.get_pixels());

    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw quad with texture
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + w, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + w, y + h);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1, &texture);
}

} // namespace reamp

