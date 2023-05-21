#include <airkit/Core/airApp.h>
#include <airkit/Core/airAlloc.h>

air::Application::Application()
{
    Thread::initMainThreadID();
    initMemSys();
}

air::Application::~Application()
{
    terminalMemSys();
}
