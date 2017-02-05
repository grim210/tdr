#include <iostream>
#include <memory>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <tdrmain.h>

int main(int argc, char* argv[])
{
    std::shared_ptr<Timer> timer = Timer::Create();
    timer->start();

#ifdef _WIN32
    // do some win32 sleep call
#else
    sleep(1);
#endif

    std::cout << "Slept for " << timer->getTime() << " seconds." << std::endl;

#ifdef _WIN32
    // more win32 sleeping
#else
    sleep(3);
#endif

    std::cout << "Slept for " << timer->getTime() << " seconds." << std::endl;

    return 0;
}
