#include "SpriteDatabaseWidget.h"
#include "moc_SpriteDatabaseWidget.h"
#include <QtGui/QBitmap>
#include <QtGui/QWheelEvent>

using namespace DATABASE;

SpriteDatabaseWidget::SpriteDatabaseWidget(QWidget *pParent) : TextureDatabaseWidget(pParent), Ui_SpriteDatabaseWidget(), m_CurScale(100)
{
    Ui_SpriteDatabaseWidget::setupUi(this);
    m_pView->installEventFilter(this);

    connectWidgets();

    // setup connections
    connect(m_pBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    connect(m_pBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    connect(m_pBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    connect(m_pBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    connect(m_pColumn, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    connect(m_pRow, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
}

void SpriteDatabaseWidget::changeCurrentItem(uint32 uiID)
{
    TextureDatabaseWidget::changeCurrentItem(uiID);
    _updatePixmap();
}

bool SpriteDatabaseWidget::getPrototypeFromWidgets(SpritePrototypePtr &proto)
{
    if (!TextureDatabaseWidget::getPrototypeFromWidgets(proto))
        return false;

    proto->setColumns(m_pColumn->value());
    proto->setRows(m_pRow->value());
    proto->setBoundingRect(UInt32Rect(Point<uint32>(m_pBoundingLeft->value(), m_pBoundingUp->value()),
        Point<uint32>(m_pBoundingRight->value(), m_pBoundingBottom->value())));
    return true;
}

bool SpriteDatabaseWidget::setWidgetsFromPrototype(const SpritePrototypePtr &proto)
{
    if (!TextureDatabaseWidget::setWidgetsFromPrototype(proto))
        return false;

    m_pRow->setValue(proto->getRows());
    m_pColumn->setValue(proto->getColumns());
    UInt32Rect boundingRect = proto->getBoundingRect();
    m_pBoundingLeft->setValue(boundingRect.getPositionX());
    m_pBoundingUp->setValue(boundingRect.getPositionY());
    m_pBoundingRight->setValue(boundingRect.getWidth());
    m_pBoundingBottom->setValue(boundingRect.getHeight());
    return true;
}

QPixmap SpriteDatabaseWidget::showPixmap(const QString &sFilePath)
{
    m_Pixmap = QPixmap(sFilePath);
    if (!m_Pixmap.isNull())
    {
        // set transparency color
        Color color(m_pTransparencyColor->text().toStdString());
        if (color.hasValidColor())
            m_Pixmap.setMask(m_Pixmap.createMaskFromColor(QColor(color.getRed(), color.getGreen(), color.getBlue())));
    }
    _updatePixmap();
    return m_Pixmap;
}

void SpriteDatabaseWidget::_updatePixmap()
{
    if (m_Pixmap.isNull())
        return;

    // draw pixmap
    Point<uint8> frames(m_pColumn->value(), m_pRow->value());
    QPixmap scenePixmap(m_Pixmap.size().width() / frames.x, m_Pixmap.size().height() / frames.y);
    QPainter painter(&scenePixmap);
    painter.drawPixmap(QRect(0, 0, scenePixmap.size().width(), scenePixmap.size().height()), m_Pixmap, QRect(0, 0, scenePixmap.size().width(), scenePixmap.size().height()));
    QGraphicsScene *pScene = new QGraphicsScene();
    pScene->addPixmap(scenePixmap);

    // draw bounding rect
    if (m_pBoundingRight->value() != 0 && m_pBoundingBottom->value() != 0)
        pScene->addRect(m_pBoundingLeft->value(), m_pBoundingUp->value(), m_pBoundingRight->value()-1, m_pBoundingBottom->value()-1, QPen(QColor(255, 0, 0)));
    m_pView->setScene(pScene);
}

void SpriteDatabaseWidget::_dataChanged(int value)
{
    updateItem();
    _updatePixmap();
}

bool SpriteDatabaseWidget::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (!pEvent)
        return false;

    if (pObj == m_pView && pEvent->type() == QEvent::Wheel)
    {
        QWheelEvent *pWheelEvent = (QWheelEvent*)(pEvent);
        // calculate new scale
        double newScale = 100.0 / m_CurScale*(m_CurScale - pWheelEvent->delta() * -0.05) / 100;
        m_pView->scale(newScale, newScale);
    }

    return false;
}