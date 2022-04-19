#pragma once

class Canvas
{
public:
    Canvas(Sprite& sprite, u32 color = 0);
    ~Canvas();
    
    void openFrame(u32 n);
    void saveFrame();
    void addFrame();

    void clear(u32 color = 0);
    void setPixel(u32 x, u32 y, u32 color);
    u32 getPixel(u32 x, u32 y);
    
    constexpr u32* getPixels() { return pixels; }
    
    constexpr u32 width() const { return sprite.width(); }
    constexpr u32 height() const { return sprite.height(); }
    size_t nrFrames() const { return sprite.nrFrames(); }
private:
    Sprite& sprite;
    Frame* frame;
    u32* pixels;
};
