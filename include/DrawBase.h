#pragma once
#include <glm/glm.hpp>
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
};