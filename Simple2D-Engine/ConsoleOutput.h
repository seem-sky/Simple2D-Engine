#ifndef LOG_CONSOLE_OUTPUT_H
#define LOG_CONSOLE_OUTPUT_H

#include <log/OutputInterface.h>

class ConsoleOutput : public LOG::OutputInterface
{
public:
    ConsoleOutput();

    void writeMessage(const std::string& msg, const std::string& fileName, uint32 line, LOG::MessageLevel lvl);
    void clear();
};
#endif
