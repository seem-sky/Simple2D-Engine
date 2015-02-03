#ifndef MAP_EDITOR_WIDGET_SCRIPT_AREA_MAPPING_H
#define MAP_EDITOR_WIDGET_SCRIPT_AREA_MAPPING_H

#include "ToolWidget.h"
#include "UI/UI_MapEditorWidgetScriptAreaMapping.h"
#include <ScriptArea.h>

class MapEditorWidgetScriptAreaMapping : public ToolWidget, Ui_MapEditorWidgetScriptAreaMapping
{
public:
    MapEditorWidgetScriptAreaMapping(QWidget* pParent = nullptr);

    MAP::SCRIPT_AREA::TriggerType getTriggerType() const;
    MAP::SCRIPT_AREA::RepeatType getRepeatType() const;
    QString getScriptName() const;

    void setTriggerType(MAP::SCRIPT_AREA::TriggerType type);
    void setRepeatType(MAP::SCRIPT_AREA::RepeatType type);
    void setScriptName(const QString scriptName);
};
#endif
