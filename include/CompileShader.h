#pragma once
#include <GL/glew.h>
namespace PointcloudToolbox::Util
{
    //! Compile a shader from source code.
    //! \param shaderType The type of the shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.).
    //! \param shaderSource The source code of the shader.
    //! \return The shader ID or 0 if the shader could not be compiled, in which case an error message is printed to std::cerr.
    int compileShader(GLenum shaderType, const char* shaderSource);

    //! Create a shader program from vertex and fragment shader source code.
    //! \param vertexShaderSource The source code of the vertex shader.
    //! \param fragmentShaderSource The source code of the fragment shader.
    //! \return The shader program ID or 0 if the shader program could not be created, in which case an error message is printed to std::cerr.
    int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
} // namespace PointcloudToolbox::Util