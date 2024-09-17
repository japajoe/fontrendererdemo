#include "font.h"
#include "../external/glad.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#define STB_TRUETYPE_IMPLEMENTATION
#include "../external/stb_truetype.h"

Font::Font() {
    texture = 0;
    pixelSize = 14;
}

Font::Font(const std::string &filepath, uint32_t pixelSize) {
    this->pixelSize = pixelSize;
    texture = 0;
    load(filepath);
}

void Font::destroy() {
    if(texture > 0) {
        glDeleteTextures(1, &texture);
        texture = 0;
    }
}

bool Font::load(const std::string &filepath) {
    std::ifstream inputStream(filepath.c_str(), std::ios::binary | std::ios::ate);
    if (!inputStream) {
        std::cerr << "Failed to open font file." << std::endl;
        return false;
    }

    std::streamsize fontFileSize = inputStream.tellg();
    inputStream.seekg(0, std::ios::beg);

    std::vector<uint8_t> fontDataBuf;
    fontDataBuf.resize(fontFileSize);

    inputStream.read((char*)fontDataBuf.data(), fontFileSize);

    stbtt_fontinfo fontInfo = {};

    uint32_t fontCount = stbtt_GetNumberOfFonts(fontDataBuf.data());
    std::cout << "Font File: " << filepath << " has " << fontCount << " fonts" << std::endl;

    if(!stbtt_InitFont(&fontInfo, fontDataBuf.data(), 0)) {
        std::cerr << "stbtt_InitFont() Failed!" << std::endl;
        return false;
    }

    const uint32_t fontAtlasWidth = 512;
    const uint32_t fontAtlasHeight = 512;
    codePointOfFirstChar = 32;                     // ASCII of ' '(Space)
    const uint32_t charsToIncludeInFontAtlas = 95; // Include 95 charecters

    std::vector<uint8_t> fontAtlasTextureData;
    std::vector<stbtt_packedchar> packedChars;
    std::vector<stbtt_aligned_quad> alignedQuads;

    fontAtlasTextureData.resize(fontAtlasWidth * fontAtlasHeight);
    packedChars.resize(charsToIncludeInFontAtlas);
    alignedQuads.resize(charsToIncludeInFontAtlas);
    characters.resize(charsToIncludeInFontAtlas);
    quads.resize(charsToIncludeInFontAtlas);

    float fontSize = (float)pixelSize;
    
    stbtt_pack_context ctx;

    stbtt_PackBegin(
        &ctx,                                     // stbtt_pack_context (this call will initialize it) 
        fontAtlasTextureData.data(),              // Font Atlas texture data
        fontAtlasWidth,                           // Width of the font atlas texture
        fontAtlasHeight,                          // Height of the font atlas texture
        0,                                        // Stride in bytes
        1,                                        // Padding between the glyphs
        nullptr);

    stbtt_PackFontRange(
        &ctx,                                     // stbtt_pack_context
        fontDataBuf.data(),                       // Font Atlas texture data
        0,                                        // Font Index                                 
        fontSize,                                 // Size of font in pixels. (Use STBTT_POINT_SIZE(fontSize) to use points) 
        codePointOfFirstChar,                     // Code point of the first charecter
        charsToIncludeInFontAtlas,                // No. of charecters to be included in the font atlas 
        packedChars.data()                        // stbtt_packedchar array, this struct will contain the data to render a glyph
    );

    stbtt_PackEnd(&ctx);

    for (int i = 0; i < charsToIncludeInFontAtlas; i++) {
        float unusedX, unusedY;

        stbtt_GetPackedQuad(
            packedChars.data(),                  // Array of stbtt_packedchar
            fontAtlasWidth,                      // Width of the font atlas texture
            fontAtlasHeight,                     // Height of the font atlas texture
            i,                                   // Index of the glyph
            &unusedX, &unusedY,                  // current position of the glyph in screen pixel coordinates, (not required as we have a different corrdinate system)
            &alignedQuads[i],                    // stbtt_alligned_quad struct. (this struct mainly consists of the texture coordinates)
            0                                    // Allign X and Y position to a integer (doesn't matter because we are not using 'unusedX' and 'unusedY')
        );
    }

    memcpy(characters.data(), packedChars.data(), packedChars.size() * sizeof(stbtt_packedchar));
    memcpy(quads.data(), alignedQuads.data(), alignedQuads.size() * sizeof(stbtt_aligned_quad));

    lineHeight = 0.0f;

    // Estimate line height based on the maximum height of all glyphs
    for (const auto &glyph : characters) {
        float glyphHeight = (glyph.y1 - glyph.y0);
        if (glyphHeight > lineHeight) {
            lineHeight = glyphHeight;
        }
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // The given texture data is a single channel 1 byte per pixel data 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, fontAtlasWidth, fontAtlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, fontAtlasTextureData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}