#pragma once

#include <queue>

class Logger {
private:
    std::queue<char> messageQueue;
    Logger();

public:
    static Logger &instance();

    void LogMessage(const char msg);
    bool HasMessage();
    const char GetMessage();
};