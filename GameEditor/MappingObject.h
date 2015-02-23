#ifndef MAPPING_OBJECT_H
#define MAPPING_OBJECT_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtGui/QKeyEvent>
#include <memory>
#include "MappingModeInterface.h"

class MapEditor;
namespace DATABASE
{
    class DatabaseMgr;
}

class MappingObject : public QObject
{
    Q_OBJECT
public:
    MappingObject(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent);

    void setMappingMode(MAPPING_MODE::Type mode);
    MAPPING_MODE::Type getMappingModeType() const;
    QStringList getMappingModeNames() const;

    const MAPPING_MODE::Interface* getMappingMode(MAPPING_MODE::Type mode) const;
    MAPPING_MODE::Interface* getMappingMode(MAPPING_MODE::Type mode);

public slots:
    void press(MapEditor& editor, QMouseEvent* pEvent);
    void release(MapEditor& editor, QMouseEvent* pEvent);
    void move(MapEditor& editor, QMouseEvent* pEvent);
    void onActionCopy(const MapEditor& editor);
    void onActionCut(const MapEditor& editor);
    void onActionPaste(MapEditor& editor, const QPoint& pos);
    void onActionDelete(MapEditor& editor);
    void onActionKeyPressed(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent);
    void onActionKeyReleased(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent);
    void onContextMenuRequest(MapEditor& editor, QContextMenuEvent* pEvent);

private:
    typedef std::unique_ptr<MAPPING_MODE::Interface> MappingModePtr;
    std::vector<MappingModePtr> m_MappingModes;
    MAPPING_MODE::Interface* m_pCurrentMappingMode = nullptr;
};
#endif
