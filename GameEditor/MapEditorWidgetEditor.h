#ifndef MAP_EDITOR_WIDGET_EDITOR_H
#define MAP_EDITOR_WIDGET_EDITOR_H

#include <Core/Cache/Manager.h>
#include "MapEditorModuleContent.h"
#include "MapEditorModuleMapTree.h"
#include "MappingObject.h"

class MapEditorModuleMappingMode;

class MapEditorWidgetEditor : public QWidget
{
    Q_OBJECT
public:
    MapEditorWidgetEditor(DATABASE::DatabaseMgr& databaseMgr, QWidget* pParent = nullptr);

    void setup();
    void projectOpened();

public slots:
    void onSaveChanges();

private:
    DATABASE::DatabaseMgr& m_DatabaseMgr;

    // cache
    CACHE::Manager m_CacheMgr;

    // modules
    MapEditorModuleMappingMode* m_pModuleMappingMode = nullptr;
    MapEditorModuleContent* m_pModuleContent = nullptr;
    MapEditorModuleMapTree* m_pModuleMapTree = nullptr;
};

#endif
