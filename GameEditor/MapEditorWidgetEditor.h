#ifndef MAP_EDITOR_WIDGET_EDITOR_H
#define MAP_EDITOR_WIDGET_EDITOR_H

#include <Core/Cache/Manager.h>
#include "MapEditorModuleContent.h"
#include "MapEditorModuleMapTree.h"
#include "MappingObject.h"

class MapEditorModuleMappingMode;

namespace database
{
    class Manager;
}

class MapEditorWidgetEditor : public QWidget
{
    Q_OBJECT
public:
    MapEditorWidgetEditor(database::Manager& mgr, QWidget* pParent = nullptr);

    void setup();
    void projectOpened();

public slots:
    void onSaveChanges();

private:
    database::Manager& m_DBMgr;

    // cache
    CACHE::Manager m_CacheMgr;

    // modules
    MapEditorModuleMappingMode* m_pModuleMappingMode = nullptr;
    MapEditorModuleContent* m_pModuleContent = nullptr;
    MapEditorModuleMapTree* m_pModuleMapTree = nullptr;
};

#endif
