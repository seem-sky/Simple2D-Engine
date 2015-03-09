#ifndef MAP_EDITOR_MODULE_MAPPING_SELECTION_H
#define MAP_EDITOR_MODULE_MAPPING_SELECTION_H

#include <QtWidgets/QComboBox>
#include "MappingObject.h"

namespace CACHE
{
    class Manager;
}

class MapEditorModuleTileSelection;
class MapEditorModuleWorldObjects;
class MapEditorModuleScriptArea;

class MapEditorModuleMappingMode : public QWidget
{
    Q_OBJECT
public:
    MapEditorModuleMappingMode(CACHE::Manager& cacheMgr, const database::Manager& DBMgr, QWidget* pParent = nullptr);

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
    MapEditorModuleScriptArea* m_pModuleScriptArea = nullptr;
    QComboBox* m_pMappingMode = nullptr;

    CACHE::Manager& m_CacheMgr;
    const database::Manager& m_DBMgr;

    MappingObject m_MappingObject;
};
#endif
