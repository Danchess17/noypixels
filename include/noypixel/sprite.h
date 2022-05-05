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
    Sprite(const Sprite& s);
    Sprite(Sprite&& s) = default;
    ~Sprite();

    Sprite& operator=(const Sprite& s);

    Frame& addFrame(const u32* frame);
    Frame& addFrame(u32 color);
    Frame& getFrame(f32 time);
    
    void resize(u32 neww, u32 newh);
    
    constexpr u32 width() const { return w; }
    constexpr u32 height() const { return h; }
    constexpr const std::vector<Frame>& getFrames() const { return frames; }

    size_t nrFrames() const;
    Frame& operator[] (u32 n);
    const Frame& operator[] (u32 n) const;
private:
    std::vector<Frame> frames;
    u32 w, h;
};
