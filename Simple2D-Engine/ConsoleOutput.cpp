#include "ConsoleOutput.h"
#include <QtCore/QDebug>

ConsoleOutput::ConsoleOutput()
{
    setMessageLevel(LOG::MessageLevel::standard);
    setMessageLevel(LOG::MessageLevel::warning);
    setMessageLevel(LOG::MessageLevel::error);
}

void ConsoleOutput::writeMessage(const std::string& msg, const std::string& fileName, uint32 line, LOG::MessageLevel lvl)
{
    if (!hasMessageLevel(lvl))
        return;
    switch (lvl)
    {
    case LOG::MessageLevel::standard: qDebug() << QString::fromStdString(msg); break;
    case LOG::MessageLevel::warning: qDebug() << "WARNING:" << QString::fromStdString(msg); break;
    case LOG::MessageLevel::error: qDebug() << "ERROR:" << QString::fromStdString(msg); break;
    }
}

void ConsoleOutput::clear()
{
    system("cls");
}