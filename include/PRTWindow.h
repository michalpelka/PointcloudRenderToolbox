#pragma once
#include "PRTAuxDraw.h"
#include <functional>
#include <imgui.h>
#include <string>
#include <unordered_map>
namespace PointcloudToolbox
{

    using WindowHandle = int;
    constexpr WindowHandle InvalidWindowHandle = -1;

    struct WindowData
    {
        std::string title;
        int width;
        int height;
        std::function<void()> ImGuiRender;
        void* NativeWindowPointer;
        ImGuiContext* cntx_imGui;
        bool visible;
        bool inFocus;
        AuxDraw auxDraw;
    };

    namespace internal
    {
        static std::unordered_map<WindowHandle, WindowData> windows;
        static WindowHandle nextHandle = 0;
        static bool initialized = false;
    } // namespace internal

    //! Create a new window and return its handle.
    WindowHandle CreateWindow(std::string title, int width, int height);

    //! Destroy the window with the given handle.
    void DestroyWindow(WindowHandle handle);

    //! Set the ImGui render callback for the window.
    void SetWindowImGuiRenderCallback(WindowHandle handle, std::function<void()> renderCallback);

    void SetViewMatrix(WindowHandle handle, const std::array<float, 16> viewMatrix);

    void DestroyWindow(WindowHandle handle);
    void ShowWindow(WindowHandle handle);
    void HideWindow(WindowHandle handle);
    bool IsWindowVisible(WindowHandle handle);

    void InitWindowSystem();
    void DestroyWindowSystem();
    //! Render windows.
    void RenderWindow(const WindowHandle handle);

    AuxDraw& GetAuxDraw(const WindowHandle handle);

} // namespace PointcloudToolbox