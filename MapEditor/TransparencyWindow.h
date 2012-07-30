#ifndef TRANSPARENCY_WINDOW_H
#define TRANSPARENCY_WINDOW_H

#include "UI/UI_ChoseTransparency.h"
#include <QtGui/QCloseEvent>

class TransparencyWindow : public QMainWindow, Ui_TransparencyWindow
{
    Q_OBJECT
public:
    TransparencyWindow(QMainWindow *parent, QPixmap &p_Pixmap);
    ~TransparencyWindow(void);

    void ShowTexture(QPixmap &p_Pixmap);

protected:
    void resizeEvent(QResizeEvent *p_Event);
    void closeEvent(QCloseEvent *p_Event);
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);

signals:
    void ColorChosen(QString p_Result);

private:
    QPixmap m_Pixmap;
    QString m_RGBresult;
};
#endif