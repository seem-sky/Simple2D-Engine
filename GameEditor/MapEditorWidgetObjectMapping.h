#ifndef MAP_EDITOR_WIDGET_OBJECT_MAPPING_H
#define MAP_EDITOR_WIDGET_OBJECT_MAPPING_H

#include "UI/UI_MapEditorWidgetObjectMapping.h"
#include "ToolWidget.h"
#include <WorldObjectInfo.h>

namespace DATABASE
{
    class DatabaseMgr;
}

class MapEditorWidgetObjectMapping : public ToolWidget, Ui_MapEditorWidgetObjectMapping
{
    Q_OBJECT
private:
    void _setupViewer();
    MAP::MAP_DATA::MapDirection _getDirection() const;

public:
    MapEditorWidgetObjectMapping(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

private slots:
    void _onDirectionChanged();

public slots:
    void onIndexChanged(int32 index);

signals:
    void changeDirection(MAP::MAP_DATA::MapDirection dir);

private:
    const DATABASE::DatabaseMgr& m_DBMgr;
    uint32 m_WorldObjectID = 0;
};
#endif
