#pragma once

#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif

#include <string>

namespace Reversi
{
    SDL_Renderer *g_renderer = nullptr;

    class image_texture
    {
    public:
        image_texture(/* args */);
        ~image_texture();
        void free();
        bool load_image(const std::string &path);
        void render(int x, int y, SDL_Rect *clip);

    private:
        SDL_Texture *m_texture;
        int m_width;
        int m_height;
    };

    image_texture::image_texture() : m_texture(nullptr), m_width(0), m_height(0)
    {
    }

    image_texture::~image_texture()
    {
        free();
    }

    bool image_texture::load_image(const std::string &path)
    {
        free();
        SDL_Surface *load = IMG_Load(path.c_str());

        SDL_SetColorKey(load, SDL_TRUE, SDL_MapRGB(load->format, 0, 0xff, 0xff));

        m_texture = SDL_CreateTextureFromSurface(g_renderer, load);
        SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

        m_width = load->w;
        m_height = load->h;

        SDL_FreeSurface(load);

        return m_texture != nullptr;
    }

    void image_texture::render(int x, int y, SDL_Rect *clip)
    {
        SDL_Rect renderQuad = {x, y, m_width, m_height};

        //Set clip rendering dimensions
        if (clip != nullptr)
        {
            // little debugging
            // std::cout << "H : " << clip->w << std::endl;
            // std::cout << "W : " << clip->h << std::endl;
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        //Render to screen
        SDL_RenderCopy(g_renderer, m_texture, clip, &renderQuad);
    }

    inline void image_texture::free(void)
    {
        if (m_texture != nullptr)
        {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }
    }

    void load_media(image_texture &image, const std::string &path, SDL_Rect *clip, int x = 0, int y = 0)
    {
        if (image.load_image(path))
            image.render(x, y, clip);
    }

} // namespace Reversi