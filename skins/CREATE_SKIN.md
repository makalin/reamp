# How to Create Your Bitmap Skin

## Simple Method (Using Any Image Editor)

### Option 1: GIMP (Free)
1. Download GIMP: https://www.gimp.org/
2. File → New → Set to 800x600 pixels
3. Design your interface
4. File → Export As → Choose "Windows BMP image"
5. Save as `skins/default/main.bmp`

### Option 2: Photoshop
1. New Document → 800x600 pixels, RGB
2. Design your interface
3. File → Save As → Choose "BMP"
4. Save as `skins/default/main.bmp`

### Option 3: Online Editor
1. Go to https://www.photopea.com/ (free, works in browser)
2. File → New → 800x600px
3. Design your interface
4. File → Export As → BMP
5. Save to `skins/default/main.bmp`

## What to Design

Your bitmap will be the **background** of the player. The UI elements (buttons, progress bar, text) will be drawn on top.

### Recommended Layout:
```
┌─────────────────────────────────┐
│  [Title Area]                    │
│                                  │
│  [Status/Time Display]           │
│  [Progress Bar]                  │
│                                  │
│  [Play] [Pause] [Stop]           │
│                                  │
│  [Volume Display]                │
│                                  │
│  [Spectrum Visualization Area]   │
└─────────────────────────────────┘
```

### Design Tips:
- **Background:** Dark colors work best (#1a1a1a, #2d2d2d)
- **Accent areas:** Use lighter colors for where UI elements will appear
- **Retro style:** Add chrome/metallic effects, bevels, gradients
- **Keep it simple:** The bitmap is the foundation, UI elements overlay it

## Testing Your Skin

1. Save your bitmap as `skins/default/main.bmp`
2. Run: `./build/bin/reamp`
3. Your bitmap will automatically load as the background!

## Example Color Palette (Retro Winamp Style)

- Background: `#1a1a1a` (dark gray)
- Accent: `#4a4a4a` (medium gray)
- Highlight: `#6a6a6a` (light gray)
- Chrome: `#c0c0c0` to `#e0e0e0` (metallic gradient)
- LCD Display: `#00ff00` (green) on `#000000` (black)

## Next Steps

Once your background bitmap is working, you can:
- Create separate button bitmaps
- Design custom progress bar graphics
- Add spectrum analyzer backgrounds
- Create a complete skin pack

Check `DESIGN_GUIDE.md` for more advanced techniques!

