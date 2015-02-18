#ifndef MAP_EDITOR_MODULE_MAPPING_SELECTION_H
#define MAP_EDITOR_MODULE_MAPPING_SELECTION_H

#include <QtWidgets/QComboBox>
#include "MappingObject.h"

namespace DATABASE
{
    class DatabaseMgr;
}

namespace CACHE
{
    class Manager;
}

class MapEditorModuleTileSelection;
class MapEditorModuleWorldObjects;

class MapEditorModuleMappingMode : public QWidget
{
    Q_OBJECT
public:
    MapEditorModuleMappingMode(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

    void setup();

    const MappingObject& getMappingMode() const;

private slots:
    void _onMappingModeChanged(int index);
    void _onWorldObjectIndexChanged(int32 index);

signals:
    void changeMappingMode(MAPPING_MODE::Type mode);
    void changeWorldObjectID(uint32 ID);

private:
    MapEditorModuleTileSelection* m_pModuleTileSelection = nullptr;
    MapEditorModuleWorldObjects* m_pModuleWorldObjects = nullptr;
    QComboBox* m_pMappingMode = nullptr;

    CACHE::Manager& m_CacheMgr;
    const DATABASE::DatabaseMgr& m_DBMgr;

    MappingObject m_MappingObject;
};
#endif
