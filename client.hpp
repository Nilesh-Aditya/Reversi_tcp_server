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

        int cnt = 0; // keeping track of client runs
    }

    class CLIENT
    {
    public:
        CLIENT();
        void init(const std::string &s1, const std::string &s2); // init port and ip_address
        void connect();
        bool error_handling();
        void send_data(tcp::socket &);
        void set_data(const std::string &msg);
        std::string check_message(tcp::socket &socket_);
        void start();
        void exit();
        virtual ~CLIENT();

    private:
        int id;
        std::string m_message;
        std::string port;
        std::string ip_address;
        std::string m_data;
        boost::system::error_code ec;
        std::thread new_thread;
        bool turn;
    };

    CLIENT::CLIENT()
    {
    }

    CLIENT::~CLIENT()
    {
    }

    inline void CLIENT::init(const std::string &s1, const std::string &s2)
    {
        port = s2;
        ip_address = s1;
    }

    std::string CLIENT::check_message(tcp::socket &socket_)
    try
    {
        asio::streambuf buf;
        asio::read_until(socket_, buf, "\n", ec);

        if (!error_handling())
            return "false";

        std::string __data = asio::buffer_cast<const char *>(buf.data());
        __data.pop_back();
        __data.pop_back();
        std::cout << __data << std::endl;
        std::cout << "Reading some data  :  : : please work" << std::endl;

        if (__data == "1" && cnt == 0)
            players = Player::player1;
        if (__data == "2" && cnt == 0)
            players = Player::player2;

        cnt++; // cnt(int)=0 declared at the top

        return __data;
    }
    catch (...)
    {
        std::cerr << magenta_fg << "MESSAGE READING ERROR!!!!" << reset << std::endl;
    }

    inline void CLIENT::send_data(tcp::socket &socket_)
    {
        m_message = message;
        asio::write(socket_, asio::buffer(message), ec);
    }

    void CLIENT::set_data(const std::string &msg)
    try
    {
        message = msg;
        m_message = message;

        int X{}, Y{};

        if (msg.length() >= 3)
        {
            int t1 = std::stoi(msg);
            X = t1 / 10;
            Y = t1 % 10;
        }
        else
        {
            X = 100;
            Y = 100;
        }

        Player temp = (players == Player::player1) ? Player::player2 : Player::player1;
        if (X < 8 && X >= 0 && Y < 8 && Y >= 0)
            board[X][Y] = static_cast<uint8_t>(temp);
        else if (cnt == 1)
        {
            id = std::stoi(msg);

            if (id == 1)
            {
                recv = true;
                send = false;
                players = Player::player1;
            }
            else
            {
                recv = false;
                send = true;
                players = Player::player2;
            }
            std::cout << green_fg << "CLIENTS HAVE CONNECTED SUCCESSFULLY" << reset << std::endl;
        }
        else
            throw "INVALID DATA SEND";
    }
    catch (const char *c)
    {
        std::cerr << magenta_fg << c << reset << std::endl;
    }
    catch (...)
    {
        std::cout << red_fg << "OUT OF THE WORLD ERROR" << reset << std::endl;
    }

    inline bool CLIENT::error_handling()
    {
        if (ec)
        {
            std::cout << magenta_fg << '\n'
                      << ec.message() << reset << std::endl;
            return false;
        }
        return true;
    }

    void CLIENT::start()
    try
    {
        new_thread = std::thread([this] { connect(); });
    }
    catch (std::system_error &e)
    {
        std::cerr << red_fg << '\n'
                  << e.what() << reset << std::endl;
        std::cout << magenta_fg << "\nTHREAD START(start fn) ERROR INSIDE (CLIENT class)" << reset << '\n'
                  << std::endl;
    }

    void CLIENT::connect()
    try
    {
        const int port_i = std::stoi(port);

        asio::io_context context;

        tcp::endpoint endpoint(asio::ip::make_address(ip_address, ec), port_i);
        if (!error_handling())
        {
            throw "boost client ec error has occurred  !!!";
        }

        asio::io_service io_service;

        tcp::socket socket_(io_service);
        socket_.connect(endpoint, ec);

        while (!end)
        {

            std::async(std::launch::async, [&] {
                std::unique_lock<std::mutex> lk2(global_mutex);

                global_status.wait(lk2, [] { return recv && !send; });

                std::string temp = check_message(socket_); // getting data and checking the data

                // if (temp.length() >= 3)

                set_data(temp);

                if (temp == "false")
                {
                    global_status.notify_all();
                    throw "boost client ec error has occurred  !!!";
                }
                if (temp == "exit" || temp == "EXIT" || temp == "Exit" or end)
                {
                    global_status.notify_all();
                    std::cout << green_fg << "Exited  the client !!! " << reset << std::endl;
                    return;
                }

                lk2.unlock();
                global_status.notify_one();
                // recv = false;
                // send = true;
            });

            std::cout << "reached middle point : "
                         "hello there "
                      << std::endl;

            std::async(std::launch::async, [&] {
                std::unique_lock<std::mutex> lk(global_mutex);
                // m_message.clear();

                global_status.wait(lk, [] { return !recv && send; });

                // asio::write(socket_, asio::buffer(m_message), ec);
                send_data(socket_);

                recv = true;
                send = false;

                if (!error_handling())
                {
                    throw "boost client ec error has occurred  !!!";
                }
                lk.unlock();
                global_status.notify_one();
            });

            std::cout << "reached end point  : "
                         "hi bro"
                      << std::endl;
        }
    }
    catch (const char *c)
    {
        std::cerr << red_fg << c << '\n'
                  << reset << std::endl;
        exit();
    }
    catch (...)
    {
        std::cout << red_fg << "\nUNEXPECTED ERROR HAS OCCURED !!!\n"
                  << reset << std::endl;
        exit();
    }

    void CLIENT::exit()
    try
    {
        {
            std::unique_lock<std::mutex> lk(global_mutex);
            end = true;
            recv = false;
            send = false;
        }
        global_status.notify_all();
        new_thread.join();
    }
    catch (std::logic_error &e)
    {
        std::cerr << red_fg << '\n'
                  << e.what() << reset << std::endl;
    }
    catch (...)
    {
        std::cout << magenta_fg << "EXIT(error)_" << reset;
        std::cout << red_fg << "\nTrying to Exit thread without connecting it to first !!\n"
                  << reset << std::endl;
    }

    //  creating my client
    CLIENT client;
}