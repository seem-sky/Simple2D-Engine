#ifndef CUSTOM_VARIABLE_STRING_PAGE_H
#define CUSTOM_VARIABLE_STRING_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableStringPage.h"
#include "GlobalVariableOutput.h"

class CustomVariableStringPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableStringPage
{
    Q_OBJECT
public:
    CustomVariableStringPageWidget(QWidget *p_pParent = NULL);
    ~CustomVariableStringPageWidget(void);

protected:
    TVariable<std::string> GetVariableFromData();
    void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0) = 0;
    void ChangeItem(uint32 p_uiID, bool p_bDelete = false) = 0;
    virtual void LoadItems() = 0;
    void ClearWidgets();
    void ConnectWidgets();
    void DisconnectWidgets();

protected slots:
    virtual void ClickNew() = 0;

private slots:
    void DefaultValueChanged(int) { ChangeItem(GetCurrentItemID()); }
};

class CustomObjectVariableStringPageWidget : public CustomVariableStringPageWidget
{
    Q_OBJECT
public:
    CustomObjectVariableStringPageWidget(QWidget *p_pParent = NULL) : CustomVariableStringPageWidget(p_pParent) {}

protected:
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0);
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    virtual void LoadItems();

protected slots:
    virtual void ClickNew();
};

class CustomGlobalVariableStringPageWidget : public CustomVariableStringPageWidget
{
    Q_OBJECT
public:
    CustomGlobalVariableStringPageWidget(QWidget *p_pParent = NULL) : CustomVariableStringPageWidget(p_pParent)
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