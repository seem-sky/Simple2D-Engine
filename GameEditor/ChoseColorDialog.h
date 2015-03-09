#pragma once

#include <QtWidgets/QDialog>
#include <QtGui/QImage>

class ChoseColorDialog : public QDialog
{
protected:
    void mouseReleaseEvent(QMouseEvent* pEvent);
    void paintEvent(QPaintEvent* pEvent);

public:
    ChoseColorDialog(const QPixmap& pixmap, QWidget* pParent = nullptr);
    QColor getColor() const;

private:
    QColor m_ColorClicked;
    const QImage m_Image;
};