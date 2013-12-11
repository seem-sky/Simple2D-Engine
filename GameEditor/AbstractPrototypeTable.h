#ifndef ABSTRACT_PROTOTYPE_TABLE_H
#define ABSTRACT_PROTOTYPE_TABLE_H

#include <QtWidgets/QTableWidget>
#include "TexturePrototypeFrame.h"
#include "DatabaseMgr.h"

class TexturePrototypeFrameEx : public TexturePrototypeFrame
{
protected:
    virtual void paintEvent(QPaintEvent *pEvent);

public:
    TexturePrototypeFrameEx(QWidget *pParent = nullptr);

    enum Selection
    {
        SELECTION_RIGHT = 0x1,
        SELECTION_LEFT  = 0x2
    };
    void addSelection(Selection selection);
    bool hasSelection(Selection selection);
    void removeSelection(Selection selection);

private:
    uint8 m_Selection;
};

class AbstractPrototypeTable : public QTableWidget
{
    Q_OBJECT
private:
    void _itemRightClicked(TexturePrototypeFrameEx *pItem);
    void _itemLeftClicked(TexturePrototypeFrameEx *pItem);
    virtual void _setup() = NULL;

protected:
    void showEvent(QShowEvent *pEvent);
    void mousePressEvent(QMouseEvent *pEvent);

public:
    AbstractPrototypeTable(QWidget *pParent = nullptr);

    void setup();
    void clear();
    inline void setDBMgr(const DATABASE::DatabaseMgr *pDBMgr) { m_pDBMgr = pDBMgr; }

    enum SelectionIndex
    {
        RIGHT,
        LEFT
    };

signals:
    void itemRightClicked(TexturePrototypeFrameEx *pItem);
    void itemLeftClicked(TexturePrototypeFrameEx *pItem);

private:
    std::array<TexturePrototypeFrameEx*, 2> m_SelectedItems;

protected:
    const DATABASE::DatabaseMgr *m_pDBMgr;
};

#endif
