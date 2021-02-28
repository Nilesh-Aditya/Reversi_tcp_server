#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace Reversi
{
    static const uint32_t PADDING = 10u;
    static const uint32_t BLOCK_SIZE = 105u;
    static const uint32_t SCREEN_HEIGHT = 750u; // board size 640
    static const uint32_t SCREEN_WIDTH = 640u;  // extra height cause of quit and draw buttons
    static const uint32_t FPS = 30u;
    static const uint32_t PIECE_SIZE = SCREEN_WIDTH / 8;

    std::vector<std::vector<int>> board(8, std::vector<int>(8, 0));

    enum class Player : uint8_t
    {
        player1 = 1u,
        player2 = 2u
    };

    Player players = Player::player1;

    enum class PIECES : uint8_t
    {
        EMPTY = 0u,
        BLACK = 1u,
        WHITE = 2u
    };

    PIECES piece = PIECES::EMPTY;
} // namespace Reversi