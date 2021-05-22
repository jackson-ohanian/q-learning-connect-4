#include "q.h"


/**
 * QLearner class
 *
 * A QLearner object represents a policy reinforcement Q learner that
 * makes moves based on the dominant outcome reward for any given Game
 * objects Game::getBoard.
 */


/**
 * QLearner Constructor
 */
QLearner::QLearner(Game * game, double a, int e, int id, int fsize) {
    this->game = game;
    this->alpha = a;
    this->epsilon = e;
    this->action = 0;
    this->state = 0;
    this->id = id;
    this->filter_size = fsize;
}

/**
 * Have this AI make a move based on the current state, training
 * follows epsilon greedy training, validation / gameplay is 100%
 * greedy. To run validation in epsilon greedy just don't call updates
 * and set epsilon.
 * @param train true for training / false for gameplay or validation
 * @return the coord to drop at (pass to Game obj.)
 */
int QLearner::makeMove(bool train) {
    if (rand()%this->epsilon != 0 || !train) {
        return this->greedyMove();
    } else {
        this->action = rand() % WIDTH;
        return this->action;
    }
}


/**
 * Make a greedy move based on the current Q table (private)
 * @return the coordinate to drop a piece with maximum reward
 */
int QLearner::greedyMove() {
    this->max_reward = 0.0;
    convGreedyDecider();
    size_t* hashes = convGreedyDecider();

    float max_so_far = -100.0;
    int to_drop = 0;

    // track the position
    int left_pos = 0;

    for (int ix = 0; ix < 12; ix++) {
        left_pos = this->sub_state_locations[ix];
        int act = bestFromState(hashes[ix], max_so_far);
        if (game->validMove(left_pos + act)) {
            continue;
        }
        if (this->max_reward > max_so_far) {
            to_drop = act + left_pos;
            this->relative_action = act;
            this->state = hashes[ix];
            max_so_far = this->max_reward;
        }
    }

    this->action = to_drop;
    return to_drop;
}


/**
 * Update the Q table for this player based on the current state.
 * @param winner the winner of this round, 0 if no winner
 * @param player the player who made the new move
 * @param move the coordinate the piece was dropped at
 * @param hash a hash of the current state
 * @return the reward function value of the new state
 */
int QLearner::update(int winner, int player, int move, size_t hash) {
    if (move == -1) {
        return -1;
    }
    // update the q table
    size_t state = this->state;
    size_t fut_state = game->getFutureBoard(move, player);
    // Get old reward of this state / init if empty
    if (!table.count(fut_state)) {
        this->table[fut_state]= new std::vector<float>(this->filter_size, (rand() % 100) * 0.01);
    }
    if (!table.count(state)) {
        this->table[state]= new std::vector<float>(this->filter_size, (rand() % 100) * 0.01);
    }

    float old_reward = table[state]->at(this->relative_action);

    // Choose a reward for the new move
    int r = 0;
    if (winner == this->id) {
        r = 500;
    } else if (winner != 0) {
        r = -500;
    } else {
        r = 1;
    }

    // update with standard Q learning / gamma decay rate 0.7
    float exp_future_reward = table[fut_state]->at(this->relative_action);
    float new_ = old_reward + 0.5 * (r + 0.7 *  exp_future_reward);
    table[state]->at(this->relative_action) = new_;
    this->state = state;

    return r;
}


/**
 * Print the rewards vector for the current state to std out
 * @return void
 */
void QLearner::showRews() {
    std::map<size_t, std::vector<float>*>::iterator it = table.find(this->state);
    for (int i = 0; i < this->filter_size; i++) {
        std::cout << it->second->at(i) << " ";
    }
    std::cout << std::endl;
    return;
}


/**
 * Save the current Q table for this AI to CSV-like file
 * (comma seperated values in newline seperated states)
 * @return 0 on success, non-zero on file error/fail to write
 */
int QLearner::saveQ(std::string fname) {
    std::ofstream stream(fname);
    int ct_saves = 0;
    for(auto& kv : this->table) {
        stream << (kv.first) << ",";
        for(int i = 0; i < this->filter_size; i++) {
            stream << kv.second->at(i)<< ",";
        }
        stream << "\n";
        ct_saves++;
    }
    std::cout <<"\033[1;32mSAVED: \033[0m"<< ct_saves << " states and reward vectors to";
    std::cout << "\033[1;32m " <<fname <<"\033[0m" << std::endl;
    stream.close();
    return 0;
}


/**
 * Load a Q table from file, and apply to this AI. Formatted as saveQ
 * formats (comma seperated values in newline seperated states)
 * @return a Q table
 */
int QLearner::loadQ(std::string fname) {
    std::fstream newfile;
    newfile.open(fname, std::ios::in);

    int ct_rows = 0;
    if(newfile.is_open()) {
        std::string line;

        // For each line
        while(std::getline(newfile, line)) {
            // identify the hash
            char * line_c = &line[0];
            size_t hash = (size_t) atoi(strtok(line_c, ","));
            this->table[hash]= new std::vector<float>(this->filter_size, 0);
            // populate the vector with rewards as read
            for (int i = 0; i < this->filter_size; i++) {
                float value = (float) atoi(strtok(line_c, ","));
                this->table[hash]->at(i) = value;
            }
            ct_rows++;

        }
        newfile.close();

        std::cout <<"\033[1;32mLOADED: \033[0m"<< ct_rows << " states and reward vectors to";
        std::cout << "\033[1;32m " <<fname <<"\033[0m";
        return 0;
    } else {
        return -1;
    }
}


/**
 * Creates hashes of the filter applied to each possible location
 * on the board.
 * @return an array of hashes in L->R T->D order
 */
size_t* QLearner::convGreedyDecider() {
    int size = this->filter_size;
    std::hash<std::string> hash;

    size_t* hashes = new size_t[HEIGHT*WIDTH];
    this->sub_state_locations = new int[HEIGHT*WIDTH];

    int conv_ct = 0;
    std::string hold_conv = "";

    // For each location, great a filter
    for (int i = 0; i < this->HEIGHT - size; i++) {
        for (int j = 0; j < this->WIDTH - size; j++) {

            // For each filter, for a string that represents piece positions
            for (int ix = 0; ix < size; ix++) {
                for (int jx = 0; jx < size; jx++) {
                    hold_conv += std::to_string((this->game->board[i+ix][j+jx]));
                }
            }

            // Note the location of this hash for immediate movement
            this->sub_state_locations[conv_ct] = j;

            // Save this hash
            hashes[conv_ct] = hash(hold_conv);
            hold_conv = "";
            conv_ct++;
        }
    }

    return hashes;
}


/**
 * Find the best move for the current sub-state
 * @return the best (most rewarded) move
 */
int QLearner::bestFromState(size_t hash, float target) {

    // init for stability on not found (end of itt)
    if (!table.count(hash)) {
        this->table[hash]= new std::vector<float>(this->filter_size, (rand() % 100) * 0.01);
    }

    // make a move in a greedy manner
    std::vector<float> * probs = table[hash];
    auto best = std::max_element(probs->begin(), probs->end());
    int max = std::distance(probs->begin(), best);

    // Invalid moves from this state are punished down to an extreme low
    // The next most rewarded value is used until a maximal valid move is found
    float best_rew = 0;
    int ct_stuck = 0;
    while (this->game->validMove(max) != 0) {
        best_rew = -100000;
        probs->at(max) = -100000;
        for (int i = 0; i < this->filter_size; i++) {
            if (i != max && probs->at(i) > best_rew) {
                max = i;
                best_rew = probs->at(i);
            }
        if (ct_stuck > this->filter_size) {
            return rand() % this->filter_size;
        }
        ct_stuck++;
        }
    }

    this->max_reward = probs->at(max);
    return max;
}
