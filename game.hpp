#pragma once

#include <iostream>
#include "constants.hpp"
#include "image_texture.hpp"
#include "color.hpp"

namespace Reversi
{
    image_texture white_piece;
    image_texture black_piece;

    void load_pieces()
    {
        for (int i = 0; i < board.size(); i++)
        {
            for (int j = 0; j < board[i].size(); j++)
            {
                if (board[i][j] == static_cast<uint8_t>(PIECES::BLACK))
                {
                    SDL_Rect rect = {0, 0, PIECE_SIZE, PIECE_SIZE};
                    load_media(black_piece, "./Assets/black.png", &rect, j * PIECE_SIZE, i * PIECE_SIZE);
                }
                else if (board[i][j] == static_cast<uint8_t>(PIECES::WHITE))
                {
                    SDL_Rect rect = {0, 0, PIECE_SIZE, PIECE_SIZE};
                    load_media(white_piece, "./Assets/white.png", &rect, j * PIECE_SIZE, i * PIECE_SIZE);
                }
            }
        }
    }
};