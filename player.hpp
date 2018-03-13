#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

private:
	Board *board;
	Side AI_side, opp_side; 

public:
    Player(Side side);
    ~Player();

    void change_board(Board *new_board);
    Move *random_AI();
    Move *heuristic_AI();
    Move *two_ply_minimax_AI();
    Move *n_ply_minimax_AI(int n);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
