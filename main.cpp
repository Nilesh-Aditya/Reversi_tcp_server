#include <iostream>
#include "display.hpp"

// client connection address and port should be entered in terminal while runnning

int main(int argc, char const *argv[])
try
{

    Reversi::init();
    Reversi::event_manager();

    return 0;
}
catch (const std::exception &e)
{
    std::cerr << e.what() << '\n';

    return -1;
}