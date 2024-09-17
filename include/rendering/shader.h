#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
#include <string>

enum class ShaderType {
    Vertex,
    Geometry,
    Fragment,
    Program
};

class Shader {
public:
    Shader();
    Shader(const std::string &vertexSource, const std::string &fragmentSource);
    uint32_t getId() const;
    void use();
    void destroy();
    int32_t getUniformLocation(const char *name);
    void setMat4(const char *name, bool transpose, const float *value);
    void setFloat(const char *name, float value);
    void setFloat2(const char *name, const float *value);
    void setFloat3(const char *name, const float *value);
    void setFloat4(const char *name, const float *value);
    void setInt(const char *name, int32_t value);

    void setMat4(int32_t location, bool transpose, const float *value);
    void setFloat(int32_t location, float value);
    void setFloat2(int32_t location, const float *value);
    void setFloat3(int32_t location, const float *value);
    void setFloat4(int32_t location, const float *value);
    void setInt(int32_t location, int32_t value);

private:
    uint32_t id;
    static uint32_t compile(const std::string &source, ShaderType shaderType);
    static bool check(uint32_t shader, ShaderType type, const std::string &source);
};

#endif