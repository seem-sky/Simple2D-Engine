#include "TransparencyWindow.h"
#include "moc_TransparencyWindow.h"
#include <Logfile.h>

TransparencyWindow::TransparencyWindow(QWidget *parent, QPixmap &p_Pixmap) : QDialog(parent), m_pPic(NULL)
{
    m_pPic = new QLabel(this);
    m_pPic->setGeometry(0, 0, 10, 10);
    ShowTexture(p_Pixmap);
    m_pPic->installEventFilter(this);
}

void TransparencyWindow::resizeEvent(QResizeEvent *p_Event)
{
    m_pPic->setFixedSize(p_Event->size());
}

void TransparencyWindow::ShowTexture(QPixmap &p_Pixmap)
{
    m_Pixmap = p_Pixmap;
    QSize t_Size = m_Pixmap.size();
    setFixedSize(t_Size);
    m_pPic->setPixmap(p_Pixmap);
}

bool TransparencyWindow::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
    if (p_pObj == m_pPic && p_pEvent->type() == QEvent::MouseButtonPress)
    {
        if (QMouseEvent* t_pEvent = (QMouseEvent*)p_pEvent)
        {
            if (t_pEvent->button() == Qt::LeftButton)
            {
                QPoint t_MouseCoord = t_pEvent->pos();
                QImage t_Image = m_Pixmap.toImage();
                QColor t_Color(t_Image.pixel(t_MouseCoord));
                int t_Red   = t_Color.red();
                int t_Green = t_Color.green();
                int t_Blue  = t_Color.blue();
                emit ColorChosen("R" + QString::number(t_Red) + "G" + QString::number(t_Green) + "B" + QString::number(t_Blue));
                close();
                return true;
            }
        }
    }
    else // pass the event on to the parent class
        return QDialog::eventFilter(p_pObj, p_pEvent);

    return false;
}