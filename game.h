#pragma once

#include <map>
#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>


/**
 * Game class
 *
 * A Game object represents a board that can be played on as well as
 * a set of tools for analyzing the state of the board, and checking
 * validity of making plays on that board.
 */

class Game {
    public:
        /**
         * Constructer
         */
        Game();

       /**
        * Prints the current game board to std out
        * @return void
        */
        void printBoard();

        /**
         * Drops a piece into the board
         * @param coord the x-coordinate to drop from
         * @param player the id of the player to mark the piece
         * @return the y-coord the piece landed at, -1 on fail/full board
         */
        int dropPiece(int coord, int player);

        /**
         * Checks if there is a winner (4 in a row) on the board
         * @return the id of the winner, 0 on no winner
         */
        int checkForWin();

        /**
         * Hashes the current board uniquely for sake of Q learner
         * @return a unique hash of the board
         */
        size_t getBoard();

        /**
         * Hashes the board after a new move uniquely for sake of Q learner
         * @return a unique hash of the board
         */
        size_t getFutureBoard(int coord, int player);


        /**
         * Checks if a certain drop is valid (not full on coord)
         * @return 0 valid, -1 invalid
         */
        int validMove(int coord);

        /**
         * Reset the board to it's initial (empty) state
         * @return void
         */
        void resetGame();

        /**
         * Identify if the current board is completely full
         * @return true if full, false otherwise
         */
         bool boardIsFull();

    private:
        // Const. Width of the board
        static const int HEIGHT = 3;
        // Const. Height of the board
        static const int WIDTH = 3;
        //pieces in a row to win
        static const int TO_WIN = 3;
        // The game board used by this Game
        int* board[WIDTH];
};
