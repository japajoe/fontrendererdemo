#include "shader.h"
#include "../external/glad.h"
#include <iostream>

Shader::Shader() {
    id = 0;
}

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) {
    id = 0;

    uint32_t vertexShader = compile(vertexSource, ShaderType::Vertex);
    
    if(!check(vertexShader, ShaderType::Vertex, vertexSource)) {
        glDeleteShader(vertexShader);
        return;
    }

    uint32_t fragmentShader = compile(fragmentSource, ShaderType::Fragment);
    
    if(!check(fragmentShader, ShaderType::Fragment, fragmentSource)) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    id = glCreateProgram();

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    
    if(!check(id, ShaderType::Program, fragmentSource)) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(id);
        id = 0;
    } else {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
}

uint32_t Shader::getId() const {
    return id;
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::destroy() {
    if(id > 0) {
        glDeleteShader(id);
        id = 0;
    }
}

int32_t Shader::getUniformLocation(const char *name) {
    return glGetUniformLocation(id, name);
}

void Shader::setMat4(const char *name, bool transpose, const float *value) {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, transpose, value);
}

void Shader::setFloat(const char *name, float value) {
    glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::setFloat2(const char *name, const float *value) {
    glUniform2fv(glGetUniformLocation(id, name), 1, value);
}

void Shader::setFloat3(const char *name, const float *value) {
    glUniform3fv(glGetUniformLocation(id, name), 1, value);
}

void Shader::setFloat4(const char *name, const float *value) {
    glUniform4fv(glGetUniformLocation(id, name), 1, value);
}

void Shader::setInt(const char *name, int32_t value) {
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::setMat4(int32_t location, bool transpose, const float *value) {
    glUniformMatrix4fv(location, 1, transpose, value);
}

void Shader::setFloat(int32_t location, float value) {
    glUniform1f(location, value);
}

void Shader::setFloat2(int32_t location, const float *value) {
    glUniform2fv(location, 1, value);
}

void Shader::setFloat3(int32_t location, const float *value) {
    glUniform3fv(location, 1, value);
}

void Shader::setFloat4(int32_t location, const float *value) {
    glUniform4fv(location, 1, value);
}

void Shader::setInt(int32_t location, int32_t value) {
    glUniform1i(location, value);
}

uint32_t Shader::compile(const std::string &source, ShaderType shaderType) {
    GLenum type = GL_VERTEX_SHADER;
    switch(shaderType) {
        case ShaderType::Vertex:
            type = GL_VERTEX_SHADER;
            break;
        case ShaderType::Geometry:
            type = GL_GEOMETRY_SHADER;
            break;
        case ShaderType::Fragment:
            type = GL_FRAGMENT_SHADER;
            break;
        default:
            return 0;
    }

    uint32_t shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    return shader;
}

bool Shader::check(uint32_t shader, ShaderType type, const std::string &source) {
    int32_t success;
    GLchar infoLog[1024];

    if (type == ShaderType::Vertex || type == ShaderType::Fragment || type == ShaderType::Geometry) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            
            std::string shaderType;

            switch(type) {
                case ShaderType::Vertex:
                    shaderType = "VERTEX";
                    break;
                case ShaderType::Geometry:
                    shaderType = "GEOMETRY";
                    break;
                case ShaderType::Fragment:
                    shaderType = "FRAGMENT";
                    break;
            }
            
            std::cerr << "------------------------\n";
            std::cerr << "ERROR: SHADER_COMPILATION_ERROR of type: " << shaderType << " " << infoLog << '\n';
            
            return false;
        }
    } else if (type == ShaderType::Program) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "------------------------\n";
            std::cerr << "ERROR: PROGRAM_LINKING_ERROR of type: PROGRAM " << infoLog << '\n';
            return false;
        }
    }

    return true;
}