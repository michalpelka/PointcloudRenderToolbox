#pragma once
#include "DrawBase.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
namespace PointcloudToolbox
{

    class AuxDraw : public DrawBase
    {
    public:
        //! Draw line from start to end with color.
        //! @param start Start point of the line.
        //! @param end End point of the line.
        //! @param color Color of the line.
        void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);

        void DrawPoint(glm::vec3 point, glm::vec3 color, float size);
        // DrawBase overrides ...
        void IssueDrawCalls(const glm::mat4& view, const glm::mat4& projection) override;
        void CompileShaders() override;
        DrawBaseStats GetStatsForDrawCall() override;

    private:


        int m_shaderProgramLines{ 0 };
        int m_shaderProgramPoints{ 0 };
        int m_shaderProgramGrid{ 0 };
        // line drawing
        std::vector<float> m_drawVerticesLinesBuffer;
        int m_drawVerticesLinesCount{ 0 };
        GLuint m_lineVAO{ 0 };
        GLuint m_lineVertexVBO{ 0 };

        std::vector<float> m_drawVerticesPointsBuffer;
        int m_drawPointsCount{ 0 };
        GLuint m_PointsVAO{ 0 };
        GLuint m_PointsVertexVBO{ 0 };


    private:
        DrawBaseStats m_stats;
    };

} // namespace PointcloudToolbox