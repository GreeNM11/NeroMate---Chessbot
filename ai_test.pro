# Qt project file for AI Bot Test
# This can be used to build and test the AI bot independently

QT += core widgets
CONFIG += console
CONFIG -= app_bundle

TARGET = ai_test
TEMPLATE = app

# Source files
SOURCES += \
    ai_test.cpp \
    chess_ai.cpp \
    ai_bot_controller.cpp

# Header files
HEADERS += \
    chess_ai.h \
    ai_bot_controller.h

# Include directories
INCLUDEPATH += . \
    ../game_client/chess_logic

# Compiler flags
QMAKE_CXXFLAGS += -std=c++17

# Debug configuration
CONFIG(debug, debug|release) {
    DEFINES += DEBUG
    QMAKE_CXXFLAGS += -g
}

# Release configuration
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
    QMAKE_CXXFLAGS += -O2
}

# Windows specific
win32 {
    QMAKE_CXXFLAGS += /W4
}

# Unix specific
unix {
    QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
}
