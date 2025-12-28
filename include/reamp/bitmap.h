#pragma once

#include <string>
#include <memory>
#include <vector>

namespace reamp {

class Bitmap {
public:
    Bitmap();
    ~Bitmap();

    bool load(const std::string& filepath);
    void unload();

    int get_width() const { return width_; }
    int get_height() const { return height_; }
    bool is_loaded() const { return loaded_; }

    // Get raw pixel data (RGBA)
    const unsigned char* get_pixels() const { return pixels_.data(); }

private:
    int width_;
    int height_;
    bool loaded_;
    std::vector<unsigned char> pixels_; // RGBA format
};

class BitmapRenderer {
public:
    BitmapRenderer();
    ~BitmapRenderer();

    bool initialize();
    void shutdown();

    // Draw bitmap at position
    void draw_bitmap(const Bitmap& bitmap, int x, int y);
    
    // Draw portion of bitmap (for sprite sheets)
    void draw_bitmap_region(const Bitmap& bitmap, 
                           int src_x, int src_y, int src_w, int src_h,
                           int dst_x, int dst_y);
    
    // Draw scaled bitmap
    void draw_bitmap_scaled(const Bitmap& bitmap, int x, int y, int w, int h);

private:
    bool initialized_;
};

} // namespace reamp

