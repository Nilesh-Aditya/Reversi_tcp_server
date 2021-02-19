#pragma once

#include "image_texture.hpp"

namespace Reversi
{
    class create_window
    {

    public:
        create_window();
        ~create_window();
        void close();

    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;
    };

    create_window::create_window() : m_window(nullptr), m_texture(nullptr), m_renderer(nullptr)
    {
    }

    create_window::~create_window()
    {
    }

} // namespace Reversi