#ifndef CUSTOM_VARIABLE_PAGE_TEMPLATE_WIDGET_H
#define CUSTOM_VARIABLE_PAGE_TEMPLATE_WIDGET_H

#include "UI/Ui_CustomVariablePageTemplate.h"
#include <Global.h>
#include <VariableHolder.h>
#include "DatabaseOutput.h"

class CustomVariablePageTemplateWidget : public QWidget, public Ui_CustomVariablePageTemplate
{
    Q_OBJECT
public:
    CustomVariablePageTemplateWidget(QWidget *p_pParent = NULL);

    inline uint32 GetCurrentItemID() const { return m_pID->value(); }
    void SetNewData(uint32 p_uiOwnerID) { m_uiOwnerID = p_uiOwnerID; LoadItems(); }
    virtual uint32 GetVariableCount() = 0;
    inline void SetVariableHolder(VariableHolder *p_pHolder, DATABASE::Prototype* p_pProto = NULL)
    {
        m_pVariableHolder = p_pHolder;
        m_pPrototype = p_pProto;
        if (p_pHolder)
            LoadItems();
    }

protected:
    virtual void ResizeVariableCount(uint32 p_uiCount) = 0;
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0) = 0;
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false) = 0;
    virtual void ConnectWidgets();
    virtual void DisconnectWidgets();
    virtual void ClearWidgets();
    virtual void LoadItems() = 0;
    int32 InsertItem(uint32 p_uiID, QString p_sData);
    void SetWidgets(uint32 p_uiID, QString p_sName, bool p_bEnabled = true);

    uint32 m_uiOwnerID;
    VariableHolder *m_pVariableHolder;
    DATABASE::Prototype *m_pPrototype;

private slots:
    void NameChanged() { ChangeItem(GetCurrentItemID()); }
    void IndexChanged(int p_Index);
    void ResizeVariablesClicked();
};
#endif