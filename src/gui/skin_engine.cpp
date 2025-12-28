#include "reamp/skin_engine.h"

namespace reamp {

SkinEngine::SkinEngine()
    : skin_loaded_(false)
{
}

SkinEngine::~SkinEngine() {
}

bool SkinEngine::load_skin(const std::string& skin_path) {
    // TODO: Implement skin loading (.wsz format support)
    current_skin_path_ = skin_path;
    skin_loaded_ = true;
    return true;
}

void SkinEngine::render() {
    // TODO: Implement skin rendering
}

} // namespace reamp

