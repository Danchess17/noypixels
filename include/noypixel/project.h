#pragma once

class Project
{
public:
    using iterator = std::unordered_map<std::string, Sprite>::iterator;

    Sprite& newSprite(const std::string& name, u32 width, u32 height);
    void deleteSprite(const std::string& name);
    Sprite& loadSprite(const std::string& name);
    
    void clear();

    iterator begin() { return sprites.begin(); }
    iterator end() { return sprites.end(); }
private:
    std::unordered_map<std::string, Sprite> sprites;
};
