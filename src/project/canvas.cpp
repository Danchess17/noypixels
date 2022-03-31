#include <noypixel/noypixel.h>

Canvas::Canvas(Sprite& sprite, u32 color) : sprite(sprite), pixels(new u32[width() * height()])
{
    ASSERT(pixels, "memory limit");

    if (!sprite.nrFrames())
    {
        clear(color);
        addFrame();
    }
    else openFrame(0);
}

Canvas::~Canvas()
{
    delete[] pixels;
}

void Canvas::openFrame(u32 n)
{
    frame = &sprite[n];
    std::memcpy(pixels, frame->pixels, width() * height() * sizeof(u32));
}

void Canvas::saveFrame()
{
    std::memcpy(frame->pixels, pixels, width() * height() * sizeof(u32));
}

void Canvas::addFrame()
{
    frame = &sprite.addFrame(pixels);
}

void Canvas::clear(u32 color)
{
    for (u32 i = 0; i < width() * height(); i++)
        pixels[i] = color;
}

void Canvas::setPixel(u32 x, u32 y, u32 color)
{
    ASSERT(x < width() && y < height(), "bad coordinates");

    u32* d = &pixels[width() * y + x];
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
    return pixels[width() * y + x];
}
