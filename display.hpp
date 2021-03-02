#pragma once

#include "image_texture.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "client.hpp"
#include <array>

namespace Reversi
{
    //  creating my client
    CLIENT client;
    void initialise_client(const std::string &s1, const std::string &s2)
    {
        client.connect();
    }

    // display functions
    bool quit = false;
    void init();
    void close();
    void event_manager();
    void mouse_event(SDL_Event &e);

    // Get mouse positions (x, y)
    std::array<int, 2> mouse_press(SDL_MouseButtonEvent &b)
    {
        int x{}, y{};
        if (b.button == SDL_BUTTON_LEFT)
        {
            SDL_GetMouseState(&x, &y);
        }
        return {x, y};
    }

    SDL_Surface *g_screenSurface = nullptr;
    SDL_Surface *g_currentSurface = nullptr;
    SDL_Window *g_window = nullptr;
    SDL_Texture *g_backbuffer = nullptr;

    // renderer is initialised in image_texture.hpp

    image_texture bg_image; // board image (background image)

    // init funtion

    void init()
    {
        // SCREEN INITIALISATION
        SDL_Init(SDL_INIT_VIDEO);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

        g_window = SDL_CreateWindow("REVERSI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_PRESENTVSYNC || SDL_RENDERER_ACCELERATED || SDL_RENDERER_TARGETTEXTURE);
        g_backbuffer = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);
        // SDL_RenderPresent(g_renderer);
        IMG_Init(IMG_INIT_PNG);

        // BOARD PIECES INITIALISATION
        board[3][3] = static_cast<uint8_t>(PIECES::WHITE);
        board[4][4] = static_cast<uint8_t>(PIECES::WHITE);
        board[4][3] = static_cast<uint8_t>(PIECES::BLACK);
        board[3][4] = static_cast<uint8_t>(PIECES::BLACK);
    }

    void event_manager()
    {
        SDL_Event e;
        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                // checking mouse events (SDL quit inside mouse events)
                mouse_event(e);
            }
            SDL_SetRenderTarget(g_renderer, g_backbuffer);
            SDL_RenderClear(g_renderer);
            load_media(bg_image, "./Assets/background.png", nullptr);
            load_pieces();
            SDL_SetRenderTarget(g_renderer, nullptr);
            SDL_RenderCopy(g_renderer, g_backbuffer, nullptr, nullptr);
            SDL_RenderPresent(g_renderer);
        }
    }

    void mouse_event(SDL_Event &e)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            quit = true;
            close();
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            auto [x, y] = mouse_press(e.button);
            const int X = x / PIECE_SIZE;
            const int Y = y / PIECE_SIZE;

            if (X < 8 and Y < 8 and X >= 0 and Y >= 0)
            {
                if (players == Player::player1)
                    piece = PIECES::BLACK;
                else
                    piece = PIECES::WHITE;
                board[X][Y] = static_cast<uint8_t>(piece);
            }
            else
            {
                std::cout << "X : " << X << " Y : " << Y << '\n';
            }
        }
    }

    void close()
    {
        black_piece.free();
        white_piece.free();
        bg_image.free();
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        SDL_DestroyTexture(g_backbuffer);
        g_window = nullptr;
        g_renderer = nullptr;
        IMG_Quit();
        SDL_Quit();
    }
} // namespace Reversi