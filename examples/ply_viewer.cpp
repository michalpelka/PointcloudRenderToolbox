#include <PRTWindow.h>
#include <iostream>
#include <PRTPointcloud.h>
#include "happly.h"
void SetSceneLook(PointcloudToolbox::WindowHandle windowHandle,
                  float translate_x, float translate_y, float translate_z, float rotate_x, float rotate_y)
{
    const auto [w, h] = PointcloudToolbox::GetWindowSize(windowHandle);
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(translate_x, translate_y, translate_z));
    view = glm::rotate(view, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rotate_y), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 projection =glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 1000.0f);

    PointcloudToolbox::SetViewAndProjectionMatrix(windowHandle, view, projection);
}
int main(int argc, char **argv)
{
    // get name of ply
    if (argc < 2)
    {
            std::cerr << "Usage: " << argv[0] << " <ply file>" << std::endl;
            return 1;
    }
    const std::string plyFile = argv[1];
    std::cout << "Loading " << plyFile << std::endl;



    float translate_x =0.0, translate_y = 0.0;
    float translate_z = -50.0;
    float rotate_x = 0.0, rotate_y = 0.0;
    float mouse_old_x, mouse_old_y = 0.0;
    PointcloudToolbox::InitWindowSystem();
    const auto window1Handle = PointcloudToolbox::CreateWindow("Hello Window1", 800, 600);

    glm::vec2 eye_pos(10.0f, 0.0f);
    glm::vec2 last_mouse_pos(-1.0f, 0.0f);
    float zoom = 1.0f;

    auto mouseBtnCallback = [&](PointcloudToolbox::MouseButton button, PointcloudToolbox::MouseAction action, float x, float y)
    {
        if (action == PointcloudToolbox::MouseAction::PRT_DOWN)
            mouse_old_x = x;
        mouse_old_y = y;
    };

    auto mouseMotion = [&](float x, float y)
    {
        const auto buttons = PointcloudToolbox::GetPressedMouseButtons(window1Handle);

        float dx, dy;
        dx = (float)(x - mouse_old_x);
        dy = (float)(y - mouse_old_y);


        if (buttons.find(PointcloudToolbox::MouseButton::PRT_LEFT_BUTTON) != buttons.end())
        {
            rotate_x += dy * 0.2f; // * mouse_sensitivity;
            rotate_y += dx * 0.2f; // * mouse_sensitivity;
        }
        if (buttons.find(PointcloudToolbox::MouseButton::PRT_RIGHT_BUTTON) != buttons.end())
        {
            translate_x += dx * 0.05f ;//* mouse_sensitivity;
            translate_y -= dy * 0.05f ;//* mouse_sensitivity;
        }

        SetSceneLook(window1Handle,translate_x,translate_y,translate_z,rotate_x,rotate_y);
        mouse_old_x = x;
        mouse_old_y = y;

    };

    auto mouseWheel = [&](int wheel, int direction, float x, float y)
    {
        if (direction > 0)
        {
            translate_z -= 0.05f * translate_z;
        }
        else
        {
            translate_z += 0.05f * translate_z;
        }
        SetSceneLook(window1Handle,translate_x,translate_y,translate_z,rotate_x,rotate_y);
    };

    PointcloudToolbox::SetMouseCallback(window1Handle, mouseBtnCallback);
    PointcloudToolbox::SetMotionCallback(window1Handle, mouseMotion);
    PointcloudToolbox::SetMouseWheelCallback(window1Handle, mouseWheel);

    SetSceneLook(window1Handle,translate_x,translate_y,translate_z,rotate_x,rotate_y);

    auto auxDraw = PointcloudToolbox::GetDrawable<PointcloudToolbox::AuxDraw>(window1Handle);

    assert(auxDraw != nullptr);


    auto pcDraw = PointcloudToolbox::GetDrawable<PointcloudToolbox::PointcloudDraw>(window1Handle);

    float length = 1.0;


    happly::PLYData plyIn(plyFile.c_str());
    const auto elementNames = plyIn.getElementNames();
    auto vertices = plyIn.getVertexPositions();
    auto& vertexElement = plyIn.getElement("vertex");
    bool hasRed = vertexElement.hasProperty("red");
    bool hasGreen = vertexElement.hasProperty("green");
    bool hasBlue = vertexElement.hasProperty("blue");
    std::vector<std::array<unsigned char, 3>> colors;
    if (hasRed && hasGreen && hasBlue)
    {
        colors = plyIn.getVertexColors();
    }
    else
    {
        colors.resize(vertices.size(), { 255, 255, 255 });
    }

    //arrange data to DataLayout::DATALAYOUT_XYZFLOAT_RGBFLOAT
    std::vector<float> pointcloudData;
    for (size_t i = 0; i < vertices.size(); i++)
    {
            pointcloudData.push_back(vertices[i][0]);
            pointcloudData.push_back(vertices[i][1]);
            pointcloudData.push_back(vertices[i][2]);
            pointcloudData.push_back(colors[i][0] / 255.0f);
            pointcloudData.push_back(colors[i][1] / 255.0f);
            pointcloudData.push_back(colors[i][2] / 255.0f);
    }

    pcDraw->AcquirePointcloudHandle(glm::mat4(1.0f), pointcloudData.data(), vertices.size(), PointcloudToolbox::PointcloudDraw::DataLayout::DATALAYOUT_XYZFLOAT_RGBFLOAT);

    while (PointcloudToolbox::IsWindowVisible(window1Handle))
    {

        auxDraw->DrawLine(glm::vec3(0, 0, 0), glm::vec3(length,0,0), glm::vec3(1, 0, 0));
        auxDraw->DrawLine(glm::vec3(0, 0, 0), glm::vec3(0,length,0), glm::vec3(0, 1, 0));
        auxDraw->DrawLine(glm::vec3(0, 0, 0), glm::vec3(0,0,length), glm::vec3(0, 0, 1));
        auxDraw->DrawPoint(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 10.0f);
        auxDraw->DrawPoint(glm::vec3(length, 0, 0), glm::vec3(1, 0, 0), 5.0f);
        auxDraw->DrawPoint(glm::vec3(0, length, 0), glm::vec3(0, 1, 0), 5.0f);
        auxDraw->DrawPoint(glm::vec3(0, 0, length), glm::vec3(0, 0, 1), 5.0f);
        PointcloudToolbox::SetWindowImGuiRenderCallback(window1Handle, [&]()
                                                        {
                                                            ImGui::Begin("Hello, world!");
                                                            ImGui::SliderFloat("Length", &length, 0.0f, 10.0f);
                                                            ImGui::End();
                                                        });
        PointcloudToolbox::RenderWindow(window1Handle);


    }
    PointcloudToolbox::DestroyWindow(window1Handle);
    //    PointcloudToolbox::DestroyWindow(window2Handle);

    PointcloudToolbox::DestroyWindowSystem();
    return 0;
}