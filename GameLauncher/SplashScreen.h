#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "Application.h"
#include <QtWidgets/QSplashScreen>

namespace SPLASH_SCREEN
{
    class CustomSplashScreen : public QSplashScreen
    {
    protected:
        void mousePressEvent(QMouseEvent* pEvent);

    public:
        CustomSplashScreen(const QPixmap&  pixmap);
    };

    class SplashScreen : public PROGRAM::Application
    {
    public:
        SplashScreen();

        int update(uint32 uiDiff);
    };
}
#endif