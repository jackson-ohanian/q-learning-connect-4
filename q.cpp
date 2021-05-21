#include "q.h"


// Construct Q learning
QLearner::QLearner(Game * game, double a, int e, int id) {
    this->game = game;
    this->alpha = a;
    this->epsilon = e;
    this->action = 0.0;
    this->state = 0;
    this->id = id;
}

int QLearner::newTrain() {
    // Goal = Epsilon greedy training over games until AI finds
    // that the game is 'solved' when it plays first
    // Stateless training - calls a single move / tracks states only
    for (int i = 0; i < 1000; i++) {
        if (i % 1000 == 0) {
            std::cout << "epoch " << i << std::endl;
        }
    }
    return 0;
}

int QLearner::makeMove(bool train) {
    if (rand()%this->epsilon != 0 || !train) {
        return this->greedyMove();
    } else {
        this->action = rand() % WIDTH;
        return this->action;
    }
}


int QLearner::greedyMove() {
    size_t hash = game->getBoard();

    std::map<size_t, std::vector<float>*>::iterator it = table.find(hash);

    // init for stability on not found (end of itt)
    if (it == table.end()) {
        it->second = new std::vector<float>(WIDTH, (rand() % 100) * 0.01);
    }

    // make a move in a greedy manner
    std::vector<float> * probs = it->second;
    auto best = std::max_element(probs->begin(), probs->end());
    int max = std::distance(probs->begin(), best);

    // lower
    this->action = max;

    // Invalid moves from this state are punished down to an extreme low
    // The next most rewarded value is used until a valid move is made
    float best_rew = 0.0;
    while (this->game->validMove(max) != 0) {
        best_rew = -100000;
        probs->at(max) = -100000;
        for (int i = 0; i < WIDTH; i++) {
            if (i != max && probs->at(i) > best_rew) {
                max = i;
                best_rew = probs->at(i);
            }
        }
    }

    this->state = hash;
    return max;
}

int QLearner::loadTrain() {

    return 0;
}

size_t QLearner::hashBoard() {
    return 0;
}

int QLearner::update(int winner, int player, int move, size_t hash) {
    if (move == -1) {
        return -1;
    }
    // update the q table
    size_t state = hash;
    size_t fut_state = game->getFutureBoard(move, player);
    // Get old reward of this state / init if empty
    std::map<size_t, std::vector<float>*>::iterator it = table.find(state);
    std::map<size_t, std::vector<float>*>::iterator it_fut = table.find(fut_state);
    if (it_fut == table.end()) {
        it_fut->second = new std::vector<float>(7, rand() % 5);
    }

    float old_reward = it->second->at(this->action);

    // Choose a reward for the new move
    int r = 0;
    if (winner == this->id) {
        r = 1000;
    } else if (winner != 0) {
        r = -200;
    } else {
        r = 0;
    }

    float exp_future_reward = it_fut->second->at(this->action);
    float new_ = old_reward + 0.5 * (r + 0.7 *  exp_future_reward);
    it->second->at(this->action) = new_;
    this->state = state;


    game->printBoard();
    showRews();


    return r;
}

void QLearner::showRews() {
    std::map<size_t, std::vector<float>*>::iterator it = table.find(this->state);
    for (int i = 0; i < WIDTH; i++) {
        std::cout << it->second->at(i) << " ";
    }
    std::cout << std::endl;
    return;
}

int QLearner::checkQTable(size_t hash) {
    std::map<size_t, std::vector<float>*>::iterator it = table.find(hash);
    return 0;
}


int QLearner::saveQ(std::string fname) {
    std::cout << "saving training data to " << fname << "..." << std::endl;
    std::ofstream stream(fname);
    for(auto& kv : this->table) {
        std::cout << "trying b" << std::endl;
        stream << (kv.first) << ",";
        for(int i = 0; i < 7; i++) {
            std::cout << i;
            stream << kv.second->at(i)<< ",";
        }
        stream << "\n";
    }
    stream.close();
    return 0;
}
