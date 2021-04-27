#pragma once

#include <iostream>
#include "constants.hpp"
#include "image_texture.hpp"
#include "color.hpp"

namespace Reversi
{
    image_texture white_piece;
    image_texture black_piece;

    bool is_valid_index(int, int);
    bool is_neighbour(int, int);
    int set_dirX(int);
    int set_dirY(int);
    bool is_at_curr_pos(int, int);
    int no_of_flipped_discs(PIECES state, int curr_y, int curr_x, int dir_y, int dir_x, int flipped_discs = 0);
    void check(PIECES, int, int);
    void declare_winner();
    bool is_valid_move(PIECES, int, int);
    void flip_discs(PIECES, int, int, int, int);
    void load_pieces();

    namespace
    {
        int white_disks{};
        int black_disks{};
        bool dirs[9];
    }

    inline bool is_valid_index(int y, int x)
    {
        if (y < 0 || x < 0 || y >= breadth || x >= length)
            return false;

        return true;
    }

    bool is_neighbour(int y, int x)
    {
        for (int j = -1; j <= 1; j++)
        {
            for (int i = -1; i <= 1; i++)
            {
                if (i == 0 && j == 0)
                    continue;
                if (!is_valid_index(y + j, x + i))
                    continue;
                if (board[y + j][x + i] == static_cast<uint8_t>(PIECES::EMPTY))
                    return true;
            }
        }

        return false;
    }

    void check(PIECES state, int y, int x)
    {
        if (is_valid_index(y, x))
        {
            if (is_valid_move(state, y, x))
            {
                for (int i = 0; i < 9; i++)
                {
                    if (dirs[i])
                    {
                        int dir_y = set_dirY(i);
                        int dir_x = set_dirX(i);
                        flip_discs(state, y + dir_y, x + dir_x, dir_y, dir_x);
                    }
                }
            }
        }
    }

    void declare_winner()
    {
        // check();
        if (white_disks > black_disks)
            std::cout << green_fg << "White Player won !!!" << std::endl;
        else
            std::cout << green_fg << "Black Player won !!!" << std::endl;
    }

    bool is_valid_move(PIECES state, int y, int x)
    {
        if (board[y][x] != static_cast<uint8_t>(PIECES::EMPTY))
            return false;
        if (!is_neighbour(y, x))
            return false;

        PIECES temp = state;
        int count{};
        bool dirs_copy[9];

        for (int j = -1; j <= 1; ++j)
        {
            for (int i = -1; i <= 1; ++i)
            {
                if (j == 0 && i == 0)
                {
                    dirs_copy[count++] = false;
                    continue;
                }

                if (is_valid_index(y + j, x + i))
                {
                    if (board[y + j][x + i] == static_cast<uint8_t>(PIECES::EMPTY))
                        dirs_copy[count] = false;
                    else if (board[y + j][x + i] == static_cast<uint8_t>(temp))
                        dirs_copy[count] = false;
                    else
                        dirs_copy[count] = true;
                }

                else
                    dirs_copy[count] = false;
                ++count;
            }
        }

        int search_dir_x, search_dir_y;
        bool flag = false;

        for (int k = 0; k < 9; k++)
        {
            if (dirs_copy[k])
            {
                search_dir_x = set_dirX(k);
                search_dir_y = set_dirY(k);

                if (no_of_flipped_discs(temp, y + search_dir_y, x + search_dir_x, search_dir_y, search_dir_x, 0) > 0)
                {
                    flag = true;
                }
            }
        }

        if (is_at_curr_pos(y, x))
        {
            for (int i = 0; i < 9; i++)
            {
                dirs[i] = dirs_copy[i];
            }
        }

        return flag;
    }

    void flip_discs(PIECES state, int curr_y, int curr_x, int dir_y, int dir_x)
    {
        if (board[curr_y][curr_x] == static_cast<uint8_t>(state))
            return;
        PIECES temp = static_cast<PIECES>(board[curr_y][curr_x]);
        flip(temp);
        board[curr_y][curr_x] = static_cast<uint8_t>(temp);

        if (state == PIECES::WHITE)
        {
            ++white_disks;
            --black_disks;
        }
        else if (state == PIECES::BLACK)
        {
            ++black_disks;
            --white_disks;
        }

        flip_discs(state, curr_y + dir_y, curr_x + dir_x, dir_y, dir_x);
    }

    inline int set_dirY(int k)
    {
        switch (k)
        {
        case 0:
        case 1:
        case 2:
            return -1;
        case 3:
        case 5:
            return 0;
        case 6:
        case 7:
        case 8:
            return +1;
        }
        return 0;
    }

    inline int set_dirX(int k)
    {
        switch (k)
        {
        case 0:
        case 3:
        case 6:
            return -1;
        case 1:
        case 7:
            return 0;
        case 2:
        case 5:
        case 8:
            return +1;
        }
        return 0;
    }

    inline bool is_at_curr_pos(int j, int i)
    {
        return i == coordinates[0] && j == coordinates[1];
    }

    int no_of_flipped_discs(PIECES state, int curr_y, int curr_x, int dir_y, int dir_x, int flipped_discs)
    {
        if (!is_valid_index(curr_y, curr_x))
            return 0;
        if (board[curr_y][curr_x] == static_cast<uint8_t>(PIECES::EMPTY))
            return 0;
        if (board[curr_y][curr_x] == static_cast<uint8_t>(state))
            return flipped_discs;
        else
            return no_of_flipped_discs(state, curr_y + dir_y, curr_x + dir_x, dir_y, dir_x, flipped_discs + 1);
    }

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