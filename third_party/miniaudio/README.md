# Miniaudio Setup

Miniaudio is a single-file audio library. To set it up:

1. Download miniaudio.h from: https://github.com/mackron/miniaudio
2. Place `miniaudio.h` in this directory (`third_party/miniaudio/`)

Or use git submodule:

```bash
git submodule add https://github.com/mackron/miniaudio.git third_party/miniaudio
```

The CMakeLists.txt is already configured to include this directory.

