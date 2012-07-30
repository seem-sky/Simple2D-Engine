#ifndef DATABSE_H
#define DATABSE_H

#include "UI/UI_Database.h"
#include <QtGui/QCloseEvent>
#include "DatabaseOutput.h"
#include <set>

class DatabaseWindow : public QMainWindow, Ui_Database
{
    Q_OBJECT
public:
    DatabaseWindow(QMainWindow *parent);
    ~DatabaseWindow(void);

    std::string GetCurrentTextureType();

private slots:
    void ClickButtonOK();
    void ClickButtonApply();

    /*####
    # Textures
    ####*/
    void ClickButtonNewTexture();
    void ClickButtonDeleteTexture();
    void TextureTabChangeCur(int p_Index);
    void TextureBoxChangeCur(int p_Index);
    void TextureChangeTransparencyColor(QString p_Color);
    void TextureTransparencyColorEntered();

    // tiles
    void TileAutotileCheckBoxChanged(int p_State);

    // objects
    void TextureObjectsBoundingChanged(int p_Value);

protected:
    virtual void closeEvent(QCloseEvent *p_pEvent);
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);

private:
    bool SelectTexture(std::string p_sType, unsigned int p_uiID);
    void ChangeTexture(unsigned int p_uiID, bool p_bDel = false);
    void ClearWidgets();
    void InsertItemInTextureBox(unsigned int p_uiID, QString p_sData);
    void ShowTexturePixmap(QPixmap &p_Pixmap);

    const DATABASE::SpritePrototype* GetActualSpritePrototype(std::string p_sType, unsigned int p_uiID);

    PassabilityFlag m_DirPressed;

    std::string m_sLogLocationName;
    std::set<unsigned int> m_uilIDCache;
};
#endif