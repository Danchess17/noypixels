#include <noypixel/noypixel.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <fstream>

SerializerImageBuffer::SerializerImageBuffer(u32 image_width, u32 default_color) : image_width(image_width), default_color(default_color)
    { }

std::pair<u32, u32> SerializerImageBuffer::put(u32 w, u32 h, u32* pixels)
{
    u32 x, y, hh;
    
    for (y = 0, hh = 0; y < lines.size(); hh += lines[y].h, y++)
        if (lines[y].h >= h && lines[y].w + w <= image_width)
            break;
    if (y == lines.size())
        newline(h);
    Line& l = lines[y];
    x = l.w;
    l.w += w;

    for (u32 yy = 0; yy < h; yy++)
        for (u32 xx = 0; xx < w; xx++)
            l.data[yy * image_width + xx + x] = pixels[yy * w + xx];

    return std::make_pair(x, hh);
}

void SerializerImageBuffer::flush(const std::string& file)
{
    u32 h = 0;
    for (auto& i : lines)
        h += i.h;
    
    u32* data = new u32[image_width * h];
    for (u32 i = 0; i < image_width * h; i++)
        data[i] = default_color;
    h = 0;

    for (auto& i : lines)
    {
        for (u32 y = 0; y < i.h; y++)
            for (u32 x = 0; x < i.w; x++)
                data[(h + y) * image_width + x] = i.data[y * image_width + x];
        h += i.h;
    }

    stbi_write_png(file.c_str(), image_width, h, sizeof(u32), data, sizeof(u32) * image_width);
    lines.clear();
    delete[] data;
}

void SerializerImageBuffer::newline(u32 h)
{
    lines.emplace_back(0, h, new u32[image_width * h]);
    for (u32 i = 0; i < image_width * h; i++)
        lines[lines.size() - 1].data[i] = default_color;
}

SerializerImageBuffer::Line::Line(Line&& line) : w(line.w), h(line.h), data(line.data)
{
    line.data = nullptr;
}

SerializerImageBuffer::Line::Line(u32 w, u32 h, u32* data) : w(w), h(h), data(data)
{ }

SerializerImageBuffer::Line::~Line()
{
    delete[] data;
}


Serializer::Serializer(Project& project) : project(project)
{ }

void Serializer::serialize(const std::string& json) const
{
    std::string png = json + ".png";
    u32 w = 0;
    for (auto& i : project)
        if (w < i.second.width() * i.second.nrFrames())
            w = i.second.width() * i.second.nrFrames();
    
    YAML::Emitter out;

    SerializerImageBuffer im(w * 2, 0xffff00ff);
    out << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap;
    out << "spritesheet" << png << "sprites" << YAML::BeginMap;

    for (const auto& s : project)
    {
        out << s.first;
        putSprite(im, out, s.second);
    }

    out << YAML::EndMap << YAML::EndMap;
    std::ofstream os(json);
    os << out.c_str();
    im.flush(png);
}

void Serializer::deserialize(const std::string& json)
{
    project.clear();

    if (!json.size())
        return;

    YAML::Node data = YAML::LoadFile(json);
    SerializerImage im;
    int ch;

    std::string png = json + ".png";

    im.data = (u32*)stbi_load(png.c_str(), (int*)&im.w, (int*)&im.h, &ch, 0);
    
    for(YAML::const_iterator it = data["sprites"].begin(); it != data["sprites"].end(); ++it)
    {
        Sprite& s = project.newSprite(it->first.as<std::string>(), it->second["width"].as<u32>(), it->second["height"].as<u32>());
        if (it->second["frames"])
            for (const auto& frame : it->second["frames"])
                getFrame(im, frame, s);
        else
            getFrame(im, it->second, s);
    }
    stbi_image_free(im.data);
}

void Serializer::putSprite(SerializerImageBuffer& im, YAML::Emitter& out, const Sprite& s) const
{
    out << YAML::BeginMap << "width" << s.width() << "height" << s.height();

    if (s.nrFrames() > 1)
        out << "frames" << YAML::BeginSeq;

    for (u32 n = 0; n < s.nrFrames(); n++)
    {
        auto[x, y] = im.put(s.width(), s.height(), s[n].pixels);
        if (s.nrFrames() > 1)
            out << YAML::BeginMap;
        out << "x" << x << "y" << y;
        if (s.nrFrames() > 1)
            out << "time" << s[n].time << YAML::EndMap;
    }

    if (s.nrFrames() > 1)
        out << YAML::EndSeq;
    out << YAML::EndMap;
}

void Serializer::getFrame(SerializerImage& im, const YAML::Node& data, Sprite& s)
{
    Frame& frame = s.addFrame(nullptr);
    
    frame.time = data["time"] ? data["time"].as<f32>() : 1;
    
    u32 x = data["x"].as<u32>();
    u32 y = data["y"].as<u32>();

    for (u32 yy = 0; yy < s.height(); yy++)
        for (u32 xx = 0; xx < s.width(); xx++)
            frame.pixels[yy * s.width() + xx] = im.data[(y + yy) * im.w + x + xx];
}
