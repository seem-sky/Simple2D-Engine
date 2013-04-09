#include "DatabaseIO.h"

using namespace XML_IO;
using namespace DATABASE;

/*####
# TileDatabase
####*/
bool TileDatabaseXMLReader::readFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("Database");
    sNodeNames.push_back("TileDatabase");
    return XMLReader::readFile(sFileName, sNodeNames, threaded);
}

bool TileDatabaseXMLReader::getAttributeFromXML(TilePrototypePtr proto, const std::string sAttributeName, CComVariant value)
{
    if (!proto)
        return false;
    if (TextureDatabaseReader::getAttributeFromXML(proto, sAttributeName, value))
        return true;

    if (sAttributeName == "passability")
    {
        if (SUCCEEDED(value.ChangeType(VT_UI1)))
            proto->addPassabilityFlag(value.bVal);
        return true;
    }
    else if (sAttributeName == "terraintype")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->setTerrainType(value.uintVal);
        return true;
    }
    return false;
}

bool TileDatabaseXMLWriter::writeFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("Database");
    sNodeNames.push_back("TileDatabase");
    return XMLWriter::writeFile(sFileName, sNodeNames, threaded);
}

void TileDatabaseXMLWriter::getXMLFromAttributes(TilePrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode)
{
    if (!proto)
        return;
    TextureDatabaseWriter::getXMLFromAttributes(proto, pNewNode);
    changeAttribute(pNewNode, proto->getTerrainType(), "terraintype");
    changeAttribute(pNewNode, proto->getPassability(), "passability");
}

/*####
# AutoTileDatabase
####*/
bool AutoTileDatabaseXMLReader::readFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("Database");
    sNodeNames.push_back("AutoTileDatabase");
    return XMLReader::readFile(sFileName, sNodeNames, threaded);
}

bool AutoTileDatabaseXMLReader::getAttributeFromXML(AutoTilePrototypePtr proto, const std::string sAttributeName, CComVariant value)
{
    if (!proto)
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, sAttributeName, value))
        return true;

    std::string sSubstr = "Index";
    uint32 uiIndex = atoi(sAttributeName.substr(sAttributeName.find(sSubstr) + sSubstr.size()).c_str());
    if (SUCCEEDED(value.ChangeType(VT_UINT)) && uiIndex < AUTO_TILE_SET_COUNT)
    {
        proto->setTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(uiIndex), value.uintVal);
        return true;
    }

    return false;
}

bool AutoTileDatabaseXMLWriter::writeFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("Database");
    sNodeNames.push_back("AutoTileDatabase");
    return XMLWriter::writeFile(sFileName, sNodeNames, threaded);
}

void AutoTileDatabaseXMLWriter::getXMLFromAttributes(AutoTilePrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, pNewNode);
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
        changeAttribute(pNewNode, proto->getTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)), ("Index" + ToString(i)).c_str());
}

/*####
# SpriteDatabase
####*/
bool SpriteDatabaseXMLReader::readFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("Database");
    sNodeNames.push_back("SpriteDatabase");
    return XMLReader::readFile(sFileName, sNodeNames, threaded);
}

bool SpriteDatabaseXMLReader::getAttributeFromXML(SpritePrototypePtr proto, const std::string sAttributeName, CComVariant value)
{
    if (!proto)
        return false;
    if (TextureDatabaseReader::getAttributeFromXML(proto, sAttributeName, value))
        return true;

    if (sAttributeName == "boundingX")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->setBoundingX(value.uintVal);
        return true;
    }
    else if (sAttributeName == "boundingY")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->setBoundingY(value.uintVal);
        return true;
    }
    else if (sAttributeName == "boundingWidth")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->setBoundingWidth(value.uintVal);
        return true;
    }
    else if (sAttributeName == "boundingHeight")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->setBoundingHeight(value.uintVal);
        return true;
    }
    else if (sAttributeName == "columns")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->setColumns(value.uintVal);
        return true;
    }
    else if (sAttributeName == "rows")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->setRows(value.uintVal);
        return true;
    }
    return false;
}

bool SpriteDatabaseXMLWriter::writeFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("Database");
    sNodeNames.push_back("SpriteDatabase");
    return XMLWriter::writeFile(sFileName, sNodeNames, threaded);
}

void SpriteDatabaseXMLWriter::getXMLFromAttributes(SpritePrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode)
{
    if (!proto)
        return;
    TextureDatabaseWriter::getXMLFromAttributes(proto, pNewNode);
    changeAttribute(pNewNode, proto->getBoundingX(), "boundingX");
    changeAttribute(pNewNode, proto->getBoundingY(), "boundingY");
    changeAttribute(pNewNode, proto->getBoundingWidth(), "boundingWidth");
    changeAttribute(pNewNode, proto->getBoundingHeight(), "boundingHeight");
    changeAttribute(pNewNode, proto->getColumns(), "columns");
    changeAttribute(pNewNode, proto->getRows(), "rows");
}

/*####
# MapDatabase
####*/
bool MapDatabaseXMLReader::readFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("MapDatabase");
    return XMLReader::readFile(sFileName, sNodeNames, threaded);
}

bool MapDatabaseXMLReader::getAttributeFromXML(MAP::MapPrototypePtr proto, const std::string sAttributeName, CComVariant value)
{
    if (!proto)
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, sAttributeName, value))
        return true;

    if (sAttributeName == "MapSizeX")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->m_Size.x = value.uintVal;
        return true;
    }
    else if (sAttributeName == "MapSizeY")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->m_Size.y = value.uintVal;
        return true;
    }
    else if (sAttributeName == "Layer")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->m_Size.z = value.uintVal;
        return true;
    }
    else if (sAttributeName == "ParentID")
    {
        if (SUCCEEDED(value.ChangeType(VT_UINT)))
            proto->setParentID(value.uintVal);
        return true;
    }
    else if (sAttributeName == "MapScript")
    {
        proto->setScriptName(std::string(_bstr_t(value)));
        return true;
    }
    else if (sAttributeName == "File")
    {
        proto->setFileName(std::string(_bstr_t(value)));
        return true;
    }
    return false;
}

bool MapDatabaseXMLWriter::writeFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("MapDatabase");
    return XMLWriter::writeFile(sFileName, sNodeNames, threaded);
}

void MapDatabaseXMLWriter::getXMLFromAttributes(MAP::MapPrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, pNewNode);
    changeAttribute(pNewNode, proto->getSize().x, "MapSizeX");
    changeAttribute(pNewNode, proto->getSize().y, "MapSizeY");
    changeAttribute(pNewNode, proto->getSize().z, "Layer");
    changeAttribute(pNewNode, proto->getParentID(), "ParentID");
    changeAttribute(pNewNode, proto->getScriptName().c_str(), "MapScript");
    changeAttribute(pNewNode, proto->getFileName().c_str(), "File");
}

/*####
# TextDatabase
####*/
bool TextDatabaseXMLReader::readFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("Database");
    sNodeNames.push_back("TextDatabase");
    return XMLReader::readFile(sFileName, sNodeNames, threaded);
}

bool TextDatabaseXMLReader::getAttributeFromXML(TextPrototypePtr proto, const std::string sAttributeName, CComVariant value)
{
    if (!proto)
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, sAttributeName, value))
        return true;

    std::string sSubstr = "Local";
    uint32 uiIndex = atoi(sAttributeName.substr(sAttributeName.find(sSubstr) + sSubstr.size()).c_str());
    if (uiIndex >= proto->getLocalCount())
        proto->setLocalCount(uiIndex+1);
    proto->setLocal(uiIndex, std::string(_bstr_t(value)));
    return true;
}

bool TextDatabaseXMLWriter::writeFile(const std::string &sFileName, const bool threaded)
{
    if (!m_pDB)
        return false;
    StdStringVector sNodeNames;
    sNodeNames.push_back("Database");
    sNodeNames.push_back("TextDatabase");
    return XMLWriter::writeFile(sFileName, sNodeNames, threaded);
}

void TextDatabaseXMLWriter::getXMLFromAttributes(TextPrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, pNewNode);
    for (uint32 i = 0; i < proto->getLocalCount(); ++i)
        changeAttribute(pNewNode, proto->getLocal(i).c_str(), ("Local" + ToString(i)).c_str());
}