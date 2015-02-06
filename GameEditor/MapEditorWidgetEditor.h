#ifndef MAP_EDITOR_WIDGET_EDITOR_H
#define MAP_EDITOR_WIDGET_EDITOR_H

#include <Core/Cache/Manager.h>
#include "MapEditorModuleTiles.h"
#include "MapEditorModuleContent.h"
#include "MapEditorModuleTileSelection.h"
#include "MapEditorModuleMapTree.h"
#include "MapEditorModuleWorldObjects.h"
#include "MappingObject.h"

class MapEditorWidgetEditor : public QWidget
{
    Q_OBJECT
public:
    MapEditorWidgetEditor(DATABASE::DatabaseMgr& databaseMgr, QWidget* pParent = nullptr);

    void setup();
    void projectOpened();

private slots:
    void _onMappingModeChanged(int index);

public slots:
    void onWorldObjectIndexChanged(int32 index);
    void onSaveChanges();

signals:
    void changeMappingMode(MAPPING_MODE::Type mode);
    void changeWorldObjectID(uint32 ID);

private:
    DATABASE::DatabaseMgr& m_DatabaseMgr;

    // cache
    CACHE::Manager m_CacheMgr;

    // modules
    MapEditorModuleContent* m_pModuleContent = nullptr;
    MapEditorModuleTileSelection* m_pModuleTileSelection = nullptr;
    MapEditorModuleMapTree* m_pModuleMapTree = nullptr;
    MapEditorModuleWorldObjects* m_pModuleWorldObjects = nullptr;

    // others
    QComboBox* m_pMappingMode = nullptr;

    MappingObject m_MappingObject;
};

#endif
