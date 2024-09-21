#include <window.h>

int main()
{
    PointcloudToolbox::InitWindowSystem();
    const auto window1Handle = PointcloudToolbox::CreateWindow("Hello Window1", 800, 600);
    const auto window2Handle = PointcloudToolbox::CreateWindow("Hello Window2", 800, 600);

    while (PointcloudToolbox::IsWindowVisible(window1Handle))
    {
        PointcloudToolbox::RenderWindow(window1Handle);
        PointcloudToolbox::RenderWindow(window2Handle);
    }
    PointcloudToolbox::DestroyWindow(window1Handle);
    PointcloudToolbox::DestroyWindow(window2Handle);
    PointcloudToolbox::DestroyWindowSystem();
    return 0;
}