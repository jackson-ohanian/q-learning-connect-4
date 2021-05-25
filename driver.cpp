#include "driver.h"

/**
 * Enter here.
 * Takes command line arguments:
 * [EPOCHS] [FILTER SIZE] [opt. LOAD/SAVE FNAME (no ext.)]
 */
int main(int argc, char *argv[]) {
    std::cout << " " << std::endl;
    srand(time(NULL));

    // Parse command line args
    if (argc < 3 || argc > 4) {
        std::cout << "USAGE" << std::endl;
        std::cout << "[EPOCHS] [FILTER SIZE] [opt. LOAD/SAVE FNAME (no ext.)]" << std::endl;
        return 0;
    }
    int n_epochs = atoi(argv[1]);
    int filter_size = atoi(argv[2]);


    // The game object the Qs will play on
    Game * game = new Game();

    // Init two AI
    QLearner * AI = new QLearner(game, 0.1, 4, 1, filter_size);
    QLearner * OPP_AI = new QLearner(game, 0.1, 2, -1, filter_size);


    // load data for main AI if applicable
    std::string fname = "";
    if (argc == 4) {
        fname = argv[3];
        fname += ".txt";
        AI->loadQ(fname);
    }

    // start training our two AI against one another
    std::cout << "\033[1;36mSTART TRAINING\033[0m" << std::endl;
    trainAI(AI, OPP_AI, game, n_epochs);

    if (argc == 4) {
        AI->saveQ(fname);
    }

    // Human v Bot gameplay loop
    while (true) {
        std::cout << std::endl << "\033[1;7;4;36m hit p to play \033[0m"  << std::endl;
        char input = 0;
        std::cin >> input;
        if (input == 'p') {
            humanMatch(AI, game);
        } else {
            break;
        }
    }

    return 0;
}


/**
 * Trains two given AI against one another in a given Game.
 * @param red the winner AI (moves first)
 * @param black the loser AI (moves second)
 * @param game the Game obj. that the two AIs are playing in
 * @param n_epochs total number of epochs to train for
 * @return non-zero on error
 */
int trainAI(QLearner * red, QLearner * black, Game * game, int n_epochs) {
    clock_t begin_time = clock();
    int red_wins = 0;
    int ties = 0;
    // how often to print info
    int info_epochs = 1000;

    // Play n_epochs matches in training mode
    for (int i = 0; i < n_epochs; i++) {

        // Small tool for tracking speed and progress of training
        if (i % info_epochs == 0 && i != 0) {
            float t = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
            std::cout << "\r\033[1;36mGAME: " << i << "/" << n_epochs << " games/sec: " <<(int)(info_epochs / t)<< "\033[0m" << std::flush;
            begin_time = clock();
        }

        // Play until a win or full board
        int current_turn = 0;
        while (true) {
            // take turns of two players dropping a piece / checking status
            // red moves first then update board
            current_turn++;

            size_t curr_board_hash = game->getBoard();
            int move = red->makeMove(true);

            // only check after win is possibe, save a little time
            int winner = 0;
            if (current_turn > 8) {
                winner = game->checkForWin();
            }

            // update Q tables of red and black
            red->update(winner, -1, move, curr_board_hash);

            game->dropPiece(move, 1);

            // update board to represent the current state
            curr_board_hash = game->getBoard();

            // iff there is no winner, black makes it's move then update board
            if (!winner && !game->boardIsFull()) {
                int move = black->makeMove(true);
                if (current_turn > 8) {
                    winner = game->checkForWin();
                }

                // update Q tables of red and black
                black->update(winner, 1, move, curr_board_hash);

                game->dropPiece(move, -1);

            }

            // On win, record, reset, and restart - adjust Qs accordingly
            if (winner) {
                if (winner == 1) {
                    red_wins++;
                    black->updateLoss();
                } else {
                    red->updateLoss();
                }
                game->resetGame();
                break;
            } else if (game->boardIsFull()) {
                ties++;
                game->resetGame();
                break;
            }
        }

    }
    std::cout << std::endl<<"\033[1;36mSTOP TRAINING\033[0m";
    std::cout << std::endl<<"\033[1;36m";
    std::cout << red_wins << ":" << (n_epochs-red_wins) << ":" << ties;
    std::cout << "\033[0m" << std::endl;

    return 0;
}


/**
 * Allows manual playing against a QLearner AI object.
 * @param AI a QLearner AI, must be trained beforehand or will lose. Plays red.
 * @param game the Game obj. to play against the AI in.
 * @return non-zero on error
 */
int humanMatch(QLearner * AI, Game * game) {
    int i = 0;
    std::string board_txt = "";
    while(1) {
        i++;
        // play red (AI) move
        int AI_move = AI->makeMove(false);
        game->dropPiece(AI_move, -1);
        int winner = game->checkForWin();
        board_txt = game->printBoard();
        AI->showRews();
        std::cout << "\r" << board_txt << std::flush;

        // if red didn't win, it is blacks(user) move - get input, make move
        if (!winner && !game->boardIsFull()) {
            while (true) {
                int move = 0;
                std::cin >> move;
                // check move is valid / open position
                if (game->validMove(move-1)) {
                    std::cout << "pick valid move 1-7" << std::endl;
                    continue;
                }
                game->dropPiece(move-1, 1);
                winner = game->checkForWin();
                break;
            }
        }
        // if a winner is found on this turn
        if (winner) {
            // Assign the name of the winner and print results
            std::string win_name = "Human";
            if (winner == -1) {
                win_name = "AI";
            }
            // print and reset board before continuing
            board_txt = game->printBoard();
            std::cout << "\r" << board_txt << std::flush;
            game->resetGame();
            std::cout << "round " << i << " won by " << win_name << std::endl;
            break;

        // iff there is no winner ^ the board is full, print tie results
        } else if (game->boardIsFull()) {
            std::cout << "round " << i << " is a draw" << std::endl;
            game->resetGame();
            break;
        }
    }
    return 0;
}
