#pragma once
#include <iostream>
#include "constants.hpp"
#include "image_texture.hpp"

namespace Reversi
{
    image_texture white_piece;
    image_texture black_piece;
    // void check()
    // {
    //     for (int i = 0; i < 8; i++)
    //     {
    //         for (int j = 0; j < 8; j++)
    //         {
    //             if (board[i][j] == static_cast<int>(PIECES::BLACK))
    //             {
    //             }
    //         }

    //         std::cout << '\n';
    //     }
    // }

    void load_pieces()
    {
        for (int i = 0; i < board.size(); i++)
        {
            for (int j = 0; j < board[i].size(); j++)
            {
                if (board[i][j] == static_cast<uint8_t>(PIECES::BLACK))
                {
                    SDL_Rect rect = {0, 0, PIECE_SIZE, PIECE_SIZE};
                    load_media(black_piece, "./Assets/black.png", &rect, i * PIECE_SIZE, j * PIECE_SIZE);
                }
                else if (board[i][j] == static_cast<uint8_t>(PIECES::WHITE))
                {
                    SDL_Rect rect = {0, 0, PIECE_SIZE, PIECE_SIZE};
                    load_media(white_piece, "./Assets/white.png", &rect, i * PIECE_SIZE, j * PIECE_SIZE);
                }
            }
        }
    }
};