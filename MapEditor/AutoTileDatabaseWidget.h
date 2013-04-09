#ifndef AUTO_TILE_DATABASE_WIDGET_H
#define AUTO_TILE_DATABASE_WIDGET_H

#include "DatabaseWidget.h"
#include "UI/UI_AutoTileDatabaseWidget.h"

class AutoTileDatabaseWidget : public DatabaseWidget<DATABASE::AutoTilePrototype>, Ui_AutoTileDatabaseWidget
{
    Q_OBJECT
private:
    QLabel* _getLabelForIndex(DATABASE::AutoTilePrototype::AUTO_TILE_INDEX index);
    DATABASE::AutoTilePrototype::AUTO_TILE_INDEX _getIndexForLabel(QLabel *pLabel);
    void _fillTileList();
    void _showTilePixmap(const DATABASE::ConstTilePrototypePtr &proto);
    QPixmap _createTilePixmap(const DATABASE::ConstTilePrototypePtr &proto);
    void _setCurrentLabel(QLabel *pLabel);

protected:
    bool getPrototypeFromWidgets(DATABASE::AutoTilePrototypePtr &proto);
    bool setWidgetsFromPrototype(const DATABASE::AutoTilePrototypePtr &proto);
    void clearWidgets();
    bool eventFilter(QObject *pObj, QEvent *pEvent);

public:
    AutoTileDatabaseWidget(QWidget *pParent = NULL);
    inline void setTileDB(DATABASE::ConstTilePrototypDatabaseChangerPtr pDB) { m_pTileDB = pDB; _fillTileList(); }
    void setFocus();

private slots:
    void _tileDoubleClicked(QTreeWidgetItem *pItem, int column);
    void _changeSelectedTile(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious);

    // parent slots
    void _resizeButtonClicked() { _resizeDatabase(getListCountValue()); }
    void _currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) { currentItemChanged(current); }
    void _changeName() { changeName(); }

private:
    DATABASE::ConstTilePrototypDatabaseChangerPtr m_pTileDB;
    QLabel *m_pCurrentLabel;
    uint32 m_uiAutoTileSet[DATABASE::AUTO_TILE_SET_COUNT];
};
#endif