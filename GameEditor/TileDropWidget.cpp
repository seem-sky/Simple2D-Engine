#include "TileDropWidget.h"
#include "moc_TileDropWidget.h"
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include <QtGui/QPainter>
#include <QtWidgets/QMenu>
#include <QtGui/QContextMenuEvent>

TileDropWidget::TileDropWidget(const helper::CreatorInterface& creator, QWidget* pParent)
    : QFrame(pParent), m_Creator(creator)
{
    setAcceptDrops(true);
}

void TileDropWidget::dragMoveEvent(QDragMoveEvent* pEvent)
{
    pEvent->accept();
}

void TileDropWidget::dragEnterEvent(QDragEnterEvent* pEvent)
{
    pEvent->acceptProposedAction();
}

void TileDropWidget::dropEvent(QDropEvent* pEvent)
{
    uint32 ID = pEvent->mimeData()->text().toUInt();
    if (ID)
        setTileID(ID);
}

void TileDropWidget::contextMenuEvent(QContextMenuEvent* pEvent)
{
    QMenu menu;
    menu.move(pEvent->globalPos());
    connect(menu.addAction("clear"), SIGNAL(triggered()), this, SLOT(_onCleared()));
    menu.exec();
}

uint32 TileDropWidget::getTileID() const
{
    return m_TileID;
}

void TileDropWidget::setTileID(uint32 uiTileID)
{
    m_TileID = uiTileID;
    m_Pixmap = m_Creator.createPixmap(m_TileID);
    repaint();
}

void TileDropWidget::paintEvent(QPaintEvent* pEvent)
{
    QFrame::paintEvent(pEvent);

    QPainter painter(this);
    painter.drawPixmap(frameWidth(), frameWidth(), m_Pixmap);
}

void TileDropWidget::_onCleared()
{
    setTileID(0);
}
