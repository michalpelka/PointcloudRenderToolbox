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