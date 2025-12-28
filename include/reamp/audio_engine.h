#pragma once

#include <string>
#include <memory>
#include <vector>

// Forward declarations for miniaudio
struct ma_device;
struct ma_decoder;
typedef unsigned int ma_uint32;

namespace reamp {

struct AudioFormat {
    int sample_rate = 44100;
    int channels = 2;
    int bits_per_sample = 16;
};

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();

    bool initialize(const AudioFormat& format = AudioFormat{});
    void shutdown();

    bool load_file(const std::string& filepath);
    void play();
    void pause();
    void stop();
    void seek(double position_seconds);

    bool is_playing() const { return playing_; }
    bool is_paused() const { return paused_; }
    double get_position() const { return position_; }
    double get_duration() const { return duration_; }
    float get_volume() const { return volume_; }
    void set_volume(float volume); // 0.0 to 1.0

    // DSP functions
    void set_equalizer_band(int band, float gain); // -12.0 to +12.0 dB
    std::vector<float> get_spectrum() const; // Frequency spectrum for visualization

private:
    bool initialized_;
    bool playing_;
    bool paused_;
    double position_;
    double duration_;
    float volume_;
    AudioFormat format_;

    // Miniaudio context, device, and decoder
    void* ma_context_;
    void* ma_device_;
    void* ma_decoder_;

    // Audio data
    std::vector<float> audio_buffer_;
    size_t current_frame_;
    std::string current_filepath_;

    static void audio_callback(ma_device* pDevice, void* pOutput, 
                              const void* pInput, ma_uint32 frameCount);
};

} // namespace reamp

