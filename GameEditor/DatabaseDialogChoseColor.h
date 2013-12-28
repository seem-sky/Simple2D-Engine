#ifndef DATABASE_DIALOG_CHOSE_COLOR_H
#define DATABASE_DIALOG_CHOSE_COLOR_H

#include <QtWidgets/QDialog>

class ChoseColorDialog : public QDialog
{
protected:
    void mouseReleaseEvent(QMouseEvent* pEvent);
    void paintEvent(QPaintEvent* pEvent);

public:
    ChoseColorDialog(const QPixmap& pixmap);

    QColor getColor() const { return m_ColorClicked; }

private:
    QColor m_ColorClicked;
    const QImage m_Image;
};

#endif
