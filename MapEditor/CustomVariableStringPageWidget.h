#ifndef CUSTOM_VARIABLE_STRING_PAGE_H
#define CUSTOM_VARIABLE_STRING_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableStringPage.h"

class CustomVariableStringPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableStringPage
{
    Q_OBJECT
public:
    CustomVariableStringPageWidget(QWidget *p_pParent = NULL);

    uint32 GetVariableCount();

protected:
    TVariable<std::string> GetVariableFromData();
    void ResizeVariableCount(uint32 p_uiCount);
    void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0);
    void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    virtual void LoadItems();
    void ClearWidgets();
    void ConnectWidgets();
    void DisconnectWidgets();

private slots:
    void DefaultValueChanged() { ChangeItem(GetCurrentItemID()); }
};
#endif