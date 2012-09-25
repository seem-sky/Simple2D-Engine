#ifndef CUSTOM_VARIABLE_BOOL_PAGE_H
#define CUSTOM_VARIABLE_BOOL_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableBoolPage.h"
#include "GlobalVariableOutput.h"

class CustomVariableBoolPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableBoolPage
{
    Q_OBJECT
public:
    CustomVariableBoolPageWidget(QWidget *p_pParent = NULL);
    ~CustomVariableBoolPageWidget(void);

protected:
    TVariable<bool> GetVariableFromData();
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0) = 0;
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false) = 0;
    virtual void LoadItems() = 0;
    void ClearWidgets();
    void ConnectWidgets();
    void DisconnectWidgets();

protected slots:
    virtual void ClickNew() = 0;

private slots:
    void DefaultValueChanged(int) { ChangeItem(GetCurrentItemID()); }
};

class CustomObjectVariableBoolPageWidget : public CustomVariableBoolPageWidget
{
    Q_OBJECT
public:
    CustomObjectVariableBoolPageWidget(QWidget *p_pParent = NULL) : CustomVariableBoolPageWidget(p_pParent) {}

protected:
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0);
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    virtual void LoadItems();

protected slots:
    virtual void ClickNew();
};

class CustomGlobalVariableBoolPageWidget : public CustomVariableBoolPageWidget
{
    Q_OBJECT
public:
    CustomGlobalVariableBoolPageWidget(QWidget *p_pParent = NULL) : CustomVariableBoolPageWidget(p_pParent)
    {
        LoadItems();
    }

protected:
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0);
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    virtual void LoadItems();

protected slots:
    virtual void ClickNew();
};
#endif