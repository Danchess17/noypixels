#pragma once

struct Frame
{
    u32* pixels;
    f32 time;
};

class Sprite
{
public:
    Sprite(u32 width = 0, u32 height = 0);
    Sprite(Sprite&& s) = default;
    ~Sprite();

    Frame& addFrame(const u32* frame);
    Frame& getFrame(f32 time);
    
    constexpr u32 width() const { return w; }
    constexpr u32 height() const { return h; }

    size_t nrFrames() const;
    Frame& operator[] (u32 n);
    const Frame& operator[] (u32 n) const;
private:
    std::vector<Frame> frames;
    const u32 w, h;
};
