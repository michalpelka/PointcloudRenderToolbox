#pragma once
#include "DrawBase.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <unordered_map>
namespace PointcloudToolbox
{
    class PointcloudDraw : public DrawBase
    {
    public:
        using PointcloudHandle = int;
        static constexpr PointcloudHandle InvalidPointcloudHandle = -1;

        //clang-format off
        enum class DataLayout : uint32_t {
            DATALAYOUT_XYZFLOAT_RGBFLOAT = 0, // 24 bytes per vertex
            DATALAYOUT_COUNT
        };


        const std::unordered_map<DataLayout, size_t>
        DataLayoutToSize = {
            {DataLayout::DATALAYOUT_XYZFLOAT_RGBFLOAT, 24}
        };

        const std::unordered_map<DataLayout, size_t>
        DataLayoutToGeometrySize = {
            {DataLayout::DATALAYOUT_XYZFLOAT_RGBFLOAT, 3*sizeof (float)}
        };

        //clang-format on

        private:
            struct PointcloudData{
                std::vector<float> vertices;
                int count;
                GLuint VAO;
                GLuint VBO;
                DataLayout layout;
                size_t sizePerVertex;
                size_t sizeOfGeometryData;
            };
            static std::unordered_map<PointcloudHandle, PointcloudData> m_pointclouds;
            int m_pointcloudShaderProgram{ 0 };

        public:

            //! Copy the pointcloud data to the GPU.
            PointcloudHandle AcquirePointcloudHandle(glm::mat4 location, float * data, int count, DataLayout layout);
            // DrawBase overrides ...
            void IssueDrawCalls(const glm::mat4& view, const glm::mat4& projection) override;
            void CompileShaders() override;
            DrawBaseStats GetStatsForDrawCall() override;
        private:
            DrawBaseStats m_stats;
    };
}