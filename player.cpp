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
 * change board to a new board given as a parameter
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
 * Given a vector, find the index of the min
 */
int min_index(vector<int> scores) {
    if (scores.size() == 0) {
        return -1;
    }

    int ind = 0;

    for (int i = 1; i < scores.size(); i++) {
        if (scores[i] < scores[ind]) {
            ind = i;
        }
    }

    return ind;
}

/*
 * Given a vector, find the index of the max
 */
int max_index(vector<int> scores) {
    if (scores.size() == 0) {
        return -1;
    }

    int ind = 0;

    for (int i = 1; i < scores.size(); i++) {
        if (scores[i] > scores[ind]) {
            ind = i;
        }
    }

    return ind;
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

    int max_min_scores_index = max_index(min_scores);

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
 * if n is odd, the n-th move is the AI's turn
 * if n is even, the n-th move is the opponent's turn
 */
Move *Player::n_ply_minimax_AI(int n) {
    // when n = 1, find the current best move on the board
    if (n == 1) {
        return heuristic_AI();
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

    board -> doMove(opponentsMove, opp_side);
    // Uncomment these next three lines, one at a time, to test the respective AI's:
    // return random_AI();
    // return heuristic_AI();
    return two_ply_minimax_AI();
    // return n_ply_minimax_AI(5);
}