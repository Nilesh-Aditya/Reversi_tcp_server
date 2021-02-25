#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

static const uint32_t SCREEN_HEIGHT = 750u; // board size 640
static const uint32_t SCREEN_WIDTH = 640u;  // extra height cause of quit and draw buttons
static const uint32_t FPS = 30u;
static uint32_t PIECE_SIZE = SCREEN_WIDTH / 8;

bool quit = false;
void init();
void close();
void event_manager();
// void load_media();

SDL_Surface *g_screenSurface = nullptr;
SDL_Surface *g_currentSurface = nullptr;
SDL_Window *g_window = nullptr;
SDL_Renderer *g_renderer = nullptr;
SDL_Texture *backbuffer = nullptr;

void init()
{
    // SCREEN INITIALISATION
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDL_Init(SDL_INIT_VIDEO);

    g_window = SDL_CreateWindow("REVERSI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_PRESENTVSYNC || SDL_RENDERER_ACCELERATED || SDL_RENDERER_TARGETTEXTURE);

    SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(g_renderer);
    SDL_RenderPresent(g_renderer);
    IMG_Init(IMG_INIT_PNG);

    // BOARD PIECES INITIALISATION
    // board[3][3] = static_cast<uint8_t>(PIECES::WHITE);
    // board[4][4] = static_cast<uint8_t>(PIECES::WHITE);
    // board[4][3] = static_cast<uint8_t>(PIECES::BLACK);
    // board[3][4] = static_cast<uint8_t>(PIECES::BLACK);
}

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
image_texture bg_image; // board image (background image)
image_texture piece;

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
    SDL_Texture *new_texture = nullptr;
    SDL_Surface *load = IMG_Load(path.c_str());

    SDL_SetColorKey(load, SDL_TRUE, SDL_MapRGB(load->format, 0, 0xff, 0xff));

    new_texture = SDL_CreateTextureFromSurface(g_renderer, load);

    m_width = load->w;
    m_height = load->h;

    SDL_FreeSurface(load);

    m_texture = new_texture;
    return m_texture != nullptr;
}

void image_texture::render(int x, int y, SDL_Rect *clip)
{
    SDL_Rect renderQuad = {x, y, m_width, m_height};

    //Set clip rendering dimensions
    if (clip != nullptr)
    {
        std::cout << "H : " << clip->w << std::endl;
        std::cout << "W : " << clip->h << std::endl;
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopy(g_renderer, m_texture, clip, &renderQuad);
}

void image_texture::free(void)
{
    if (m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

void load_media(image_texture &image, const std::string &path, SDL_Rect *clip, int x = 0, int y = 0)
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
        load_media(bg_image, "./Assets/background.png", nullptr);
        // SDL_RenderPresent(g_renderer);
        // SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
        SDL_Rect rect = {0, 0, PIECE_SIZE - 10, PIECE_SIZE - 10};
        load_media(piece, "./Assets/black.png", &rect, 80, 80);

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

int main(int argc, char *argv[])
{
    init();
    event_manager();

    return 0;
}