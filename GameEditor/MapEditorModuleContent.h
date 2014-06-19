#ifndef MAP_EDITOR_MODULE_CONTENT_H
#define MAP_EDITOR_MODULE_CONTENT_H

#include <QtWidgets/QWidget>
#include "UI/UI_MapEditorModuleContent.h"
#include <unordered_map>
#include <DatabaseMgr.h>

class MapViewer;

class MapEditorModuleContent : public QWidget, Ui_MapEditorModuleContent
{
    Q_OBJECT
public:
    MapEditorModuleContent(DATABASE::DatabaseMgr& databaseMgr, QWidget* pWidget = nullptr);

    MapViewer* getTab(uint32 mapID);

private slots:
    void _onCurrentChanged(int index);
    void _onLayerChanged(int value);
    void _onLayerTypeChanged();
    void _onZoomChanged(int value);
    void _onGridShowChanged(int state);
    void _onRevertPressed();

    void _onMapChanged(MapViewer* pMapViewer);
    void _onTabCloseRequested(int index);

public slots:
    void onMapOpened(uint32 mapID);
    void onMapClosed(uint32 mapID);
    void onMapEdited(uint32 mapID);

signals:
    void registerTab(MapViewer* pTab);

private:
    DATABASE::DatabaseMgr& m_DBMgr;
};

#endif
