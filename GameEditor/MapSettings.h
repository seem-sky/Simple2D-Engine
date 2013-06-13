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
    MapSettings(const MAP::MapPrototypePtr &map, QWidget *pParent = NULL);

private slots:
    void _accepted();

private:
    MAP::MapPrototypePtr m_pMap;
};

#endif