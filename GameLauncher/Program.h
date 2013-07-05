#ifndef PROGRAM_H
#define PROGRAM_H

#include "Application.h"
#include <QtWidgets/QApplication>

typedef std::unique_ptr<QApplication> QApplicationPtr;

namespace PROGRAM
{
    class Program
    {
    public:
        void setApplication(ApplicationPtr pApp);

        void init(int argc, char *argv[]);
        int exec();

    private:
        ApplicationPtr m_pApplication;
        QApplicationPtr m_pQtApplication;
    };
    static Program Simple2D_Engine_Program;
}
#endif