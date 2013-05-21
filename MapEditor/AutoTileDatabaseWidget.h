#ifndef AUTO_TILE_DATABASE_WIDGET_H
#define AUTO_TILE_DATABASE_WIDGET_H

#include "DatabaseWidget.h"
#include "UI/UI_AutoTileDatabaseWidget.h"

class AutoTileDatabaseWidget : public DatabaseWidget<DATABASE::AutoTilePrototype>, Ui_AutoTileDatabaseWidget
{
    Q_OBJECT
private:
    TileDropLabel* _getLabelForIndex(DATABASE::AutoTilePrototype::AUTO_TILE_INDEX index);
    DATABASE::AutoTilePrototype::AUTO_TILE_INDEX _getIndexForLabel(TileDropLabel *pLabel);

protected:
    bool getPrototypeFromWidgets(DATABASE::AutoTilePrototypePtr &proto);
    bool setWidgetsFromPrototype(const DATABASE::AutoTilePrototypePtr &proto);
    void clearWidgets();

public:
    AutoTileDatabaseWidget(QWidget *pParent = NULL);
    void setTileDB(const DATABASE::ConstTileDatabaseChangerPtr &pDB);
    void setFocus();

private slots:
    void _onTileDrop(uint32 uiID, const Point<int32> &pos);

    // parent slots
    void _resizeButtonClicked() { resizeDatabase(getListCountValue()); }
    void _currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) { currentItemChanged(current); }
    void _changeName() { changeName(); }

private:
    DATABASE::ConstTileDatabaseChangerPtr m_pTileDB;
    QLabel *m_pCurrentLabel;
    uint32 m_uiAutoTileSet[DATABASE::AUTO_TILE_SET_COUNT];
};
#endif