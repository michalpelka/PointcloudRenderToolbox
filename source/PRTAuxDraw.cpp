#include "PRTAuxDraw.h"

#include "CompileShader.h"
#include <GL/Assert.h>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
// Vertex Shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 v_color;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    v_color = vec4(aColor,1.0);
}
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec4 v_color;

void main() {
    FragColor = v_color;
}
)";

const char* vertexShaderSourcePoints = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float fPointSize;

out vec4 v_color;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_PointSize = fPointSize;
    gl_Position = projection * view * vec4(aPos, 1.0);
    v_color = vec4(aColor,1.0);
}
)";

// Fragment Shader
const char* fragmentShaderSourcePoints = R"(
#version 330 core
out vec4 FragColor;

in vec4 v_color;

void main() {
    FragColor = v_color;
}
)";


// inifinite grid vs shader
const char* vertexShaderSourceGrid = R"(
#version 330 core

uniform mat4 view;
uniform mat4 projection;

uniform float gGridSize = 1000.0;
out vec4 worldPos;
const vec3 Pos[4] = vec3[4](
    vec3(-1.0, -1.0, 0.0),      // bottom left
    vec3( 1.0, -1.0, 0.0),      // bottom right
    vec3( 1.0, 1.0,  0.0),      // top right
    vec3(-1.0, 1.0,  0.0)       // top left
);
const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

mat4 viewNoTranslation = mat4(
        vec4(view[0][0], view[0][1], view[0][2], view[0][3]),
        vec4(view[1][0], view[1][1], view[1][2], view[1][3]),
        vec4(view[2][0], view[2][1], view[2][2], view[2][3]),
        vec4(0, 0, view[3][2], view[3][3]));

void main()
{
    int Index = Indices[gl_VertexID];
    vec3 vPos3 = Pos[Index] * gGridSize;
    vec4 vPos4 = vec4(vPos3, 1.0);
    worldPos = vPos4;

    gl_Position = projection * view *  vPos4;

}

)";

const char* fragmentShaderSourceGrid = R"(
#version 330 core
out vec4 FragColor;
in vec4 worldPos;

void main() {
    float i =0;
    const float gridSize = 1.f;
    float lx = worldPos.x - gridSize * round(worldPos.x*gridSize);
    float ly = worldPos.y - gridSize * round(worldPos.y*gridSize);

    float distanceToLineX = abs(lx);
    float distanceToLineY = abs(ly);
    float distanceToLine = min(distanceToLineX,distanceToLineY);
    i = mix(0,1.0,distanceToLine*50);


    FragColor = vec4(i,i,i,i);
}
)";

namespace PointcloudToolbox
{

    void AuxDraw::CompileShaders()
    {
        m_shaderProgramLines = PointcloudToolbox::Util::createShaderProgram(vertexShaderSource, fragmentShaderSource);
        m_shaderProgramPoints = PointcloudToolbox::Util::createShaderProgram(vertexShaderSourcePoints, fragmentShaderSourcePoints);
        m_shaderProgramGrid = PointcloudToolbox::Util::createShaderProgram(vertexShaderSourceGrid, fragmentShaderSourceGrid);

        // setup line drawing
        GL_CALL(glGenVertexArrays(1, &m_lineVAO));
        GL_CALL(glGenBuffers(1, &m_lineVertexVBO));

        GL_CALL(glBindVertexArray(m_lineVAO));

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexVBO));
        // Bind the vertex buffer object and copy the vertex data
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_drawVerticesLinesBuffer.size()*sizeof(float), m_drawVerticesLinesBuffer.data(), GL_DYNAMIC_DRAW));


        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // setup point drawing
        GL_CALL(glGenVertexArrays(1, &m_PointsVAO));
        GL_CALL(glGenBuffers(1, &m_PointsVertexVBO));

        GL_CALL(glBindVertexArray(m_PointsVAO));

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_PointsVertexVBO));
        // Bind the vertex buffer object and copy the vertex data
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_drawVerticesPointsBuffer.size()*sizeof(float), m_drawVerticesPointsBuffer.data(), GL_DYNAMIC_DRAW));

        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Point size attribute (location = 2)
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

    }

    void AuxDraw::DrawPoint(glm::vec3 point, glm::vec3 color, float size)
    {
        m_drawVerticesPointsBuffer.push_back(point.x);
        m_drawVerticesPointsBuffer.push_back(point.y);
        m_drawVerticesPointsBuffer.push_back(point.z);
        m_drawVerticesPointsBuffer.push_back(color.x);
        m_drawVerticesPointsBuffer.push_back(color.y);
        m_drawVerticesPointsBuffer.push_back(color.z);
        m_drawVerticesPointsBuffer.push_back(size);

        m_drawPointsCount += 1;
    }

    void AuxDraw::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color)
    {
        m_drawVerticesLinesBuffer.push_back(start.x);
        m_drawVerticesLinesBuffer.push_back(start.y);
        m_drawVerticesLinesBuffer.push_back(start.z);
        m_drawVerticesLinesBuffer.push_back(color.x);
        m_drawVerticesLinesBuffer.push_back(color.y);
        m_drawVerticesLinesBuffer.push_back(color.z);
        m_drawVerticesLinesBuffer.push_back(end.x);
        m_drawVerticesLinesBuffer.push_back(end.y);
        m_drawVerticesLinesBuffer.push_back(end.z);
        m_drawVerticesLinesBuffer.push_back(color.x);
        m_drawVerticesLinesBuffer.push_back(color.y);
        m_drawVerticesLinesBuffer.push_back(color.z);

        m_drawVerticesLinesCount += 2;
    }

    void AuxDraw::IssueDrawCalls(const glm::mat4& view, const glm::mat4& projection)
    {

        GL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));

        //// grid
        GL_CALL(glUseProgram(m_shaderProgramGrid));
        const GLint viewLocGrid = glGetUniformLocation(m_shaderProgramGrid, "view");
        CheckUniformLocation(viewLocGrid, "view");
        const GLint projectionLocGrid = glGetUniformLocation(m_shaderProgramGrid, "projection");
        CheckUniformLocation(projectionLocGrid, "projection");
        GL_CALL(glUniformMatrix4fv(viewLocGrid, 1, GL_FALSE, glm::value_ptr(view)));
        GL_CALL(glUniformMatrix4fv(projectionLocGrid, 1, GL_FALSE, glm::value_ptr(projection)));
        GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));


        //points
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_PointsVertexVBO));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_drawVerticesPointsBuffer.size()*sizeof(float), m_drawVerticesPointsBuffer.data(), GL_DYNAMIC_DRAW));
        GL_CALL(glUseProgram(m_shaderProgramPoints));
        // Set uniform matrices in the shader
        const GLint viewLocPoints = glGetUniformLocation(m_shaderProgramPoints, "view");
        CheckUniformLocation(viewLocPoints, "view");
        const GLint projectionLocPoints = glGetUniformLocation(m_shaderProgramPoints, "projection");
        CheckUniformLocation(projectionLocPoints, "projection");
        GL_CALL(glUniformMatrix4fv(viewLocPoints, 1, GL_FALSE, glm::value_ptr(view)));
        GL_CALL(glUniformMatrix4fv(projectionLocPoints, 1, GL_FALSE, glm::value_ptr(projection)));
        GL_CALL(glBindVertexArray(m_PointsVAO));
        GL_CALL(glDrawArrays(GL_POINTS, 0, m_drawPointsCount));


        // lines
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexVBO));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_drawVerticesLinesBuffer.size()*sizeof(float), m_drawVerticesLinesBuffer.data(), GL_DYNAMIC_DRAW));
        GL_CALL(glUseProgram(m_shaderProgramLines));
        // Set uniform matrices in the shader
        const GLint viewLoc = glGetUniformLocation(m_shaderProgramLines, "view");
        CheckUniformLocation(viewLoc, "view");
        const GLint projectionLoc = glGetUniformLocation(m_shaderProgramLines, "projection");
        CheckUniformLocation(projectionLoc, "projection");
        GL_CALL(glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)));
        GL_CALL(glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)));
        GL_CALL(glBindVertexArray(m_lineVAO));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexVBO));
        GL_CALL(glDrawArrays(GL_LINES, 0, m_drawVerticesLinesCount));
        m_stats = DrawBaseStats();
        m_stats.m_numberOfVertex = m_drawVerticesLinesCount + m_drawPointsCount;
        m_stats.m_numberOfDrawCalls += m_drawVerticesLinesCount > 0 ? 1 : 0;
        m_stats.m_numberOfDrawCalls += m_drawPointsCount > 0 ? 1 : 0;


        // clear the buffer, to simulate a one-time draw
        m_drawVerticesLinesCount = 0;
        m_drawVerticesLinesBuffer.clear();

        m_drawPointsCount = 0;
        m_drawVerticesPointsBuffer.clear();



    }

    DrawBaseStats AuxDraw::GetStatsForDrawCall()
    {
        return m_stats;
    }
} // namespace PointcloudToolbox
