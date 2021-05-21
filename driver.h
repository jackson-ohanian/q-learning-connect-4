#pragma once

#include "game.h"
#include "game.cpp"
#include <iostream>
#include "q.h"
#include "q.cpp"
#include <ctime>

/**
 * Trains two given AI against one another in a given Game.
 * @param red the winner AI (moves first)
 * @param black the loser AI (moves second)
 * @param game the Game obj. that the two AIs are playing in
 * @param n_epochs total number of epochs to train for
 * @return non-zero on error
 */
int trainAI(QLearner * red, QLearner * black, Game * game, int n_epochs);

/**
 * Allows manual playing against a QLearner AI object.
 * @param AI a QLearner AI, must be trained beforehand or will lose. Plays red.
 * @param game the Game obj. to play against the AI in.
 * @return non-zero on error
 */
int humanMatch(QLearner * AI, Game * game);
