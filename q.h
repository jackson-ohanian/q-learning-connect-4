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
        QLearner(Game * game, double a, int e, int id);

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
         * Save the current Q table for this AI to CSV-like file
         * @return 0 on success, non-zero on file error/fail to write
         */
        int saveQ(std::string fname);

        /**
         * Load a Q table from file, and apply to this AI. Formatted as saveQ
         * formats
         * @return a Q table
         */
        std::map<size_t, std::vector<float>*> loadQ(std::string fname);

        /**
         * Print the rewards vector for the current state to std out
         * @return void
         */
        void showRews();

    private:
        // The Q table for this QLearner
        std::map<size_t, std::vector<float>*> table;

        /**
         * Make a greedy move based on the current Q table
         * @return the coordinate to drop a piece with maximum reward
         */
        int greedyMove();

        // The game that this QLearner is playing in
        Game * game;

        // The current state of the current game
        size_t state;

        // The current (most recent) action taken by this learner
        int action;

        // The game id (-1/1) = (red/black) of this QLearner
        int id;

        // Learning rate alpha
        double alpha;

        // define epsilon greedy action with random action chance 1/epsilon
        int epsilon;

        // a file to save Q Table to
        std::ofstream save_movement;

        // The board height
        const int HEIGHT = 3;

        // The board width
        const int WIDTH = 3;


};
