#include "game.h"

// Game Constructor
Game::Game() {
    // init board to 0
    for (int i = 0; i < HEIGHT; i++) {
        board[i] = new int [HEIGHT];
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = 0;
        }
    }
}

void Game::resetGame() {
    // reset all values to empty (0)
    for (int i = 0; i < HEIGHT; i++) {
        board[i] = new int [HEIGHT];
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = 0;
        }
    }
    return;
}

/** Not meant to be visually parsable - meant for Q AI */
/** Hashes board layout */
size_t Game::getBoard() {
    // using builtin std::hash, hash any board to a representative size_t
    std::hash<std::string> hash;
    std::string s = "";

    // Parse the board into a string of form s_00s_01...s_hw
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            s += std::to_string(board[i][j]);
        }
    }

    return hash(s);
}

size_t Game::getFutureBoard(int coord, int player) {
    std::hash<std::string> hash;
    std::string s = "";
    int y = dropPiece(coord, player);
    size_t hashed = getBoard();
    if (y >= 0) {
        board[y][coord] = 0;
    }
    return hashed;
}


/** Print current Game Board */
void Game::printBoard() {
    std::cout << std::endl;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {

            // empty box utf iff empty
            if (board[i][j] == 0) {
                std::cout << "\u25a1" << " ";
            }

            // assign pieces marked 1 as an x
            else if (board[i][j] == 1) {
                std::cout << "x" << " ";

            // assign pieces marked -1 as an o
            } else {
                std::cout << "o" << " ";
            }
        }
        std::cout << std::endl;
    }
    return;
}

int Game::validMove(int coord) {
    // A move is invalid if the column is full, or if the coord is invalid
    if (coord < 0 || coord >= WIDTH || board[0][coord] != 0) {
        return -1;
    }
    // else the move is valid / return
    return 0;
}

/* Drop piece off of given x coord */
/* Return y-coord placed, -1 on full */
int Game::dropPiece(int coord, int player) {
    // Column is full, return -1
    if (validMove(coord) != 0) {
        return -1;
    }

    // find bottom-most row open in column
    int bottom = 0;
    for (int i = 0; i < HEIGHT; i++) {
        if (board[i][coord] == 0) {
            bottom = i;
        }
    }

    // The piece is 'dropped' to the lowest open space in the column coord
    board[bottom][coord] = player;
    // returns the y coordinate of the piece dropped
    return bottom;
}

bool Game::boardIsFull() {
    for (int i = 0; i < HEIGHT; i++) {
        if (board[0][i] == 0)
            return false;
    }
    return true;
}

/* Check for win on current board */
int Game::checkForWin() {
    int current = 0;
    int streak = 0;
    // Check for Vertical 4 in a row wins
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == current && current) {
                streak++;
            } else {
                current = board[i][j]; streak = 0;
            }
            if (streak == TO_WIN - 1) {
                return current;
            }
        }
        current = 0;
        streak = 0;
    }

    // Check for Horizontal 4 in a row wins
    for (int j = 0; j < WIDTH; j++) {
        for (int i = 0; i < HEIGHT; i++) {
            if (board[i][j] == current && current) {
                streak++;
            } else {
                current = board[i][j]; streak = 0;
            }
            if (streak == TO_WIN - 1) {
                return current;
            }
        }
        current = 0;
        streak = 0;
    }

    return 0;
}
