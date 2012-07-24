#ifndef DATABSE_H
#define DATABSE_H

#include "UI/UI_Database.h"
#include <QtGui/QCloseEvent>

class DatabaseWindow : public QMainWindow, Ui_Database
{
    Q_OBJECT
public:
    DatabaseWindow(QMainWindow *parent);
    ~DatabaseWindow(void);

private slots:
    void ClickButtonOK() {};
    void ClickButtonApply() {};
    void TextureTabChangeCur(int p_Index);
    void TextureBoxChangeCur(int p_Index);
    void ClickButtonNewTexture();
    void ClickButtonDeleteTexture();

protected:
    virtual void closeEvent(QCloseEvent *p_pEvent);
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);

private:
    bool SelectTexture(std::string p_sType, unsigned int p_uiID);
};
#endif