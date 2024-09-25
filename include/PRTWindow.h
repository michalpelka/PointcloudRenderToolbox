#pragma once
#include "PRTAuxDraw.h"
#include <functional>
#include <imgui.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <iostream>
namespace PointcloudToolbox
{

    enum MouseButton
    {
        PRT_LEFT_BUTTON = 0,
        PRT_RIGHT_BUTTON = 1,
        PRT_MIDDLE_BUTTON = 2
    };

    enum MouseAction
    {
        PRT_DOWN = 0,
        PRT_UP = 1
    };

    //! Callback function for mouse button events.
    //! @param1 button The button that was pressed or released.
    //! @param2 action The action that was performed.
    //! @param3 and @param4 are the coordinate of the mouse cursor.
    using MouseButtonCallback = std::function<void(MouseButton, MouseAction, float, float)>;

    //! Callback function for mouse motion events.
    //! @param1 The x coordinate of the mouse cursor.
    //! @param2 The y coordinate of the mouse cursor.
    using MotionCallback = std::function<void(float, float)>;

    //! Callback function for mouse wheel events.
    //! @param1 is wheel number
    //! @param2 is direction The direction of the wheel.
    //! @param3 and @param4 are the coordinate of the mouse cursor.
    using MouseWheelCallback = std::function<void(int, int, float, float)>;

    using WindowHandle = int;
    constexpr WindowHandle InvalidWindowHandle = -1;

    struct WindowData
    {
        std::string title;
        int width;
        int height;

        void* NativeWindowPointer;
        ImGuiContext* cntx_imGui;
        bool visible;
        bool inFocus;

        std::vector<std::shared_ptr<DrawBase>> m_drawables; //! registered drawables

        //! projection and view matrix
        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;

        // callbacks
        std::function<void()> m_ImGuiRender;
        MouseButtonCallback m_mouseButtonCallback;
        MotionCallback m_motionCallback;
        MouseWheelCallback m_mouseWheelCallback;
    };

    namespace internal
    {
        static std::unordered_map<WindowHandle, WindowData> windows;
        static WindowHandle nextHandle = 0;
        static bool initialized = false;

        WindowData &GetWindowData(WindowHandle handle);
    } // namespace internal

    //! Create a new window and return its handle.
    WindowHandle CreateWindow(std::string title, int width, int height, std::vector<std::shared_ptr<DrawBase>> drawables = {std::make_shared<AuxDraw>()});

    //! Destroy the window with the given handle.
    void DestroyWindow(WindowHandle handle);

    //! Set the ImGui render callback for the window.
    void SetWindowImGuiRenderCallback(WindowHandle handle, std::function<void()> renderCallback);

    //! Set the mouse callback for the window.
    //! @param mouseCallback Callback function that will be called when the mouse is moved.
    void SetMouseCallback(WindowHandle handle, MouseButtonCallback callback);

    //! Set the motion callback for the window (move of the mouse).
    //! @param motionCallback Callback function that will be called when the mouse is moved.
    void SetMotionCallback(WindowHandle handle, MotionCallback callback);

    //! Set the mouse wheel callback for the window.
    //! @param mouseWheelCallback Callback function that will be called when the mouse wheel is moved.
    void SetMouseWheelCallback(WindowHandle handle, MouseWheelCallback callback);

    void SetViewAndProjectionMatrix(WindowHandle handle, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    void DestroyWindow(WindowHandle handle);
    void ShowWindow(WindowHandle handle);
    void HideWindow(WindowHandle handle);
    bool IsWindowVisible(WindowHandle handle);

    std::pair<float, float> GetWindowSize(WindowHandle handle);
    std::unordered_set<MouseButton> GetPressedMouseButtons(WindowHandle handle);

    void InitWindowSystem();
    void DestroyWindowSystem();

    void RenderWindow(const WindowHandle handle);

    std::vector<std::shared_ptr<DrawBase>>& GetDrawables(const WindowHandle handle);

    template<typename T>
    static std::shared_ptr<T> GetDrawable(const WindowHandle handle)
    {
        auto& drawables = GetDrawables(handle);
        for (auto drawable : drawables)
        {
            if (auto t = std::dynamic_pointer_cast<T>(drawable))
            {
                return t;
            }
        }
        return nullptr;
    }
} // namespace PointcloudToolbox