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
    QLearner * AI = new QLearner(game, 0.5, 5, 1, 4);
    QLearner * OPP_AI = new QLearner(game, 0.5, 2, -1, 4);


    // load data for main AI if applicable
    std::string fname = "";
    if (argc == 4) {
        fname = argv[3];
        fname += ".qtable";
        AI->loadQ(fname);
    }

    // start training our two AI against one another
    std::cout << std::endl << "\033[1;7;36mSTART TRAINING\033[0m" << std::endl;
    trainAI(AI, OPP_AI, game, n_epochs);
    std::cout << std::endl<<"\033[1;7;36mSTOP TRAINING\033[0m" << std::endl;


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
    int games_won[2] = {0, 0};
    clock_t begin_time = clock();

    // how often to print info
    int info_epochs = n_epochs / 10;

    // Play n_epochs matches in training mode
    for (int i = 0; i < n_epochs; i++) {

        // Small tool for tracking speed and progress of training
        if (i % info_epochs == 0 && i != 0) {
            float t = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
            std::cout << "\r\033[1;7;36mGAME: " << i << "/" << n_epochs << " games/sec: " <<(int)(info_epochs / t)<< "\033[0m" << std::flush;
            begin_time = clock();
        }

        // Play until a win or full board
        while (true) {
            // take turns of two players dropping a piece / checking status
            // red moves first then update board

            size_t curr_board_hash = game->getBoard();
            int move = red->makeMove(true);
            int winner = game->checkForWin();

            // update Q tables of red and black
            red->update(winner, -1, move, curr_board_hash);
            black->update(winner, -1, move, curr_board_hash);

            game->dropPiece(move, -1);

            // update board to represent the current state
            curr_board_hash = game->getBoard();

            // iff there is no winner, black makes it's move then update board
            if (!winner && !game->boardIsFull()) {
                int move = black->makeMove(true);
                winner = game->checkForWin();

                // update Q tables of red and black
                black->update(winner, 1, move, curr_board_hash);
                red->update(winner, 1, move, curr_board_hash);

                game->dropPiece(move, 1);

            }

            // On win, record, reset, and restart
            if (winner) {
                switch (winner) {
                    case -1:
                        games_won[0]++;
                    case 1:
                        games_won[1]++;
                }
                game->resetGame();
                break;
            } else if (game->boardIsFull()) {
                game->resetGame();
                break;
            }
        }

    }
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
        std::cout << "\r" << board_txt << std::flush;

        AI->showRews();
        // if red didn't win, it is blacks(user) move - get input, make move
        if (!winner && !game->boardIsFull()) {
            int move = 0;
            std::cin >> move;
            game->dropPiece(move-1, 1);
            winner = game->checkForWin();
        }
        // if a winner is found on this turn
        if (winner) {

            // print and reset board before continuing
            game->printBoard();
            game->resetGame();

            // Assign the name of the winner and print results
            std::string win_name = "Human";
            if (winner == -1) {
                win_name = "AI";
            }
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
