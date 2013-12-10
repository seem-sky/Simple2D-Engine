#include "MapEditorWidgetEditor.h"

MapEditorWidgetEditor::MapEditorWidgetEditor(DATABASE::DatabaseMgr &databaseMgr, QWidget *pParent) : QWidget(pParent),
    m_pModuleTiles(new MapEditorModuleTiles(this)), m_DatabaseMgr(databaseMgr)
{
}

void MapEditorWidgetEditor::setup()
{
    m_pModuleTiles->setup(m_DatabaseMgr.getTileDatabase());
}
