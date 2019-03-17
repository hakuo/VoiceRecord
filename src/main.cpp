#include <iostream>
#include "voip/VoIP.h"
#include "media/Media.h"
int main()
{
    VoIP test;
    test.start_pipeline(5000);

    while(1)
    {
        std::cin.get();
        std::cout << "toggle" << std::endl;
        test.toggle_test();
    }
    
    return 0;
}
