#include <iostream>
#include <thread>

void test() {
    for (int i = 0; i < 100; ++i);
    std::cout << "Counted to 100 in thread\n";
}
int main()
{
    std::thread thread1(test);
    thread1.join();
    return 0;
}
