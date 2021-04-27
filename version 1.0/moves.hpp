#pragma once

#include <iostream>
#include "constants.hpp"

namespace Reversi
{
    // void check_row(Player temp, int x, int y)
    // {
    //     uint32_t play = static_cast<uint8_t>(players);
    //     uint32_t col = 1;

    //     if (play == 1)
    //         col = 1;
    //     else
    //         col = 2;

    //     for (int i = x; i < 8; i++)
    //     {
    //     }
    // }

    void check_pieces(Player temp, int x, int y)
    {
        uint32_t play = static_cast<uint8_t>(temp);
        uint32_t col = 1;

        if (play == 1)
            col = 1;
        else
            col = 2;

        if (x == 5 and y == 4)
        {
            board[x - 1][y] = col;
        }

        if (x == 5 and y == 5)
        {
            board[x - 1][y - 1] = col;
        }

        if (x == 4 and y == 5)
        {
            board[x][y - 1] = col;
        }

        if (x == 3 and y == 5)
        {
            board[x][y - 1] = col;
            board[x + 1][y] = col;
        }
    }

} // Reversi namespace