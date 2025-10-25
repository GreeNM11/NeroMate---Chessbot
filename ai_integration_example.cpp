/*
 * AI Integration Example for ChessQt
 * 
 * This file shows how to integrate the AI bot with the existing chess game.
 * You'll need to modify your main chess game files to include AI functionality.
 */

#include "ai_bot_controller.h"
#include "game_client/chess_logic/board_state.h"
#include "game_client/chess_logic/chess_game.h"
#include <QDebug>

class ChessGameWithAI : public QObject
{
    Q_OBJECT

public:
    ChessGameWithAI() {
        // Initialize AI bot controller for black pieces (AI plays as black)
        aiBot = new AIBotController(false, this); // false = black pieces
        
        // Connect AI signals
        connect(aiBot, &AIBotController::aiMoveReady, this, &ChessGameWithAI::onAIMove);
        connect(aiBot, &AIBotController::aiThinking, this, &ChessGameWithAI::onAIThinking);
        connect(aiBot, &AIBotController::aiMessage, this, &ChessGameWithAI::onAIMessage);
        
        // Configure AI
        aiBot->setDifficulty(3); // Medium difficulty
        aiBot->setThinkingTime(1500); // 1.5 seconds thinking time
        aiBot->enableAI(true);
    }
    
    void connectToGame(chess_game* game) {
        this->game = game;
        
        if (game) {
            // Connect to the game's board state
            // You'll need to add a getter for the board_state in chess_game
            // board_state* boardState = game->getBoardState();
            // aiBot->connectToGame(boardState);
            
            // Connect to game signals
            // connect(game, &chess_game::moveMade, this, &ChessGameWithAI::onPlayerMove);
            // connect(game, &chess_game::turnChanged, this, &ChessGameWithAI::onTurnChanged);
            
            qDebug() << "AI connected to chess game";
        }
    }

private slots:
    void onAIMove(QString move) {
        qDebug() << "AI wants to make move:" << move;
        
        if (game) {
            // Execute the AI move
            // You'll need to add a method to execute moves in your chess_game
            // game->executeMove(move);
            
            // Or if using the board_state directly:
            // game->getBoardState()->receive_move(move);
        }
    }
    
    void onAIThinking(bool thinking) {
        if (thinking) {
            qDebug() << "AI is thinking...";
            // Update UI to show AI is thinking
        } else {
            qDebug() << "AI finished thinking";
            // Update UI to show AI is ready
        }
    }
    
    void onAIMessage(QString message) {
        qDebug() << "AI:" << message;
        // Display AI message in UI
    }
    
    void onPlayerMove() {
        // Called when the human player makes a move
        qDebug() << "Player made a move, AI will respond";
        
        // Trigger AI to think about its next move
        aiBot->makeMove();
    }
    
    void onTurnChanged(bool isWhiteTurn) {
        // Called when the turn changes
        if (!isWhiteTurn) {
            // It's AI's turn (black pieces)
            aiBot->makeMove();
        }
    }

private:
    AIBotController* aiBot;
    chess_game* game;
};

/*
 * Integration Steps:
 * 
 * 1. Add AI Bot Controller to your main window or chess game class:
 *    - Include "ai_bot_controller.h"
 *    - Create an AIBotController instance
 *    - Connect its signals to your UI
 * 
 * 2. Modify your chess_game class to expose board_state:
 *    - Add a getter method: board_state* getBoardState() { return game_state.get(); }
 *    - Add signals for move events and turn changes
 * 
 * 3. Update your board_state class to support AI integration:
 *    - Add signals for when moves are made
 *    - Add methods to get current board state
 *    - Add methods to execute moves programmatically
 * 
 * 4. Update your UI to show AI status:
 *    - Add labels to show when AI is thinking
 *    - Add controls to set AI difficulty
 *    - Add controls to enable/disable AI
 * 
 * 5. Handle AI moves in your game loop:
 *    - When AI makes a move, update the board
 *    - Update the UI to reflect the move
 *    - Switch turns appropriately
 */

#include "ai_integration_example.moc"
