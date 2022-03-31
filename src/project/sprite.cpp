#include <noypixel/noypixel.h>

Sprite::Sprite(u32 width, u32 height) : w(width), h(height)
{ }

Sprite::~Sprite()
{
    for (auto& i : frames)
        delete[] i.pixels;
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
