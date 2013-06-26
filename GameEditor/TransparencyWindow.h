#ifndef TRANSPARENCY_WINDOW_H
#define TRANSPARENCY_WINDOW_H

#include <QtGui/QResizeEvent>
#include <QtWidgets/QDialog>
#include "Ui/UI_TransparentColorDialog.h"
#include <Color.h>

class TransparencyWindow : public QDialog, Ui_TransparentColorDialog
{
public:
    TransparencyWindow(QWidget *parent, QPixmap &pixmap);

    void showTexture(QPixmap &p_Pixmap);
    Color getColor() const { return m_RGBresult; }

protected:
    void resizeEvent(QResizeEvent *pEvent);
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private:
    QPixmap m_Pixmap;
    Color m_RGBresult;
};
#endif