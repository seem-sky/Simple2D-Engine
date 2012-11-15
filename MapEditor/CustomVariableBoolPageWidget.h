#ifndef CUSTOM_VARIABLE_BOOL_PAGE_H
#define CUSTOM_VARIABLE_BOOL_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableBoolPage.h"

class CustomVariableBoolPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableBoolPage
{
    Q_OBJECT
public:
    CustomVariableBoolPageWidget(QWidget *p_pParent = NULL);

    uint32 GetVariableCount();

protected:
    TVariable<bool> GetVariableFromData();
    void ResizeVariableCount(uint32 p_uiCount);
    void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0);
    void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    void LoadItems();
    void ClearWidgets();
    void ConnectWidgets();
    void DisconnectWidgets();

private slots:
    void DefaultValueChanged(int) { ChangeItem(GetCurrentItemID()); }
};
#endif