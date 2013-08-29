#ifndef AUTO_TILE_DATABASE_WIDGET_H
#define AUTO_TILE_DATABASE_WIDGET_H

#include "DatabaseWidget.h"
#include "UI/UI_AutoTileDatabaseWidget.h"
#include <array>

class AutoTileDatabaseWidget : public DatabaseWidget<DATABASE::AUTO_TILE::AutoTilePrototype>, Ui_AutoTileDatabaseWidget
{
    Q_OBJECT
private:
    TileDropLabel* _getLabelForIndex(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index);
    DATABASE::AUTO_TILE::AUTO_TILE_INDEX _getIndexForLabel(TileDropLabel *pLabel);

protected:
    bool getItemFromWidgets(DATABASE::AutoTilePrototypePtr &proto);
    bool setWidgetsFromPrototype(const DATABASE::AutoTilePrototypePtr &proto);
    void clearWidgets();

public:
    AutoTileDatabaseWidget(QWidget *pParent = NULL);
    void setTileDB(const DATABASE::ConstTileDatabaseChangerPtr &pDB);
    void setFocus();

private slots:
    void _onTileDrop(uint32 uiID, const Int32Point &pos);

    DATABASE_WIDGET_SLOTS

private:
    DATABASE::ConstTileDatabaseChangerPtr m_pTileDB;
    QLabel *m_pCurrentLabel;
    std::array<uint32, DATABASE::AUTO_TILE::AUTO_TILE_SET_COUNT> m_uiAutoTileSet;
};
#endif