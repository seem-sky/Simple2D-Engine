#ifndef MAP_EDITOR_WIDGET_EDITOR_H
#define MAP_EDITOR_WIDGET_EDITOR_H

#include "MapEditorModuleTiles.h"
#include "MapEditorModuleContent.h"
#include "MapEditorModuleTileSelection.h"
#include "MapEditorModuleMapTree.h"

class MapEditorWidgetEditor : public QWidget
{
    Q_OBJECT
public:
    MapEditorWidgetEditor(DATABASE::DatabaseMgr& databaseMgr, QWidget* pParent = nullptr);

    void setup();
    void projectOpened();

public slots:
    void onRegisterTab(MapViewer* pTab);

private:
    DATABASE::DatabaseMgr& m_DatabaseMgr;

    // modules
    MapEditorModuleContent* m_pModuleContent;
    MapEditorModuleTileSelection* m_pModuleTileSelection;
    MapEditorModuleMapTree* m_pModuleMapTree;
};

#endif
