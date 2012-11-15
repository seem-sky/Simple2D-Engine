#ifndef CUSTOM_VARIABLE_INT_PAGE_H
#define CUSTOM_VARIABLE_INT_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableIntPage.h"

class CustomVariableIntPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableIntPage
{
    Q_OBJECT
public:
    CustomVariableIntPageWidget(QWidget *p_pParent = NULL);

    uint32 GetVariableCount();

protected:
    TVariable<int> GetVariableFromData();
    void ResizeVariableCount(uint32 p_uiCount);
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0);
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    virtual void LoadItems();
    void ClearWidgets();
    void ConnectWidgets();
    void DisconnectWidgets();

private slots:
    void DefaultValueChanged() { ChangeItem(GetCurrentItemID()); }
};
#endif