#pragma once

#include "image_texture.hpp"
#include "constants.hpp"
#include "game.hpp"

namespace Reversi
{

    bool quit = false;
    void init();
    void close();
    void event_manager();
    // void load_media();

    SDL_Surface *g_screenSurface = nullptr;
    SDL_Surface *g_currentSurface = nullptr;
    SDL_Window *g_window = nullptr;
    SDL_Texture *g_backbuffer = nullptr;

    image_texture bg_image; // board image (background image)

    void init()
    {
        // SCREEN INITIALISATION
        SDL_Init(SDL_INIT_VIDEO);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

        g_window = SDL_CreateWindow("REVERSI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_PRESENTVSYNC || SDL_RENDERER_ACCELERATED || SDL_RENDERER_TARGETTEXTURE);
        g_backbuffer = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 750);

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
            if (SDL_PollEvent(&e) != 0)
            {
                //User requests quit
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    close();
                }
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

    void close()
    {
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