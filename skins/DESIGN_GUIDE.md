# ReAmp Bitmap Design Guide

## Quick Start

1. **Create your bitmap images** in BMP format (SDL2 supports BMP natively)
2. **Save them** in the `skins/` directory or a subdirectory
3. **Load them** in your code using the `Bitmap` class

## Supported Formats

- **BMP** - Native SDL2 support (recommended for retro feel)
- **PNG/JPG** - Requires SDL2_image (install with `brew install sdl2_image` and rebuild with `-DUSE_SDL2_IMAGE=ON`)

## Example Usage

```cpp
#include "reamp/bitmap.h"
#include "reamp/bitmap_renderer.h"

// Load a bitmap
reamp::Bitmap skin;
skin.load("skins/my_skin/main.bmp");

// Render it
reamp::BitmapRenderer renderer;
renderer.initialize();
renderer.draw_bitmap(skin, 0, 0);
```

## Design Tips for Retro Skins

### Color Palette
- Use limited palettes (16-256 colors) for that classic look
- High contrast for readability
- Metallic/chrome effects for buttons

### Dimensions
- **Main Window:** 800x600px (or your window size)
- **Buttons:** 80x40px each
- **Progress Bar:** Custom width, 20px height
- **Spectrum:** Match your visualization area

### Style Elements
- Pixel-perfect designs
- Beveled edges for 3D effect
- LCD-style displays for text
- Glossy/metallic surfaces
- Drop shadows for depth

## File Structure

```
skins/
  my_skin/
    main.bmp          - Main player background
    button_play.bmp    - Play button
    button_pause.bmp    - Pause button
    button_stop.bmp    - Stop button
    progress.bmp       - Progress bar graphics
    spectrum.bmp       - Spectrum analyzer background
```

## Creating Bitmaps

You can use any image editor:
- **GIMP** (free) - Export as BMP
- **Photoshop** - Save as BMP
- **Aseprite** (pixel art) - Export as BMP
- **Paint.NET** - Save as BMP

Make sure to:
- Use 24-bit or 32-bit color depth
- Save as uncompressed BMP for best compatibility
- Test your images in the player

