#ifndef TRANSPARENCY_WINDOW_H
#define TRANSPARENCY_WINDOW_H

#include <QtGui/QCloseEvent>
#include <QtGui/QDialog>
#include <QtGui/QLabel>

class TransparencyWindow : public QDialog
{
    Q_OBJECT
public:
    TransparencyWindow(QWidget *parent, QPixmap &p_Pixmap);

    void ShowTexture(QPixmap &p_Pixmap);

protected:
    void resizeEvent(QResizeEvent *p_Event);
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);

signals:
    void ColorChosen(QString p_Result);

private:
    QLabel *m_pPic;
    QPixmap m_Pixmap;
    QString m_RGBresult;
};
#endif