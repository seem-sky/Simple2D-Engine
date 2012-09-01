#ifndef CUSTOM_VARIABLE_INT_PAGE_H
#define CUSTOM_VARIABLE_INT_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableIntPage.h"

class CustomVariableIntPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableIntPage
{
    Q_OBJECT
public:
    CustomVariableIntPageWidget(QWidget *p_pParent = NULL);
    ~CustomVariableIntPageWidget(void);

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