#include "player.hpp"

// Hi, my name is James.
// Hi, my name is Shiva. 

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    playerBoard = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */

/*
     playerBoard -> doMove(opponentsMove, WHITE); 
    //how can I make this check the side of the opponent instead of 
    // hardcoding "WHITE?"

    Move* playerMove = new Move(0, 0);

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            playerMove -> setX(i);
            playerMove -> setY(j);
            if (playerBoard -> checkMove(playerMove, BLACK)){
                playerBoard -> doMove(playerMove, BLACK);
                return playerMove;}
        }

    }

    delete playerMove;
    return nullptr;
*/

    playerBoard -> doMove(opponentsMove, WHITE); 
    //how can I make this check the side of the opponent instead of 
    // hardcoding "WHITE?"

    Move* playerMove = new Move(0, 0);
    Move* tempMove = new Move(0,0);
    int score = -1000;
    bool changed = false;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            tempMove -> setX(x);
            tempMove -> setY(y);

            if (playerBoard -> checkMove(tempMove, BLACK)) {
                changed = true;

                Board* copyBoard = playerBoard -> copy();
                copyBoard -> doMove(tempMove, BLACK);

                if (copyBoard -> boardScore(tempMove) > score) {
                    score = copyBoard -> boardScore(tempMove);
                    playerMove -> setX(tempMove -> getX());
                    playerMove -> setY(tempMove -> getY());
                }

                delete copyBoard;
            }
        }
    }

    if (changed == false) {
        delete tempMove;
        delete playerMove;
        return nullptr;
    }
    
    delete tempMove; 
    playerBoard -> doMove(playerMove, BLACK);
    return playerMove;

   
}
