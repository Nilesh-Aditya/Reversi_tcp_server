#include <iostream>
#include "display.hpp"

// client connection address and port should be entered in terminal while runnning

int main(int argc, char const *argv[])
try
{
    Reversi::initialise_client(argv[1], argv[2]); // client init
    Reversi::init();                              // display init
    Reversi::event_manager();                     // event manager

    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << red_fg << e.what() << '\n';
    std::cerr << magenta_fg << "\n\nServer will run on PORT 8080\n"
              << std::endl;
    std::cerr << "[value should be entered in following order]" << std::endl;
    std::cerr << "./main [Ip Adress] [PORT] \n"
              << std::endl;

    std::cerr << "Example.... \n"
              << std::endl;

    std::cerr << "./main 127.0.0.1 8080\n"
              << reset << std::endl;

    return EXIT_FAILURE;
}