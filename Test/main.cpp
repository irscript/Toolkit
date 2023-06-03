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
        astr.append("123-");
        astr.append(str);
        print("%s\t%s cmp: %d %d", str.cstr(), str2.cstr(), str == str2, str != str2);
        print("%s", astr.cstr());
        astr.tolower();
        print("%s", astr.cstr());
        astr.toupper();
        print("%s", astr.cstr());
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