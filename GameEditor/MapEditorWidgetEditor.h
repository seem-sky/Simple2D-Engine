#ifndef MAP_EDITOR_WIDGET_EDITOR_H
#define MAP_EDITOR_WIDGET_EDITOR_H

#include "MapEditorModuleTiles.h"

class MapEditorWidgetEditor : public QWidget
{
public:
    MapEditorWidgetEditor(DATABASE::DatabaseMgr &databaseMgr, QWidget *pParent = nullptr);

    void setup();

private:
    MapEditorModuleTiles *m_pModuleTiles;
    DATABASE::DatabaseMgr &m_DatabaseMgr;
};

#endif
