#ifndef MAP_SETTINGS_H
#define MAP_SETTINGS_H

#include "ui/UI_MapSettings.h"
#include "MapDatabase.h"

class MapSettings : public QDialog, Ui_MapSettings
{
    Q_OBJECT
private:
    void updateWidgets();

public:
    MapSettings(const DATABASE::MAP_STRUCTURE::MapPrototypePtr &map, QWidget *pParent = NULL);

private slots:
    void _accepted();

private:
    DATABASE::MAP_STRUCTURE::MapPrototypePtr m_pMap;
};

#endif