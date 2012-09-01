#include "TransparencyWindow.h"
#include "moc_TransparencyWindow.h"
#include <Logfile.h>

TransparencyWindow::TransparencyWindow(QMainWindow *parent, QPixmap &p_Pixmap) : QMainWindow(parent), Ui_TransparencyWindow()
{
    setupUi(this);
    ShowTexture(p_Pixmap);
    TextureView->installEventFilter(this);
}

TransparencyWindow::~TransparencyWindow(void)
{
    if (QWidget *t_pParent = (QWidget*)parent())
        t_pParent->setEnabled(true);
}

void TransparencyWindow::closeEvent(QCloseEvent *p_Event)
{
    p_Event->accept();
    delete this;
}

void TransparencyWindow::resizeEvent(QResizeEvent *p_Event)
{
    TextureView->setFixedSize(p_Event->size());
}

void TransparencyWindow::ShowTexture(QPixmap &p_Pixmap)
{
    m_Pixmap = p_Pixmap;
    QSize t_Size = m_Pixmap.size();
    setFixedSize(t_Size);
    TextureView->setPixmap(p_Pixmap);
}

bool TransparencyWindow::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
    if (p_pObj == TextureView && p_pEvent->type() == QEvent::MouseButtonPress)
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
                emit ColorChosen(QString(("R" + ToString(t_Red) + "G" + ToString(t_Green) + "B" + ToString(t_Blue)).c_str()));
                close();
                return true;
            }
        }
    }
    else // pass the event on to the parent class
        return QMainWindow::eventFilter(p_pObj, p_pEvent);

    return false;
}