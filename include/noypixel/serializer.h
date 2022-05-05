#pragma once

#include <yaml-cpp/yaml.h>

struct SerializerImageBuffer
{
public:
    SerializerImageBuffer(u32 image_width, u32 default_color);
    std::pair<u32, u32> put(u32 w, u32 h, u32* pixels);
    void flush(const std::string& file);
private:
    void newline(u32 h);

    struct Line
    {
        Line(Line&& line);
        Line(u32 w, u32 h, u32* data);
        ~Line();
        u32 w, h;
        u32* data;
    };

    std::vector<Line> lines;
    u32 image_width, default_color;
};

struct SerializerImage
{
    u32 w, h;
    u32* data;
};

class Serializer
{
public:
    Serializer(Project& project);

    void serialize(const std::string& json) const;
    void deserialize(const std::string& json);
private:
    void putSprite(SerializerImageBuffer& im, YAML::Emitter& out, const Sprite& s) const;
    void getFrame(SerializerImage& im, const YAML::Node& data, Sprite& s);

    Project& project;
};
