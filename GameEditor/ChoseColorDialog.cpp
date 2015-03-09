#include "ChoseColorDialog.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <Global.h>

static const uint32 SCALE = 4;

ChoseColorDialog::ChoseColorDialog(const QPixmap& pixmap, QWidget* pParent)
    : QDialog(pParent), m_Image(pixmap.toImage())
{
    resize(m_Image.width() * SCALE, m_Image.height() * SCALE);
    setMinimumSize(size());
    setMaximumSize(size());
}

void ChoseColorDialog::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() != Qt::LeftButton)
        return;
    auto pos = pEvent->pos() / SCALE;
    if (pos.x() <= 0 || pos.x() >= m_Image.width() ||
        pos.x() <= 0 || pos.x() >= m_Image.height())
        return;

    m_ColorClicked = m_Image.pixel(pos);
    accept();
}

void ChoseColorDialog::paintEvent(QPaintEvent* pEvent)
{
    QDialog::paintEvent(pEvent);
    QPainter painter(this);
    painter.scale(SCALE, SCALE);
    painter.drawImage(0, 0, m_Image);
}

QColor ChoseColorDialog::getColor() const
{
    return m_ColorClicked;
}
