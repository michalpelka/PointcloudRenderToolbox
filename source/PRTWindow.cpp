#include "PRTWindow.h"

#include <GL/glew.h> // GLEW
#include <GLFW/glfw3.h> // GLFW
#include <iostream>

#include <GL/Assert.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// GLFW implemetation
namespace PointcloudToolbox
{

    MouseButton MouseButtonFromGLFW(int button)
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            return MouseButton::PRT_LEFT_BUTTON;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return MouseButton::PRT_RIGHT_BUTTON;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return MouseButton::PRT_MIDDLE_BUTTON;
        default:
            return MouseButton::PRT_LEFT_BUTTON;
        }
    }

    MouseAction MouseActionFromGLFW(int action)
    {
        switch (action)
        {
        case GLFW_PRESS:
            return MouseAction::PRT_DOWN;
        case GLFW_RELEASE:
            return MouseAction::PRT_UP;
        default:
            return MouseAction::PRT_DOWN;
        }
    }

    bool IsGLFWWindowFocused(GLFWwindow* window)
    {
        const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
        return windowData->inFocus;
    }

    void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (IsGLFWWindowFocused(window))
        {
            const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        }
    }

    void GLFW_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
        std::string windowTitle = windowData->title;
        if (IsGLFWWindowFocused(window))
        {
            auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

            if (!ImGui::GetIO().WantCaptureMouse && windowData->m_mouseButtonCallback)
            {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                auto buttonGlutLike = MouseButtonFromGLFW(button);
                windowData->m_mouseButtonCallback(buttonGlutLike, MouseActionFromGLFW(action), xpos, ypos);
            }
        }
    }

    void GLFW_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (IsGLFWWindowFocused(window))
        {
            const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

            if (!ImGui::GetIO().WantCaptureMouse && windowData->m_mouseWheelCallback)
            {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                windowData->m_mouseWheelCallback(0, yoffset, xpos, ypos);
            }
        }
    }

    void GLFW_CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (IsGLFWWindowFocused(window))
        {
            const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            ImGui::SetCurrentContext(windowData->cntx_imGui);
            ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
            if (!ImGui::GetIO().WantCaptureMouse && windowData->m_motionCallback)
            {
                windowData->m_motionCallback(xpos, ypos);
            }
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
    void GLFW_WindowScaleCallback(GLFWwindow* window, float xscale, float yscale)
    {
        const auto* windowData = (WindowData*)glfwGetWindowUserPointer(window);
    }

    WindowHandle CreateWindow(std::string title, int width, int height, std::vector<std::shared_ptr<DrawBase>> drawables)
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
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // Setup Platform/Renderer bindings

        windowData.cntx_imGui = ImGui::CreateContext();
        ImGui::SetCurrentContext(windowData.cntx_imGui);

        auto* glfwWindow = (GLFWwindow*)windowData.NativeWindowPointer;

        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, false);
        ImGui_ImplOpenGL3_Init("#version 330");

        glfwSetWindowUserPointer(glfwWindow, &windowData);
        glfwSetCursorPosCallback(glfwWindow, GLFW_CursorPosCallback);
        glfwSetMouseButtonCallback(glfwWindow, GLFW_MouseButtonCallback);
        glfwSetScrollCallback(glfwWindow, GLFW_ScrollCallback);
        glfwSetKeyCallback(glfwWindow, GLFW_KeyCallback);
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

        glfwSetWindowContentScaleCallback(glfwWindow, GLFW_WindowScaleCallback);

        windowData.m_drawables = drawables;
        for (auto& drawable : windowData.m_drawables)
        {
            drawable->CompileShaders();
        }
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

        // Todo(mpelka) maybe we leak imgui cotnext here
        internal::windows.erase(handle);
    }

    void SetWindowImGuiRenderCallback(WindowHandle handle, std::function<void()> renderCallback)
    {
        internal::windows[handle].m_ImGuiRender = renderCallback;
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
    WindowData& internal::GetWindowData(WindowHandle handle)
    {
        assert(internal::windows.find(handle) != internal::windows.end());
        return internal::windows[handle];
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
        int width = 0, height = 0;
        glfwGetWindowSize(glfwWindow, &width, &height);
        GL_CALL(glViewport(0, 0, width, height));

        // imgui context
        ImGui::SetCurrentContext(windowData.cntx_imGui);

        // Clear the screen
        GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL_CALL(glfwWindowHint(GLFW_DEPTH_BITS, 32));
        GL_CALL(glEnable(GL_DEPTH_TEST));
        GL_CALL(glEnable(GL_BLEND));
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        DrawBaseStats stats;
        // User's drawables
        for (auto& drawable : windowData.m_drawables)
        {
            drawable->IssueDrawCalls(windowData.m_viewMatrix, windowData.m_projectionMatrix);
            const auto thisDrawStats = drawable->GetStatsForDrawCall();
            stats += thisDrawStats;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // User's ImGui render callback
        if (windowData.m_ImGuiRender)
        {
            windowData.m_ImGuiRender();
        }
        //        ImGui::ShowDemoWindow(nullptr);
        // In your render loop, directly render text
        ImGui::SetNextWindowPos(ImVec2(10, 10)); // Position it at top-left corner
        ImGui::SetNextWindowSize(ImVec2(0, 0)); // Automatically size based on text

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::Begin(
            "TextOverlay",
            nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav );

        // Display your statistics as plain text
        ImGui::Text("Frame rate: %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Text("Draw Calls: %d", stats.m_numberOfDrawCalls);
        ImGui::Text("Total vertices : %d", stats.m_numberOfVertex);
        // view matrix
        ImGui::Text("View Matrix");
        for (int i = 0; i < 4; i++)
        {
            ImGui::Text(
                "%f %f %f %f",
                windowData.m_viewMatrix[i][0],
                windowData.m_viewMatrix[i][1],
                windowData.m_viewMatrix[i][2],
                windowData.m_viewMatrix[i][3]);
        }
        ImGui::End();
        ImGui::PopStyleColor();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers((GLFWwindow*)windowData.NativeWindowPointer);
        glfwPollEvents();
    }

    void ShowWindow(WindowHandle handle)
    {
        auto& windowData = internal::GetWindowData(handle);
        windowData.visible = true;
        glfwShowWindow((GLFWwindow*)windowData.NativeWindowPointer);
    }

    void HideWindow(WindowHandle handle)
    {
        auto& windowData = internal::GetWindowData(handle);
        windowData.visible = false;
        glfwHideWindow((GLFWwindow*)windowData.NativeWindowPointer);
    }

    bool IsWindowVisible(WindowHandle handle)
    {
        auto& windowData = internal::GetWindowData(handle);
        return windowData.visible;
    }

    void SetMouseCallback(WindowHandle handle, MouseButtonCallback callback)
    {
        auto& windowData = internal::GetWindowData(handle);
        windowData.m_mouseButtonCallback = callback;
    }

    void SetMotionCallback(WindowHandle handle, MotionCallback callback)
    {
        auto& windowData = internal::GetWindowData(handle);
        windowData.m_motionCallback = callback;
    }

    void SetViewAndProjectionMatrix(WindowHandle handle, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        auto& windowData = internal::GetWindowData(handle);
        windowData.m_viewMatrix = viewMatrix;
        windowData.m_projectionMatrix = projectionMatrix;
    }

    void SetMouseWheelCallback(WindowHandle handle, MouseWheelCallback callback)
    {
        auto& windowData = internal::GetWindowData(handle);
        windowData.m_mouseWheelCallback = callback;
    }

    std::pair<float, float> GetWindowSize(WindowHandle handle)
    {
        auto& windowData = internal::GetWindowData(handle);
        return { windowData.width, windowData.height };
    }

    std::unordered_set<MouseButton> GetPressedMouseButtons(WindowHandle handle)
    {
        auto& windowData = internal::GetWindowData(handle);
        auto glfwWindow = (GLFWwindow*)windowData.NativeWindowPointer;
        assert(glfwWindow);
        std::unordered_set<MouseButton> buttons;
        for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
        {
            if (glfwGetMouseButton((GLFWwindow*)windowData.NativeWindowPointer, i) == GLFW_PRESS)
            {
                buttons.insert(MouseButtonFromGLFW(i));
            }
        }
        return buttons;
    }

    std::vector<std::shared_ptr<DrawBase>>& GetDrawables(const WindowHandle handle)
    {
        auto& windowData = internal::GetWindowData(handle);
        return internal::windows[handle].m_drawables;
    }

    glm::mat4 GetViewMatrix(WindowHandle handle)
    {
        auto& windowData = internal::GetWindowData(handle);
        return windowData.m_viewMatrix;
    }

    glm::mat4 GetProjectionMatrix(WindowHandle handle)
    {
        auto& windowData = internal::GetWindowData(handle);
        return windowData.m_projectionMatrix;
    }
} // namespace PointcloudToolbox