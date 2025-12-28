#pragma once

#include "audio_engine.h"
#include <string>
#include <vector>

namespace reamp {

class Player {
public:
    Player();
    ~Player();

    bool initialize();
    void shutdown();

    bool load_file(const std::string& filepath);
    void play();
    void pause();
    void stop();
    void seek(double position_seconds);

    bool is_playing() const;
    bool is_paused() const;
    double get_position() const;
    double get_duration() const;
    float get_volume() const;
    void set_volume(float volume);
    std::vector<float> get_spectrum() const;

private:
    AudioEngine audio_engine_;
    std::string current_file_;
};

} // namespace reamp

