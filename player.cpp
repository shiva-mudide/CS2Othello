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

/*
 * change board to a new board given as a parameter
 */
void Player::change_board(Board *new_board) {
    board = new_board;
}

/*
 * Implementation of a working AI that plays the first move it finds that is valid.
 */
Move *Player::random_AI() {
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
Move *Player::heuristic_AI() {
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
Move *Player::minimax_AI() {
    vector<Move *> firstMove;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Move *tmpMove = new Move(x, y);

            if (board->checkMove(tmpMove, playerSide))
                firstMove.push_back(tmpMove);
            else
                delete tmpMove;
        }
    }

    // number of available moves after opponents move is performed
    int num_moves = firstMove.size();

    // no moves availble, return nullptr
    if (num_moves == 0)
        return nullptr;

    // initialize vector to 1000
    vector<int> minScore(num_moves, 1000);

    for (int i = 0; i < num_moves; i++) {
        // create a copy of the board and do the move
        Board* copyBoard = board->copy();
        copyBoard->doMove(firstMove[i], playerSide);

        // go through all the spaces, checking if there is a valid move there for the opponent
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                Move *tmpMove = new Move(x, y);

                if (copyBoard->checkMove(tmpMove, oppoSide)) {
                    // create another copy and perform the opponents move
                    Board *copyCopyBoard = copyBoard->copy();
                    copyCopyBoard->doMove(tmpMove, oppoSide);
                    
                    // find the minimum score if we choose to perfom move firstMove[i]
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

    // find index of maximum of the minimum scores
    for (int i = 1; i < minScore.size(); i++) {
        if (minScore[i] > minScore[maxMinScoreIndex]) {
            maxMinScoreIndex = i;
        }
    }

    // delete all moves except the actual move to be performed
    for (int i = 0; i < num_moves; i++) {
        if (i != maxMinScoreIndex) {
            delete firstMove[i];
        }
    }

    board->doMove(firstMove[maxMinScoreIndex], playerSide);
    return firstMove[maxMinScoreIndex];
}

// look ahead n moves to find the best move to do
Move *Player::n_ply_minimax_AI(int n) {
    // when n = 1, find the current best move on the board
    if (n == 1) {
        vector<Move *> moves;

        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                Move *tmpMove = new Move(x, y);

                if (board->checkMove(tmpMove, playerSide))
                    firstMove.push_back(tmpMove);
                else
                    delete tmpMove;
            }
        }
    }
    
    return nullptr;
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
    // Uncomment these next three lines, one at a time, to test the respective AI's:
    // return random_AI();
    // return heuristic_AI();
    return minimax_AI();   
}