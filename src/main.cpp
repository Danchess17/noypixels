#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <noypixel/noypixel.h>

#include <unistd.h>
#include <fstream>

//#define usleep(x)

template<typename T>
void test()
{
    T t;
}

u32 screen[40 * 24];

Project p;

void render(const Sprite& sprite, u32 xx = 0, u32 yy = 0, u32 frame = 0)
{
    for (u32 y = 0; y < sprite.height(); y++)
        for (u32 x = 0; x < sprite.width(); x++)
            if (sprite[frame].pixels[y * sprite.width() + x] & 0xff000000)
                screen[(y + yy) * 40 + x + xx] = sprite[frame].pixels[y * sprite.width() + x] & 0xf;
}

void flush()
{
    for (u32 y = 0; y < 24; y++) {
        for (u32 x = 0; x < 40; x++)
            printf("\033[4%dm  ", screen[y * 40 + x]);
        printf("\033[0m\n");
    }
}

class FallingBall
{
public:
    FallingBall() : sprite(p.newSprite("falling_ball", 40, 24)), canvas(sprite)
    {
        int y = -7;
        for (;;)
        {
            y++;
            if (y == 33) break;

            canvas.clear(0xff000002);
            print_ball(20, y, 8, 0xff000004);
            canvas.saveFrame();
            render(sprite);
            flush();

            usleep(200000);
        }
    }
private:
    Sprite& sprite;
    Canvas canvas;

    void print_ball(s32 x, s32 y, s32 r, u32 col)
    {
        for (s32 yy = y - r; yy <= y + r; yy++)
            for (s32 xx = x - r; xx <= x + r; xx++)
            {
                if (xx < 0 || yy < 0) continue;
                if (xx >= canvas.width() || yy >= canvas.height()) continue;
                if ((xx - x) * (xx - x) + (yy - y) * (yy - y) <= r * r)
                    canvas.setPixel(xx, yy, col);
            }
    }

};

class Blinker
{
public:
    Blinker() : screen(p.newSprite("blinker_screen", 40, 24)), screen_canvas(screen),
        blinker(p.newSprite("blinker", 5, 5)), blinker_canvas(blinker)
    {
        screen_canvas.clear(0xff000000);
        screen_canvas.saveFrame();

        for (u32 i = 0; i < 5; i++)
            blinker_canvas.setPixel(2, i, 0xff000001);
        blinker_canvas.saveFrame();

        blinker_canvas.addFrame();
        blinker_canvas.clear();
        for (u32 i = 0; i < 5; i++)
            blinker_canvas.setPixel(i, 2, 0xff000001);
        blinker_canvas.saveFrame();

        for (u32 i = 0; i < 50; i++)
        {
            render(screen);
            render(blinker, 15, 10, i % 2);

            flush();
            usleep(200000);
        }
    }
private:
    Sprite& screen;
    Canvas screen_canvas;
    Sprite& blinker;
    Canvas blinker_canvas;
};

class Animation
{
public:
    Animation() : screen(p.newSprite("anim_screen", 40, 24)), screen_canvas(screen),
        anim(p.newSprite("anim", 5, 5)), anim_canvas(anim)
    {
        screen_canvas.clear(0xff000000);
        screen_canvas.saveFrame();

        anim_canvas.addFrame();
        anim_canvas.setPixel(0, 0, 0xff000001);
        anim_canvas.saveFrame();

        anim_canvas.addFrame();
        anim_canvas.setPixel(0, 1, 0xff000001);
        anim_canvas.saveFrame();

        anim_canvas.addFrame();
        anim_canvas.setPixel(0, 2, 0xff000003);
        anim_canvas.saveFrame();

        anim[0].time = 0.6f;
        anim[1].time = 0.6f;
        anim[2].time = 1.2f;
        anim[3].time = 0.6f;

        for (u32 i = 0; i < 12; i++)
        {
            render(screen);
            render(anim, 15, 10, i % 4);

            flush();
            usleep(anim[i % 4].time * 1000000);
        }
    }
private:
    Sprite& screen;
    Canvas screen_canvas;
    Sprite& anim;
    Canvas anim_canvas;
};

int main()
{
    test<FallingBall>();
    test<Blinker>();
    test<Animation>();

    Sprite& sp = p.newSprite("test", 16, 16);
    Canvas c(sp, 0xff442288);
    c.saveFrame();

    Serializer s(p);
    s.serialize("a.png", "a.json");
}
