#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    AI_side = side;
    opp_side = (AI_side == BLACK ? WHITE : BLACK);

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
 * change board to a new board given as a parametpper
 */
void Player::change_board(Board *new_board) {
    board = new_board;
}

/*
 * Implementation of a working AI that plays the first move it finds that is valid.
 */
Move *Player::random_AI() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Move *move = new Move(x, y);
            if (board->checkMove(move, AI_side)) {
                board->doMove(move, AI_side);
                return move;
            } else {
                delete move;
            }
        }

    }

    return nullptr;
}

/*
 * Simple heuristic AI that is good enough to beat SimplePlayer. Plays the move that gets the
 * maximum score looking at only the current board (depth level = 1).
 */
Move *Player::heuristic_AI() {
    Move *move = nullptr;
    int max_score = -1000;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Move *tmp_move = new Move(x, y);

            if (board->checkMove(tmp_move, AI_side)) {
                Board *next_board = board->copy();
                next_board->doMove(tmp_move, AI_side);
                int next_score = next_board->boardScore(AI_side);

                if (next_score > max_score) {
                    max_score = next_score;
                    move = tmp_move;
                }

                delete next_board;
            }

            delete tmp_move;
        }
    }

    if (!move)
        return nullptr;
    
    board->doMove(move, AI_side);
    return move;
}

/*
 * Minimax AI that only goes to a depth level of 2s
 */
Move *Player::two_ply_minimax_AI() {
    vector<Move *> moves;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Move *tmp_move = new Move(x, y);

            if (board->checkMove(tmp_move, AI_side))
                moves.push_back(tmp_move);
            else
                delete tmp_move;
        }
    }

    // number of available moves after opponents move is performed
    int num_moves = moves.size();

    // no moves availble, return nullptr
    if (num_moves == 0)
        return nullptr;

    // initialize vector to 1000
    vector<int> min_scores(num_moves, 1000);

    for (int i = 0; i < num_moves; i++) {
        // create a copy of the board and do the move
        Board *next_board = board->copy();
        next_board->doMove(moves[i], AI_side);

        // go through all the spaces, checking if there is a valid move there for the opponent
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                Move *tmp_move = new Move(x, y);

                if (next_board->checkMove(tmp_move, opp_side)) {
                    // create another copy and perform the opponents move
                    Board *next_next_board = next_board->copy();
                    next_next_board->doMove(tmp_move, opp_side);
                    
                    // find the minimum score if we choose to perfom move moves[i]
                    if (next_next_board->boardScore(opp_side) < min_scores[i]) {
                        min_scores[i] = next_next_board->boardScore(opp_side);
                    }

                    delete next_next_board;
                }

                delete tmp_move;
            }
        }

        delete next_board;
    }

    int max_min_scores_index = 0;

    for (int i = 1; i < num_moves; i++) {
        if (min_scores[i] > min_scores[max_min_scores_index]) {
            max_min_scores_index = i;
        }
    }

    // delete all moves except the actual move to be performed
    for (int i = 0; i < num_moves; i++) {
        if (i != max_min_scores_index) {
            delete moves[i];
        }
    }

    board->doMove(moves[max_min_scores_index], AI_side);
    return moves[max_min_scores_index];
}

/*
 * helper function: return the score given a board and move performed by side
 */
int get_score(Board *board, Move *move, Side side) {
    board->doMove(move, side);
    return board->boardScore(side);
}

/*
 * look ahead n moves to find the best move to do
 * n should be odd
 */
Move *Player::n_ply_minimax_AI(int n) {
   

    long bestMoveVal = -10000000;
    Move *bestMove = new Move(0,0);
    bool foundBestMove = false;
    Move *aiMove = new Move(0,0);
/*
    bool lastResort = false;
    Move *lastResortMove = new Move(0,0);
    bool lastResortCond = false;
*/

// Store the first allowable move as a last resort if none of the moves offer an optimal score
/*
     for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) { 
            lastResortMove -> setX(x);
            lastResortMove -> setY(y);

            if (board -> checkMove(lastResortMove, AI_side)) {

                goto algorithm;
            }
        }
    }

algorithm:

*/
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) { 
            aiMove -> setX(x);
            aiMove -> setY(y);

            if (board -> checkMove(aiMove, AI_side)) {
                //lastResort = true;

                Board copy = *board;
                copy.doMove(aiMove, AI_side);
                long moveVal;
                moveVal = n_ply_minimax_AI_val(copy, 
                    aiMove, opp_side, n);

                if (moveVal >= bestMoveVal) {
                    bestMoveVal = moveVal;
                    *bestMove = *aiMove; 
                    foundBestMove = true;
                }

            }
        }
    }

    delete aiMove;
    if(foundBestMove) {
        board -> doMove(bestMove, AI_side);
        return bestMove;
    }
/*
    else if (lastResort){
        board -> doMove(lastResortMove, AI_side);
        return lastResortMove;
    }
*/
    else {
        return nullptr;
    }
    
}

/***

Psuedocode for algorithm

the game state consists of the following information:
Board calcBoard, Move *m, Side side

long minimax_val(OthelloGameState s, int depth)
{
    if (depth == 0 or reached a final state)
        return evaluation of s
    else
    {
        if (it's AI turn to move)
        {
            for each valid move that AI can make from s
            {
                make that move on s yielding a state s'
                search(s', depth - 1)
            }
            
            return the maximum value returned from recursive search calls
            stored in vector
        }
        else
        {
            for each valid move that opponent can make from s
            {
                make that move on s yielding a state s'
                search(s', depth - 1)
            }
            
            return the minimum value returned from recursive search calls
            stored in vector
        }
    }
}

***/

long Player::n_ply_minimax_AI_val(Board calcBoard, Move *m, Side side, int depth) {
    if (depth == 0) {
        return calcBoard.boardScore(side);
    }

    if (side != AI_side) {
        vector<Move> possMoves;
        vector<long> vals;
        long minVal = 10000000;

        // find all possible moves for the opponent

        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                Move move(0,0);
                move.setX(x);
                move.setY(y);

                if (calcBoard.checkMove(&move, side)) {
                    possMoves.push_back(move);
                }

            }
        }

        for (vector<Move>::iterator it = possMoves.begin(); it != possMoves.end(); ++it) {
            // copy the board
            Board copy = calcBoard;
            calcBoard.doMove(&(*it), side);
            vals.push_back(n_ply_minimax_AI_val(calcBoard, &(*it), AI_side, depth - 1));
            calcBoard = copy;
        }

        for (vector<long>::iterator it = vals.begin(); it != vals.end(); ++it) {
            if (*it < minVal ) {
                minVal = *it;
            }
        }
        
        return minVal;
    } else {
        vector<Move> possMoves;
        vector<long> vals;
        long maxVal = -10000000;

        // find all possible moves for the opponent
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                Move move(0,0);
                move.setX(x);
                move.setY(y);

                if(calcBoard.checkMove(&move, side)) {
                    possMoves.push_back(move);
                }
            }
        }

        for (vector<Move>::iterator it = possMoves.begin(); it != possMoves.end(); ++it) {
            // copy the board
            Board copy = calcBoard;
            calcBoard.doMove(&(*it), side);
            vals.push_back(n_ply_minimax_AI_val(calcBoard, &(*it), opp_side, depth - 1));
            calcBoard = copy;
        }

        for (vector<long>::iterator it = vals.begin(); it != vals.end(); ++it) {
            if (*it > maxVal) {
                maxVal = *it;
            }
        }
        
        return maxVal;
    }
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

    int minimaxDepth = 7;
    // this can be changed to the desired value

    board -> doMove(opponentsMove, opp_side);
    // Uncomment these next four lines, one at a time, to test the respective AI's:

    // return random_AI();
    // return heuristic_AI();
    // return two_ply_minimax_AI();
    return n_ply_minimax_AI(minimaxDepth);

}