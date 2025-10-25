# CMakeLists.txt for NeroMate Chess Bot
# This file should be included in your main ChessQt CMakeLists.txt

# Add AI bot source files
set(AI_BOT_SOURCES
    chess_ai.cpp
    ai_bot_controller.cpp
)

set(AI_BOT_HEADERS
    chess_ai.h
    ai_bot_controller.h
)

# Create AI bot library
add_library(ChessAI STATIC ${AI_BOT_SOURCES} ${AI_BOT_HEADERS})

# Link with Qt libraries
target_link_libraries(ChessAI
    Qt5::Core
    Qt5::Widgets
)

# Include directories
target_include_directories(ChessAI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../game_client/chess_logic
)

# Set C++ standard
target_compile_features(ChessAI PRIVATE cxx_std_17)

# Compiler-specific options
if(MSVC)
    target_compile_options(ChessAI PRIVATE /W4)
else()
    target_compile_options(ChessAI PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional: Create a simple test executable
option(BUILD_AI_TESTS "Build AI bot tests" OFF)

if(BUILD_AI_TESTS)
    add_executable(ai_test
        ai_integration_example.cpp
    )
    
    target_link_libraries(ai_test
        ChessAI
        Qt5::Core
        Qt5::Widgets
    )
endif()
