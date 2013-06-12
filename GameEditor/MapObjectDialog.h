#ifndef MAP_OBJECT_DIALOG_H
#define MAP_OBJECT_DIALOG_H

#include "UI/ui_MapObjectDialog.h"
#include "MapDatabase.h"

class MapObjectDialog : public QDialog, Ui_MapObjectDialog
{
    Q_OBJECT
private:
    void _storeData();
    void _getDataFromObject();
    void _setAnimationForDirection();

protected:
    void accept();

public:
    MapObjectDialog(DATABASE::ConstWorldObjectDatabasePtr pWODB, DATABASE::ConstAnimationDatabasePtr pAnimationDB,
        DATABASE::ConstSpriteDatabasePtr pSpriteDB, MAP::MapObjectPtr pObject, QWidget *pParent = NULL);

private slots:
    void _directionButtonToggled(bool checked);

private:
    MAP::MapObjectPtr m_pMapObject;
    DATABASE::ConstWorldObjectDatabasePtr m_pWorldObjectDB;
};
#endif