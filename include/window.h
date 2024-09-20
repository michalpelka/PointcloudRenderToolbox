#pragma once
#include <string>
#include <functional>
#include <unordered_map>
namespace PointcloudToolbox
{
    using WindowHandle = int;
    constexpr WindowHandle InvalidWindowHandle = -1;

    struct WindowData{
        std::string title;
        int width;
        int height;
        std::function<void()> ImGuiRender;
    };
    namespace internal{
        static std::unordered_map<WindowHandle, WindowData> windows;
    }

    //! Create a new window and return its handle.
    WindowHandle CreateWindow(std::string title, int width, int height);

    //! Destroy the window with the given handle.
    void DestroyWindow(WindowHandle handle);

    //! Set the ImGui render callback for the window.
    void SetWindowImGuiRenderCallback(WindowHandle handle, std::function<void()> renderCallback);

}