#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airAlloc.h>
#include <airkit/Plat/airThread.h>
#include <airkit/Core/airApp.h>
#include <airkit/Struct/airList.h>
#include <airkit/Struct/airArray.h>
#include <airkit/Struct/airMap.h>
#include <airkit/Struct/airEntrust.h>

using namespace air;

class TestApp : public Application
{
public:
    virtual int32 run() override
    {
        Map<int32, int32> set;
        // 插入数据
        for (int32 i = 0; i < 100; ++i)
            set.insert(i, i + 100);
        // 打印
        for (auto it = set.begin(); it != set.end(); ++it)
            printf("[key:%d val:%d]\t", it.key(), it.value());
        printf("\ncount:%lld\n", set.getCount());
        // 查询数据
        auto itf = set.find(10);
        if (itf != set.end())
            printf("find:[key:%d val:%d]\n", itf.key(), itf.value());
        itf = set.find(5);
        if (itf != set.end())
            printf("find:[key:%d val:%d]\n", itf.key(), itf.value());
        printf("min: [key:%d val:%d] root: [key:%d val:%d] max:[key:%d val:%d]\n",
               set.getMin().key(), set.getMin().value(),
               set.getRoot().key(), set.getRoot().value(),
               set.getMax().key(), set.getMax().value());
        // 删除数据
        printf("开始删除数据\n");
        while (20 != set.getCount())
        {
            auto it = set.getRoot();
            printf("remove:[key:%d val:%d]\n", it.key(), it.value());
            set.remove(it);
            for (auto it = set.begin(); it != set.end(); ++it)
                printf("[key:%d val:%d]\t", it.key(), it.value());
            printf("\ncount:%lld\n", set.getCount());
        }
        printf("插入并打印数据\n");
        // 插入数据
        set.insert(100, 1000);
        itf = set.find(10);
        if (itf != set.end())
            set.remove(itf);
        itf = set.find(10);
        if (itf != set.end())
            set.remove(itf);
        itf = set.find(12);
        if (itf != set.end())
            set.remove(itf);
        itf = set.find(9);
        if (itf != set.end())
            set.remove(itf);
        set.insert(10, 110);
        // 打印
        int i=0;
        for (auto it = set.begin(); it != set.end(); ++i,++it)
        {
            // printf("[key:%d val:%d clr:%d]\t", it.key(), it.value(), (*it).getColor());
            auto left = (*it).getLeft();
            auto right = (*it).getRight();
            auto root = (*it).getParent();
            printf("%d\t[key:%d val:%d clr:%d\tleft:%d\tparent:%d\tright:%d]\n",i,
                   it.key(), it.value(), (*it).getColor(),
                   left != nullptr ? left->mKey : -1, root != nullptr ? root->mKey : -1, right != nullptr ? right->mKey : -1);
        }

        printf("\ncount:%lld\n", set.getCount());
        printf("min: [key:%d val:%d clr:%d] root: [key:%d val:%d clr:%d] max:[key:%d val:%d clr:%d]\n",
               set.getMin().key(), set.getMin().value(), (*set.getMin()).getColor(),
               set.getRoot().key(), set.getRoot().value(), (*set.getRoot()).getColor(),
               set.getMax().key(), set.getMax().value(), (*set.getMax()).getColor());
        printf("清空数据\n");
        set.clear();
        for (auto it = set.begin(); it != set.end(); ++it)
            printf("[key:%d val:%d clr:%d]\t", it.key(), it.value(), (*it).getColor());
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