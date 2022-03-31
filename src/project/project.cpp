#include <noypixel/noypixel.h>

Sprite& Project::newSprite(const std::string& name, u32 width, u32 height)
{
    ASSERT(sprites.find(name) == sprites.end(), "sprite already exist");
    return sprites.emplace(std::piecewise_construct,
        std::forward_as_tuple(name),
        std::forward_as_tuple(width, height)).first->second;
}

void Project::deleteSprite(const std::string& name)
{
    ASSERT(sprites.find(name) != sprites.end(), "sprite not found");
    sprites.erase(name);
}

Sprite& Project::loadSprite(const std::string& name)
{
    ASSERT(sprites.find(name) != sprites.end(), "sprite not found");
    return sprites[name];
}

void Project::clear()
{
    sprites.clear();
}
