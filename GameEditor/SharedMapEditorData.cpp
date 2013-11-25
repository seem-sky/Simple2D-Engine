#include "SharedMapEditorData.h"

SharedMapEditorData::SharedMapEditorData() : m_pTileCache(new TileCache()), m_pAutoTileCache(new AutoTileCache(m_pTileCache)),
m_pMapDBChanger(new DATABASE::MAP_STRUCTURE::MapDatabaseChanger()), m_CurrentLayer(MAP::LAYER_BACKGROUND)
{}

void SharedMapEditorData::_setDatabaseMgr(DATABASE::DatabaseMgrPtr pDBMgr)
{
    m_pDatabaseMgr = pDBMgr;
    if (m_pDatabaseMgr)
        m_pMapDBChanger->setDB(m_pDatabaseMgr->getMapDatabase());
    _updateData();
}

void SharedMapEditorData::_updateData()
{
    if (!m_pDatabaseMgr)
        return;
    m_pTileCache->setTileDB(m_pDatabaseMgr->getTileDatabase());
    m_pAutoTileCache->setAutoTileDB(m_pDatabaseMgr->getAutoTileDatabase());
}

DATABASE::MAP_STRUCTURE::MapDatabaseChangerPtr SharedMapEditorData::getMapDatabase()
{
    return m_pMapDBChanger;
}

DATABASE::ConstTileDatabasePtr SharedMapEditorData::getTileDatabase() const
{
    if (m_pDatabaseMgr)
        return m_pDatabaseMgr->getTileDatabase();
    return DATABASE::ConstTileDatabasePtr();
}

DATABASE::ConstTileSetDatabasePtr SharedMapEditorData::getTileSetDatabase() const
{
    if (m_pDatabaseMgr)
        return m_pDatabaseMgr->getTileSetDatabase();
    return DATABASE::ConstTileSetDatabasePtr();
}

DATABASE::ConstAutoTileDatabasePtr SharedMapEditorData::getAutoTileDatabase() const
{
    if (m_pDatabaseMgr)
        return m_pDatabaseMgr->getAutoTileDatabase();
    return DATABASE::ConstAutoTileDatabasePtr();
}

DATABASE::ConstSpriteDatabasePtr SharedMapEditorData::getSpriteDatabase() const
{
    if (m_pDatabaseMgr)
        return m_pDatabaseMgr->getSpriteDatabase();
    return DATABASE::ConstSpriteDatabasePtr();
}

DATABASE::ConstAnimationDatabasePtr SharedMapEditorData::getAnimationDatabase() const
{
    if (m_pDatabaseMgr)
        return m_pDatabaseMgr->getAnimationDatabase();
    return DATABASE::ConstAnimationDatabasePtr();
}

DATABASE::ConstWorldObjectDatabasePtr SharedMapEditorData::getWorldObjectDatabase() const
{
    if (m_pDatabaseMgr)
        return m_pDatabaseMgr->getWorldObjectDatabase();
    return DATABASE::ConstWorldObjectDatabasePtr();
}

DATABASE::ConstDynamicObjectDatabasePtr SharedMapEditorData::getDynamicObjectDatabase() const
{
    if (m_pDatabaseMgr)
        return m_pDatabaseMgr->getDynamicObjectDatabase();
    return DATABASE::ConstDynamicObjectDatabasePtr();
}