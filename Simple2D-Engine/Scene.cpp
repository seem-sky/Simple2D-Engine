#include "Scene.h"
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsSceneWheelEvent>
#include <QtGui/QGuiApplication>
#include <QtGui/QWindow>
#include "GameWindow.h"

using namespace GAME_LOGIC;
using namespace SCENE;

const uint8 FPS_SPACE = 25;
const uint8 FPS_BORDER = 15;

Scene::Scene(Game &game) : m_Game(game), m_pSceneView(nullptr), m_ShowFPS(false)
{}

void SceneView::drawFPSCounter(QPainter *painter, const QRectF &rect)
{
    if (m_pScene->isFPSShown())
    {
        if (QGraphicsView *pView = dynamic_cast<QGraphicsView*>(parent()))
        {
            QString frameString = QString::number(m_FPSCounter.elapsed() ? IN_MILLISECOND/m_FPSCounter.elapsed() : 0);
            QFont newFont(painter->font());
            newFont.setBold(true);
            newFont.setPointSize(FPS_SPACE);
            painter->setFont(newFont);
            painter->setPen(QColor(255,215,0));
            // calculate pView scaling
            QPointF textPos = pView->mapToScene(pView->width()-(FPS_SPACE*frameString.length()+FPS_BORDER)*pView->transform().m11(),
                (FPS_SPACE+FPS_BORDER)*pView->transform().m22());
            painter->drawText(textPos, frameString);
        }
    }
    m_FPSCounter.restart();
}

void SceneView::drawForeground(QPainter *painter, const QRectF &rect)
{
    drawFPSCounter(painter, rect);
}