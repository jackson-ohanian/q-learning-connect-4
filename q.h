#pragma once

#include <map>
#include <iostream>
#include <iomanip>
#include <string>
#include <functional>
#include <random>
#include "game.h"
#include <time.h>
#include "fstream"


/**
 * QLearner class
 *
 * A QLearner object represents a policy reinforcement Q learner that
 * makes moves based on the dominant outcome reward for any given Game
 * objects Game::getBoard.
 */

class QLearner {
    public:
        /**
         * QLearner Constructor
         */
        QLearner(Game * game, double a, int e, int id, int fsize);

        /**
         * Have this AI make a move based on the current state, training
         * follows epsilon greedy training, validation / gameplay is 100%
         * greedy. To run validation in epsilon greedy just don't call updates
         * and set epsilon.
         * @param train true for training / false for gameplay / validation
         * @return the coord to drop at (pass to Game obj.)
         */
        int makeMove(bool train);

        /**
         * Update the Q table for this player based on the current state.
         * @param winner the winner of this round, 0 if no winner
         * @param player the player who made the new move
         * @param move the coordinate the piece was dropped at
         * @param hash a hash of the current state
         * @return the reward function value of the new state
         */
        int update(int winner, int player, int move, size_t hash);

        /**
         * Save the current Q table for this AI to CSV-like file.
         * (comma seperated values in newline seperated states)
         * @return 0 on success, non-zero on file error/fail to write
         */
        int saveQ(std::string fname);

        /**
         * Load a Q table from file, and apply to this AI. Formatted as saveQ
         * formats (comma seperated values in newline seperated states)
         * @return a Q table
         */
        int loadQ(std::string fname);

        /**
         * Print the rewards vector for the current state to std out
         * @return void
         */
        void showRews();

        /**
         * Update a loss on this player
         * @return void
         */
        void updateLoss();

    private:
        // The Q table for this QLearner
        std::map<size_t, std::vector<float>*> table;

        /**
         * Make a greedy move based on the current Q table
         * @return the coordinate to drop a piece with maximum reward
         */
        int greedyMove();

        /**
         * Creates hashes of the filter applied to each possible location
         * on the board.
         * @return an array of hashes in L->R T->D order
         */
        size_t* convGreedyDecider();

        /**
         * Find the best move for the current sub-state
         * @return the best (most rewarded) move
         */
        int bestFromState(size_t state, float target, int left_pos);

        /**
         * Make a hash for a board at the given position
         * return the hash
         */
        size_t getSubHash(int i, int j, int board[6][7]);

        // The game that this QLearner is playing in
        Game * game;
        // A temporary future
        Game * fut_game;
        // The current state of the current game
        size_t state;
        // The current (most recent) action taken by this learner
        int action;
        // The game id (-1/1) = (red/black) of this QLearner
        int id;
        // int hash index / locs
        int hash_loc;
        // Learning rate alpha
        double alpha;
        // define epsilon greedy action with random action chance 1/epsilon
        int epsilon;
        // total filters on the convulation
        int total_filters;
        // a file to save Q Table to
        std::ofstream save_movement;
        // The board height
        const int HEIGHT = 6;
        // The board width
        const int WIDTH = 7;
        // The maximum reward in the current state
        float max_reward;
        // The size of the filters used
        int filter_size;
        // The relative action taken in the current sub-state
        int relative_action;

        // the locations of each current sub-state
        int* sub_state_locations_x;
        int* sub_state_locations_y;


};
