#ifndef CONDITION_WIDGET_H
#define CONDITION_WIDGET_H

#include <QtGui/QWidget>
#include "ui/UI_ConditionWidget.h"
#include <EventScriptCommand.h>

class ConditionWidget : public QWidget, Ui_ConditionWidget
{
    Q_OBJECT
public:
    ConditionWidget(QWidget *p_pParent = NULL);
    void LoadValuesFromEventScriptCommand(const EVENT_SCRIPT::ConditionSettingsPtr &p_pCondition);
    bool GetCondition(EVENT_SCRIPT::ConditionSettingsPtr &p_pResult) const;
    void ReInitialize();

private slots:
    void ConditionTypeChanged(int p_Index);
    void VariableTypeChanged(bool p_bChecked);
};
#endif