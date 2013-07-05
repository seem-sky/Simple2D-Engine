#ifndef APPLICATION_H
#define APPLICATION_H

#include "Global.h"
#include <QtWidgets/QWidget>

typedef std::unique_ptr<QWidget> QWidgetPtr;

namespace PROGRAM
{
    class Application
    {
    public:
        virtual int update(uint32 uiDiff) = 0;

    protected:
        QWidgetPtr m_pWidget;
    };

    typedef std::unique_ptr<Application> ApplicationPtr;
}
#endif