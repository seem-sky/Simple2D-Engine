#ifndef CUSTOM_VARIABLE_PAGE_TEMPLATE_WIDGET_H
#define CUSTOM_VARIABLE_PAGE_TEMPLATE_WIDGET_H

#include "UI/Ui_CustomVariablePageTemplate.h"
#include <Global.h>

class CustomVariablePageTemplateWidget : public QWidget, public Ui_CustomVariablePageTemplate
{
    Q_OBJECT
public:
    CustomVariablePageTemplateWidget(QWidget *p_pParent = NULL);
    ~CustomVariablePageTemplateWidget(void);

    inline uint32 GetCurrentItemID() const { return m_pID->value(); }
    void SetNewData(uint32 p_uiOwnerID) { m_uiOwnerID = p_uiOwnerID; LoadItems(); }

protected:
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0) = 0;
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false) = 0;
    virtual void ConnectWidgets();
    virtual void DisconnectWidgets();
    virtual void ClearWidgets();
    virtual void LoadItems() = 0;
    int InsertItem(uint32 p_uiID, QString p_sData);
    void SetWidgets(uint32 p_uiID, QString p_sName, bool p_bEnabled = true);

    uint32 m_uiOwnerID;

protected slots:
    virtual void ClickNew() = 0;

private slots:
    void NameChanged() { ChangeItem(GetCurrentItemID()); }
    void IndexChanged(int p_Index);
    void ClickDelete();
};
#endif