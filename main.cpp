#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

// Function to create and initialize a window with a context
GLFWwindow* CreateWindow(const char* title, int width, int height) {
  GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window: " << title << std::endl;
    return nullptr;
  }
  return window;
}

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  // Set OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create first window
  GLFWwindow* window1 = CreateWindow("Window 1", 800, 600);
  if (!window1) {
    glfwTerminate();
    return -1;
  }

  // Make context current for the first window and initialize OpenGL loader
  glfwMakeContextCurrent(window1);

  glfwSwapInterval(1); // Enable vsync

  // Create second window
  GLFWwindow* window2 = CreateWindow("Window 2", 800, 600);
  if (!window2) {
    glfwDestroyWindow(window1);
    glfwTerminate();
    return -1;
  }

  // ImGui setup for Window 1
  IMGUI_CHECKVERSION();
  ImGuiContext* imgui_context1 = ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window1, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // ImGui setup for Window 2 (separate context)
  ImGuiContext* imgui_context2 = ImGui::CreateContext();
  ImGui::SetCurrentContext(imgui_context2);
  ImGui_ImplGlfw_InitForOpenGL(window2, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // Main loop
  while (!glfwWindowShouldClose(window1) && !glfwWindowShouldClose(window2)) {
    // Window 1 rendering
    glfwMakeContextCurrent(window1);
    ImGui::SetCurrentContext(imgui_context1);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Window 1 UI");
    ImGui::Text("Hello from Window 1!");
    ImGui::End();
    ImGui::Render();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window1);

    // Window 2 rendering
    glfwMakeContextCurrent(window2);
    ImGui::SetCurrentContext(imgui_context2);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Window 2 UI");
    ImGui::Text("Hello from Window 2!");
    ImGui::End();
    ImGui::Render();
    glClearColor(0.30f, 0.40f, 0.50f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window2);

    // Poll and handle events
    glfwPollEvents();
  }

  // Cleanup ImGui for Window 2
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(imgui_context2);

  // Cleanup ImGui for Window 1
  glfwMakeContextCurrent(window1);
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // Cleanup GLFW
  glfwDestroyWindow(window1);
  glfwDestroyWindow(window2);
  glfwTerminate();

  return 0;
}
