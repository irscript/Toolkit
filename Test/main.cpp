#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airApp.h>

#include <airkit/Struct/airString.h>
#include <airkit/Math/airMath.h>

using namespace air;

class ThreadTask : public Thread
{
public:
    static String mstr;
    ~ThreadTask() { stop(); }
    uint worker() override
    {
        String str(0.3333f), str2(str), astr = mstr;
        // astr = "Abc";
        astr.append('d');
        astr.append("123-");
        astr.append(str);
        print("%s\t%s cmp: %d %d", str.cstr(), str2.cstr(), str == str2, str != str2);
        print("%s", astr.cstr());
        astr.tolower();
        print("%s", astr.cstr());
        astr.toupper();
        print("%s", astr.cstr());
        print("字符查找匹配测试");
        print("find-ch:%d", astr.findFirst('1'));
        print("find-ch:%d", astr.findLast('1'));
        print("match-list:%d", astr.matchFirst("01234", 5));
        print("match-list:%d", astr.matchLast("01234", 5));
        print("match-list:%d", astr.matchFirstNot("01234", 5));
        print("match-list:%d", astr.matchLastNot("01234", 5));
        print("字符串查找匹配测试");
        print("find-str:%d", astr.find("123"));
        String find("123");
        print("find-str:%d", astr.find(find));
        find = "3333";
        print("find-str:%d", astr.find(find));
        print("子串测试：%s", astr.substr(4, 3).cstr());
        print("替换测试");
        print("replace-ch:%s \t%d", astr.replace('3', '5', 7).cstr(), astr.length());
        print("replace-str:%s \t%d", astr.replace("55", "66", 7).cstr(), astr.length());
        print("\t%d", astr.length());
        print("replace-str:%s \t%d", astr.replace("66", "7", 7).cstr(), astr.length());
        print("\t%d", astr.length());
        print("replace-str:%s \t%d", astr.replace("7", "897", 7).cstr(), astr.length());
        print("\t%d", astr.length());
        print("replace-str:%s \t%d", astr.replace("897", "", 7).cstr(), astr.length());
        print("\t%d", astr.length());
        print("移除测试");
        print("remove-str:%s \t%d", astr.remove('A').cstr(), astr.length());
        print("\t%d", astr.length());
        print("remove-str:%s \t%d", astr.remove("BCD").cstr(), astr.length());
        print("\t%d", astr.length());
        print("remove-str:%s \t%d", astr.remove("00").cstr(), astr.length());
        print("\t%d", astr.length());
        print("追加测试");
        str = "int:";
        str << 123 << "\tuint:" << 456u << "\tint64:" << 789ll << "\tint64:" << 789ull;
        str << "\n\tflt32:" << 32.12345678 << "\tflt64:" << 64.12345678;
        print("append-str:\n\t%s", str.cstr());
        print("数学转换测试");
        Vec2r v2(0, 2);
        print(v2.toString().cstr());
        Vec3r v3(0, 0, 3);
        print(v3.toString().cstr());
        Vec4r v4(0, 0, 0, 4);
        print(v4.toString().cstr());

        Recti ir(0, 0, 100, -100);
        print(ir.toString().cstr());

        print("任务内容结束");
        return 0;
    }
};
String ThreadTask::mstr;
class TestApp : public Application
{
public:
    virtual int32 run() override
    {
        ThreadTask::mstr = "Main-App";
        ThreadTask task;
        task.start();

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