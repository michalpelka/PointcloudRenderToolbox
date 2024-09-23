#include "PRTAuxDraw.h"

#include "CompileShader.h"
#include <GL/Assert.h>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
// Vertex Shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

namespace PointcloudToolbox
{
    float vertices[] = {
        0.0f,  0.5f, 0.0f,   // Top vertex
        -0.5f, -0.5f, 0.0f,   // Bottom left vertex
        0.5f, -0.5f, 0.0f    // Bottom right vertex
    };

    void AuxDraw::CompileShaders()
    {
        // create coordinate system
//        DrawLine(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0));
//        DrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
//        DrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1));

        m_drawVerticesLines.push_back(glm::vec3(0.0f,  0.5f, 0.0f));
        m_drawVerticesLines.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
        m_drawVerticesLines.push_back(glm::vec3( 0.5f, -0.5f, 0.0f ));
        int vertexShader = Util::compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        int fragmentShader = Util::compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
        m_shaderProgram = glCreateProgram();
        GL_CALL(glAttachShader(m_shaderProgram, vertexShader));
        GL_CALL(glAttachShader(m_shaderProgram, fragmentShader));
        GL_CALL(glLinkProgram(m_shaderProgram));
        int success;
        char infoLog[512];
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
            std::cerr << "Shader Linking Failed: " << infoLog << std::endl;
        }
        GL_CALL(glDeleteShader(vertexShader));
        GL_CALL(glDeleteShader(fragmentShader));

        GL_CALL(glGenVertexArrays(1, &m_lineVAO));
        GL_CALL(glGenBuffers(1, &m_lineVertexVBO));

        GL_CALL(glBindVertexArray(m_lineVAO));

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexVBO));
        // Bind the vertex buffer object and copy the vertex data
        std::cout << "Size of m_drawVerticesLines: " << sizeof(glm::vec3) << std::endl;
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));


        // Define the vertex attributes (position in this case)
        GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
        GL_CALL(glEnableVertexAttribArray(0));

    }
    void AuxDraw::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color)
    {
        m_drawVerticesLines.push_back(start);
        m_drawVerticesLines.push_back(color);
        m_drawVerticesLines.push_back(end);
        m_drawVerticesLines.push_back(color);
    }

    void AuxDraw::IssueDrawCalls()
    {
        //GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_drawVerticesLines.size(), m_drawVerticesLines.data(),GL_DYNAMIC_DRAW));

        //set uniforms
        // View and Projection matrices
        glm::mat4 view = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,0,1));
        glm::mat4 projection = glm::perspective(glm::radians(90.f), 800.0f / 600.0f, 0.1f, 100.0f);


        // Set uniform matrices in the shader
//        int viewLoc = glGetUniformLocation(m_shaderProgram, "view");
//        int projectionLoc = glGetUniformLocation(m_shaderProgram, "projection");
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


        GL_CALL(glUseProgram(m_shaderProgram));
        GL_CALL(glBindVertexArray(m_lineVAO));
        GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));

    }
} // namespace PointcloudToolbox
