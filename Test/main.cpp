#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airAlloc.h>
#include <airkit/Plat/airThread.h>
#include <airkit/Core/airApp.h>
#include <airkit/Struct/airList.h>

using namespace air;

class TestApp : public Application
{
public:
    List<int32> list;

    void print()
    {
        for (auto iter = list.begin(); iter != list.end(); ++iter)
        {
            air::print("%d\t", *iter);
            if (*iter % 10 == 0)
                air::print("\n");
        }
    }

    virtual int32 run() override
    {
        air::print("插入测试\n");
        // 插入
        for (auto i = 1; i < 10; ++i)
            list.insertTail(i);

        list.insertEntry(200);
        // 打印
        print();
        air::print("删除测试\n");
        // 删除测试
        list.removeEntry();
        list.removeTail();

        auto front = list.begin();
        list.remove(++front);
        // 打印
        print();

        return 0;
    }
};

int main(int argc, char *argv[])
{
    system("chcp 65001");
    TestApp app;

    return app.run();
}