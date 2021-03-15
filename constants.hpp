#pragma once

#include <cstdint>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>
#include <condition_variable>

namespace Reversi
{
    static constexpr const uint32_t PADDING = 10u;
    static constexpr const uint32_t BLOCK_SIZE = 105u;
    static constexpr const uint32_t SCREEN_HEIGHT = 750u; // board size 640
    static constexpr const uint32_t SCREEN_WIDTH = 640u;  // extra height cause of quit and draw buttons
    static constexpr const uint32_t FPS = 30u;
    static constexpr const uint32_t PIECE_SIZE = SCREEN_WIDTH / 8;

    // mouse click event co-ordinates
    std::array<int, 2> coordinates;

    // thread constants
    std::mutex global_mutex;
    std::mutex files_lock;
    std::condition_variable global_status;
    std::atomic<bool> recv = true;
    std::atomic<bool> send = false;
    bool end = false;

    // client constants
    std::string message;
    std::string data;

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