#include "ai_bot_controller.h"
#include <QDebug>
#include <QApplication>

AIBotController::AIBotController(bool isWhite, QObject *parent)
    : QObject(parent), isWhite(isWhite), aiEnabled(false), thinkingTime(2000)
{
    // Initialize AI
    ai = std::make_unique<ChessAI>(isWhite, this);
    
    // Connect AI signals
    connect(ai.get(), &ChessAI::aiMoveReady, this, &AIBotController::onAIMoveReady);
    connect(ai.get(), &ChessAI::aiThinking, this, &AIBotController::aiThinking);
    
    // Initialize thinking timer
    thinkingTimer = new QTimer(this);
    thinkingTimer->setSingleShot(true);
    connect(thinkingTimer, &QTimer::timeout, this, &AIBotController::onThinkingTimeout);
    
    qDebug() << "AI Bot Controller initialized for" << (isWhite ? "White" : "Black") << "pieces";
}

AIBotController::~AIBotController() = default;

void AIBotController::connectToGame(board_state* gameBoard) {
    this->gameBoard = gameBoard;
    
    if (gameBoard) {
        // Connect to game signals to know when it's AI's turn
        // Note: You'll need to add these signals to board_state if they don't exist
        // connect(gameBoard, &board_state::turnChanged, this, &AIBotController::onGameStateChanged);
        // connect(gameBoard, &board_state::moveMade, this, &AIBotController::onOpponentMove);
        
        qDebug() << "AI Bot connected to game board";
    }
}

void AIBotController::setDifficulty(int level) {
    if (ai) {
        ai->setDifficulty(level);
        qDebug() << "AI difficulty set to level" << level;
    }
}

void AIBotController::setThinkingTime(int milliseconds) {
    thinkingTime = std::max(100, milliseconds); // Minimum 100ms
    qDebug() << "AI thinking time set to" << thinkingTime << "ms";
}

void AIBotController::enableAI(bool enabled) {
    aiEnabled = enabled;
    if (!enabled) {
        stopThinking();
    }
    qDebug() << "AI" << (enabled ? "enabled" : "disabled");
}

void AIBotController::makeMove() {
    if (!aiEnabled || !gameBoard || !isAITurn) {
        return;
    }
    
    startThinking();
}

void AIBotController::onOpponentMove() {
    // Called when the opponent makes a move
    qDebug() << "Opponent made a move, AI will respond";
    
    // Check if it's now AI's turn
    QString currentState = getCurrentBoardState();
    if (currentState != lastBoardState) {
        lastBoardState = currentState;
        isAITurn = true;
        
        if (aiEnabled) {
            // Small delay to make AI response feel more natural
            QTimer::singleShot(500, this, &AIBotController::makeMove);
        }
    }
}

void AIBotController::onGameStateChanged() {
    // Called when the game state changes
    QString currentState = getCurrentBoardState();
    if (currentState != lastBoardState) {
        lastBoardState = currentState;
        
        // Determine if it's AI's turn
        // This would need to be implemented based on your game's turn tracking
        isAITurn = true; // Simplified - would need proper turn detection
        
        if (aiEnabled && isAITurn) {
            QTimer::singleShot(300, this, &AIBotController::makeMove);
        }
    }
}

void AIBotController::onThinkingTimeout() {
    // AI thinking time is up, make a move
    if (aiEnabled && isAITurn && gameBoard) {
        qDebug() << "AI thinking timeout, making move";
        
        // Get current board state
        QString board[8][8];
        Piece* pieceBoard[8][8];
        
        // This would need to be implemented to get the current board state
        // from the gameBoard object
        
        // For now, we'll emit a signal that the AI is ready
        emit aiMessage("AI is thinking...");
        
        // In a real implementation, you'd call:
        // Move bestMove = ai->getBestMove(board, pieceBoard, lastMoved, maxDepth);
        // Then execute the move
        
        // For demonstration, we'll just emit a placeholder move
        emit aiMoveReady("0000"); // Placeholder move
    }
}

void AIBotController::onAIMoveReady(QString move) {
    // AI has chosen a move
    qDebug() << "AI chose move:" << move;
    
    if (gameBoard && isAITurn) {
        // Execute the move through the game board
        // This would need to be implemented based on your game's move execution
        // gameBoard->executeMove(move);
        
        isAITurn = false;
        stopThinking();
        
        emit aiMessage("AI made move: " + move);
    }
}

void AIBotController::startThinking() {
    if (thinkingTimer->isActive()) {
        thinkingTimer->stop();
    }
    
    emit aiThinking(true);
    emit aiMessage("AI is thinking...");
    
    // Start the thinking timer
    thinkingTimer->start(thinkingTime);
    
    // Also start the actual AI thinking process
    if (ai) {
        // This would trigger the AI to start thinking
        // In a real implementation, you'd call the AI's getBestMove method
        // and handle the result asynchronously
    }
}

void AIBotController::stopThinking() {
    if (thinkingTimer->isActive()) {
        thinkingTimer->stop();
    }
    
    emit aiThinking(false);
}

bool AIBotController::isGameOver() {
    // Check if the game is over
    // This would need to be implemented based on your game's state checking
    return false; // Simplified
}

QString AIBotController::getCurrentBoardState() {
    // Get the current board state as a string for comparison
    // This would need to be implemented to extract the board state
    // from the gameBoard object
    return "current_board_state"; // Placeholder
}
