#ifndef CUSTOM_VARIABLE_STRING_PAGE_H
#define CUSTOM_VARIABLE_STRING_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableStringPage.h"

class CustomVariableStringPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableStringPage
{
    Q_OBJECT
public:
    CustomVariableStringPageWidget(QWidget *p_pParent = NULL);
    ~CustomVariableStringPageWidget(void);

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