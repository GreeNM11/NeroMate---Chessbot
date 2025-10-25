# NeroMate Chess Bot

An AI chess bot designed to play against human players in the ChessQt application.

## Features

- **Minimax Algorithm**: Uses minimax with alpha-beta pruning for optimal move selection
- **Configurable Difficulty**: 5 difficulty levels (1-5) affecting search depth
- **Position Evaluation**: Evaluates positions based on piece values and positional bonuses
- **Move Generation**: Generates all legal moves for each piece type
- **Integration Ready**: Designed to integrate seamlessly with the existing ChessQt game

## Architecture

### Core Components

1. **ChessAI** (`chess_ai.h/cpp`)
   - Main AI engine implementing minimax algorithm
   - Position evaluation and move generation
   - Configurable difficulty levels

2. **AIBotController** (`ai_bot_controller.h/cpp`)
   - High-level controller for AI behavior
   - Handles timing and game state integration
   - Manages AI thinking process

3. **Integration Example** (`ai_integration_example.cpp`)
   - Shows how to integrate AI with existing chess game
   - Provides integration patterns and examples

## Usage

### Basic Integration

```cpp
#include "ai_bot_controller.h"

// Create AI bot for black pieces
AIBotController* aiBot = new AIBotController(false, this);

// Configure AI
aiBot->setDifficulty(3); // Medium difficulty
aiBot->setThinkingTime(1500); // 1.5 seconds
aiBot->enableAI(true);

// Connect to game
aiBot->connectToGame(boardState);

// Connect signals
connect(aiBot, &AIBotController::aiMoveReady, this, &MyGame::onAIMove);
```

### Difficulty Levels

- **Level 1**: Very Easy (2-ply search)
- **Level 2**: Easy (3-ply search)
- **Level 3**: Medium (4-ply search)
- **Level 4**: Hard (5-ply search)
- **Level 5**: Expert (6-ply search)

### AI Configuration

```cpp
// Set difficulty (1-5)
aiBot->setDifficulty(4);

// Set thinking time in milliseconds
aiBot->setThinkingTime(2000);

// Enable/disable AI
aiBot->enableAI(true);
```

## Integration with ChessQt

### Required Modifications

1. **Add AI signals to board_state**:
   ```cpp
   signals:
       void moveMade(QString move);
       void turnChanged(bool isWhiteTurn);
   ```

2. **Add board state getter to chess_game**:
   ```cpp
   board_state* getBoardState() { return game_state.get(); }
   ```

3. **Update UI for AI controls**:
   - Add difficulty slider
   - Add AI enable/disable toggle
   - Add thinking indicator

### Example Integration

See `ai_integration_example.cpp` for a complete example of how to integrate the AI bot with your existing chess game.

## Building

The AI bot is designed to be built as part of the main ChessQt project. Add the AI bot source files to your main CMakeLists.txt:

```cmake
# Add AI bot to your main target
target_sources(ChessQt PRIVATE
    NeroMate---Chessbot/chess_ai.cpp
    NeroMate---Chessbot/ai_bot_controller.cpp
)

target_include_directories(ChessQt PRIVATE
    NeroMate---Chessbot
)
```

## Algorithm Details

### Minimax with Alpha-Beta Pruning

The AI uses the minimax algorithm with alpha-beta pruning to find the best move:

1. **Maximizing Player**: AI tries to maximize its position evaluation
2. **Minimizing Player**: AI assumes opponent tries to minimize AI's position
3. **Alpha-Beta Pruning**: Eliminates branches that won't affect the final decision

### Position Evaluation

The AI evaluates positions based on:

- **Piece Values**: Pawn=100, Knight=320, Bishop=330, Rook=500, Queen=900, King=20000
- **Positional Bonuses**: Pawn advancement, piece activity, king safety
- **Material Balance**: Total piece value comparison

### Move Generation

The AI generates moves for each piece type:

- **Pawns**: Forward moves, captures, en passant
- **Knights**: L-shaped moves
- **Bishops**: Diagonal moves
- **Rooks**: Horizontal and vertical moves
- **Queen**: Combines rook and bishop moves
- **King**: One square in any direction, castling

## Future Enhancements

- **Opening Book**: Add common opening patterns
- **Endgame Database**: Use endgame tablebase for perfect endgame play
- **Machine Learning**: Implement neural network evaluation
- **Time Management**: Dynamic thinking time based on position complexity
- **Pondering**: Think during opponent's turn

## Contributing

To contribute to the AI bot:

1. Fork the repository
2. Create a feature branch
3. Implement your changes
4. Add tests if applicable
5. Submit a pull request

## License

This AI bot is part of the ChessQt project and follows the same license terms.
