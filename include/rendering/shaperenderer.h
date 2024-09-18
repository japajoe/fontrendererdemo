#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H

#include "vertex.h"
#include "shader.h"
#include <cstdint>
#include <vector>

enum class ShapeType {
    Circle,
    Rectangle
};

struct ShapeRendererBuffer {
    uint32_t VAO;
    uint32_t VBO;
    uint32_t vertexIndex;
    std::vector<Vertex2D> vertices;
    ShapeType shapeType;
};

enum ShapeShaderUniform {
    ShapeShaderUniform_Projection,
    ShapeShaderUniform_Texture,
    ShapeShaderUniform_COUNT
};

class ShapeRenderer {
public:
    ShapeRenderer();
    void initialize();
    void deinitialize();
    void addRectangle(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
    void addCircle(const glm::vec2 &position, float radius, const glm::vec4 &color);
    void newFrame();
private:
    std::vector<ShapeRendererBuffer> buffers;
    Shader shader;
    uint32_t screenHeight;
    uint32_t screenWidth;
    glm::mat4 projectionMatrix;
    int32_t uniforms[ShapeShaderUniform_COUNT];
    ShapeRendererBuffer *getBuffer(ShapeType shapeType);
    ShapeRendererBuffer *createBuffer(ShapeType shapeType);
    void createShader();
    void createProjectionMatrix();
    void addVertices(const Vertex2D *vertices, size_t count, ShapeType shapeType);
};

#endif