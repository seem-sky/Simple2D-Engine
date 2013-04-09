#include "TransparencyWindow.h"

TransparencyWindow::TransparencyWindow(QWidget *parent, QPixmap &pixmap) : QDialog(parent), Ui_TransparentColorDialog()
{
    setupUi(this);
    showTexture(pixmap);
    m_pPic->installEventFilter(this);
}

void TransparencyWindow::resizeEvent(QResizeEvent *pEvent)
{
    m_pPic->resize(m_Pixmap.size());
}

void TransparencyWindow::showTexture(QPixmap &pixmap)
{
    m_Pixmap = pixmap;
    QSize size = m_Pixmap.size();
    setFixedSize(size);
    m_pPic->setPixmap(pixmap);
}

bool TransparencyWindow::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (pObj && pEvent && pObj == m_pPic && pEvent->type() == QEvent::MouseButtonPress)
    {
        if (QMouseEvent* pMouseEvent = (QMouseEvent*)pEvent)
        {
            if (pMouseEvent->button() == Qt::LeftButton)
            {
                QPoint mouseCoord = pMouseEvent->pos();
                QImage image = m_Pixmap.toImage();
                QColor color(image.pixel(mouseCoord));
                m_RGBresult = Color(color.red(), color.green(), color.blue());
                accept();
                return true;
            }
        }
    }
    else // pass the event on to the parent class
        return QDialog::eventFilter(pObj, pEvent);

    return false;
}