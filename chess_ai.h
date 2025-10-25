#ifndef CHESS_AI_H
#define CHESS_AI_H

#include <QObject>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <climits>

// Forward declarations
class Piece;
class King;

struct Move {
    int from_row, from_col;
    int to_row, to_col;
    int value; // evaluation value for this move
    
    Move(int fr, int fc, int tr, int tc, int val = 0) 
        : from_row(fr), from_col(fc), to_row(tr), to_col(tc), value(val) {}
};

class ChessAI : public QObject
{
    Q_OBJECT

public:
    explicit ChessAI(bool isWhite, QObject *parent = nullptr);
    ~ChessAI();

    // Main AI function - returns the best move
    Move getBestMove(const QString board[8][8], Piece* piece_board[8][8], 
                     Piece* last_moved, int depth = 3);

    // Set AI difficulty (affects search depth)
    void setDifficulty(int level);

signals:
    void aiMoveReady(QString move); // Signal when AI has chosen a move
    void aiThinking(bool thinking); // Signal AI thinking status

private:
    // AI Configuration
    bool isWhite;
    int difficulty; // 1-5, affects search depth
    int maxDepth;
    
    // Evaluation weights
    static const int PAWN_VALUE = 100;
    static const int KNIGHT_VALUE = 320;
    static const int BISHOP_VALUE = 330;
    static const int ROOK_VALUE = 500;
    static const int QUEEN_VALUE = 900;
    static const int KING_VALUE = 20000;
    
    // Minimax algorithm with alpha-beta pruning
    int minimax(const QString board[8][8], Piece* piece_board[8][8], 
                  Piece* last_moved, int depth, int alpha, int beta, bool maximizingPlayer);
    
    // Position evaluation
    int evaluatePosition(const QString board[8][8], Piece* piece_board[8][8]);
    int evaluatePiece(const QString& piece, int row, int col);
    
    // Move generation
    std::vector<Move> generateAllMoves(const QString board[8][8], 
                                      Piece* piece_board[8][8], 
                                      Piece* last_moved, bool isWhiteTurn);
    
    // Helper functions
    bool isInCheck(const QString board[8][8], Piece* piece_board[8][8], bool isWhite);
    bool isCheckmate(const QString board[8][8], Piece* piece_board[8][8], bool isWhite);
    bool isStalemate(const QString board[8][8], Piece* piece_board[8][8], bool isWhite);
    
    // Move validation
    bool isValidMove(const QString board[8][8], Piece* piece_board[8][8], 
                     Piece* last_moved, int from_row, int from_col, 
                     int to_row, int to_col, bool isWhiteTurn);
    
    // Utility functions
    QString moveToString(int from_row, int from_col, int to_row, int to_col);
    void sortMoves(std::vector<Move>& moves); // Sort moves for better alpha-beta pruning
};

#endif // CHESS_AI_H
