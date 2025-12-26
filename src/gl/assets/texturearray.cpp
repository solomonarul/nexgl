#include "gl/assets/texturearray.hpp"

using namespace NEX::GL;

TextureArray::TextureArray(std::vector<TextureConfig> cfgs)
{
    for (auto cfg : cfgs) this->textures.push_back(std::make_shared<Texture>(cfg));
}
