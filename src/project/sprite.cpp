#include <noypixel/noypixel.h>

Sprite::Sprite(u32 width, u32 height) : w(width), h(height)
{ }

Sprite::Sprite(const Sprite& s) : Sprite(s.w, s.h)
{
    for (auto& i : s.frames)
        addFrame(i.pixels).time = i.time;
}

Sprite::~Sprite()
{
    for (auto& i : frames)
        delete[] i.pixels;
}

Sprite& Sprite::operator=(const Sprite& s)
{
    for (auto& i : frames)
        delete[] i.pixels;
    frames.clear();

    w = s.w;
    h = s.h;
    for (auto& i : s.frames)
        addFrame(i.pixels).time = i.time;
    return *this;
}

Frame& Sprite::addFrame(const u32* frame)
{
    u32* pixels = new u32[width() * height()];
    ASSERT(pixels, "memory limit");

    if (frame)
        std::memcpy(pixels, frame, width() * height() * sizeof(u32));
    frames.push_back({ pixels, 1.0f });
    return frames[nrFrames() - 1];
}

Frame& Sprite::addFrame(u32 color)
{
    u32* pixels = new u32[width() * height()];
    ASSERT(pixels, "memory limit");

    for (u32 i = 0; i < width() * height(); i++)
        pixels[i] =  color;

    frames.push_back({ pixels, 1.0f });
    return frames[nrFrames() - 1];
}

Frame& Sprite::getFrame(f32 time)
{
    u32 i;
    for (i = 0; i < nrFrames(); i++)
    {
        time -= frames[i].time;
        if (time < 0)
            break;
    }
    ASSERT(i < nrFrames(), "bad timecode");
    
    return frames[i];
}

void Sprite::resize(u32 neww, u32 newh)
{
    for (auto& i : frames)
    {
        u32* pixels = new u32[neww * newh];
        for (u32 y = 0; y < newh; y++)
            for (u32 x = 0; x < neww; x++)
                pixels[y * neww + x] = (y < h && x < w) ? i.pixels[y * w + x] : 0xff7f007f;
        delete[] i.pixels;
        i.pixels = pixels;
    }
    w = neww;
    h = newh;
}

size_t Sprite::nrFrames() const
{
    return frames.size();
}

Frame& Sprite::operator[] (u32 n)
{
    ASSERT(n < nrFrames(), "bad sprite frame");
    return frames[n];
}

const Frame& Sprite::operator[] (u32 n) const
{
    ASSERT(n < nrFrames(), "bad sprite frame");
    return frames[n];
}
