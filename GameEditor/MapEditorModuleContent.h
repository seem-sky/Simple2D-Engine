#ifndef MAP_EDITOR_MODULE_CONTENT_H
#define MAP_EDITOR_MODULE_CONTENT_H

#include <QtWidgets/QWidget>
#include "UI/UI_MapEditorModuleContent.h"
#include <Global.h>
#include "MappingObject.h"

namespace CACHE
{
    class Manager;
}

class MapEditor;

class MapEditorModuleContent : public QWidget, Ui_MapEditorModuleContent
{
    Q_OBJECT
public:
    MapEditorModuleContent(CACHE::Manager& cacheMgr, const MappingObject& mappingObject, database::Manager& databaseMgr, QWidget* pWidget = nullptr);

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
    database::Manager& m_DBMgr;
    CACHE::Manager& m_CacheMgr;
    const MappingObject& m_MappingObject;
};

#endif
