#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
namespace PointcloudToolbox
{

    class AuxDraw
    {
    public:
        //! Draw line from start to end with color.
        //! @param start Start point of the line.
        //! @param end End point of the line.
        //! @param color Color of the line.
        void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);

        void IssueDrawCalls();
        void CompileShaders();


    private:
        std::vector<glm::vec3> m_drawVerticesLines;

        int m_shaderProgram{ 0 };
        GLuint m_lineVAO{ 0 };
        GLuint m_lineVertexVBO{ 0 };


    };

} // namespace PointcloudToolbox