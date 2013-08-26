#include "DatabaseIO.h"

using namespace XML_IO;
using namespace DATABASE;

/*####
# TileDatabase
####*/
bool TileDatabaseXMLReader::getAttributeFromXML(TilePrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (TextureDatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    if (attributeName == "passability")
    {
        proto->addPassabilityFlag(attributeValue.toUShort());
        return true;
    }
    else if (attributeName == "terraintype")
    {
        proto->setTerrainType(attributeValue.toUInt());
        return true;
    }
    return false;
}

void TileDatabaseXMLWriter::getXMLFromAttributes(TilePrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    TextureDatabaseWriter::getXMLFromAttributes(proto, writer);
    writer.writeAttribute("terraintype", QString::number(proto->getTerrainType()));
    writer.writeAttribute("passability", QString::number(proto->getPassability()));
}

/*####
# TileSetDatabase
####*/
bool TileSetDatabaseXMLReader::getAttributeFromXML(TileSetPrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    if (attributeName == "columnCount")
    {
        proto->resizeTiles(UInt32Point(attributeValue.toUInt(), proto->getTileCount().y));
        return true;
    }
    else if (attributeName == "rowCount")
    {
        proto->resizeTiles(UInt32Point(proto->getTileCount().x, attributeValue.toUInt()));
        return true;
    }
    // check tile IDs
    else
    {
        uint32 uiDelimiter1 = attributeName.indexOf("x"), uiDelimiter2 = attributeName.indexOf("y");
        proto->setTileID(UInt32Point(attributeName.mid(uiDelimiter1+1, uiDelimiter2-uiDelimiter1-1).toUInt(), attributeName.right(attributeName.length()-uiDelimiter2-1).toUInt()), attributeValue.toUInt());
    }
    return false;
}

void TileSetDatabaseXMLWriter::getXMLFromAttributes(TileSetPrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    writer.writeAttribute("columnCount", QString::number(proto->getTileCount().x));
    writer.writeAttribute("rowCount", QString::number(proto->getTileCount().y));
    // get tile IDs
    for (uint32 x = 0; x < proto->getTileCount().x; ++x)
    {
        for (uint32 y = 0; y < proto->getTileCount().y; ++y)
            writer.writeAttribute("x"+QString::number(x)+"y"+QString::number(y), QString::number(proto->getTileID(UInt32Point(x, y))));
    }
}

/*####
# AutoTileDatabase
####*/
bool AutoTileDatabaseXMLReader::getAttributeFromXML(AutoTilePrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    QString tempName = attributeName;
    uint32 uiIndex = tempName.remove("Index").toUInt();
    if (uiIndex < AUTO_TILE::AUTO_TILE_SET_COUNT)
    {
        proto->setTileID(static_cast<AUTO_TILE::AUTO_TILE_INDEX>(uiIndex), attributeValue.toUInt());
        return true;
    }

    return false;
}

void AutoTileDatabaseXMLWriter::getXMLFromAttributes(AutoTilePrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    for (uint32 i = 0; i < AUTO_TILE::AUTO_TILE_SET_COUNT; ++i)
        writer.writeAttribute("Index" + QString::number(i), QString::number(proto->getTileID(static_cast<AUTO_TILE::AUTO_TILE_INDEX>(i))));
}

/*####
# AnimationDatabase
####*/
bool AnimationDatabaseXMLReader::_getFrameAttribute(AnimationPrototype::Frame &frame, const QString &nodeName, const QString &nodeValue)
{
    if (nodeName == "time_msec")
    {
        frame.m_uiMsecTime = nodeValue.toUInt();
        return true;
    }
    return false;
}

bool AnimationDatabaseXMLReader::_getSpriteAttribute(AnimationPrototype::Sprite &sprite, const QString &nodeName, const QString &nodeValue)
{
    if (nodeName == "spriteID")
    {
        sprite.m_uiSpriteID = nodeValue.toUInt();
        return true;
    }
    else if (nodeName == "posX")
    {
        sprite.m_Pos.x = nodeValue.toInt();
        return true;
    }
    else if (nodeName == "posY")
    {
        sprite.m_Pos.y = nodeValue.toInt();
        return true;
    }
    else if (nodeName == "scale")
    {
        sprite.m_uiScale = nodeValue.toUInt();
        return true;
    }
    else if (nodeName == "rotation")
    {
        sprite.m_uiRotation = nodeValue.toUInt();
        return true;
    }
    else if (nodeName == "opacity")
    {
        sprite.m_uiOpacity = nodeValue.toUShort();
        return true;
    }
    return false;
}

bool AnimationDatabaseXMLReader::getChildrenFromXML(const QDomNode &node, AnimationPrototypePtr proto, const QString &childName)
{
    if (node.isNull() || !proto || childName.isEmpty())
        return false;

    // checkout frame and sprites
    if (childName == "Frame" || childName == "Sprite")
    {
        AnimationPrototype::Frame newFrame;
        AnimationPrototype::Sprite newSprite;
        // get attributes
        QDomNamedNodeMap attributeNodes = node.attributes();
        for (int32 i = 0; i < attributeNodes.length(); ++i)
        {
            QDomNode attributeNode = attributeNodes.item(i);
            if (attributeNode.isNull())
                continue;
            if (childName == "Frame")
                _getFrameAttribute(newFrame, attributeNode.nodeName(), attributeNode.nodeValue());
            else
                _getSpriteAttribute(newSprite, attributeNode.nodeName(), attributeNode.nodeValue());
        }
        if (childName == "Frame")
        {
            // get sprites
            proto->setFrame(proto->getFrameCount(), newFrame);
            checkoutChildren(node, proto);
            return true;
        }
        else
        {
            // add sprite to last frame
            if (proto->getFrame(proto->getFrameCount()-1, newFrame))
            {
                newFrame.m_Sprites.push_back(newSprite);
                proto->setFrame(proto->getFrameCount()-1, newFrame);
            }
        }
    }
    return false;
}

void AnimationDatabaseXMLWriter::getXMLFromAttributes(AnimationPrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    // remove all empty frames from the end
    while (proto->getFrameCount() > 0)
    {
        AnimationPrototype::Frame frame;
        uint32 uiIndex = proto->getFrameCount()-1;
        if (!proto->getFrame(uiIndex, frame) || !frame.m_Sprites.empty())
            break;
        proto->removeFrame(uiIndex);
    }

    // store frames
    for (uint32 i = 0; i < proto->getFrameCount(); ++i)
    {
        AnimationPrototype::Frame frame;
        if (!proto->getFrame(i, frame))
            continue;
        writer.writeStartElement("Frame");
        writer.writeAttribute("time_msec", QString::number(frame.m_uiMsecTime));
        // store sprites
        for (uint32 j = 0; j < frame.m_Sprites.size(); ++j)
        {
            writer.writeEmptyElement("Sprite");
            writer.writeAttribute("spriteID", QString::number(frame.m_Sprites.at(j).m_uiSpriteID));
            writer.writeAttribute("posX", QString::number(frame.m_Sprites.at(j).m_Pos.x));
            writer.writeAttribute("posY", QString::number(frame.m_Sprites.at(j).m_Pos.y));
            writer.writeAttribute("scale", QString::number(frame.m_Sprites.at(j).m_uiScale));
            writer.writeAttribute("rotation", QString::number(frame.m_Sprites.at(j).m_uiRotation));
            writer.writeAttribute("opacity", QString::number(frame.m_Sprites.at(j).m_uiOpacity));
        }
        writer.writeEndElement();
    }
}

/*####
# MapDatabase
####*/
bool MapDatabaseXMLReader::getAttributeFromXML(MAP_STRUCTURE::MapPrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    if (attributeName == "MapSizeX")
    {
        proto->setSizeX(attributeValue.toUInt());
        return true;
    }
    else if (attributeName == "MapSizeY")
    {
        proto->setSizeY(attributeValue.toUInt());
        return true;
    }
    else if (attributeName == "BackgroundLayer")
    {
        proto->setLayerSize(attributeValue.toUInt(), MAP::LAYER_BACKGROUND);
        return true;
    }
    else if (attributeName == "ForegroundLayer")
    {
        proto->setLayerSize(attributeValue.toUInt(), MAP::LAYER_FOREGROUND);
        return true;
    }
    else if (attributeName == "ParentID")
    {
        proto->setParentID(attributeValue.toUInt());
        return true;
    }
    else if (attributeName == "MapScript")
    {
        proto->setScriptName(attributeValue);
        return true;
    }
    else if (attributeName == "File")
    {
        proto->setFileName(attributeValue);
        return true;
    }
    return false;
}

void MapDatabaseXMLWriter::getXMLFromAttributes(MAP_STRUCTURE::MapPrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    writer.writeAttribute("MapSizeX", QString::number(proto->getSize().x));
    writer.writeAttribute("MapSizeY", QString::number(proto->getSize().y));
    writer.writeAttribute("BackgroundLayer", QString::number(proto->getLayerSize(MAP::LAYER_BACKGROUND)));
    writer.writeAttribute("ForegroundLayer", QString::number(proto->getLayerSize(MAP::LAYER_FOREGROUND)));
    writer.writeAttribute("ParentID", QString::number(proto->getParentID()));
    writer.writeAttribute("MapScript", proto->getScriptName());
    writer.writeAttribute("File", proto->getFileName());
}

/*####
# LocalsDatabase
####*/
bool LocalsDatabaseXMLReader::getAttributeFromXML(LocalisationPrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    QString tempName = attributeName;
    uint32 uiIndex = tempName.remove("Local").toUInt();
    proto->setLocalisation(uiIndex, attributeValue);
    return true;
}

void LocalsDatabaseXMLWriter::getXMLFromAttributes(LocalisationPrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    for (uint32 i = 0; i < proto->getLocalsCount(); ++i)
        writer.writeAttribute("Local" + QString::number(i), proto->getLocalisation(i));
}

/*####
# DynamicObjectDatabase
####*/
bool DynamicObjectDatabaseXMLReader::getAttributeFromXML(DynamicObjectPrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (ObjectDatabaseXMLReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    if (attributeName == "MSpeed")
    {
        proto->setMovementSpeed(attributeValue.toUShort());
        return true;
    }
    return false;
}

void DynamicObjectDatabaseXMLWriter::getXMLFromAttributes(DynamicObjectPrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    writer.writeAttribute("MSpeed", QString::number(proto->getMovementSpeed()));
}