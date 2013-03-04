#ifndef DATABASE_PAGE_TEMPLATE_H
#define DATABASE_PAGE_TEMPLATE_H

#include "UI/UI_DatabasePageTemplate.h"
#include "DatabaseOutput.h"
#include "ModifyObject.h"
#include <set>

class DatabasePageTemplate : public QWidget, public Ui_DatabasePageTemplate
{
    Q_OBJECT
public:
    DatabasePageTemplate(QWidget *p_pParent = NULL);
    ~DatabasePageTemplate(void);

    inline uint32 GetCurrentItemID() { return m_pID->value(); }
    uint32 GetNewID();

protected slots:
    virtual void ClickButtonNew() = 0;
    void ClickButtonDelete();
    void ItemBoxChangeCurrent(int p_Index);
    void NameChanged() { ChangeItem(GetCurrentItemID()); }

protected:
    virtual void ConnectWidgets();
    virtual void DisconnectWidgets();
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.resizeEvent(this); }
    int32 InsertItem(uint32 p_uiID, QString p_sData);
    virtual void ChangeItem(uint32 p_uiID, bool p_Delete = false) = 0;
    virtual bool SelectItem(uint32 p_uiID) = 0;

    virtual void ClearWidgets();

    std::set<uint32> m_uilIDCache;
    std::string m_sLogLocationName;
    ModifyObject m_ResizeObj;
};
#endif