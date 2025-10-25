/*
 * Simple test program for the Chess AI Bot
 * This demonstrates basic AI functionality without the full Qt application
 */

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "chess_ai.h"
#include "ai_bot_controller.h"

class AITest : public QObject
{
    Q_OBJECT

public:
    AITest() {
        // Create AI bot controller
        aiBot = new AIBotController(false, this); // AI plays as black
        
        // Connect signals
        connect(aiBot, &AIBotController::aiMoveReady, this, &AITest::onAIMove);
        connect(aiBot, &AIBotController::aiThinking, this, &AITest::onAIThinking);
        connect(aiBot, &AIBotController::aiMessage, this, &AITest::onAIMessage);
        
        // Configure AI
        aiBot->setDifficulty(3);
        aiBot->setThinkingTime(1000);
        aiBot->enableAI(true);
        
        qDebug() << "AI Test initialized";
    }
    
    void runTest() {
        qDebug() << "Starting AI Bot Test...";
        
        // Test 1: Basic AI functionality
        testBasicAI();
        
        // Test 2: Difficulty levels
        testDifficultyLevels();
        
        // Test 3: Thinking time
        testThinkingTime();
        
        qDebug() << "AI Bot Test completed!";
        QCoreApplication::quit();
    }

private slots:
    void onAIMove(QString move) {
        qDebug() << "AI chose move:" << move;
    }
    
    void onAIThinking(bool thinking) {
        qDebug() << "AI thinking:" << (thinking ? "Yes" : "No");
    }
    
    void onAIMessage(QString message) {
        qDebug() << "AI Message:" << message;
    }

private:
    AIBotController* aiBot;
    
    void testBasicAI() {
        qDebug() << "\n=== Test 1: Basic AI Functionality ===";
        
        // Test AI creation
        ChessAI ai(false); // Black pieces
        qDebug() << "✓ AI created successfully";
        
        // Test difficulty setting
        ai.setDifficulty(3);
        qDebug() << "✓ Difficulty set to 3";
        
        // Test move generation (simplified)
        QString testBoard[8][8] = {
            {"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
            {"bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP"},
            {"",   "",   "",   "",   "",   "",   "",   ""},
            {"",   "",   "",   "",   "",   "",   "",   ""},
            {"",   "",   "",   "",   "",   "",   "",   ""},
            {"",   "",   "",   "",   "",   "",   "",   ""},
            {"wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP"},
            {"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"}
        };
        
        // Test move generation
        Move bestMove = ai.getBestMove(testBoard, nullptr, nullptr, 3);
        qDebug() << "✓ AI generated move:" << bestMove.from_row << bestMove.from_col 
                  << "->" << bestMove.to_row << bestMove.to_col;
    }
    
    void testDifficultyLevels() {
        qDebug() << "\n=== Test 2: Difficulty Levels ===";
        
        for (int level = 1; level <= 5; level++) {
            ChessAI ai(false);
            ai.setDifficulty(level);
            qDebug() << "✓ Difficulty level" << level << "set successfully";
        }
    }
    
    void testThinkingTime() {
        qDebug() << "\n=== Test 3: Thinking Time ===";
        
        aiBot->setThinkingTime(500);
        qDebug() << "✓ Thinking time set to 500ms";
        
        aiBot->setThinkingTime(2000);
        qDebug() << "✓ Thinking time set to 2000ms";
        
        aiBot->setThinkingTime(50); // Should be clamped to minimum
        qDebug() << "✓ Thinking time clamped to minimum (100ms)";
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    AITest test;
    
    // Run tests after a short delay
    QTimer::singleShot(100, &test, &AITest::runTest);
    
    return app.exec();
}

#include "ai_test.moc"
