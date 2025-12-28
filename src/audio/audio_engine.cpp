#include "reamp/audio_engine.h"
#define MINIAUDIO_IMPLEMENTATION
#include "../../third_party/miniaudio/miniaudio.h"

#include <algorithm>
#include <cmath>

namespace reamp {

AudioEngine::AudioEngine()
    : initialized_(false)
    , playing_(false)
    , paused_(false)
    , position_(0.0)
    , duration_(0.0)
    , volume_(1.0f)
    , ma_context_(nullptr)
    , ma_device_(nullptr)
    , ma_decoder_(nullptr)
    , current_frame_(0)
{
}

AudioEngine::~AudioEngine() {
    shutdown();
}

bool AudioEngine::initialize(const AudioFormat& format) {
    if (initialized_) {
        return true;
    }

    format_ = format;
    
    // Initialize miniaudio context
    ma_context* context = new ma_context();
    ma_result result = ma_context_init(nullptr, 0, nullptr, context);
    if (result != MA_SUCCESS) {
        delete context;
        return false;
    }
    ma_context_ = context;

    // Initialize device
    ma_device_config device_config = ma_device_config_init(ma_device_type_playback);
    device_config.playback.format = ma_format_f32;
    device_config.playback.channels = format.channels;
    device_config.sampleRate = format.sample_rate;
    device_config.dataCallback = audio_callback;
    device_config.pUserData = this;

    ma_device* device = new ma_device();
    result = ma_device_init(nullptr, &device_config, device);
    if (result != MA_SUCCESS) {
        ma_context_uninit(static_cast<ma_context*>(ma_context_));
        delete static_cast<ma_context*>(ma_context_);
        delete device;
        return false;
    }
    ma_device_ = device;

    initialized_ = true;
    return true;
}

void AudioEngine::shutdown() {
    if (!initialized_) {
        return;
    }

    stop();

    if (ma_decoder_) {
        ma_decoder_uninit(static_cast<ma_decoder*>(ma_decoder_));
        delete static_cast<ma_decoder*>(ma_decoder_);
        ma_decoder_ = nullptr;
    }

    if (ma_device_) {
        ma_device_uninit(static_cast<ma_device*>(ma_device_));
        delete static_cast<ma_device*>(ma_device_);
        ma_device_ = nullptr;
    }

    if (ma_context_) {
        ma_context_uninit(static_cast<ma_context*>(ma_context_));
        delete static_cast<ma_context*>(ma_context_);
        ma_context_ = nullptr;
    }

    initialized_ = false;
}

bool AudioEngine::load_file(const std::string& filepath) {
    if (!initialized_) {
        return false;
    }

    stop();

    // Clean up previous decoder
    if (ma_decoder_) {
        ma_decoder_uninit(static_cast<ma_decoder*>(ma_decoder_));
        delete static_cast<ma_decoder*>(ma_decoder_);
        ma_decoder_ = nullptr;
    }

    // Initialize decoder
    ma_decoder_config decoder_config = ma_decoder_config_init(ma_format_f32, format_.channels, format_.sample_rate);
    ma_decoder* decoder = new ma_decoder();
    
    ma_result result = ma_decoder_init_file(filepath.c_str(), &decoder_config, decoder);
    if (result != MA_SUCCESS) {
        delete decoder;
        return false;
    }

    ma_decoder_ = decoder;

    // Get duration
    ma_uint64 total_frames;
    if (ma_decoder_get_available_frames(decoder, &total_frames) == MA_SUCCESS) {
        duration_ = static_cast<double>(total_frames) / format_.sample_rate;
    } else {
        duration_ = 0.0;
    }

    // Load entire file into buffer (for simplicity, can be optimized later)
    audio_buffer_.clear();
    const size_t buffer_size = 1024 * 1024; // 1MB chunks
    std::vector<float> temp_buffer(buffer_size);
    
    while (true) {
        ma_uint64 frames_to_read = buffer_size / format_.channels;
        ma_uint64 frames_read_this_iteration = 0;
        
        result = ma_decoder_read_pcm_frames(decoder, temp_buffer.data(), frames_to_read, &frames_read_this_iteration);
        if (result != MA_SUCCESS || frames_read_this_iteration == 0) {
            break;
        }
        
        size_t samples_read = frames_read_this_iteration * format_.channels;
        audio_buffer_.insert(audio_buffer_.end(), temp_buffer.begin(), temp_buffer.begin() + samples_read);
    }

    // Reset decoder position for playback
    ma_decoder_seek_to_pcm_frame(decoder, 0);

    current_filepath_ = filepath;
    current_frame_ = 0;
    position_ = 0.0;

    return true;
}

void AudioEngine::play() {
    if (!initialized_ || audio_buffer_.empty()) {
        return;
    }

    if (paused_) {
        paused_ = false;
    } else {
        ma_device_start(static_cast<ma_device*>(ma_device_));
    }

    playing_ = true;
}

void AudioEngine::pause() {
    if (!initialized_ || !playing_) {
        return;
    }

    ma_device_stop(static_cast<ma_device*>(ma_device_));
    paused_ = true;
    playing_ = false;
}

void AudioEngine::stop() {
    if (!initialized_) {
        return;
    }

    if (playing_ || paused_) {
        ma_device_stop(static_cast<ma_device*>(ma_device_));
    }

    playing_ = false;
    paused_ = false;
    current_frame_ = 0;
    position_ = 0.0;
}

void AudioEngine::seek(double position_seconds) {
    if (!initialized_ || audio_buffer_.empty()) {
        return;
    }

    size_t sample_rate = format_.sample_rate;
    size_t channels = format_.channels;
    size_t target_frame = static_cast<size_t>(position_seconds * sample_rate);
    
    if (target_frame * channels < audio_buffer_.size()) {
        current_frame_ = target_frame;
        position_ = position_seconds;
    }
}

void AudioEngine::set_volume(float volume) {
    volume_ = std::clamp(volume, 0.0f, 1.0f);
}

void AudioEngine::set_equalizer_band(int band, float gain) {
    // TODO: Implement EQ
}

std::vector<float> AudioEngine::get_spectrum() const {
    // TODO: Implement FFT-based spectrum analysis
    return std::vector<float>(256, 0.0f);
}

void AudioEngine::audio_callback(ma_device* pDevice, void* pOutput,
                                 const void* pInput, ma_uint32 frameCount) {
    (void)pInput; // Unused for playback
    
    AudioEngine* engine = static_cast<AudioEngine*>(pDevice->pUserData);
    float* output_f32 = static_cast<float*>(pOutput);
    
    if (!engine->playing_ || engine->audio_buffer_.empty()) {
        // Output silence
        std::fill(output_f32, output_f32 + frameCount * engine->format_.channels, 0.0f);
        return;
    }

    size_t channels = engine->format_.channels;
    size_t frames_to_copy = frameCount;
    size_t available_frames = (engine->audio_buffer_.size() / channels) - engine->current_frame_;

    if (frames_to_copy > available_frames) {
        frames_to_copy = available_frames;
        engine->playing_ = false;
    }

    // Copy and apply volume
    for (size_t i = 0; i < frames_to_copy * channels; ++i) {
        size_t src_idx = engine->current_frame_ * channels + i;
        output_f32[i] = engine->audio_buffer_[src_idx] * engine->volume_;
    }

    // Fill remaining with silence if needed
    if (frames_to_copy < frameCount) {
        std::fill(output_f32 + frames_to_copy * channels,
                  output_f32 + frameCount * channels, 0.0f);
    }

    engine->current_frame_ += frames_to_copy;
    engine->position_ = static_cast<double>(engine->current_frame_) / engine->format_.sample_rate;
}

} // namespace reamp

