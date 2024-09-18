#include "shaperenderer.h"
#include "../core/screen.h"
#include "../external/glad.h"
#include <cstring>

constexpr uint32_t SHAPE_BUFFER_MAX_VERTICES = 2 << 15;

ShapeRenderer::ShapeRenderer() {
    screenWidth = 1;
    screenHeight = 1;
}

void ShapeRenderer::initialize() {
    createShader();
}

void ShapeRenderer::deinitialize() {
    for(auto &buffer : buffers) {
        if(buffer.VAO > 0) {
            glDeleteVertexArrays(1, &buffer.VAO);
        }
        if(buffer.VBO > 0) {
            glDeleteBuffers(1, &buffer.VBO);
        }
    }

    buffers.clear();
}

void ShapeRenderer::addRectangle(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 pos = position + (size * 0.5f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    Vertex2D vertices[] = {
        // Triangle 1
        { glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 0.0f), color }, // Bottom-left
        { glm::vec2(-0.5f,  0.5f), glm::vec2(0.0f, 1.0f), color }, // Top-left
        { glm::vec2( 0.5f, -0.5f), glm::vec2(1.0f, 0.0f), color }, // Bottom-right
        
        // Triangle 2
        { glm::vec2( 0.5f, -0.5f), glm::vec2(1.0f, 0.0f), color }, // Bottom-right
        { glm::vec2(-0.5f,  0.5f), glm::vec2(0.0f, 1.0f), color }, // Top-left
        { glm::vec2( 0.5f,  0.5f), glm::vec2(1.0f, 1.0f), color }  // Top-right
    };

    for(size_t i = 0; i < 6; i++) {
        glm::vec4 worldPos = model * glm::vec4(vertices[i].position, 0.0f, 1.0f);
        vertices[i].position = glm::vec2(worldPos.x, worldPos.y);
    }
    
    addVertices(vertices, 6, ShapeType::Rectangle);
}

void ShapeRenderer::addCircle(const glm::vec2 &position, float radius, const glm::vec4 &color) {

}

void ShapeRenderer::newFrame() {
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

    for(size_t i = 0; i < buffers.size(); i++) {
        if(buffers[i].vertexIndex == 0)
            continue;
        
        shader.setMat4(uniforms[ShapeShaderUniform_Projection], false, glm::value_ptr(projectionMatrix));

        glBindBuffer(GL_ARRAY_BUFFER, buffers[i].VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, buffers[i].vertexIndex * sizeof(Vertex2D), buffers[i].vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(buffers[i].VAO);
        glDrawArrays(GL_TRIANGLES, 0, buffers[i].vertexIndex);
        glBindVertexArray(0);

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

ShapeRendererBuffer *ShapeRenderer::getBuffer(ShapeType shapeType) {
    for(size_t i = 0; i < buffers.size(); i++) {
        if(buffers[i].shapeType == shapeType) {
            return &buffers[i];
        }
    }

    return createBuffer(shapeType);
}

ShapeRendererBuffer *ShapeRenderer::createBuffer(ShapeType shapeType) {
    buffers.push_back(ShapeRendererBuffer());
    size_t index = buffers.size() - 1;
    buffers[index].shapeType = shapeType;
    buffers[index].vertexIndex = 0;
    buffers[index].VAO = 0;
    buffers[index].VBO = 0;
    buffers[index].vertices.resize(SHAPE_BUFFER_MAX_VERTICES);

    glGenVertexArrays(1, &buffers[index].VAO);
    glBindVertexArray(buffers[index].VAO);
    glObjectLabel(GL_VERTEX_ARRAY, buffers[index].VAO, -1, "ShapeRendererVAO");

    glGenBuffers(1, &buffers[index].VBO);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[index].VBO);
    glObjectLabel(GL_BUFFER, buffers[index].VBO, -1, "ShapeRendererVBO");

    glBufferData(GL_ARRAY_BUFFER, SHAPE_BUFFER_MAX_VERTICES * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);
    
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

void ShapeRenderer::createShader() {
    std::string vertexSource =
R"(#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;

uniform mat4 uProjection;

out vec2 oTexCoord;
out vec4 oColor;

void main()
{
    gl_Position = uProjection * vec4(aPosition, 0.0, 1.0);
    oTexCoord = aTexCoord;
    oColor = aColor;
})";

    std::string fragmentSource =
R"(#version 330 core

in vec2 oTexCoord;
in vec4 oColor;

out vec4 FragColor;

void main()
{
    FragColor = oColor;
})";

    shader = Shader(vertexSource, fragmentSource);
    uniforms[ShapeShaderUniform_Projection] = shader.getUniformLocation("uProjection");
}

void ShapeRenderer::createProjectionMatrix() {
    uint32_t width = Screen::getInstance()->getWidth();
    uint32_t height = Screen::getInstance()->getHeight();

    if(screenWidth != width || screenHeight != height) {
        screenWidth = width;
        screenHeight = height;
        projectionMatrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    }
}

void ShapeRenderer::addVertices(const Vertex2D *vertices, size_t count, ShapeType shapeType) {
    //Gets a buffer that already is using this shape, or creates a new one if it wasn't found
    ShapeRendererBuffer *buffer = getBuffer(shapeType);
    
    if(!buffer)
        return;

    //Check if the buffer has enough capacity for the new vertices, if not, create a new one
    if((buffer->vertexIndex + count) >= SHAPE_BUFFER_MAX_VERTICES) {
        buffer = createBuffer(shapeType);
        if(!buffer)
            return;
    }

    memcpy(&buffer->vertices[buffer->vertexIndex], vertices, count * sizeof(Vertex2D));

    buffer->vertexIndex += count;
}