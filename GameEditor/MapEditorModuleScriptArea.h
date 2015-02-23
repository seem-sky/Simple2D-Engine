#ifndef MAP_EDITOR_MODULE_SCRIPT_AREA_H
#define MAP_EDITOR_MODULE_SCRIPT_AREA_H

#include <QtWidgets/QWidget>
#include "UI/UI_MapEditorModuleScriptAreaMapping.h"
#include <map/ScriptArea/ScriptArea.h>

class MapEditorModuleScriptArea : public QWidget, Ui_MapEditorModuleScriptAreaMapping
{
    Q_OBJECT
public:
    MapEditorModuleScriptArea(QWidget* pParent = nullptr);

    void setup();

    MAP::SCRIPT_AREA::TriggerType getTriggerType() const;
    MAP::SCRIPT_AREA::RepeatType getRepeatType() const;
    QString getScriptName() const;

    void setTriggerType(MAP::SCRIPT_AREA::TriggerType type);
    void setRepeatType(MAP::SCRIPT_AREA::RepeatType type);
    void setScriptName(const QString scriptName);

    private slots:
    void _onScriptTypeChanged(int index);
};
#endif
