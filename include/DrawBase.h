#pragma once
#include <glm/glm.hpp>

struct DrawBaseStats
{
    uint32_t m_numberOfDrawCalls{ 0 };
    uint32_t m_numberOfVertex{ 0 };

    DrawBaseStats& operator+=(const DrawBaseStats& other)
    {
        this->m_numberOfDrawCalls += other.m_numberOfDrawCalls;
        this->m_numberOfVertex += other.m_numberOfVertex;
        return *this;
    }
};

class DrawBase
{
public:
    //! Initialize all shaders, buffers, etc.
    //! This function should be called once before any other function.
    //! This function should prepare all shaders, buffers, etc.
    virtual void CompileShaders() = 0;

    //! Issue draw calls for all primitives.
    //! This function should be called once per frame.
    //! @param view View matrix.
    //! @param projection Projection matrix.
    virtual void IssueDrawCalls(const glm::mat4& view, const glm::mat4& projection) = 0;

    //! Returns statistics for last executed DrawCall
    virtual DrawBaseStats GetStatsForDrawCall()
    {
        return DrawBaseStats();
    };
};