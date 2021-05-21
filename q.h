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

class QLearner {
    public:
        QLearner(Game * game, double a, int e, int id);
        int newTrain();

        int loadTrain();

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
        int checkQTable(size_t hash);
        void showRews();

    private:
        std::map<size_t, std::vector<float>*> table;
        size_t hashBoard();
        int greedyMove();
        Game * game;
        size_t state;
        int action;
        int id;

        // Learning rate alpha
        double alpha;

        // Discount factor gamma
        int epsilon;

        // training track
        std::ofstream save_movement;

        const int HEIGHT = 3;
        const int WIDTH = 3;


};
