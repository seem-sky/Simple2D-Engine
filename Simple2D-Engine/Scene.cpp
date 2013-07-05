#include "Scene.h"
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsView>

using namespace GAME_LOGIC;
using namespace SCENE;

const UInt32Point FPS_SPACE(75, 40);

Scene::Scene(SceneMgr &sceneMgr, const KEY::Keyboard &keyboard) : m_SceneMgr(sceneMgr), m_pSceneView(nullptr), m_ShowFPS(false), m_Keyboard(keyboard),
     m_pPlayer(new PLAYER::Player(keyboard))
{}

void SceneView::drawFPSCounter(QPainter *painter, const QRectF &rect)
{
    if (m_pScene->isFPSShown())
    {
        if (QGraphicsView *pView = dynamic_cast<QGraphicsView*>(parent()))
        {
            QFont newFont(painter->font());
            newFont.setBold(true);
            newFont.setPointSize(FPS_SPACE.x/3);
            painter->setFont(newFont);
            painter->setPen(QColor(255,215,0));
            QPointF textPos = pView->mapToScene(pView->width()-FPS_SPACE.x, FPS_SPACE.y);
            painter->drawText(textPos, QString::number(m_FPSCounter.elapsed() ? IN_MILLISECOND/m_FPSCounter.elapsed() : 0));
        }
    }
    m_FPSCounter.restart();
}

void SceneView::drawForeground(QPainter *painter, const QRectF &rect)
{
    drawFPSCounter(painter, rect);
}