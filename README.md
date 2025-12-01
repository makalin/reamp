# ReAmp ⚡

> **No Electron. No Bloat. Just Sound.**
> *The spiritual successor to the golden age of media players, re-engineered for the modern streaming era.*

![ReAmp Screenshot](assets/screen1.png)

## 📜 Manifest

Modern music players have lost their way. They are memory-hungry web browsers disguised as apps. **ReAmp** is a return to form.

We believe a music player should open instantly, use minimal RAM, and look like a piece of high-tech machinery, not a flat website. ReAmp combines the tactile satisfaction of 90s interfaces (think Winamp 2.9) with the infinite library of 2025 (Spotify).

## ✨ Key Features

  * **Truly Native & Blazing Fast:** Written in low-level code (C++/Rust) for zero latency. Uses a fraction of the RAM of modern streaming apps.
  * **Retro GUI Engine:** Support for classic `.wsz` style skins, bitmap fonts, and pixel-perfect layouts.
  * **The Hybrid Engine:**
      * **Local Playback:** FLAC, MP3, OGG, WAV support with audiophile-grade decoding.
      * **Spotify Integration:** Stream your Spotify library directly through the ReAmp interface. (Requires Spotify Premium).
  * **Modular Plugin System:** Community-driven DSP effects, visualizations, and input plugins.
  * **Live Lyrics:** Auto-syncing lyrics fetched in real-time, displayed in a dedicated retro LCD-style window.
  * **Cross-Platform:** Desktop (macOS, Windows, Linux) and Mobile (iOS, Android) syncing via local network.

-----

## 🛠 Tech Stack

ReAmp is built on the metal, not on Chrome.

  * **Core Audio Engine:** Rust / C++ (Miniaudio backend)
  * **GUI:** Native rendering (Metal for Mac, DirectX for Windows, Vulkan for Linux/Android).
  * **Scripting:** Lua integration for lightweight skin scripting and plugins.

-----

## 🚀 Getting Started

### Prerequisites

  * **Windows:** C++ Build Tools / Rust Cargo
  * **Mac:** Xcode Command Line Tools
  * **Spotify:** Client ID (for API integration)

### Installation

Clone the repo and build from source:

```bash
git clone https://github.com/makalin/reamp.git
cd reamp
# Build the core engine
make build-core
# Run the desktop client
./bin/reamp
```

-----

## 🔌 Plugin Architecture

ReAmp supports three types of modules:

1.  **DSP (Digital Signal Processing):** EQ, Reverb, Bass Boost.
2.  **Vis (Visualizations):** Spectrum analyzers, oscilloscopes (MilkDrop compatible).
3.  **Source:** Add support for Soundcloud, Tidal, or FTP servers.

*Example Lua script for a simple visualization:*

```lua
function onRender(frequencyData)
    for i, val in ipairs(frequencyData) do
        drawBar(i, val, "0x00FF00") -- Classic Green
    end
end
```

-----

## 🗺 Roadmap

  - [ ] **v0.1 (Alpha):** Basic local playback, default "Classic" skin, 10-band EQ.
  - [ ] **v0.5 (Beta):** Spotify Connect integration, Playlist management.
  - [ ] **v1.0 (Gold):** Plugin marketplace, Mobile companion app (iOS/Android).
  - [ ] **Future:** Cloud sync for settings and skins.

-----

## 🤝 Contributing

We welcome pull requests from those who miss the days when software was optimized.

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/RetroSkin`)
3.  Commit your Changes (`git commit -m 'Add support for .wsz skins'`)
4.  Push to the Branch (`git push origin feature/RetroSkin`)
5.  Open a Pull Request

-----

## 👤 Author

**Mehmet T. AKALIN**

  * **Company:** [Digital Vision](https://dv.com.tr)
  * **GitHub:** [@makalin](https://github.com/makalin)
  * **LinkedIn:** [Mehmet T. Akalin](https://www.linkedin.com/in/makalin/)
  * **X (Twitter):** [@makalin](https://x.com/makalin)

*Made with ❤️ in Türkiye. Keeping the retro spirit alive.*
