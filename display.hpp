#pragma once

#include "image_texture.hpp"
#include "constants.hpp"

namespace Reversi
{

    bool quit = false;
    void init();
    void close();
    void event_manager();
    void load_media();

    SDL_Surface *g_screenSurface = nullptr;
    SDL_Surface *g_currentSurface = nullptr;
    SDL_Window *g_window = nullptr;

    image_texture bg_image; // board image (background image)

    void init()
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        SDL_Init(SDL_INIT_VIDEO);

        g_window = SDL_CreateWindow("REVERSI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);
        SDL_RenderPresent(g_renderer);
        IMG_Init(IMG_INIT_PNG);
    }

    void load_media(image_texture &image, const std::string &path, int x = 0, int y = 0, SDL_Rect *clip = nullptr)
    {
        image.load_image(path);
        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);

        image.render(x, y, clip);
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
            load_media(bg_image, "./Assets/background.png");
            SDL_RenderPresent(g_renderer);
        }
    }

    void close()
    {
        bg_image.free();
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        g_window = nullptr;
        g_renderer = nullptr;
        IMG_Quit();
        SDL_Quit();
    }
} // namespace Reversi