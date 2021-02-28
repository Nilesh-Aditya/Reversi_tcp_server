#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/system/error_code.hpp>

namespace Reversi
{
    namespace asio = boost::asio;
    namespace ip = boost::asio::ip;

    void network()
    {
        const int port = 8080;
        boost::system::error_code ec;

        asio::io_context context;

        asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), port);

        asio::io_service io_service;

        asio::ip::tcp::socket socket_(io_service);
        socket_.connect(endpoint, ec);

        asio::streambuf buf;
        asio::read(socket_, buf, asio::transfer_all(), ec);

        if (ec)
        {
            std::cout << ec.message() << std::endl;
            return;
        }

        std::string data = asio::buffer_cast<const char *>(buf.data());

        std::cout << data << std::endl;
    }
}