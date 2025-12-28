#include "reamp/playlist.h"
#include <algorithm>

namespace reamp {

Playlist::Playlist()
    : current_index_(0)
{
}

Playlist::~Playlist() {
}

void Playlist::add_item(const std::string& filepath) {
    PlaylistItem item;
    item.filepath = filepath;
    item.title = filepath; // TODO: Extract metadata
    item.artist = "Unknown";
    item.duration = 0.0;
    items_.push_back(item);
}

void Playlist::remove_item(size_t index) {
    if (index < items_.size()) {
        items_.erase(items_.begin() + index);
        if (current_index_ >= items_.size() && !items_.empty()) {
            current_index_ = items_.size() - 1;
        }
    }
}

void Playlist::clear() {
    items_.clear();
    current_index_ = 0;
}

const PlaylistItem& Playlist::get_item(size_t index) const {
    return items_[index];
}

PlaylistItem& Playlist::get_item(size_t index) {
    return items_[index];
}

void Playlist::set_current_index(size_t index) {
    if (index < items_.size()) {
        current_index_ = index;
    }
}

void Playlist::next() {
    if (!items_.empty()) {
        current_index_ = (current_index_ + 1) % items_.size();
    }
}

void Playlist::previous() {
    if (!items_.empty()) {
        if (current_index_ == 0) {
            current_index_ = items_.size() - 1;
        } else {
            current_index_--;
        }
    }
}

const PlaylistItem* Playlist::get_current() const {
    if (current_index_ < items_.size()) {
        return &items_[current_index_];
    }
    return nullptr;
}

} // namespace reamp

