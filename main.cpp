#include <iostream>
#include "display.hpp"

// client connection address and port should be entered in terminal while runnning

int main(int argc, char const *argv[])
try
{
    Reversi::initialise_client(argv[1], argv[2]); // client init
    Reversi::init();                              // display init
    Reversi::event_manager();                     // event manager

    return 0;
}
catch (const std::exception &e)
{
    std::cerr << e.what() << '\n';
    std::cout << "\n\nServer is running on PORT 8080" << std::endl;
    std::cout << "[value should be entered in following order]" << std::endl;
    std::cout << "./main [Ip Adress] [PORT] \n"
              << std::endl;

    std::cout << "Example.... \n"
              << std::endl;

    std::cout << "./main 127.0.0.1 8080\n"
              << std::endl;

    return -1;
}