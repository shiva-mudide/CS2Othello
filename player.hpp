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
	Side playerSide, oppoSide; 

public:
    Player(Side side);
    ~Player();

    void change_board(Board *new_board);
    Move *random_AI(Move *opponentsMove);
    Move *heuristic_AI(Move *opponentsMove);
    Move *minimax_AI(Move *opponentsMove);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
