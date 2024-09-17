#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include "font.h"
#include "vertex.h"
#include "shader.h"
#include "../external/glm.h"
#include <cstdint>
#include <string>
#include <vector>

struct FontRendererBuffer {
    uint32_t VAO;
    uint32_t VBO;
    uint32_t vertexIndex;
    Font *font;
    std::vector<Vertex2D> vertices;
};

enum FontShaderUniform {
    FontShaderUniform_Projection,
    FontShaderUniform_Texture,
    FontShaderUniform_COUNT
};

class FontRenderer {
public:
    FontRenderer();
    void initialize();
    void deinitialize();
    void newFrame();
    void addText(Font *font, const std::string &text, const glm::vec2 &position, const glm::vec4 &color, float size);
private:
    std::vector<FontRendererBuffer> buffers;
    Shader shader;
    uint32_t screenHeight;
    uint32_t screenWidth;
    glm::mat4 projectionMatrix;
    int32_t uniforms[FontShaderUniform_COUNT];
    FontRendererBuffer *getBuffer(Font *font);
    FontRendererBuffer *createBuffer(Font *font);
    void createProjectionMatrix();
    void createShader();
};

#endif