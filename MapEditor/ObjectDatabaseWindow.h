#ifndef OBJECT_DATABASE_WINDOW_H
#define OBJECT_DATABASE_WINDOW_H

#include "DatabasePageTemplate.h"
#include "UI/UI_ObjectDatabase.h"

class ObjectDatabaseWindow : public DatabasePageTemplate, public Ui_ObjectDatabase
{
    Q_OBJECT
public:
    ObjectDatabaseWindow(QWidget *p_pParent = NULL);
    ~ObjectDatabaseWindow(void);

    static bool IsParentOf(uint32 p_uiInheritorID, uint32 p_uiParentID);

protected:
    void ConnectWidgets();
    void DisconnectWidgets();
    bool SelectItem(uint32 p_uiID);
    void ChangeItem(uint32 p_uiID, bool p_Delete = false);

    void ClearWidgets();

private:
    void LoadObjects();
    uint32 GetCurrentTextureID();
    void ChangeTextureView(std::string p_sType);
    void SetSelectableTextures(std::string p_sType);

    void SetParentBoxIndexToID(uint32 p_uiID);
    void FillObjectAndParentBox();

protected slots:
    void ClickButtonNew();

private slots:
    void NPCAttributeChanged(int p_Index) { ChangeItem(GetCurrentItemID()); }
    void TypeChanged(int p_Index);
    void TextureIndexChanged(int p_Index);
    void ParentObjectAdded();
    void ParentObjectRemoved();

    void ClickOpenEventEditor();
};
#endif