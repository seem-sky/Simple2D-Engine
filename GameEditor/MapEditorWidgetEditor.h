#ifndef MAP_EDITOR_WIDGET_EDITOR_H
#define MAP_EDITOR_WIDGET_EDITOR_H

#include "MapEditorModuleTiles.h"
#include "UI/UI_MapEditorWidget.h"

class MapEditorWidgetEditor : public QWidget, Ui_MapEditorWidget
{
    Q_OBJECT
private:
    void _setupTileModules();

public:
    MapEditorWidgetEditor(DATABASE::DatabaseMgr &databaseMgr, QWidget *pParent = nullptr);

    void setup();

private:
    DATABASE::DatabaseMgr &m_DatabaseMgr;
};

#endif
