#include "CompileShader.h"
#include "GL/Assert.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int PointcloudToolbox::Util::compileShader(GLenum type, const char* source)
{
    int shader = glCreateShader(type);
    GL_CALL(glShaderSource(shader, 1, &source, nullptr));
    GL_CALL(glCompileShader(shader));

    int success;
    char infoLog[512];
    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GL_CALL(glGetShaderInfoLog(shader, 512, nullptr, infoLog));
        std::cerr << "Shader Compilation Failed: " << infoLog << std::endl;
    }
    return shader;
}

int PointcloudToolbox::Util::createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    int shaderProgram = glCreateProgram();
    GL_CALL(glAttachShader(shaderProgram, vertexShader));
    GL_CALL(glAttachShader(shaderProgram, fragmentShader));
    GL_CALL(glLinkProgram(shaderProgram));

    int success;
    char infoLog[512];
    GL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
    if (!success)
    {
        GL_CALL(glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog));
        std::cerr << "Shader Linking Failed: " << infoLog << std::endl;
    }

    GL_CALL(glDeleteShader(vertexShader));
    GL_CALL(glDeleteShader(fragmentShader));

    return shaderProgram;
}