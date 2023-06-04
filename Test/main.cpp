#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airApp.h>

#include <airkit/Struct/airString.h>

using namespace air;

class TestApp : public Application
{
public:
    virtual int32 run() override
    {
        String str(0.3333f), str2(str), astr;
        astr = "Abc";
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