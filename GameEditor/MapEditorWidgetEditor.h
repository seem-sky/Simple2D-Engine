#ifndef MAP_EDITOR_WIDGET_EDITOR_H
#define MAP_EDITOR_WIDGET_EDITOR_H

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
    void onRegisterTab(MapViewer* pTab);

private:
    DATABASE::DatabaseMgr& m_DatabaseMgr;

    // modules
    MapEditorModuleContent* m_pModuleContent = nullptr;
    MapEditorModuleTileSelection* m_pModuleTileSelection = nullptr;
    MapEditorModuleMapTree* m_pModuleMapTree = nullptr;
    MapEditorModuleWorldObjects* m_pModuleWorldObjects = nullptr;

    MappingObject m_MappingObject;

    // others
    QComboBox* m_pMappingMode = nullptr;
};

#endif
