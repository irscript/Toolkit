#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airAlloc.h>
#include <airkit/Plat/airThread.h>
#include <airkit/Core/airApp.h>
#include <airkit/Struct/airList.h>
#include <airkit/Struct/airArray.h>
#include <airkit/Struct/airMap.h>
#include <airkit/Struct/airSet.h>
#include <airkit/Struct/airEntrust.h>

using namespace air;

class TestApp : public Application
{
public:
    virtual int32 run() override
    {
        Set<int32> set;
        // 插入数据
        for (int32 i = 0; i < 30; ++i)
            set.insert(i);
        // 打印
        for (auto it = set.begin(); it != set.end(); ++it)
            printf("%d\t", *it);
        printf("\ncount:%lld\n", set.getCount());
        // 查询数据
        auto itf = set.find(10);
        if (itf != set.end())
        {
            printf("find:%d\n", *itf);
            set.remove(itf);
        }
        itf = set.find(15);
        if (itf != set.end())
        {
            printf("find:%d\n", *itf);
            set.remove(itf);
        }

        printf("min: %d root: %d max: %d\n", *set.getMin(), *set.getRoot(), *set.getMax());
        // 删除数据

        while (0 != set.getCount())
        {
            auto it = set.getRoot();
            printf("remove:%d\n", *it);
            set.remove(it);
            for (auto it = set.begin(); it != set.end(); ++it)
                printf("%d\t", *it);
            printf("\ncount:%lld\n", set.getCount());
        }

        // 情空数据
        set.clear();
        for (auto it = set.begin(); it != set.end(); ++it)
            printf("%d\t", *it);
        printf("\ncount:%lld\n", set.getCount());
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