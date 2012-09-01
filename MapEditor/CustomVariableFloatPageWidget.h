#ifndef CUSTOM_VARIABLE_FLOAT_PAGE_H
#define CUSTOM_VARIABLE_FLOAT_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableFloatPage.h"

class CustomVariableFloatPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableFloatPage
{
    Q_OBJECT
public:
    CustomVariableFloatPageWidget(QWidget *p_pParent = NULL);
    ~CustomVariableFloatPageWidget(void);

protected:
    void SelectItem(uint32 p_uiID);
    void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    void ClearWidgets();
    void ConnectWidgets();
    void DisconnectWidgets();
    void LoadItems();

protected slots:
    void ClickNew();

private slots:
    void DefaultValueChanged(int) { ChangeItem(GetCurrentItemID()); }
};
#endif