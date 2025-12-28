#include "reamp/player.h"

namespace reamp {

Player::Player() {
}

Player::~Player() {
    shutdown();
}

bool Player::initialize() {
    return audio_engine_.initialize();
}

void Player::shutdown() {
    audio_engine_.shutdown();
}

bool Player::load_file(const std::string& filepath) {
    current_file_ = filepath;
    return audio_engine_.load_file(filepath);
}

void Player::play() {
    audio_engine_.play();
}

void Player::pause() {
    audio_engine_.pause();
}

void Player::stop() {
    audio_engine_.stop();
}

void Player::seek(double position_seconds) {
    audio_engine_.seek(position_seconds);
}

bool Player::is_playing() const {
    return audio_engine_.is_playing();
}

bool Player::is_paused() const {
    return audio_engine_.is_paused();
}

double Player::get_position() const {
    return audio_engine_.get_position();
}

double Player::get_duration() const {
    return audio_engine_.get_duration();
}

float Player::get_volume() const {
    return audio_engine_.get_volume();
}

void Player::set_volume(float volume) {
    audio_engine_.set_volume(volume);
}

std::vector<float> Player::get_spectrum() const {
    return audio_engine_.get_spectrum();
}

} // namespace reamp

