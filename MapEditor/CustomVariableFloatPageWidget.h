#ifndef CUSTOM_VARIABLE_FLOAT_PAGE_H
#define CUSTOM_VARIABLE_FLOAT_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableFloatPage.h"

class CustomVariableFloatPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableFloatPage
{
    Q_OBJECT
public:
    CustomVariableFloatPageWidget(QWidget *p_pParent = NULL);

    uint32 GetVariableCount();

protected:
    TVariable<float> GetVariableFromData();
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