#ifndef PROGRAM_H
#define PROGRAM_H

#include "Application.h"
#include <QtWidgets/QApplication>

typedef std::unique_ptr<QApplication> QApplicationPtr;

namespace PROGRAM
{
    class CMD_Params
    {
    public:
        CMD_Params();

        void parseParam(const QString& param);

        bool isDebugMode() const { return m_DebugMode; }

    private:
        bool m_DebugMode;
    };

    class Program
    {
    public:
        void setApplication(ApplicationPtr pApp);

        void init(int argc, char* argv[]);
        int exec();

    private:
        ApplicationPtr m_pApplication;
        QApplicationPtr m_pQtApplication;
        CMD_Params m_CMDparams;
    };
    static Program Simple2D_Engine_Program;
}
#endif