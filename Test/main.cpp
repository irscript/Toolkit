#include <airkit/Plat/airLoger.h>
#include <airkit/Core/airAlloc.h>

using namespace air;
int main(int argc, char *argv[])
{
    memlog("starting...\n");

    air::MemAlloctor mem;
    uint32 *arr[1024] = {0};

    for (int32 i = 0; i < 64; ++i)
    {
        arr[i] = (uint32 *)mem.alloctor(sizeof(uint32) * (i + 1), this_file(), this_line());
    }
    uintptr big = mem.alloctor(sizeof(uint32) * 1024, this_file(), this_line());
    /*
        for (int32 i = 0; i < 64; ++i)
        {
            mem.dealloctor(arr[i]);
        }*/
    return 0;
}