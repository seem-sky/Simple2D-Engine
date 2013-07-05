#include "SplashScreen.h"
#include "Program.h"
#include <QtGui/QPixmap>
#include <QtWidgets/QSplashScreen>

using namespace SPLASH_SCREEN;

CustomSplashScreen::CustomSplashScreen(const QPixmap &pixmap) : QSplashScreen(pixmap)
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void CustomSplashScreen::mousePressEvent(QMouseEvent *pEvent)
{}

SplashScreen::SplashScreen() : Application()
{
    m_pWidget = QWidgetPtr(new CustomSplashScreen(QPixmap("icons/Logo.png")));
    m_pWidget->show();
}

int SplashScreen::update(uint32 uiDiff)
{
    return 0;
}
