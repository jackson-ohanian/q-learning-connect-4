#include "game.h"

/**
 * Game class
 *
 * A Game object represents a board that can be played on as well as
 * a set of tools for analyzing the state of the board, and checking
 * validity of making plays on that board.
 */


/**
 * Game constructor
 */
Game::Game() {
    // init board to empty (0)
    for (int i = 0; i < HEIGHT; i++) {
        board[i] = new int [HEIGHT];
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = 0;
        }
    }
}


/*
* Reset the board to it's initial (empty) state
* @return void
*/
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


/**
 * Hashes the current board uniquely for sake of Q learner
 * @return a unique hash of the board
 */
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


/**
 * Hashes the board after a new move uniquely for sake of Q learner
 * @return a unique hash of the board
 */
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


/**
 * Prints the current game board to std out
 * @return void
 */
 std::string Game::printBoard() {
    std::string brd = "";
    brd += "\n";

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {

            // empty box utf iff empty
            if (board[i][j] == 0) {
                brd += "\u25a1 ";
            }

            // assign pieces marked 1 as an x
            else if (board[i][j] == 1) {
                brd += "\033[1;34m\u25c9\033[0m ";

            // assign pieces marked -1 as an o
            } else {
                brd += "\033[1;31m\u25c9\033[0m ";
            }
        }
        brd += "\n";
    }
    return brd;
}


/**
 * Checks if a certain drop is valid (not full on coord)
 * @return 0 valid, -1 invalid
 */
int Game::validMove(int coord) {
    // A move is invalid if the column is full, or if the coord is invalid
    if (coord < 0 || coord >= WIDTH || board[0][coord] != 0) {
        return -1;
    }
    // else the move is valid / return
    return 0;
}


/**
 * Drops a piece into the board
 * @param coord the x-coordinate to drop from
 * @param player the id of the player to mark the piece
 * @return the y-coord the piece landed at, -1 on fail/full board
 */
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


/**
 * Identify if the current board is completely full
 * @return true if full, false otherwise
 */
bool Game::boardIsFull() {
    for (int i = 0; i < HEIGHT; i++) {
        if (board[0][i] == 0)
            return false;
    }
    return true;
}


/**
 * Checks if there is a winner (4 in a row) on the board
 * @return the id of the winner, 0 on no winner
 */
int Game::checkForWin() {
    int current = 0;
    int streak = 0;
    // Check for horizontal 4 in a row wins
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == current && current) {
                streak++;
            } else {
                current = board[i][j]; streak = 0;
            }
            if (streak == TO_WIN - 1) {
                std::cout << "win in row " << i << std::endl;
                return current;
            }
        }
        current = 0;
        streak = 0;
    }

    // Check for vertical 4 in a row wins
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

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int cont = 0;
            int streak = 0;
            current = 0;
            //Check for SE diaganol
            while (i + cont < HEIGHT && j + cont < WIDTH) {
                if (board[i + cont][j + cont] == current && current) {
                    streak++;
                } else {
                    current = board[i + cont][j + cont]; streak = 0;
                }
                if (streak == TO_WIN - 1) {
                    return current;
                }
                cont++;
            }
            cont = 0;
            streak = 0;
            current = 0;
            // Check for NW diaganol
            while (i - cont >= 0 && j - cont >= 0) {
                if (board[i - cont][j - cont] == current && current) {
                    streak++;
                } else {
                    current = board[i - cont][j - cont]; streak = 0;
                }
                if (streak == TO_WIN - 1) {
                    return current;
                }
                cont++;
            }

        }
    }

    return 0;
}
