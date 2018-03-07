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
    playerSide = side;
    oppoSide = (playerSide == BLACK ? WHITE : BLACK);




    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    board = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

void Player::change_board(Board *new_board) {
    board = new_board;
}

/*
 * Implementation of a working AI that plays the first move it finds that is valid.
 */
Move *Player::random_AI(Move *opponentsMove) {
    Move *playerMove = new Move(0, 0);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            playerMove -> setX(i);
            playerMove -> setY(j);
            if (board -> checkMove(playerMove, playerSide)) {
                board -> doMove(playerMove, playerSide);
                return playerMove;
            }
        }

    }

    delete playerMove;
    return nullptr;
}

/*
 * Simple heuristic AI that is good enough to beat SimplePlayer. Plays the move that gets the
 * maximum score with only a depth level of 1.
 */
Move *Player::heuristic_AI(Move *opponentsMove) {
    Move* playerMove = new Move(0, 0);
    Move* tempMove = new Move(0,0);
    int score = -1000;
    bool changed = false;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            tempMove -> setX(x);
            tempMove -> setY(y);

            if (board -> checkMove(tempMove, playerSide)) {
                changed = true;

                Board* copyBoard = board -> copy();
                copyBoard -> doMove(tempMove, playerSide);

                if (copyBoard -> boardScore(tempMove, playerSide) > score) {
                    score = copyBoard -> boardScore(tempMove, playerSide);
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
    board -> doMove(playerMove, playerSide);
    return playerMove;
}

/*
 * Minimax AI that only goes to a depth level of 2s
 */
Move *Player::minimax_AI(Move *opponentsMove) {
    vector<Move *> firstMove;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Move *tmpMove = new Move(x, y);

            if (board->checkMove(tmpMove, playerSide))
                firstMove.push_back(tmpMove);
        }
    }

    if (firstMove.size() == 0)
        return nullptr;

    vector<int> minScore(firstMove.size(), 1000);

    for (int i = 0; i < firstMove.size(); i++) {
        Board* copyBoard = board->copy();
        copyBoard->doMove(firstMove[i], playerSide);

        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                Move *tmpMove = new Move(x, y);

                if (copyBoard->checkMove(tmpMove, oppoSide)) {
                    Board *copyCopyBoard = copyBoard->copy();
                    copyCopyBoard->doMove(tmpMove, oppoSide);
                    
                    if (copyCopyBoard->boardScore(tmpMove, oppoSide) < minScore[i]) {
                        minScore[i] = copyCopyBoard->boardScore(tmpMove, oppoSide);
                    }

                    delete copyCopyBoard;
                }

                delete tmpMove;
            }
        }

        delete copyBoard;
    }

    int maxMinScoreIndex = 0;

    for (int i = 1; i < minScore.size(); i++) {
        if (minScore[i] > minScore[maxMinScoreIndex]) {
            maxMinScoreIndex = i;
        }
    }

    board->doMove(firstMove[maxMinScoreIndex], playerSide);
    return firstMove[maxMinScoreIndex];
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

    board -> doMove(opponentsMove, oppoSide);
    return minimax_AI(opponentsMove);   
}