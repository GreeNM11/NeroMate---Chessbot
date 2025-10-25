#include "simple_ai_bot.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QTimer>

SimpleAIBot::SimpleAIBot(QObject *parent)
    : QObject(parent), difficulty(3), thinkingTime(1500), aiEnabled(false), 
      isAITurn(false), moveCount(0), isBlackPlayer(true)
{
    thinkingTimer = new QTimer(this);
    thinkingTimer->setSingleShot(true);
    connect(thinkingTimer, &QTimer::timeout, this, &SimpleAIBot::makeMove);
    
    qDebug() << "Simple AI Bot initialized as Black player";
}

SimpleAIBot::~SimpleAIBot() {
    // Clean up timer
    if (thinkingTimer) {
        thinkingTimer->stop();
        thinkingTimer->deleteLater();
    }
}

void SimpleAIBot::setDifficulty(int level) {
    difficulty = std::max(1, std::min(5, level));
    qDebug() << "AI difficulty set to" << difficulty;
}

void SimpleAIBot::setThinkingTime(int milliseconds) {
    thinkingTime = std::max(500, milliseconds);
    qDebug() << "AI thinking time set to" << thinkingTime << "ms";
}

void SimpleAIBot::enableAI(bool enabled) {
    aiEnabled = enabled;
    qDebug() << "AI" << (enabled ? "enabled" : "disabled");
}

void SimpleAIBot::onHumanMove(const QString& move) {
    qDebug() << "=== SimpleAIBot::onHumanMove called ===";
    qDebug() << "Received move:" << move;
    
    lastHumanMove = move;
    moveCount++;
    
    qDebug() << "Human (white) moved:" << move << "Move count:" << moveCount;
    qDebug() << "AI (black) enabled:" << aiEnabled << "Thinking time:" << thinkingTime;
    qDebug() << "Timer available:" << (thinkingTimer != nullptr);
    
    if (aiEnabled) {
        qDebug() << "AI (black) starting thinking process";
        // Start thinking after a short delay
        QTimer::singleShot(300, this, [this]() {
            qDebug() << "AI (black) thinking delay completed, starting timer";
            emit aiThinking(true);
            emit aiMessage("AI (Black) is thinking...");
            thinkingTimer->start(thinkingTime);
        });
    } else {
        qDebug() << "AI (black) is not enabled, not responding to human move";
    }
    qDebug() << "=== SimpleAIBot::onHumanMove completed ===";
}

void SimpleAIBot::startGame() {
    moveCount = 0;
    lastHumanMove = "";
    qDebug() << "AI game started";
}

void SimpleAIBot::stopGame() {
    qDebug() << "Stopping AI game";
    
    try {
        // Stop any ongoing thinking
        if (thinkingTimer) {
            thinkingTimer->stop();
            qDebug() << "Thinking timer stopped";
        }
        
        // Reset state
        moveCount = 0;
        lastHumanMove = "";
        aiEnabled = false;
        
        qDebug() << "AI game stopped successfully";
    } catch (...) {
        qDebug() << "Error stopping AI game";
        aiEnabled = false;
    }
}

void SimpleAIBot::makeMove() {
    qDebug() << "makeMove() called - AI (black) enabled:" << aiEnabled << "Timer available:" << (thinkingTimer != nullptr);
    
    if (!aiEnabled || !thinkingTimer) {
        qDebug() << "AI (black) bot not enabled or timer not available";
        return;
    }
    
    qDebug() << "AI (black) bot making move - difficulty:" << difficulty;
    
    try {
        QString move;
    
    // Generate move based on difficulty
    switch (difficulty) {
        case 1: // Very Easy - Random moves
            move = generateRandomMove();
            break;
        case 2: // Easy - Simple moves
            move = generateSimpleMove();
            break;
        case 3: // Medium - Slightly better moves
            move = generateSimpleMove();
            break;
        case 4: // Hard - Better moves
            move = generateSimpleMove();
            break;
        case 5: // Expert - Best moves
            move = generateSimpleMove();
            break;
        default:
            move = generateRandomMove();
            break;
    }
    
        qDebug() << "AI (black) chose move:" << move << "Format: 4-digit (row/col, row/col)";
        
        emit aiThinking(false);
        emit aiMessage("AI (Black) made move: " + move);
        emit aiMoveReady(move);
        
        qDebug() << "AI (black) move completed successfully - Move sent to game";
    } catch (...) {
        qDebug() << "Error in AI makeMove";
        emit aiThinking(false);
        emit aiMessage("AI error occurred");
    }
}

QString SimpleAIBot::generateRandomMove() {
    // Generate a random valid chess move for black player in 4-digit format (row/col, row/col)
    // Black pieces start at rows 0-1, so valid starting positions are (0,0) to (1,7)
    QString moves[] = {
        "1020", "1121", "1222", "1323", "1424", "1525", "1626", "1727", // Black pawn moves from row 1 to row 2
        "1030", "1131", "1232", "1333", "1434", "1535", "1636", "1737", // Black pawn moves from row 1 to row 3
        "1020", "1121", "1222", "1323", "1424", "1525", "1626", "1727", // More pawn moves
        "1020", "1121", "1222", "1323", "1424", "1525", "1626", "1727"  // Additional moves
    };
    
    int randomIndex = QRandomGenerator::global()->bounded(32);
    return moves[randomIndex];
}

QString SimpleAIBot::generateSimpleMove() {
    // Generate a simple but valid move for black player in 4-digit format
    // This is a placeholder - in a real implementation, you'd analyze the board
    
    // For now, return a simple move pattern for black in 4-digit format
    QString simpleMoves[] = {
        "1020", "1121", "1222", "1323", "1424", "1525", "1626", "1727" // Black pawn moves from row 1 to row 2
    };
    
    int moveIndex = moveCount % 8;
    return simpleMoves[moveIndex];
}

bool SimpleAIBot::isValidMove(const QString& move) {
    // Basic validation - check if move format is correct
    if (move.length() != 4) return false;
    
    for (int i = 0; i < 4; i++) {
        if (!move[i].isDigit()) return false;
        int digit = move[i].digitValue();
        if (digit < 0 || digit > 7) return false;
    }
    
    return true;
}
