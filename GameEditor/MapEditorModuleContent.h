#ifndef MAP_EDITOR_MODULE_CONTENT_H
#define MAP_EDITOR_MODULE_CONTENT_H

#include <QtWidgets/QWidget>
#include "UI/UI_MapEditorModuleContent.h"

class MapEditorModuleContent : public QWidget, Ui_MapEditorModuleContent
{
    Q_OBJECT
public:
    MapEditorModuleContent(QWidget *pWidget = nullptr);

private slots:
    void _onCurrentChanged(int index);
    void _onLayerChanged(int value);
    void _onLayerTypeChanged();
    void _onZoomChanged(int value);
};

#endif
