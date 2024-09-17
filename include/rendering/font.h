#ifndef FONT_H
#define FONT_H

#include <string>
#include <cstdint>
#include <vector>

struct PackedChar
{
   unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
   float xoff, yoff, xadvance;
   float xoff2, yoff2;
};

struct AlignedQuad
{
   float x0, y0, s0, t0; // top-left
   float x1, y1, s1, t1; // bottom-right
};

class Font {
public:
    Font();
    Font(const std::string &filepath, uint32_t pixelSize);
    Font(const uint8_t *fontData, uint32_t pixelSize);
    inline uint32_t getPixelSize() const { return pixelSize; }
    inline uint32_t getTexture() const { return texture; }
    inline std::vector<PackedChar> &getCharacters() { return characters; }
    inline std::vector<AlignedQuad> &getQuads() { return quads; }
    inline uint32_t getCodePointOfFirstChar() { return codePointOfFirstChar; }
    inline float getLineHeight() const { return lineHeight; }
    void destroy();
private:
    uint32_t pixelSize;
    uint32_t texture;
    uint32_t codePointOfFirstChar;
    float lineHeight;
    std::vector<PackedChar> characters;
    std::vector<AlignedQuad> quads;
    bool loadFromFile(const std::string &filepath);
    bool loadFromMemory(const uint8_t *fontData);
    bool load(const uint8_t *data);
};

#endif