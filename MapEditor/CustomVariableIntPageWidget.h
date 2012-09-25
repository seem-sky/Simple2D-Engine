#ifndef CUSTOM_VARIABLE_INT_PAGE_H
#define CUSTOM_VARIABLE_INT_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableIntPage.h"
#include "GlobalVariableOutput.h"

class CustomVariableIntPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableIntPage
{
    Q_OBJECT
public:
    CustomVariableIntPageWidget(QWidget *p_pParent = NULL);
    ~CustomVariableIntPageWidget(void);

protected:
    TVariable<int> GetVariableFromData();
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

class CustomObjectVariableIntPageWidget : public CustomVariableIntPageWidget
{
    Q_OBJECT
public:
    CustomObjectVariableIntPageWidget(QWidget *p_pParent = NULL) : CustomVariableIntPageWidget(p_pParent) {}

protected:
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0);
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    virtual void LoadItems();

protected slots:
    virtual void ClickNew();
};

class CustomGlobalVariableIntPageWidget : public CustomVariableIntPageWidget
{
    Q_OBJECT
public:
    CustomGlobalVariableIntPageWidget(QWidget *p_pParent = NULL) : CustomVariableIntPageWidget(p_pParent)
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