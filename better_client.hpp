#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <iostream>
#include <string>
#include <future>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <sstream>
#include <vector>
#include <boost/asio.hpp>
#include "constants.hpp"
#include "color.hpp"

namespace Reversi
{

    namespace
    {
        // shortening some namespaces

        namespace asio = boost::asio;
        namespace ip = boost::asio::ip;
        using tcp = ip::tcp;

        uint32_t count{};
    }

    class Client
    {
    public:
        Client();
        ~Client();
        void init(const std::string &ip_addr, const std::string &port);
        void decode_message(const std::string &data);
        void recieve();
        void send(const std::string &message);
        void exit();
        void start_recieve();

    private:
        uint32_t id;
        std::string m_message;
        boost::system::error_code ec;
        std::thread client_thread;
        bool turn;
        std::unique_ptr<tcp::socket> __socket;
    };

    Client::Client()
    {
    }

    Client::~Client()
    {
    }

    inline void Client::init(const std::string &ip_addr, const std::string &port)
    {
        const uint32_t port_i = std::stoi(port);

        asio::io_context context;

        tcp::endpoint endpoint(asio::ip::make_address(ip_addr, ec), port_i);
        asio::io_service io_service;

        __socket = std::unique_ptr<tcp::socket>(new tcp::socket(io_service));
        (*__socket).connect(endpoint, ec);
    }

    void Client::recieve()
    {
        asio::streambuf buf;
        asio::read_until((*__socket), buf, "\n", ec);

        std::string __data = asio::buffer_cast<const char *>(buf.data());
        __data.pop_back();
        __data.pop_back();

        std::cout << "inside recieve : " << __data << std::endl;

        // std::cout << __data << count << std::endl;
        if (count == 0)
        {
            id = std::stoi(__data);

            if (id == 1)
                players = Player::player1;
            else
                players = Player::player2;

            // std::cout << "YES" << std::endl;

            count++;
        }
        else
            decode_message(__data);

        count++;
    }

    void Client::send(const std::string &message)
    {
        asio::write((*__socket), asio::buffer(message), ec);
    }

    void Client::decode_message(const std::string &data)
    {
        std::stringstream ss(data);
        std::string temp;
        std::vector<std::string> positions;

        if (data == "Exit" || data == "exit")
        {
            end = true;
            return;
        }

        if (data.length() >= 5)
        {
            while (std::getline(ss, temp, ' '))
            {
                positions.push_back(temp);
            }
        }
        else
        {
            positions.push_back(data);
        }

        for (auto x : positions)
        {
            if (x.length() > 1)
            {
                // std::cout << x << std::endl;
                uint32_t Y = x[0] - '0';
                uint32_t X = x[1] - '0';
                char color = x[2];
                PIECES temp = (color == 'B') ? PIECES::BLACK : PIECES::WHITE;

                board[Y][X] = static_cast<uint8_t>(temp);
            }
        }
    }

    void Client::start_recieve()
    {
        client_thread = std::thread([this] {
            while (!end)
            {
                recieve();
            }
        });
    }

    void Client::exit()
    {
        client_thread.join();
    }

    Client client;

} // Reversi