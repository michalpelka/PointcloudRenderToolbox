#include "window.h"

namespace PointcloudToolbox
{


    WindowHandle CreateWindow(std::string title, int width, int height)
    {
        WindowHandle nextHandle  = internal::windows.size()+1;
        internal::windows[nextHandle] = {.title = title, .width = width, .height = height};
        return nextHandle;
    }

    void DestroyWindow(WindowHandle handle)
    {
        internal::windows.erase(handle);
    }

    void SetWindowImGuiRenderCallback(WindowHandle handle, std::function<void()> renderCallback)
    {
        internal::windows[handle].ImGuiRender = renderCallback;
    }
}