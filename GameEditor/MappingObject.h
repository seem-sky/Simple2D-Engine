#ifndef MAPPING_OBJECT_H
#define MAPPING_OBJECT_H

#include <QtCore/QObject>
#include <memory>
#include "MappingModeInterface.h"
#include <QtCore/QStringList>

class MapViewerScene;
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
    void press(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button);
    void release(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button);
    void move(MapViewerScene* pScene, QPoint pos);
    void copy(MapViewerScene* pScene, QPoint pos);
    void insert(MapViewerScene* pScene, QPoint pos);
    void cutOut(MapViewerScene* pScene, QPoint pos);
    void keyPress(MapViewerScene* pScene, int32 key);

private:
    typedef std::unique_ptr<MAPPING_MODE::Interface> MappingModePtr;
    std::vector<MappingModePtr> m_MappingModes;
    MAPPING_MODE::Interface* m_pCurrentMappingMode = nullptr;
};
#endif
