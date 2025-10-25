#ifndef AI_BOT_CONTROLLER_H
#define AI_BOT_CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <memory>
#include "chess_ai.h"

// Forward declarations
class board_state;

class AIBotController : public QObject
{
    Q_OBJECT

public:
    explicit AIBotController(bool isWhite, QObject *parent = nullptr);
    ~AIBotController();

    // Connect to the game's board state
    void connectToGame(board_state* gameBoard);
    
    // AI Configuration
    void setDifficulty(int level);
    void setThinkingTime(int milliseconds);
    
    // Control AI behavior
    void enableAI(bool enabled);
    void makeMove(); // Force AI to make a move

public slots:
    void onOpponentMove(); // Called when opponent makes a move
    void onGameStateChanged(); // Called when game state changes

signals:
    void aiMoveReady(QString move); // Signal when AI has chosen a move
    void aiThinking(bool thinking); // Signal AI thinking status
    void aiMessage(QString message); // Signal for AI status messages

private slots:
    void onThinkingTimeout(); // Called when thinking time is up
    void onAIMoveReady(QString move); // Handle AI move completion

private:
    // AI Components
    std::unique_ptr<ChessAI> ai;
    board_state* gameBoard;
    
    // AI Configuration
    bool isWhite;
    bool aiEnabled;
    int thinkingTime; // milliseconds
    QTimer* thinkingTimer;
    
    // Game state tracking
    QString lastBoardState;
    bool isAITurn;
    
    // Helper functions
    void startThinking();
    void stopThinking();
    bool isGameOver();
    QString getCurrentBoardState();
};

#endif // AI_BOT_CONTROLLER_H
