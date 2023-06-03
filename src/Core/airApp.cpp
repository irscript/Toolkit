#include <airkit/Core/airApp.h>
#include <airkit/Core/airAlloc.h>

air::Application::Application()
{
    setlocale(LC_ALL, "zh_CN.UTF-8"); 
    Thread::initMainThreadID();
    initMemSys();
}

air::Application::~Application()
{
    terminalMemSys();
}
