#include "chess_ai.h"
#include <QDebug>
#include <random>
#include <chrono>

ChessAI::ChessAI(bool isWhite, QObject *parent)
    : QObject(parent), isWhite(isWhite), difficulty(3), maxDepth(3)
{
    // Set max depth based on difficulty
    switch(difficulty) {
        case 1: maxDepth = 2; break;
        case 2: maxDepth = 3; break;
        case 3: maxDepth = 4; break;
        case 4: maxDepth = 5; break;
        case 5: maxDepth = 6; break;
        default: maxDepth = 3; break;
    }
}

ChessAI::~ChessAI() = default;

void ChessAI::setDifficulty(int level) {
    difficulty = std::max(1, std::min(5, level));
    switch(difficulty) {
        case 1: maxDepth = 2; break;
        case 2: maxDepth = 3; break;
        case 3: maxDepth = 4; break;
        case 4: maxDepth = 5; break;
        case 5: maxDepth = 6; break;
    }
}

Move ChessAI::getBestMove(const QString board[8][8], Piece* piece_board[8][8], 
                         Piece* last_moved, int depth) {
    emit aiThinking(true);
    
    // Generate all possible moves for the AI
    std::vector<Move> moves = generateAllMoves(board, piece_board, last_moved, isWhite);
    
    if (moves.empty()) {
        emit aiThinking(false);
        return Move(-1, -1, -1, -1, 0); // No moves available
    }
    
    // If only one move, return it
    if (moves.size() == 1) {
        emit aiThinking(false);
        return moves[0];
    }
    
    // Sort moves for better alpha-beta pruning
    sortMoves(moves);
    
    int bestValue = isWhite ? INT_MIN : INT_MAX;
    Move bestMove = moves[0];
    
    // Evaluate each move
    for (auto& move : moves) {
        // Create a copy of the board state for simulation
        QString tempBoard[8][8];
        Piece* tempPieceBoard[8][8];
        
        // Copy current state
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                tempBoard[i][j] = board[i][j];
                tempPieceBoard[i][j] = piece_board[i][j];
            }
        }
        
        // Simulate the move
        tempBoard[move.to_row][move.to_col] = tempBoard[move.from_row][move.from_col];
        tempBoard[move.from_row][move.from_col] = "";
        
        // Evaluate the position after this move
        int value = minimax(tempBoard, tempPieceBoard, last_moved, 
                           depth - 1, INT_MIN, INT_MAX, !isWhite);
        
        // Update best move based on whether we're maximizing or minimizing
        if ((isWhite && value > bestValue) || (!isWhite && value < bestValue)) {
            bestValue = value;
            bestMove = move;
        }
    }
    
    bestMove.value = bestValue;
    emit aiThinking(false);
    
    qDebug() << "AI chose move:" << moveToString(bestMove.from_row, bestMove.from_col, 
                                                 bestMove.to_row, bestMove.to_col) 
             << "with value:" << bestValue;
    
    return bestMove;
}

int ChessAI::minimax(const QString board[8][8], Piece* piece_board[8][8], 
                     Piece* last_moved, int depth, int alpha, int beta, bool maximizingPlayer) {
    // Terminal conditions
    if (depth == 0) {
        return evaluatePosition(board, piece_board);
    }
    
    // Check for checkmate/stalemate
    if (isCheckmate(board, piece_board, maximizingPlayer)) {
        return maximizingPlayer ? -10000 : 10000; // Opponent is checkmated
    }
    
    if (isStalemate(board, piece_board, maximizingPlayer)) {
        return 0; // Stalemate
    }
    
    // Generate moves for current player
    std::vector<Move> moves = generateAllMoves(board, piece_board, last_moved, maximizingPlayer);
    
    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (const auto& move : moves) {
            // Simulate move
            QString tempBoard[8][8];
            Piece* tempPieceBoard[8][8];
            
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    tempBoard[i][j] = board[i][j];
                    tempPieceBoard[i][j] = piece_board[i][j];
                }
            }
            
            tempBoard[move.to_row][move.to_col] = tempBoard[move.from_row][move.from_col];
            tempBoard[move.from_row][move.from_col] = "";
            
            int eval = minimax(tempBoard, tempPieceBoard, last_moved, 
                              depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            
            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& move : moves) {
            // Simulate move
            QString tempBoard[8][8];
            Piece* tempPieceBoard[8][8];
            
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    tempBoard[i][j] = board[i][j];
                    tempPieceBoard[i][j] = piece_board[i][j];
                }
            }
            
            tempBoard[move.to_row][move.to_col] = tempBoard[move.from_row][move.from_col];
            tempBoard[move.from_row][move.from_col] = "";
            
            int eval = minimax(tempBoard, tempPieceBoard, last_moved, 
                              depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return minEval;
    }
}

int ChessAI::evaluatePosition(const QString board[8][8], Piece* piece_board[8][8]) {
    int score = 0;
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (!board[row][col].isEmpty()) {
                score += evaluatePiece(board[row][col], row, col);
            }
        }
    }
    
    return score;
}

int ChessAI::evaluatePiece(const QString& piece, int row, int col) {
    if (piece.isEmpty()) return 0;
    
    bool isWhitePiece = piece[0] == 'w';
    int multiplier = isWhitePiece ? 1 : -1;
    
    int baseValue = 0;
    int positionalBonus = 0;
    
    // Base piece values
    switch (piece[1].toLatin1()) {
        case 'P': baseValue = PAWN_VALUE; break;
        case 'N': baseValue = KNIGHT_VALUE; break;
        case 'B': baseValue = BISHOP_VALUE; break;
        case 'R': baseValue = ROOK_VALUE; break;
        case 'Q': baseValue = QUEEN_VALUE; break;
        case 'K': baseValue = KING_VALUE; break;
    }
    
    // Simple positional bonuses (can be expanded)
    if (piece[1] == 'P') {
        // Pawn advancement bonus
        if (isWhitePiece) {
            positionalBonus += (7 - row) * 10; // Closer to promotion
        } else {
            positionalBonus += row * 10;
        }
    }
    
    return (baseValue + positionalBonus) * multiplier;
}

std::vector<Move> ChessAI::generateAllMoves(const QString board[8][8], 
                                           Piece* piece_board[8][8], 
                                           Piece* last_moved, bool isWhiteTurn) {
    std::vector<Move> moves;
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (!board[row][col].isEmpty() && 
                ((board[row][col][0] == 'w' && isWhiteTurn) || 
                 (board[row][col][0] == 'b' && !isWhiteTurn))) {
                
                // Get piece moveset (simplified - would need to integrate with existing piece logic)
                std::vector<std::pair<int, int>> pieceMoves;
                
                // This is a simplified move generation - in practice, you'd use the existing
                // piece logic from the chess game
                char pieceType = board[row][col][1].toLatin1();
                
                switch (pieceType) {
                    case 'P': // Pawn
                        generatePawnMoves(board, row, col, isWhiteTurn, pieceMoves);
                        break;
                    case 'N': // Knight
                        generateKnightMoves(board, row, col, isWhiteTurn, pieceMoves);
                        break;
                    case 'B': // Bishop
                        generateBishopMoves(board, row, col, isWhiteTurn, pieceMoves);
                        break;
                    case 'R': // Rook
                        generateRookMoves(board, row, col, isWhiteTurn, pieceMoves);
                        break;
                    case 'Q': // Queen
                        generateQueenMoves(board, row, col, isWhiteTurn, pieceMoves);
                        break;
                    case 'K': // King
                        generateKingMoves(board, row, col, isWhiteTurn, pieceMoves);
                        break;
                }
                
                // Add valid moves to the list
                for (const auto& move : pieceMoves) {
                    if (isValidMove(board, piece_board, last_moved, row, col, 
                                   move.first, move.second, isWhiteTurn)) {
                        moves.emplace_back(row, col, move.first, move.second);
                    }
                }
            }
        }
    }
    
    return moves;
}

void ChessAI::generatePawnMoves(const QString board[8][8], int row, int col, 
                               bool isWhite, std::vector<std::pair<int, int>>& moves) {
    int direction = isWhite ? -1 : 1;
    int startRow = isWhite ? 6 : 1;
    
    // Forward move
    if (row + direction >= 0 && row + direction < 8 && board[row + direction][col].isEmpty()) {
        moves.emplace_back(row + direction, col);
        
        // Double move from starting position
        if (row == startRow && board[row + 2 * direction][col].isEmpty()) {
            moves.emplace_back(row + 2 * direction, col);
        }
    }
    
    // Capture moves
    for (int dc = -1; dc <= 1; dc += 2) {
        int newCol = col + dc;
        if (newCol >= 0 && newCol < 8 && row + direction >= 0 && row + direction < 8) {
            if (!board[row + direction][newCol].isEmpty() && 
                board[row + direction][newCol][0] != (isWhite ? 'w' : 'b')) {
                moves.emplace_back(row + direction, newCol);
            }
        }
    }
}

void ChessAI::generateKnightMoves(const QString board[8][8], int row, int col, 
                                 bool isWhite, std::vector<std::pair<int, int>>& moves) {
    int knightMoves[8][2] = {{2,1}, {2,-1}, {-2,1}, {-2,-1}, {1,2}, {1,-2}, {-1,2}, {-1,-2}};
    
    for (int i = 0; i < 8; i++) {
        int newRow = row + knightMoves[i][0];
        int newCol = col + knightMoves[i][1];
        
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            if (board[newRow][newCol].isEmpty() || 
                board[newRow][newCol][0] != (isWhite ? 'w' : 'b')) {
                moves.emplace_back(newRow, newCol);
            }
        }
    }
}

void ChessAI::generateBishopMoves(const QString board[8][8], int row, int col, 
                                bool isWhite, std::vector<std::pair<int, int>>& moves) {
    int directions[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    
    for (int d = 0; d < 4; d++) {
        for (int i = 1; i < 8; i++) {
            int newRow = row + i * directions[d][0];
            int newCol = col + i * directions[d][1];
            
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) break;
            
            if (board[newRow][newCol].isEmpty()) {
                moves.emplace_back(newRow, newCol);
            } else {
                if (board[newRow][newCol][0] != (isWhite ? 'w' : 'b')) {
                    moves.emplace_back(newRow, newCol);
                }
                break;
            }
        }
    }
}

void ChessAI::generateRookMoves(const QString board[8][8], int row, int col, 
                               bool isWhite, std::vector<std::pair<int, int>>& moves) {
    int directions[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    
    for (int d = 0; d < 4; d++) {
        for (int i = 1; i < 8; i++) {
            int newRow = row + i * directions[d][0];
            int newCol = col + i * directions[d][1];
            
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) break;
            
            if (board[newRow][newCol].isEmpty()) {
                moves.emplace_back(newRow, newCol);
            } else {
                if (board[newRow][newCol][0] != (isWhite ? 'w' : 'b')) {
                    moves.emplace_back(newRow, newCol);
                }
                break;
            }
        }
    }
}

void ChessAI::generateQueenMoves(const QString board[8][8], int row, int col, 
                                bool isWhite, std::vector<std::pair<int, int>>& moves) {
    // Queen combines rook and bishop moves
    generateRookMoves(board, row, col, isWhite, moves);
    generateBishopMoves(board, row, col, isWhite, moves);
}

void ChessAI::generateKingMoves(const QString board[8][8], int row, int col, 
                               bool isWhite, std::vector<std::pair<int, int>>& moves) {
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            
            int newRow = row + dr;
            int newCol = col + dc;
            
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                if (board[newRow][newCol].isEmpty() || 
                    board[newRow][newCol][0] != (isWhite ? 'w' : 'b')) {
                    moves.emplace_back(newRow, newCol);
                }
            }
        }
    }
}

bool ChessAI::isValidMove(const QString board[8][8], Piece* piece_board[8][8], 
                         Piece* last_moved, int from_row, int from_col, 
                         int to_row, int to_col, bool isWhiteTurn) {
    // Basic validation - in practice, you'd integrate with the existing validation logic
    if (from_row < 0 || from_row >= 8 || from_col < 0 || from_col >= 8 ||
        to_row < 0 || to_row >= 8 || to_col < 0 || to_col >= 8) {
        return false;
    }
    
    if (board[from_row][from_col].isEmpty()) return false;
    
    // Check if it's the right color's turn
    if ((board[from_row][from_col][0] == 'w' && !isWhiteTurn) ||
        (board[from_row][from_col][0] == 'b' && isWhiteTurn)) {
        return false;
    }
    
    // Check if destination is occupied by own piece
    if (!board[to_row][to_col].isEmpty() && 
        board[to_row][to_col][0] == board[from_row][from_col][0]) {
        return false;
    }
    
    return true;
}

bool ChessAI::isInCheck(const QString board[8][8], Piece* piece_board[8][8], bool isWhite) {
    // Simplified check detection - would need full implementation
    return false;
}

bool ChessAI::isCheckmate(const QString board[8][8], Piece* piece_board[8][8], bool isWhite) {
    // Simplified checkmate detection - would need full implementation
    return false;
}

bool ChessAI::isStalemate(const QString board[8][8], Piece* piece_board[8][8], bool isWhite) {
    // Simplified stalemate detection - would need full implementation
    return false;
}

void ChessAI::sortMoves(std::vector<Move>& moves) {
    // Sort moves by value (captures first, then by piece value)
    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        return a.value > b.value;
    });
}

QString ChessAI::moveToString(int from_row, int from_col, int to_row, int to_col) {
    return QString("%1%2%3%4").arg(from_row).arg(from_col).arg(to_row).arg(to_col);
}
