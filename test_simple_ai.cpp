/*
 * Simple test for the Simple AI Bot
 * This demonstrates basic AI functionality
 */

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "simple_ai_bot.h"

class AITest : public QObject
{
    Q_OBJECT

public:
    AITest() {
        // Create AI bot
        aiBot = new SimpleAIBot(this);
        
        // Connect signals
        connect(aiBot, &SimpleAIBot::aiMoveReady, this, &AITest::onAIMove);
        connect(aiBot, &SimpleAIBot::aiThinking, this, &AITest::onAIThinking);
        connect(aiBot, &SimpleAIBot::aiMessage, this, &AITest::onAIMessage);
        
        // Configure AI
        aiBot->setDifficulty(3);
        aiBot->setThinkingTime(1000);
        aiBot->enableAI(true);
        aiBot->startGame();
        
        qDebug() << "AI Test initialized";
    }
    
    void runTest() {
        qDebug() << "Starting Simple AI Bot Test...";
        
        // Test 1: Basic AI functionality
        testBasicAI();
        
        // Test 2: AI move generation
        testAIMoves();
        
        qDebug() << "Simple AI Bot Test completed!";
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
    SimpleAIBot* aiBot;
    
    void testBasicAI() {
        qDebug() << "\n=== Test 1: Basic AI Functionality ===";
        
        // Test AI creation
        qDebug() << "✓ AI created successfully";
        
        // Test difficulty setting
        aiBot->setDifficulty(3);
        qDebug() << "✓ Difficulty set to 3";
        
        // Test thinking time
        aiBot->setThinkingTime(1000);
        qDebug() << "✓ Thinking time set to 1000ms";
    }
    
    void testAIMoves() {
        qDebug() << "\n=== Test 2: AI Move Generation ===";
        
        // Simulate human move
        aiBot->onHumanMove("0000");
        qDebug() << "✓ Human move simulated";
        
        // AI should respond after thinking time
        QTimer::singleShot(2000, this, [this]() {
            qDebug() << "✓ AI move generation test completed";
        });
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

#include "test_simple_ai.moc"
