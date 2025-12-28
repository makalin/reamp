# Quick Start: Creating Your First Skin

## Step 1: Create Your Bitmap

1. Open any image editor (GIMP, Photoshop, Paint.NET, etc.)
2. Create a new image:
   - **Width:** 800 pixels
   - **Height:** 600 pixels
   - **Color Mode:** RGB (24-bit) or RGBA (32-bit with transparency)
3. Design your player interface:
   - Draw the main background
   - Add buttons (Play, Pause, Stop)
   - Create a progress bar area
   - Design spectrum visualization area
   - Add any retro/chrome effects you want

## Step 2: Save as BMP

1. **File → Save As**
2. Choose **BMP format**
3. Save as: `skins/default/main.bmp`
4. Make sure it's in the project root directory

## Step 3: Run ReAmp

```bash
./build/bin/reamp
```

Your custom bitmap will automatically load and display!

## Example Design Ideas

### Retro Winamp Style
- Dark background (#1a1a1a)
- Chrome/metallic buttons
- LCD-style display areas
- Beveled edges
- Pixel-perfect design

### High-Tech Machinery
- Industrial gray/blue palette
- Gauge-style meters
- Circuit board patterns
- Glowing indicators
- Technical aesthetic

### Minimalist
- Clean lines
- Subtle gradients
- Modern but retro-inspired
- Focus on functionality

## Tips

- **Use layers** in your image editor to organize elements
- **Export at exact size** (800x600) for pixel-perfect rendering
- **Test frequently** - reload the app to see changes
- **Keep it readable** - high contrast for text areas
- **Save backups** - keep versions of your designs

## File Locations

The app will look for skins in this order:
1. `skins/default/main.bmp`
2. `assets/skin.bmp`
3. `assets/skin.png`
4. `skins/main.bmp`

Place your bitmap in any of these locations!

