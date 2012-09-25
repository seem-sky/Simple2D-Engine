#ifndef CUSTOM_VARIABLE_FLOAT_PAGE_H
#define CUSTOM_VARIABLE_FLOAT_PAGE_H

#include "CustomVariablePageTemplateWidget.h"
#include "UI/UI_CustomVariableFloatPage.h"
#include "GlobalVariableOutput.h"

class CustomVariableFloatPageWidget : public CustomVariablePageTemplateWidget, public Ui_CustomVariableFloatPage
{
    Q_OBJECT
public:
    CustomVariableFloatPageWidget(QWidget *p_pParent = NULL);
    ~CustomVariableFloatPageWidget(void);

protected:
    TVariable<float> GetVariableFromData();
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

class CustomObjectVariableFloatPageWidget : public CustomVariableFloatPageWidget
{
    Q_OBJECT
public:
    CustomObjectVariableFloatPageWidget(QWidget *p_pParent = NULL) : CustomVariableFloatPageWidget(p_pParent) {}

protected:
    virtual void SelectItem(uint32 p_uiID, uint32 p_uiParentID = 0);
    virtual void ChangeItem(uint32 p_uiID, bool p_bDelete = false);
    virtual void LoadItems();

protected slots:
    virtual void ClickNew();
};

class CustomGlobalVariableFloatPageWidget : public CustomVariableFloatPageWidget
{
    Q_OBJECT
public:
    CustomGlobalVariableFloatPageWidget(QWidget *p_pParent = NULL) : CustomVariableFloatPageWidget(p_pParent)
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