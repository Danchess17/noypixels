#include <noypixel/noypixel.h>

Canvas::Canvas(Sprite& sprite, u32 color) : sprite(sprite), copy(sprite), frame(nullptr)
{
    if (!copy.nrFrames())
        frame = &copy.addFrame(color);
    else openFrame((u32)0);
}

void Canvas::openFrame(u32 n)
{
    frame = &copy[n];
}

void Canvas::openFrame(f32 time)
{
    frame = &copy.getFrame(time);
}

void Canvas::save()
{
    sprite = copy;
}

void Canvas::addFrame()
{
    f32 time = frame ? frame->time : 1.0f;
    frame = &copy.addFrame(frame->pixels);
    frame->time = time;
}

void Canvas::resize(u32 neww, u32 newh)
{
    copy.resize(neww, newh);
}

void Canvas::clear(u32 color)
{
    for (u32 i = 0; i < width() * height(); i++)
        frame->pixels[i] = color;
}

void Canvas::setPixel(u32 x, u32 y, u32 color)
{
    ASSERT(x < width() && y < height(), "bad coordinates");

    u32* d = &frame->pixels[width() * y + x];
    u8 a0 = ((u8*)&color)[3];
    u8 a1 = ((u8*)d)[3];
    u8 a = (255 - a0) * a1 / 255;
    u8 a01 = a + a0;

    *d = (a01 << 24) |
        ((u32)((u8*)d)[0] * a + ((((u8*)&color)[0] * a0) / a01)) | 
        ((u32)(((u8*)d)[1] * a + ((((u8*)&color)[1] * a0) / a01)) << 8) | 
        ((u32)(((u8*)d)[2] * a + ((((u8*)&color)[2] * a0) / a01)) << 16);
}

u32 Canvas::getPixel(u32 x, u32 y)
{
    ASSERT(x < width() && y < height(), "bad coordinates");
    return frame->pixels[width() * y + x];
}

float Canvas::frameTime() const
{
    float res = 0.0f;
    for (auto& f : getFrames())
    {
        if (&f == frame)
            break;
        res += f.time;
    }
    return res;
}
