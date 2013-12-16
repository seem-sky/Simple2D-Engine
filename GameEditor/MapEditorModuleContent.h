#ifndef MAP_EDITOR_MODULE_CONTENT_H
#define MAP_EDITOR_MODULE_CONTENT_H

#include <QtWidgets/QWidget>
#include "UI/UI_MapEditorModuleContent.h"

class MapEditorModuleContent : public QWidget, Ui_MapEditorModuleContent
{
    Q_OBJECT
public:
    MapEditorModuleContent(QWidget *pWidget = nullptr);
};

#endif
