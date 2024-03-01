#include "gamelogs.h"

Logger::Logger() :
  messageQueue(std::queue<char>()) {
}

Logger &Logger::instance() {
    static Logger l = Logger();
    return l;
}

void Logger::LogMessage(const char msg) {
    messageQueue.push(msg);
}

const char Logger::GetMessage() {
    const char msg = messageQueue.front();
    messageQueue.pop();
    return msg;
}

bool Logger::HasMessage() { return !messageQueue.empty(); }