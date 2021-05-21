#include "driver.h"

int main(int argc, char *argv[]) {

    srand(time(NULL));

    // Parse command line args
    if (argc != 5) {
        std::cout << "USAGE" << std::endl;
        std::cout << "[WIDTH] [HEIGHT] [PIECES TO WIN] [EPOCHS]" << std::endl;
        return 0;
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int to_win = atoi(argv[3]);
    int n_epochs = atoi(argv[4]);;

    // The game object the Qs will play on
    Game * game = new Game();

    // Init two AI
    QLearner * AI = new QLearner(game, 0.5, 10, 1);
    QLearner * OPP_AI = new QLearner(game, 0.5, 2, -1);

    // start training our two AI against one another
    std::cout << std::endl << "start training for " << n_epochs << " epochs..." << std::endl;

    trainAI(AI, OPP_AI, game, n_epochs);
    AI->saveQ("AI_MAIN.txt");

    std::cout << std::endl << "select 1/2 close/play" << std::endl;
    int input = 0;
    std::cin >> input;
    if (input == 2) {
        humanMatch(AI, game);
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
    float info_epochs = 10000;

    // Play n_epochs matches in training mode
    for (int i = 0; i < n_epochs; i++) {

        // Small tool for tracking speed and progress of training
        if (i % 10000 == 0 && i != 0) {
            float t = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
            std::cout << "game: " << i << "/" << n_epochs << " games/sec: " <<(int)(info_epochs / t)<< std::endl;
            begin_time = clock();
        }

        // Play until a win or full board
        while (true) {
            // take turns of two players dropping a piece / checking status
            // red moves first then update board
            size_t curr_board_hash = game->getBoard();
            int move = red->makeMove(true);
            game->dropPiece(move, -1);
            int winner = game->checkForWin();

            // update Q tables of red and black
            red->update(winner, -1, move, curr_board_hash);
            black->update(winner, -1, move, curr_board_hash);

            // update board to represent the current state
            curr_board_hash = game->getBoard();

            // iff there is no winner, black makes it's move then update board
            if (!winner && !game->boardIsFull()) {
                int move = black->makeMove(true);
                game->dropPiece(move, 1);
                winner = game->checkForWin();

                // update Q tables of red and black
                black->update(winner, 1, move, curr_board_hash);
                red->update(winner, 1, move, curr_board_hash);
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
    std::cout << std::endl << "training complete... " << games_won[0] << ":" << games_won[1] << std::endl;
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
    while(1) {
        i++;
        // play red (AI) move
        game->dropPiece(AI->makeMove(false), -1);
        int winner = game->checkForWin();
        game->printBoard();

        // if red didn't win, it is blacks(user) move - get input, make move
        if (!winner && !game->boardIsFull()) {
            int move = 0;
            std::cin >> move;
            game->dropPiece(move, 1);
            winner = game->checkForWin();
        }
        AI->showRews();
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
