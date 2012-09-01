#ifndef CUSTOM_VARIABLE_BOOL_PAGE_H
#define CUSTOM_VARIABLE_BOOL_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableBoolPage.h"

class CustomVariableBoolPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableBoolPage
{
    Q_OBJECT
public:
    CustomVariableBoolPageWidget(QWidget *p_pParent = NULL);
    ~CustomVariableBoolPageWidget(void);

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