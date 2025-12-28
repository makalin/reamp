# ReAmp Skins

This directory contains bitmap-based skins for ReAmp.

## Skin Structure

Create your skin images in this directory. Recommended structure:

```
skins/
  your_skin_name/
    main.bmp          - Main player window
    buttons.bmp       - Button sprites
    spectrum.bmp       - Spectrum analyzer background
    eq.bmp            - Equalizer panel
```

## Image Format

- **Format:** BMP, PNG, or JPG
- **Recommended:** BMP for retro feel
- **Color depth:** 24-bit or 32-bit (with alpha)
- **Style:** Retro, pixel-art, or high-tech machinery aesthetic

## Creating Skins

1. Design your bitmap images using any image editor (GIMP, Photoshop, Aseprite, etc.)
2. Save as BMP, PNG, or JPG
3. Place in a subdirectory under `skins/`
4. Update the skin loading code to use your images

## Example Skin Elements

- **Main Window:** 800x600px player interface
- **Buttons:** 80x40px each (Play, Pause, Stop, etc.)
- **Progress Bar:** Custom progress bar graphics
- **Spectrum:** Background for audio visualization
- **EQ Sliders:** Equalizer control graphics

## Retro Style Tips

- Use pixel-perfect designs
- Limited color palettes (16-256 colors)
- High contrast for readability
- Metallic/chrome effects for that 90s look
- LCD-style displays for text areas

