#pragma once

class Canvas
{
public:
    Canvas(Sprite& sprite, u32 color = 0);

    void save();
    void openFrame(u32 n);
    void openFrame(f32 time);
    void addFrame();
    void resize(u32 neww, u32 newh);

    void clear(u32 color = 0);
    void setPixel(u32 x, u32 y, u32 color);
    u32 getPixel(u32 x, u32 y);

    constexpr u32* getPixels() { return frame->pixels; }
    
    constexpr u32 width() const { return copy.width(); }
    constexpr u32 height() const { return copy.height(); }
    constexpr const std::vector<Frame>& getFrames() const { return copy.getFrames(); }

    float frameTime() const;
    size_t nrFrames() const { return copy.nrFrames(); }
private:
    Sprite& sprite;
    Sprite copy;
    Frame* frame;
};
