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
        void init(const std::string &s1, const std::string &s2); // init port and ip_address
        void connect();
        bool error_handling();
        bool check_message(tcp::socket &socket_);
        virtual ~CLIENT();

    private:
        std::string message;
        std::string port;
        std::string ip_address;
        boost::system::error_code ec;
    };

    CLIENT::CLIENT()
    {
    }

    CLIENT::~CLIENT()
    {
    }

    void CLIENT::init(const std::string &s1, const std::string &s2)
    {
        port = s2;
        ip_address = s1;
    }

    bool CLIENT::check_message(tcp::socket &socket_)
    {
        asio::streambuf buf;
        asio::read_until(socket_, buf, "\n", ec);

        if (!error_handling())
            return false;

        std::string data = asio::buffer_cast<const char *>(buf.data());
        data.pop_back();
        std::cout << data << std::endl;

        if (data == "exit" || data == "EXIT" || data == "Exit")
            return false;

        return true;
    }

    bool CLIENT::error_handling()
    {
        if (ec)
        {
            std::cout << ec.message() << std::endl;
            return false;
        }
        return true;
    }

    void CLIENT::connect()
    try
    {
        const int port_i = std::stoi(port);

        asio::io_context context;

        tcp::endpoint endpoint(asio::ip::make_address(ip_address, ec), port_i);
        if (!error_handling())
            return;

        asio::io_service io_service;

        tcp::socket socket_(io_service);
        socket_.connect(endpoint, ec);

        while (true)
        {
            // input message
            std::cin.clear();
            message.clear();
            std::getline(std::cin, message);
            asio::write(socket_, asio::buffer(message), ec);
            if (!error_handling())
                return;

            if (!error_handling())
                return;
            if (!check_message(socket_))
                break;
        }

        if (!error_handling())
            throw "boost client ec error has occurred  !!!";
    }
    catch (const char *c)
    {
        std::cout << c << std::endl;
    }
    catch (...)
    {
        std::cout << "UNEXPECTED ERROR HAS OCCURED" << std::endl;
    }
}