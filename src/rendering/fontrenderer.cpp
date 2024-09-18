#include "fontrenderer.h"
#include "../core/screen.h"
#include "../external/glad.h"
#include <iostream>

constexpr uint32_t FONT_BUFFER_MAX_VERTICES = 2 << 15;

FontRenderer::FontRenderer() {
    screenWidth = 1;
    screenHeight = 1;
}

void FontRenderer::initialize() {
    createShader();
}

void FontRenderer::deinitialize() {
    for(auto &buffer : buffers) {
        if(buffer.VAO > 0) {
            glDeleteVertexArrays(1, &buffer.VAO);
        }
        if(buffer.VBO > 0) {
            glDeleteBuffers(1, &buffer.VBO);
        }
    }

    buffers.clear();
    shader.destroy();
}

void FontRenderer::newFrame() {
    if(buffers.size() == 0)
        return;

    GLint oldSrcBlend, oldDstBlend, oldBlendEquation;
    GLboolean oldIsBlendEnabled = glIsEnabled(GL_BLEND);
    GLboolean oldIsDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);

    glGetIntegerv(GL_BLEND_SRC, &oldSrcBlend);
    glGetIntegerv(GL_BLEND_DST, &oldDstBlend);
    glGetIntegerv(GL_BLEND_EQUATION_RGB, &oldBlendEquation);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    createProjectionMatrix();
    
    shader.use();

    glActiveTexture(GL_TEXTURE0);

    for(size_t i = 0; i < buffers.size(); i++) {
        if(buffers[i].vertexIndex == 0)
            continue;
        
        glBindTexture(GL_TEXTURE_2D, buffers[i].font->getTexture());

        shader.setMat4(uniforms[FontShaderUniform_Projection], false, glm::value_ptr(projectionMatrix));
        shader.setInt(uniforms[FontShaderUniform_Texture], 0);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[i].VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, buffers[i].vertexIndex * sizeof(Vertex2D), buffers[i].vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(buffers[i].VAO);
        glDrawArrays(GL_TRIANGLES, 0, buffers[i].vertexIndex);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        buffers[i].vertexIndex = 0;
    }

    glBlendFunc(oldSrcBlend, oldDstBlend);
    glBlendEquation(oldBlendEquation);
    
    if (oldIsBlendEnabled) 
        glEnable(GL_BLEND); 
    else 
        glDisable(GL_BLEND);

    if(oldIsDepthTestEnabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void FontRenderer::addText(Font *font, const std::string &text, const glm::vec2 &position, const glm::vec4 &color, float size) {
    if(!font)
        return;
    if(text.size() == 0 || text.size() > FONT_BUFFER_MAX_VERTICES)
        return;

    //Gets a buffer that already is using this font, or creates a new one if it wasn't found
    FontRendererBuffer *buffer = getBuffer(font);
    
    if(!buffer)
        return;

    //Check if the buffer has enough capacity for the new vertices, if not, create a new one
    if((buffer->vertexIndex + (text.size() * 6)) >= FONT_BUFFER_MAX_VERTICES) {
        buffer = createBuffer(font);
        if(!buffer)
            return;
    }

    if(size <= 0.0f)
        size = 1.0f;

    size = size / font->getPixelSize();

    int order[6] = { 0, 1, 2, 0, 2, 3 };
    const uint32_t codePointOfFirstChar = buffer->font->getCodePointOfFirstChar();

    auto &packedChars = font->getCharacters();
    auto &alignedQuads = font->getQuads();

    glm::vec2 pos = position;
    
    //Compute the max height of the first line in order to position the text properly
    pos.y += font->computeLineHeight(text) * size;

    //Store the original X position so it can be used when adding a new line
    float posX = position.x;

    for(char ch : text) {
        if(ch == '\n') {
            pos.x = posX;
            pos.y += font->getLineHeight() * size;
            continue;
        }

        int glyphIndex = (ch - codePointOfFirstChar);

        //Skip unknown glyphs
        if(glyphIndex < 0 || glyphIndex >= font->getNumberOfCharacters())
            continue;

        // Retrieve the data that is used to render a glyph of charecter 'ch'
        auto packedChar = &packedChars[glyphIndex]; 
        auto alignedQuad = &alignedQuads[glyphIndex];

        // The units of the fields of the above structs are in pixels, 
        // convert them to a unit of what we want be multilplying to pixelScale  
        glm::vec2 glyphSize = {
            (packedChar->x1 - packedChar->x0) * size,
            (packedChar->y1 - packedChar->y0) * size
        };

        glm::vec2 glyphBoundingBoxBottomLeft = {
            pos.x + (packedChar->xoff * size),
            pos.y + (packedChar->yoff + packedChar->y1 - packedChar->y0) * size
        };

        // The order of vertices of a quad goes top-right, top-left, bottom-left, bottom-right
        glm::vec2 glyphVertices[4] = {
            { glyphBoundingBoxBottomLeft.x + glyphSize.x, glyphBoundingBoxBottomLeft.y - glyphSize.y },
            { glyphBoundingBoxBottomLeft.x, glyphBoundingBoxBottomLeft.y - glyphSize.y },
            { glyphBoundingBoxBottomLeft.x, glyphBoundingBoxBottomLeft.y },
            { glyphBoundingBoxBottomLeft.x + glyphSize.x, glyphBoundingBoxBottomLeft.y },
        };

        glm::vec2 glyphTextureCoords[4] = {
            { alignedQuad->s1, alignedQuad->t0 },
            { alignedQuad->s0, alignedQuad->t0 },
            { alignedQuad->s0, alignedQuad->t1 },
            { alignedQuad->s1, alignedQuad->t1 },
        };

        // We need to fill the vertex buffer by 6 vertices to render a quad as we are rendering a quad as 2 triangles
        // The order used is in the 'order' array
        // order = [0, 1, 2, 0, 2, 3] is meant to represent 2 triangles: 
        // one by glyphVertices[0], glyphVertices[1], glyphVertices[2] and one by glyphVertices[0], glyphVertices[2], glyphVertices[3]
        for(int i = 0; i < 6; i++) {
            buffer->vertices[buffer->vertexIndex + i].position = glyphVertices[order[i]];
            buffer->vertices[buffer->vertexIndex + i].color = color;
            buffer->vertices[buffer->vertexIndex + i].uv = glyphTextureCoords[order[i]];
        }

        buffer->vertexIndex += 6;

        // Update the position to render the next glyph specified by packedChar->xadvance.
        pos.x += packedChar->xadvance * size;
    }
}

FontRendererBuffer *FontRenderer::getBuffer(Font *font) {
    if(!font)
        return nullptr;

    for(size_t i = 0; i < buffers.size(); i++) {
        if(buffers[i].font == font) {
            return &buffers[i];
        }
    }

    return createBuffer(font);
}

FontRendererBuffer *FontRenderer::createBuffer(Font *font) {
    if(!font)
        return nullptr;
    
    buffers.push_back(FontRendererBuffer());
    size_t index = buffers.size() - 1;
    buffers[index].font = font;
    buffers[index].vertexIndex = 0;
    buffers[index].VAO = 0;
    buffers[index].VBO = 0;
    buffers[index].vertices.resize(FONT_BUFFER_MAX_VERTICES);

    glGenVertexArrays(1, &buffers[index].VAO);
    glBindVertexArray(buffers[index].VAO);
    glObjectLabel(GL_VERTEX_ARRAY, buffers[index].VAO, -1, "FontRendererVAO");

    glGenBuffers(1, &buffers[index].VBO);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[index].VBO);
    glObjectLabel(GL_BUFFER, buffers[index].VBO, -1, "FontRendererVBO");

    glBufferData(GL_ARRAY_BUFFER, FONT_BUFFER_MAX_VERTICES * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, position));
    glEnableVertexAttribArray(0);

    // UV coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, uv));
    glEnableVertexAttribArray(1);

    // Color coordinate attribute
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, color));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return &buffers[index];
}

void FontRenderer::createProjectionMatrix() {
    uint32_t width = Screen::getInstance()->getWidth();
    uint32_t height = Screen::getInstance()->getHeight();

    if(screenWidth != width || screenHeight != height) {
        screenWidth = width;
        screenHeight = height;
        projectionMatrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    }
}

void FontRenderer::createShader() {
    std::string vertex =
R"(#version 330 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoordinate;
layout (location = 2) in vec4 aColor;

uniform mat4 uProjection;

out vec2 oTexCoords;
out vec4 oColor;

void main() {
    gl_Position = uProjection * vec4(aPosition.xy, 0.0, 1.0);
    oTexCoords = aTexCoordinate.xy;
    oColor = aColor;
})";

    std::string fragment =
R"(#version 330 core
uniform sampler2D uTexture;

in vec2 oTexCoords;
in vec4 oColor;
out vec4 color;

vec4 GammaCorrection(vec4 color) {
    return vec4(pow(color.rgb, vec3(1.0/2.2)), 1.0);
}

void main() {
    vec4 sampled = texture(uTexture, oTexCoords);

    if(sampled.r == 0.0)
        discard;

    vec4 outColor = vec4(oColor.rgb, 1.0) * sampled.r;
    
    color = GammaCorrection(outColor);
    color.a = oColor.a;

    // Looks better under some circumstances but probably works best with SDF fonts
    // float d = texture(uTexture, oTexCoords).r;
    // float aaf = fwidth(d);
    // float alpha = smoothstep(0.5 - aaf, 0.5 + aaf, d);
    // gl_FragColor = vec4(oColor.rgb, alpha) * oColor;
})";

    shader = Shader(vertex, fragment);

    uniforms[FontShaderUniform_Projection] = glGetUniformLocation(shader.getId(), "uProjection");
    uniforms[FontShaderUniform_Texture] = glGetUniformLocation(shader.getId(), "uTexture");
}