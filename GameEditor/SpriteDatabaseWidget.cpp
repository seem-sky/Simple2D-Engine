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
}

void SpriteDatabaseWidget::changeCurrentItem(uint32 uiID)
{
    TextureDatabaseWidget::changeCurrentItem(uiID);
    _updatePixmap();
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
    QGraphicsScene *pScene = new QGraphicsScene();
    pScene->addPixmap(m_Pixmap);
    m_pView->setScene(pScene);
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