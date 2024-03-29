#include "MappingObject.h"
#include "MapEditor.h"
#include "TileMappingMode.h"
#include "ObjectMappingMode.h"
#include "ScriptAreaMappingMode.h"
#include "PresentationMappingMode.h"
#include "moc_MappingObject.h"

MappingObject::MappingObject(const database::Manager& DBMgr, QWidget* pParent)
    : QObject(pParent)
{
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Tile(DBMgr)));
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Object()));
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::ScriptArea()));
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Presentation()));
    setMappingMode(MAPPING_MODE::Type::TILE_MAPPING);
}

const MAPPING_MODE::Interface* MappingObject::getMappingMode(MAPPING_MODE::Type mode) const
{
    return m_MappingModes.at(static_cast<std::size_t>(mode)).get();
}

MAPPING_MODE::Interface* MappingObject::getMappingMode(MAPPING_MODE::Type mode)
{
    return m_MappingModes.at(static_cast<std::size_t>(mode)).get();
}

void MappingObject::setMappingMode(MAPPING_MODE::Type mode)
{
    m_pCurrentMappingMode = nullptr;
    for (auto& pMode : m_MappingModes)
    {
        if (pMode->getModeType() == mode)
            m_pCurrentMappingMode = pMode.get();
    }
}

MAPPING_MODE::Type MappingObject::getMappingModeType() const
{
    return m_pCurrentMappingMode->getModeType();
}

QStringList MappingObject::getMappingModeNames() const
{
    QStringList list;
    for (auto& pMode : m_MappingModes)
        list.push_back(pMode->getModeName());
    return list;
}

void MappingObject::press(MapEditor& editor, QMouseEvent* pEvent)
{
    m_pCurrentMappingMode->press(editor, pEvent);
}

void MappingObject::release(MapEditor& editor, QMouseEvent* pEvent)
{
    m_pCurrentMappingMode->release(editor, pEvent);
}

void MappingObject::move(MapEditor& editor, QMouseEvent* pEvent)
{
    m_pCurrentMappingMode->move(editor, pEvent);
}

void MappingObject::onActionCopy(const MapEditor& editor)
{
    m_pCurrentMappingMode->copy(editor);
}

void MappingObject::onActionCut(const MapEditor& editor)
{
    m_pCurrentMappingMode->cut(editor);
}

void MappingObject::onActionDelete(MapEditor& editor)
{
    m_pCurrentMappingMode->remove(editor);
}

void MappingObject::onActionPaste(MapEditor& editor, const QPoint& pos)
{
    m_pCurrentMappingMode->paste(editor, pos);
}

void MappingObject::onActionKeyPressed(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent)
{
    m_pCurrentMappingMode->keyPress(editor, pos, pEvent);
}

void MappingObject::onActionKeyReleased(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent)
{
    m_pCurrentMappingMode->keyRelease(editor, pos, pEvent);
}

void MappingObject::onContextMenuRequest(MapEditor& editor, QContextMenuEvent* pEvent)
{
    m_pCurrentMappingMode->contextMenuRequest(editor, pEvent);
}
