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
    if (uiIndex < AUTO_TILE_SET_COUNT)
    {
        proto->setTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(uiIndex), attributeValue.toUInt());
        return true;
    }

    return false;
}

void AutoTileDatabaseXMLWriter::getXMLFromAttributes(AutoTilePrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
        writer.writeAttribute("Index" + QString::number(i), QString::number(proto->getTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i))));
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
        for (uint32 i = 0; i < attributeNodes.length(); ++i)
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
bool MapDatabaseXMLReader::getAttributeFromXML(MAP::MapPrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    if (attributeName == "MapSizeX")
    {
        proto->m_Size.x = attributeValue.toUInt();
        return true;
    }
    else if (attributeName == "MapSizeY")
    {
        proto->m_Size.y = attributeValue.toUInt();
        return true;
    }
    else if (attributeName == "Layer")
    {
        proto->m_Size.z = attributeValue.toUInt();
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

void MapDatabaseXMLWriter::getXMLFromAttributes(MAP::MapPrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    writer.writeAttribute("MapSizeX", QString::number(proto->getSize().x));
    writer.writeAttribute("MapSizeY", QString::number(proto->getSize().y));
    writer.writeAttribute("Layer", QString::number(proto->getSize().z));
    writer.writeAttribute("ParentID", QString::number(proto->getParentID()));
    writer.writeAttribute("MapScript", proto->getScriptName());
    writer.writeAttribute("File", proto->getFileName());
}

/*####
# TextDatabase
####*/
bool LocalsDatabaseXMLReader::getAttributeFromXML(TextPrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    QString tempName = attributeName;
    uint32 uiIndex = tempName.remove("Local").toUInt();
    if (uiIndex >= proto->getLocalCount())
        proto->setLocalCount(uiIndex+1);
    proto->setLocal(uiIndex, attributeValue);
    return true;
}

void LocalsDatabaseXMLWriter::getXMLFromAttributes(TextPrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    for (uint32 i = 0; i < proto->getLocalCount(); ++i)
        writer.writeAttribute("Local" + QString::number(i), proto->getLocal(i));
}

/*####
# WorldObjectDatabase
####*/
bool WorldObjectDatabaseXMLReader::getAttributeFromXML(WorldObjectPrototypePtr proto, const QString &attributeName, const QString &attributeValue)
{
    if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
        return false;
    if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
        return true;

    if (attributeName == "boundingX")
    {
        proto->setBoundingX(attributeValue.toUInt());
        return true;
    }
    else if (attributeName == "boundingY")
    {
        proto->setBoundingY(attributeValue.toUInt());
        return true;
    }
    else if (attributeName == "boundingWidth")
    {
        proto->setBoundingWidth(attributeValue.toUInt());
        return true;
    }
    else if (attributeName == "boundingHeight")
    {
        proto->setBoundingHeight(attributeValue.toUInt());
        return true;
    }
    else if (attributeName == "animationSpeed")
    {
        proto->setAnimationSpeed(attributeValue.toUShort());
        return true;
    }
    else if (attributeName == "scriptName")
    {
        proto->setScriptName(attributeValue);
        return true;
    }
    return false;
}

bool WorldObjectDatabaseXMLReader::getChildrenFromXML(const QDomNode &node, WorldObjectPrototypePtr proto, const QString &childName)
{
    if (node.isNull() || !proto || childName.isEmpty())
        return false;

    // checkout animation info
    if (childName == "AnimationInfo")
    {
        WorldObjectPrototype::AnimationInfo newAnimationInfo;
        QDomNamedNodeMap attributeList = node.attributes();
        // get attributes
        for (uint32 i = 0; i < attributeList.length(); ++i)
        {
            QDomNode tempNode = attributeList.item(i);
            if (tempNode.nodeName() == "animationID")
                newAnimationInfo.m_uiAnimationID = tempNode.nodeValue().toUInt();
            else if (tempNode.nodeName() == "animationTypeID")
                newAnimationInfo.m_uiObjectAnimationTypeID = tempNode.nodeValue().toUInt();
        }
        if (newAnimationInfo.m_uiObjectAnimationTypeID <= MIN_WORLD_OBJECT_POSE)
            proto->setAnimationInfo(newAnimationInfo.m_uiObjectAnimationTypeID-1, newAnimationInfo);
        else
            proto->setAnimationInfo(proto->getAnimationCount(), newAnimationInfo);
    }
    return false;
}

void WorldObjectDatabaseXMLWriter::getXMLFromAttributes(WorldObjectPrototypePtr proto, QXmlStreamWriter &writer)
{
    if (!proto)
        return;
    DatabaseWriter::getXMLFromAttributes(proto, writer);
    writer.writeAttribute("boundingX", QString::number(proto->getBoundingX()));
    writer.writeAttribute("boundingY", QString::number(proto->getBoundingY()));
    writer.writeAttribute("boundingWidth", QString::number(proto->getBoundingWidth()));
    writer.writeAttribute("boundingHeight", QString::number(proto->getBoundingHeight()));
    writer.writeAttribute("animationSpeed", QString::number(proto->getAnimationSpeed()));
    writer.writeAttribute("scriptName", proto->getScriptName());

    // store animation infos
    for (uint32 i = 0; i < proto->getAnimationCount(); ++i)
    {
        WorldObjectPrototype::AnimationInfo animationInfo = proto->getAnimationInfo(i);
        if (animationInfo.m_uiAnimationID == 0 && animationInfo.m_uiObjectAnimationTypeID == 0)
            continue;
        writer.writeEmptyElement("AnimationInfo");
        writer.writeAttribute("animationID", QString::number(animationInfo.m_uiAnimationID));
        writer.writeAttribute("animationTypeID", QString::number(animationInfo.m_uiObjectAnimationTypeID));
    }
}