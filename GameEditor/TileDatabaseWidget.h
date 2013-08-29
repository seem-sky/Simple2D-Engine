#ifndef TILE_DATABASE_WIDGET_H
#define TILE_DATABASE_WIDGET_H

#include "UI/UI_TileDatabaseWidget.h"
#include "TextureDatabaseWidget.h"

class TileDatabaseWidget : public TextureDatabaseWidget<DATABASE::TilePrototype>, Ui_TileDatabaseWidget
{
    Q_OBJECT
private:
    void _setPassabilityIcons();

protected:
    void clearWidgets();
    bool getItemFromWidgets(DATABASE::TilePrototypePtr &proto);
    bool setWidgetsFromPrototype(const DATABASE::TilePrototypePtr &proto);

public:
    TileDatabaseWidget(QWidget *pParent = NULL);
    QPixmap showPixmap(const DATABASE::TilePrototypePtr &proto);

private slots:
    void _passabilityButtonClicked(bool clicked);

    TEXTURE_DATABASE_WIDGET_SLOTS
};
#endif