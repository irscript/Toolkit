#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airAlloc.h>
#include <airkit/Plat/airThread.h>

using namespace air;

class Print : public Thread
{
private:
    int mStart, mEnd;

public:
    Print(int start, int end) : mStart(start), mEnd(end) {}
    ~Print() {stop();}

    virtual uint worker() override
    {
        uint32 *arr[1024] = {0};

        for (int32 i = 0; i < 64; ++i)
        {
            auto size = sizeof(uint32) * (i + 1);
            arr[i] = (uint32 *)alloc(size);
            *arr[i] = size;
        }
        uintptr big = alloc(sizeof(uint32) * 1024);
        /*
            for (int32 i = 0; i < 64; ++i)
            {
                mem.dealloctor(arr[i]);
            }*/
        for (int32 i = 0; i < 64; ++i)
        {
            memlog("ID:%lx\t size = %d\n", Thread::getCurrentThreadID(), *arr[i]);
        }
        checkMemSys();
        return 0;
    }
};

void apprun()
{
    ///*
    memlog.start();
    Print tp(0, 10), tp2(11, 20), tp3(21, 30);
    tp.start();
    tp2.start();
    tp3.start();
    tp3.start(EBool::False);
    tp3.detach();
    memlog.end();
    //*/
    //_sleep(1000);
    uint32 *arr[1024] = {0};

    for (int32 i = 0; i < 64; ++i)
    {
        auto size = sizeof(uint32) * (i + 1);
        arr[i] = (uint32 *)alloc(size);
        *arr[i] = size;
    }
    uintptr big = alloc(sizeof(uint32) * 1024);
    /*
        for (int32 i = 0; i < 64; ++i)
        {
            mem.dealloctor(arr[i]);
        }
    for (int32 i = 0; i < 64; ++i)
    {
        memlog("ID:%lx\t size = %d\n",Thread::getCurrentThreadID(), *arr[i]);
    }*/
}

int main(int argc, char *argv[])
{
    Thread::initMainThreadID();
    initMemSys();
    apprun();
    checkMemSys();
    //_sleep(10000);
    terminalMemSys();
    return 0;
}