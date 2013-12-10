#ifndef MAP_EDITOR_MODULE_TILES_H
#define MAP_EDITOR_MODULE_TILES_H

#include <QtWidgets/QTableWidget>
#include "DatabaseMgr.h"
#include "TexturePrototypeFrame.h"

class TexturePrototypeFrameEx : public TexturePrototypeFrame
{
protected:
    virtual void paintEvent(QPaintEvent *pEvent);

public:
    TexturePrototypeFrameEx(QWidget *pParent = nullptr);

    void setTexturePrototype(const DATABASE::TexturePrototype *pPrototype);
    const DATABASE::TexturePrototype* getTexturePrototype() const;

    enum Selection
    {
        RIGHT = 0x1,
        LEFT = 0x2
    };
    void addSelection(Selection selection);
    bool hasSelection(Selection selection);
    void removeSelection(Selection selection);

private:
    const DATABASE::TexturePrototype *m_pTexturePrototype;
    uint8 m_Selection;
};

class MapEditorModuleTiles : public QTableWidget
{
    Q_OBJECT
private:
    void _itemRightClicked(TexturePrototypeFrameEx *pItem);
    void _itemLeftClicked(TexturePrototypeFrameEx *pItem);

protected:
    void mousePressEvent(QMouseEvent *pEvent);

public:
    MapEditorModuleTiles(QWidget *pParent = nullptr);

    void setup(const DATABASE::TileDatabase *pTileDB);
    void clear();

signals:
    void itemRightClicked(TexturePrototypeFrameEx *pItem);
    void itemLeftClicked(TexturePrototypeFrameEx *pItem);

private:
    std::array<TexturePrototypeFrameEx*, 2> m_SelectedItems;
};

#endif
