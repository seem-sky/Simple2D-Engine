#include "DatabaseDialogChoseColor.h"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

ChoseColorDialog::ChoseColorDialog(const QPixmap& pixmap) : m_Image(pixmap.toImage())
{
    resize(m_Image.width(), m_Image.height());
    setMinimumSize(size());
    setMaximumSize(size());
}

void ChoseColorDialog::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() != Qt::LeftButton)
        return;
    auto pos = pEvent->pos();
    if (pos.x() <= 0 || pos.x() >= m_Image.width() ||
        pos.x() <= 0 || pos.x() >= m_Image.height())
        return;

    m_ColorClicked = m_Image.pixel(pos);
    accept();
}

void ChoseColorDialog::paintEvent(QPaintEvent *pEvent)
{
    QDialog::paintEvent(pEvent);
    QPainter painter(this);
    painter.drawImage(0, 0, m_Image);
}
