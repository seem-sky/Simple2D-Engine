#ifndef MAP_EDITOR_MODULE_CONTENT_H
#define MAP_EDITOR_MODULE_CONTENT_H

#include <QtWidgets/QWidget>
#include "UI/UI_MapEditorModuleContent.h"
#include <Global.h>
#include "MappingObject.h"

namespace DATABASE
{
    class DatabaseMgr;
}

class MapEditor;

class MapEditorModuleContent : public QWidget, Ui_MapEditorModuleContent
{
    Q_OBJECT
public:
    MapEditorModuleContent(const MappingObject& mappingObject, DATABASE::DatabaseMgr& databaseMgr, QWidget* pWidget = nullptr);

    MapEditor* getTab(uint32 mapID);

    void saveMaps();

private slots:
    void _onCurrentChanged(int index);
    void _onLayerChanged(int value);
    void _onLayerTypeChanged();
    void _onZoomChanged(int value);
    void _onGridShowChanged(int state);
    void _onRevertPressed();

    void _onMapChanged(uint32 mapID);
    void _onTabCloseRequested(int index);

public slots:
    void onMapOpened(uint32 mapID);
    void onMapClosed(uint32 mapID);
    void onMapEdited(uint32 mapID);

    void onMappingModeChanged(MAPPING_MODE::Type mode);

signals:
    void save();
    void changeMappingMode(MAPPING_MODE::Type mode);

private:
    DATABASE::DatabaseMgr& m_DBMgr;
    const MappingObject& m_MappingObject;
};

#endif
