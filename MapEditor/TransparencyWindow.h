#ifndef TRANSPARENCY_WINDOW_H
#define TRANSPARENCY_WINDOW_H

#include <QtGui/QResizeEvent>
#include <QtGui/QDialog>
#include "Ui/UI_TransparentColorDialog.h"
#include <Color.h>

class TransparencyWindow : public QDialog, Ui_TransparentColorDialog
{
    Q_OBJECT
public:
    TransparencyWindow(QWidget *parent, QPixmap &p_Pixmap);

    void ShowTexture(QPixmap &p_Pixmap);

protected:
    void resizeEvent(QResizeEvent *p_Event);
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);

signals:
    void ColorChosen(const Color &p_Result);

private:
    QPixmap m_Pixmap;
    QString m_RGBresult;
};
#endif