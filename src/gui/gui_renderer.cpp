#include "reamp/gui_renderer.h"
#include <SDL2/SDL_opengl.h>
#include <algorithm>
#include <cmath>

namespace reamp {

GuiRenderer::GuiRenderer()
    : width_(0)
    , height_(0)
    , initialized_(false)
{
}

GuiRenderer::~GuiRenderer() {
    shutdown();
}

bool GuiRenderer::initialize(int width, int height) {
    width_ = width;
    height_ = height;
    initialized_ = true;
    
    return true;
}

void GuiRenderer::shutdown() {
    initialized_ = false;
}

void GuiRenderer::resize(int width, int height) {
    width_ = width;
    height_ = height;
}

void GuiRenderer::begin_frame() {
    glViewport(0, 0, width_, height_);
    
    // Clear with dark background
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    setup_ortho();
    
    // Enable 2D rendering
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GuiRenderer::end_frame() {
    // Frame complete
}

void GuiRenderer::setup_ortho() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width_, height_, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GuiRenderer::draw_text(float x, float y, const std::string& text, float scale) {
    // Simple placeholder - will be replaced with bitmap font rendering
    // For now, just draw a small rectangle to indicate text position
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
    draw_rect(x, y, text.length() * 8.0f * scale, 16.0f * scale, 0.8f, 0.8f, 0.8f, 0.3f);
}

void GuiRenderer::draw_rect(float x, float y, float w, float h, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
    glFlush(); // Ensure rendering happens
}

void GuiRenderer::draw_button(float x, float y, float w, float h, const std::string& label, bool& clicked) {
    // Simple button - just a rectangle for now
    // In a real implementation, this would check mouse position and clicks
    draw_rect(x, y, w, h, 0.2f, 0.4f, 0.6f, 1.0f);
    draw_text(x + 10, y + h/2 - 8, label);
    clicked = false; // TODO: Implement click detection
}

void GuiRenderer::draw_progress_bar(float x, float y, float w, float h, float progress) {
    progress = std::clamp(progress, 0.0f, 1.0f);
    
    // Background
    draw_rect(x, y, w, h, 0.2f, 0.2f, 0.2f, 1.0f);
    
    // Progress fill
    draw_rect(x, y, w * progress, h, 0.0f, 0.6f, 1.0f, 1.0f);
    
    // Border
    glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void GuiRenderer::draw_spectrum(float x, float y, float w, float h, const float* data, int bands) {
    if (!data || bands <= 0) return;
    
    float bar_width = w / bands;
    
    for (int i = 0; i < bands; ++i) {
        float value = std::clamp(data[i], 0.0f, 1.0f);
        float bar_height = h * value;
        
        // Color gradient: blue to green to red
        float r = std::min(value * 2.0f, 1.0f);
        float g = value < 0.5f ? value * 2.0f : 1.0f;
        float b = value < 0.5f ? 1.0f : (1.0f - (value - 0.5f) * 2.0f);
        
        draw_rect(x + i * bar_width, y + h - bar_height, bar_width - 1, bar_height, r, g, b, 1.0f);
    }
}

} // namespace reamp

