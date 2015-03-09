#ifndef MAP_EDITOR_DIALOG_MAP_SETTINGS_H
#define MAP_EDITOR_DIALOG_MAP_SETTINGS_H

#include <QtWidgets/QDialog>
#include "UI/UI_MapEditorDialogMapSettings.h"

namespace database
{
    namespace prototype
    {
        class Map;
    }
}

class MapEditorDialogMapSettings : public QDialog, Ui_MapEditorDialogMapSettings
{
    Q_OBJECT
private:
    void _setup();
    void _save();

public:
    MapEditorDialogMapSettings(database::prototype::Map* pPrototype, QWidget* pParent = nullptr);

private slots:
    void _onAccepted();

private:
    database::prototype::Map* m_pPrototype;
};

#endif
