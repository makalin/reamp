#pragma once

#include <string>
#include <memory>

namespace reamp {

class SkinEngine {
public:
    SkinEngine();
    ~SkinEngine();

    bool load_skin(const std::string& skin_path);
    void render();

private:
    bool skin_loaded_;
    std::string current_skin_path_;
};

} // namespace reamp

