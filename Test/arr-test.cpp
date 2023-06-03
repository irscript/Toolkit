#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airAlloc.h>
#include <airkit/Plat/airThread.h>
#include <airkit/Core/airApp.h>
#include <airkit/Struct/airList.h>
#include <airkit/Struct/airArray.h>

using namespace air;

class TestApp : public Application
{
public:
    virtual int32 run() override
    {
        int32 v = 0;
        print("一维数组...");
        Array<int32, 10> arr;
        auto it = arr.begin();
        for (; it != arr.end(); ++it)
        {
            arr[it] = ++v;
            print("%d:\t%d", *it, arr[it]);
        }

        print("二维数组...");
        Array2<int32, 5, 5> arr2;
        v = 0;
        auto it2 = arr2.begin();
        for (; it2 != arr2.end(); ++it2)
        {
            arr2[it2] = ++v;
            print("[%d,%d]:\t%d", it2.i(), it2.j(), arr2[it2]);
        }

        print("三维数组...");
        Array3<int32, 5, 5, 5> arr3;
        v = 0;
        auto it3 = arr3.begin();
        for (; it3 != arr3.end(); ++it3)
        {
            arr3[it3] = ++v;
            print("[%d,%d,%d]:\t%d", it3.i(), it3.j(), it3.k(), arr3[it3]);
        }

        return 0;
    }
};

int main(int argc, char *argv[])
{
    system("chcp 65001");
    system("cls");
    TestApp app;

    return app.run();
}