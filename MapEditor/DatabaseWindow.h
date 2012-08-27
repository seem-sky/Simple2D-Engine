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
    std::string GetCurrentObjectType();
    std::string GetCurrentDatabaseSection();

private slots:
    void ClickButtonOK();
    void ClickButtonApply();
    void ChangeDatabaseSection(int p_Index);

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
    void TextureBoundingChanged(int p_Value);

    // NPCs
    void TextureNPCSpriteCountChanged(int p_Value);

    /*####
    # Objects
    ####*/
    void ClickButtonNewObject();
    void ClickButtonDeleteObject();
    void ObjectBoxChangeCur(int p_Index);
    void ChangeObjectName();
    void ChangeObjectType(int p_Index);
    void ChangeObjectTextureID(int p_Index, bool p_bChangeData = true);
    void ChangeObjectNPCAttribute(int p_Index);
    void ButtonObjectOpenEventEditor();

    // Custom Variables
    void ClickButtonNewObjectVariable();
    void ClickButtonDeleteObjectVariable();
    void ObjectCustomVariableChanged(int p_Unused = 0);
    void ObjectVariableTypeIndexChanged(int p_Index);
    void ObjectVariableIndexChanged(int p_Index);

protected:
    virtual void closeEvent(QCloseEvent *p_pEvent);
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);

private:
    /*####
    # Textures
    ####*/
    bool SelectTexture(std::string p_sType, unsigned int p_uiID);
    void ChangeTexture(unsigned int p_uiID, bool p_bDel = false);
    void InsertItemInTextureBox(unsigned int p_uiID, QString p_sData);
    void ShowTexturePixmap(QPixmap &p_Pixmap);

    void ChangeTextureBounding();

    const DATABASE::SpritePrototype* GetActualSpritePrototype(std::string p_sType, unsigned int p_uiID);

    PassabilityFlag m_DirPressed;

    /*####
    # Objects
    ####*/
    bool SelectObject(unsigned int p_uiID);
    void ChangeObject(unsigned int p_uiID, bool p_bDel = false);
    void InsertItemInObjectBox(unsigned int p_uiID, QString p_sData);
    const DATABASE::ObjectPrototype* GetActualObjectPrototype(unsigned int p_uiID);
    void ChangeObjectTexturesInBox(std::string p_sType);
    unsigned int GetCurrentObjectTextureID();
    std::set<unsigned int> m_uilObjectVariableIDCache;

    // custom variables
    void NewObjectCustomVariable(DATABASE::CUSTOM_VARIABLE_TYPE p_Type);
    void DeleteObjectCustomVariable(DATABASE::CUSTOM_VARIABLE_TYPE p_Type, UINT p_uiID);
    QComboBox* GetObjectVariableBox(DATABASE::CUSTOM_VARIABLE_TYPE p_Type);
    DATABASE::CUSTOM_VARIABLE_TYPE GetCurrentObjectCustomVariablesType();
    void SelectObjectCustomVariable(DATABASE::CUSTOM_VARIABLE_TYPE p_Type, UINT p_uiID);
    int InsertItemInObjectCustomVariablesBox(DATABASE::CUSTOM_VARIABLE_TYPE p_Type, UINT p_uiID, QString p_sData);
    void ChangeObjectCustomVariable(UINT p_uiObjectID, DATABASE::CUSTOM_VARIABLE_TYPE p_Type, UINT p_uiVariableID);

    /*####
    # window
    ####*/
    void ClearWidgets();
    std::string m_sLogLocationName;
    std::set<unsigned int> m_uilIDCache;
};
#endif