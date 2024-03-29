#ifndef ABSTRACT_PROTOTYPE_TABLE_H
#define ABSTRACT_PROTOTYPE_TABLE_H

#include <QtWidgets/QTableWidget>
#include "TexturePrototypeFrame.h"
#include "MapEditorWidgetBrush.h"

class AbstractPixmapWidget : public QWidget
{
protected:
    virtual void paintEvent(QPaintEvent* pEvent);
    virtual void drawPixmap() = 0;

    void drawSelection();

public:
    AbstractPixmapWidget(uint32 ID, QWidget* pParent = nullptr);

    enum Selection
    {
        SELECTION_RIGHT = 0x1,
        SELECTION_LEFT  = 0x2
    };
    void addSelection(Selection selection);
    bool hasSelection(Selection selection);
    void removeSelection(Selection selection);

    inline uint32 getID() const { return m_ID; }

private:
    uint8 m_Selection;
    uint32 m_ID;
};

namespace database
{
    class Manager;
}

class AbstractPrototypeTable : public QTableWidget
{
    Q_OBJECT
private:
    virtual void _setup() = 0;

protected:
    void showEvent(QShowEvent* pEvent);
    void mousePressEvent(QMouseEvent* pEvent);
    void clickItem(BRUSH::BrushIndex brush, AbstractPixmapWidget* pItem);

public:
    AbstractPrototypeTable(const database::Manager& DBMgr, QWidget* pParent = nullptr);

    void setup();
    void clear();

    virtual MAP::BRUSH::BrushInfo::Type getType() const = 0;

signals:
    void selectionChanged(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo::Type selectioType, uint32 ID);
    void itemClicked(BRUSH::BrushIndex brush, AbstractPixmapWidget* pWidget);

protected:
    const database::Manager& m_DBMgr;
};
#endif
