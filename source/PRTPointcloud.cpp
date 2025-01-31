#include "PRTPointcloud.h"

#include "CompileShader.h"
#include <GL/Assert.h>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shader
const char* vertexShaderSourcePointcloudRGB = R"(
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
const char* fragmentShaderSourcePointcloudRGB = R"(
#version 330 core
out vec4 FragColor;

in vec4 v_color;

void main() {
    FragColor = v_color;
}
)";

namespace PointcloudToolbox
{

    std::unordered_map<PointcloudDraw::PointcloudHandle, PointcloudDraw::PointcloudData> PointcloudDraw::m_pointclouds = {};


    void PointcloudDraw::CompileShaders()
    {
        m_pointcloudShaderProgram =
            PointcloudToolbox::Util::createShaderProgram(vertexShaderSourcePointcloudRGB, fragmentShaderSourcePointcloudRGB);
    }

    //! Copy the pointcloud data to the GPU.
    PointcloudDraw::PointcloudHandle PointcloudDraw::AcquirePointcloudHandle(glm::mat4 location, float * data, int count,  DataLayout layout)
    {
        assert(DataLayoutToSize.find(layout) != DataLayoutToSize.end());
        const auto layoutSize = DataLayoutToSize.at(layout);

        assert(DataLayoutToGeometrySize.find(layout) != DataLayoutToGeometrySize.end());
        const auto geometrySize = DataLayoutToGeometrySize.at(layout);

        PointcloudHandle handle = m_pointclouds.size();
        PointcloudData pointcloudData;
        pointcloudData.count = count;
        pointcloudData.vertices = std::vector<float>(data, data + count * layoutSize);
        pointcloudData.VAO = 0;
        pointcloudData.VBO = 0;
        pointcloudData.layout = layout;
        pointcloudData.sizePerVertex = layoutSize;

        GL_CALL(glGenVertexArrays(1, &pointcloudData.VAO));
        GL_CALL(glGenBuffers(1, &pointcloudData.VBO));

        GL_CALL(glBindVertexArray(pointcloudData.VAO));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, pointcloudData.VBO));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, pointcloudData.vertices.size() * pointcloudData.sizePerVertex,  pointcloudData.vertices.data(), GL_STATIC_DRAW));

        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  pointcloudData.sizePerVertex, (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  pointcloudData.sizePerVertex, (void*)(geometrySize));
        glEnableVertexAttribArray(1);

        m_pointclouds[handle] = pointcloudData;
        return handle;
    }

    void PointcloudDraw::IssueDrawCalls(const glm::mat4& view, const glm::mat4& projection)
    {
        m_stats = DrawBaseStats();
        for (const auto& [handle, pointcloudData] : m_pointclouds)
        {

            const auto vertexCount = pointcloudData.count;
            const auto vertexVBO = pointcloudData.VBO;
            const auto vertexVAO = pointcloudData.VAO;
            GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexVBO));
            GL_CALL(glUseProgram(m_pointcloudShaderProgram));
            // Set uniform matrices in the shader
            const GLint viewLoc = glGetUniformLocation(m_pointcloudShaderProgram, "view");
            CheckUniformLocation(viewLoc, "view");
            const GLint projectionLoc = glGetUniformLocation(m_pointcloudShaderProgram, "projection");
            CheckUniformLocation(projectionLoc, "projection");
            GL_CALL(glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)));
            GL_CALL(glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)));
            GL_CALL(glBindVertexArray(vertexVAO));
            GL_CALL(glDrawArrays(GL_POINTS, 0, vertexCount));
            m_stats.m_numberOfVertex += vertexCount;
            m_stats.m_numberOfDrawCalls += vertexCount > 0 ? 1 : 0;
        }

        //
        //
        //        // lines
        //        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexVBO));
        //        GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_drawVerticesLinesBuffer.size()*sizeof(float), m_drawVerticesLinesBuffer.data(),
        //        GL_DYNAMIC_DRAW)); GL_CALL(glUseProgram(m_shaderProgramLines));
        //        // Set uniform matrices in the shader
        //        const GLint viewLoc = glGetUniformLocation(m_shaderProgramLines, "view");
        //        CheckUniformLocation(viewLoc, "view");
        //        const GLint projectionLoc = glGetUniformLocation(m_shaderProgramLines, "projection");
        //        CheckUniformLocation(projectionLoc, "projection");
        //        GL_CALL(glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)));
        //        GL_CALL(glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)));
        //        GL_CALL(glBindVertexArray(m_lineVAO));
        //        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexVBO));
        //        GL_CALL(glDrawArrays(GL_LINES, 0, m_drawVerticesLinesCount));
        //        m_stats = DrawBaseStats();
        //        m_stats.m_numberOfVertex = m_drawVerticesLinesCount + m_drawPointsCount;
        //        m_stats.m_numberOfDrawCalls += m_drawVerticesLinesCount > 0 ? 1 : 0;
        //        m_stats.m_numberOfDrawCalls += m_drawPointsCount > 0 ? 1 : 0;
        //
        //
        //        // clear the buffer, to simulate a one-time draw
        //        m_drawVerticesLinesCount = 0;
        //        m_drawVerticesLinesBuffer.clear();
        //
        //        m_drawPointsCount = 0;
        //        m_drawVerticesPointsBuffer.clear();
    }

    DrawBaseStats PointcloudDraw::GetStatsForDrawCall()
    {
        return m_stats;
    }
} // namespace PointcloudToolbox
