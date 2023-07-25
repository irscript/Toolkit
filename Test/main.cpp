#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airApp.h>

#include <airkit/Struct/airString.h>
#include <airkit/Math/airMath.h>

#include <airkit/Struct/airVector.h>

#include <airkit/VKRender/airVkRender.h>

using namespace air;

class TestApp : public Application
{
public:
    virtual int32 run() override
    {
        VkRender render;
        if (render.init() == false)
            return 1;

        auto window = render.createWindow("air-app", 800, 600);


        // pipe->onRenderWindow(window);

        render.destroyWindow(window);

        render.terminal();
        return 0;
    }
};

int main(int argc, char *argv[])
{
    // system("chcp 65001");
    // system("cls");
    TestApp app;

    return app.run();
}
