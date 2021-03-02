#pragma once

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
    using tcp = ip::tcp;

    class CLIENT
    {
    public:
        CLIENT();
        void init();
        void connect();
        void get_ip(const std::string &s) { this->ip_address = s; }
        void get_port(const std::string &s) { this->port = s; }
        std::string get_message();
        std::string send_message();
        virtual ~CLIENT();

    private:
        std::string message;
        std::string port;
        std::string ip_address;
    };

    CLIENT::CLIENT()
    {
    }

    CLIENT::~CLIENT()
    {
    }

    void CLIENT::connect()
    {
        const int port_i = std::stoi(port);
        boost::system::error_code ec;

        asio::io_context context;

        tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), port_i);

        asio::io_service io_service;

        tcp::socket socket_(io_service);
        socket_.connect(endpoint, ec);

        asio::write(socket_, asio::buffer(message), ec);

        asio::streambuf buf;
        asio::read_until(socket_, buf, "\n", ec);

        if (ec)
        {
            std::cout << ec.message() << std::endl;
            return;
        }

        std::string data = asio::buffer_cast<const char *>(buf.data());

        std::cout << data << std::endl;
    }
}