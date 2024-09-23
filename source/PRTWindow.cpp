#include "PRTWindow.h"

#include <GL/glew.h> // GLEW
#include <GLFW/glfw3.h> // GLFW
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// GLFW implemetation
namespace PointcloudToolbox
{

    bool IsGLFWWindowFocused(GLFWwindow* window)
    {
        const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
        return windowData->inFocus;
    }

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (IsGLFWWindowFocused(window))
        {
            const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        }
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
        std::string windowTitle = windowData->title;
        if (IsGLFWWindowFocused(window))
        {
            const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        }
    }

    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (IsGLFWWindowFocused(window))
        {
            const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
        }
    }

    void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (IsGLFWWindowFocused(window))
        {
            const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        }
    }

    void CharCallback(GLFWwindow* window, unsigned int c)
    {
        if (IsGLFWWindowFocused(window))
        {
            const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_CharCallback(window, c);
        }
    }

    WindowHandle CreateWindow(std::string title, int width, int height)
    {
        internal::nextHandle++;
        internal::windows[internal::nextHandle] = { .title = title, .width = width, .height = height, .visible = true };

        if (!internal::initialized)
        {
            std::cerr << "Window system not initialized" << std::endl;
            std::abort();
        }

        auto& windowData = internal::windows[internal::nextHandle];
        windowData.NativeWindowPointer = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!windowData.NativeWindowPointer)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            std::abort();
        }
        glfwMakeContextCurrent((GLFWwindow*)windowData.NativeWindowPointer);
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            std::abort();
        }
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // Setup Platform/Renderer bindings

        windowData.cntx_imGui = ImGui::CreateContext();
        ImGui::SetCurrentContext(windowData.cntx_imGui);

        auto* glfwWindow = (GLFWwindow*)windowData.NativeWindowPointer;

        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, false);
        ImGui_ImplOpenGL3_Init("#version 330");

        glfwSetWindowUserPointer(glfwWindow, &windowData);
        glfwSetCursorPosCallback(glfwWindow, CursorPosCallback);
        glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
        glfwSetScrollCallback(glfwWindow, ScrollCallback);
        glfwSetKeyCallback(glfwWindow, KeyCallback);
        glfwSetCharCallback(glfwWindow, CharCallback);
        glfwSetCursorEnterCallback(
            glfwWindow,
            [](GLFWwindow* window, int entered)
            {
                auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
                windowData->inFocus = entered;
            });

        glfwSetWindowFocusCallback(
            glfwWindow,
            [](GLFWwindow* window, int focused)
            {
                auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
                windowData->inFocus = focused;
            });

        // intialize aux draw
        windowData.auxDraw.CompileShaders();
        return internal::nextHandle;
    }

    void DestroyWindow(WindowHandle handle)
    {
        // invalidate the window
        auto& windowData = internal::windows[handle];
        if (windowData.NativeWindowPointer)
        {
            glfwDestroyWindow((GLFWwindow*)windowData.NativeWindowPointer);
        }

        if (windowData.cntx_imGui)
        {
            ImGui::SetCurrentContext(windowData.cntx_imGui);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        internal::windows.erase(handle);
    }

    void SetWindowImGuiRenderCallback(WindowHandle handle, std::function<void()> renderCallback)
    {
        internal::windows[handle].ImGuiRender = renderCallback;
    }

    void InitWindowSystem()
    {
        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            std::abort();
        }

        // Configure GLFW for OpenGL version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glewExperimental = GL_TRUE;

        internal::initialized = true;
    }
    void DestroyWindowSystem()
    {
        if (internal::initialized)
        {
            for (auto& window : internal::windows)
            {
                DestroyWindow(window.first);
            }
            internal::windows.clear();
            internal::initialized = false;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
    }

    void RenderWindow(const WindowHandle handle)
    {
        assert(internal::initialized);
        assert(internal::windows.find(handle) != internal::windows.end());

        auto& windowData = internal::windows[handle];
        if (!windowData.visible)
        {
            return;
        }

        auto* glfwWindow = (GLFWwindow*)windowData.NativeWindowPointer;
        assert(glfwWindow);
        // glfw context
        glfwMakeContextCurrent(glfwWindow);

        if (glfwWindowShouldClose(glfwWindow))
        {
            windowData.visible = false;
            glfwHideWindow(glfwWindow);
            return;
        }
        // imgui context
        ImGui::SetCurrentContext(windowData.cntx_imGui);

        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw aux
        windowData.auxDraw.IssueDrawCalls();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("Window %s", windowData.title.c_str());
        ImGui::End();

        if (windowData.ImGuiRender)
        {
            windowData.ImGuiRender();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers((GLFWwindow*)windowData.NativeWindowPointer);
        glfwPollEvents();
    }

    void ShowWindow(WindowHandle handle)
    {
        assert(internal::windows.find(handle) != internal::windows.end());
        auto& windowData = internal::windows[handle];
        windowData.visible = true;
        glfwShowWindow((GLFWwindow*)windowData.NativeWindowPointer);
    }

    void HideWindow(WindowHandle handle)
    {
        assert(internal::windows.find(handle) != internal::windows.end());
        auto& windowData = internal::windows[handle];
        windowData.visible = false;
        glfwHideWindow((GLFWwindow*)windowData.NativeWindowPointer);
    }

    bool IsWindowVisible(WindowHandle handle)
    {
        assert(internal::windows.find(handle) != internal::windows.end());
        auto& windowData = internal::windows[handle];
        return windowData.visible;
    }

    void SetViewMatrix(WindowHandle handle, const std::array<float, 16> viewMatrix)
    {
        assert(internal::windows.find(handle) != internal::windows.end());
        auto& windowData = internal::windows[handle];
        //  windowData.viewMatrix = viewMatrix;
    }

    AuxDraw& GetAuxDraw(const WindowHandle handle)
    {
        assert(internal::windows.find(handle) != internal::windows.end());
        return internal::windows[handle].auxDraw;
    }
} // namespace PointcloudToolbox