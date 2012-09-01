#ifndef TEXTURE_DATABASE_WINDOW_H
#define TEXTURE_DATABASE_WINDOW_H

#include "UI/UI_TextureDatabase.h"
#include "DatabasePageTemplate.h"

class TextureDatabaseWindow : public DatabasePageTemplate, Ui_TextureDatabase
{
    Q_OBJECT
public:
    TextureDatabaseWindow(QWidget *p_pParent = NULL);
    ~TextureDatabaseWindow(void);

    inline std::string GetCurrentType() const {return m_pTab->tabText(m_pTab->currentIndex()).toStdString(); }
    static const DATABASE::SpritePrototype* GetLatestPrototype(std::string p_sType, uint32 p_uiID);

private:
    void SetTexturePixmap(QPixmap &p_Pixmap);
    void ChangeBounding();

    PassabilityFlag m_DirPressed;

protected:
    void ConnectWidgets();
    void DisconnectWidgets();
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);
    void ChangeItem(uint32 p_uiID, bool p_Delete = false);
    bool SelectItem(uint32 p_uiID);
    void ClearWidgets();

protected slots:
    void ClickButtonNew();

private slots:
    void TransparencyColorChanged() { ChangeItem(GetCurrentItemID()); }
    void CurrentTabChanged(int p_Index);
    void TransparencyColorChanged(QString p_Color) { m_pTransparencyColor->setText(p_Color); }
    void TileAutotileStateChanged(int p_State) { ChangeItem(GetCurrentItemID()); }
    void BoundingChanged(int p_Value);
    void NPCSpriteCountChanged(int p_Value) { ChangeItem(GetCurrentItemID()); }
};
#endif