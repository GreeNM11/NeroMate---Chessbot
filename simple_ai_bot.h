#ifndef SIMPLE_AI_BOT_H
#define SIMPLE_AI_BOT_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <vector>
#include <utility>

class SimpleAIBot : public QObject
{
    Q_OBJECT

public:
    explicit SimpleAIBot(QObject *parent = nullptr);
    ~SimpleAIBot();

    // Configuration
    void setDifficulty(int level);
    void setThinkingTime(int milliseconds);
    void enableAI(bool enabled);

    // Game integration
    void onHumanMove(const QString& move);
    void startGame();
    void stopGame();

signals:
    void aiMoveReady(QString move);
    void aiThinking(bool thinking);
    void aiMessage(QString message);

private slots:
    void makeMove();

private:
    // AI Configuration
    int difficulty;
    int thinkingTime;
    bool aiEnabled;
    bool isAITurn;
    
    QTimer* thinkingTimer;
    
    // Simple AI logic
    QString generateRandomMove();
    QString generateSimpleMove();
    
    // Move validation
    bool isValidMove(const QString& move);
    
    // Game state tracking
    QString lastHumanMove;
    int moveCount;
    bool isBlackPlayer; // AI is always black
};

#endif // SIMPLE_AI_BOT_H
