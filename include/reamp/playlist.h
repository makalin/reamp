#pragma once

#include <string>
#include <vector>

namespace reamp {

struct PlaylistItem {
    std::string filepath;
    std::string title;
    std::string artist;
    double duration;
};

class Playlist {
public:
    Playlist();
    ~Playlist();

    void add_item(const std::string& filepath);
    void remove_item(size_t index);
    void clear();

    size_t size() const { return items_.size(); }
    const PlaylistItem& get_item(size_t index) const;
    PlaylistItem& get_item(size_t index);

    size_t get_current_index() const { return current_index_; }
    void set_current_index(size_t index);
    void next();
    void previous();

    const PlaylistItem* get_current() const;

private:
    std::vector<PlaylistItem> items_;
    size_t current_index_;
};

} // namespace reamp

