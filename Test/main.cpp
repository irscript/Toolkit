#include <airkit/Plat/airLoger.h>

int main(int argc, char *argv[])
{
    for (int i = 0; i < 10;)
        air::memlog("this a test %d!\n", ++i);
    return 0;
}