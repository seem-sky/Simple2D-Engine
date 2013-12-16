#ifndef MAP_EDITOR_DIALOG_MAP_SETTINGS_H
#define MAP_EDITOR_DIALOG_MAP_SETTINGS_H

#include <QtWidgets/QDialog>
#include "UI/UI_MapEditorDialogMapSettings.h"
#include "DatabasePrototypes.h"

class MapEditorDialogMapSettings : public QDialog, Ui_MapEditorDialogMapSettings
{
    Q_OBJECT
private:
    void _setup();
    void _save();

public:
    MapEditorDialogMapSettings(DATABASE::MAP_STRUCTURE::MapPrototype *pPrototype, QWidget *pParent = nullptr);

private slots:
    void _onAccepted();

private:
    DATABASE::MAP_STRUCTURE::MapPrototype *m_pPrototype;
};

#endif
