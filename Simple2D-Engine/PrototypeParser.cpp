#include "PrototypeParser.h"
#include <QtCore/QStringList>

using namespace DATABASE;

//void PrototypeParser::parseToXML(PrototypePtr pProto,  QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//
//    writer.writeAttribute("ID", QString::number(pProto->getID()));
//    writer.writeAttribute("name", pProto->getName());
//}
//
//void PrototypeParser::parseToXML(TexturePrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    writer.writeAttribute("file", pProto->getFileName());
//    writer.writeAttribute("path", pProto->getPath());
//    writer.writeAttribute("transparency_color", QString::fromStdString(pProto->getTransparencyColor().getColorString()));
//    parseToXML(std::static_pointer_cast<Prototype>(pProto), writer);
//}
//
//void PrototypeParser::parseToXML(TilePrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    writer.writeAttribute("terraintype", QString::number(pProto->getTerrainType()));
//    writer.writeAttribute("passability", QString::number(pProto->getPassability()));;
//    parseToXML(std::static_pointer_cast<TexturePrototype>(pProto), writer);
//}
//
//void PrototypeParser::parseToXML(TileSetPrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    writer.writeAttribute("columns", QString::number(pProto->getTileCount().x));
//    writer.writeAttribute("rows", QString::number(pProto->getTileCount().y));
//    parseToXML(std::static_pointer_cast<Prototype>(pProto), writer);
//
//     get tile IDs
//    QString tiles;
//    for (uint32 x = 0; x < pProto->getTileCount().x; ++x)
//    {
//        for (uint32 y = 0; y < pProto->getTileCount().y; ++y)
//        {
//            if (x || y)
//                tiles.append(",");
//            tiles.append(QString::number(pProto->getTileID(UInt32Point(x, y))));
//        }
//    }
//    if (!tiles.isEmpty())
//        writer.writeAttribute("tiles", tiles);
//}
//
//void PrototypeParser::parseToXML(AutoTilePrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    parseToXML(std::static_pointer_cast<Prototype>(pProto), writer);
//
//    QString tiles;
//    for (uint32 i = 0; i < AUTO_TILE::AUTO_TILE_SET_COUNT; ++i)
//    {
//        if (i)
//            tiles.append(",");
//        tiles.append(QString::number(pProto->getTileID(static_cast<AUTO_TILE::AUTO_TILE_INDEX>(i))));
//    }
//    if (!tiles.isEmpty())
//        writer.writeAttribute("tiles", tiles);
//}
//
//void PrototypeParser::parseToXML(AnimationPrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    parseToXML(std::static_pointer_cast<Prototype>(pProto), writer);
//
//     remove all empty frames from the end
//    while (pProto->getFrameCount() > 0)
//    {
//        AnimationPrototype::Frame frame;
//        uint32 uiIndex = pProto->getFrameCount()-1;
//        if (!pProto->getFrame(uiIndex, frame) || !frame.isEmpty())
//            break;
//        pProto->removeFrame(uiIndex);
//    }
//
//     store frames
//    for (uint32 i = 0; i < pProto->getFrameCount(); ++i)
//    {
//        AnimationPrototype::Frame frame;
//        if (!pProto->getFrame(i, frame))
//            continue;
//        writer.writeStartElement("frame");
//        writer.writeAttribute("time", QString::number(frame.getTimeInMsec()));
//         store sprites
//        for (uint32 j = 0; j < frame.getSpriteCount(); ++j)
//        {
//            writer.writeEmptyElement("sprite");
//            AnimationPrototype::Sprite sprite = frame.getSprite(j);
//            writer.writeAttribute("ID", QString::number(sprite.m_uiSpriteID));
//            writer.writeAttribute("x", QString::number(sprite.m_Pos.x));
//            writer.writeAttribute("y", QString::number(sprite.m_Pos.y));
//            writer.writeAttribute("scale", QString::number(sprite.m_uiScale));
//            writer.writeAttribute("rotation", QString::number(sprite.m_uiRotation));
//            writer.writeAttribute("opacity", QString::number(sprite.m_uiOpacity));
//        }
//        writer.writeEndElement();
//    }
//}
//
//void PrototypeParser::parseToXML(MAP_STRUCTURE::MapPrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    writer.writeAttribute("sizeX", QString::number(pProto->getSize().x));
//    writer.writeAttribute("sizeY", QString::number(pProto->getSize().y));
//    writer.writeAttribute("backLayer", QString::number(pProto->getLayerSize(MAP::LAYER_BACKGROUND)));
//    writer.writeAttribute("foreLayer", QString::number(pProto->getLayerSize(MAP::LAYER_FOREGROUND)));
//    writer.writeAttribute("parentID", QString::number(pProto->getParentID()));
//    writer.writeAttribute("script", pProto->getScriptName());
//    writer.writeAttribute("file", pProto->getFileName());
//    parseToXML(std::static_pointer_cast<Prototype>(pProto), writer);
//}
//
//void PrototypeParser::parseToXML(LocalisationPrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    parseToXML(std::static_pointer_cast<Prototype>(pProto), writer);
//
//    for (uint32 i = 0; i < pProto->getLocalsCount(); ++i)
//    {
//        writer.writeEmptyElement("local");
//        writer.writeAttribute("entry", QString::number(i));
//        writer.writeAttribute("text", pProto->getLocalisation(i));
//    }
//}
//
//void PrototypeParser::parseToXML(WorldObjectPrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    writer.writeAttribute("boundingX", QString::number(pProto->getBoundingX()));
//    writer.writeAttribute("boundingY", QString::number(pProto->getBoundingY()));
//    writer.writeAttribute("boundingWidth", QString::number(pProto->getBoundingWidth()));
//    writer.writeAttribute("boundingHeight", QString::number(pProto->getBoundingHeight()));
//    writer.writeAttribute("animationSpeed", QString::number(pProto->getAnimationSpeed()));
//    writer.writeAttribute("script", pProto->getScriptName());
//    parseToXML(std::static_pointer_cast<Prototype>(pProto), writer);
//
//     store animation infos
//    for (uint32 i = 1; i <= pProto->getAnimationCount(); ++i)
//    {
//        MAP_OBJECT::AnimationInfo animationInfo = pProto->getAnimationInfo(i);
//        if (animationInfo.m_uiAnimationID == 0 && animationInfo.m_uiAnimationTypeID == 0)
//            continue;
//        writer.writeEmptyElement("animation");
//        writer.writeAttribute("entry", QString::number(i));
//        writer.writeAttribute("ID", QString::number(animationInfo.m_uiAnimationID));
//        writer.writeAttribute("typeID", QString::number(animationInfo.m_uiAnimationTypeID));
//    }
//}
//
//void PrototypeParser::parseToXML(DynamicObjectPrototypePtr pProto, QXmlStreamWriter& writer)
//{
//    if (!pProto)
//        return;
//    writer.writeAttribute("movementSpeed", QString::number(pProto->getMovementSpeed()));
//    parseToXML(std::static_pointer_cast<MAP_OBJECT::WorldObjectPrototype>(pProto), writer);
//}
//
///*#####
//# reading: attributes
//#####*/
//void PrototypeParser::parseAttributesFromXML(PrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    pProto->setID(attributes.value("ID").toUInt());
//    pProto->setName(attributes.value("name").toString());
//}
//
//void PrototypeParser::parseAttributesFromXML(TexturePrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    parseAttributesFromXML(std::static_pointer_cast<Prototype>(pProto), attributes);
//
//    pProto->setFileName(attributes.value("file").toString());
//    pProto->setPath(attributes.value("path").toString());
//    pProto->setTransparencyColor(Color(attributes.value("transparency_color").toString().toStdString()));
//}
//
//void PrototypeParser::parseAttributesFromXML(SpritePrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    parseAttributesFromXML(std::static_pointer_cast<TexturePrototype>(pProto), attributes);
//}
//
//void PrototypeParser::parseAttributesFromXML(TilePrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    parseAttributesFromXML(std::static_pointer_cast<TexturePrototype>(pProto), attributes);
//
//    pProto->setTerrainType(attributes.value("terraintype").toUInt());
//    pProto->addPassabilityFlag(attributes.value("passability").toUInt());
//}
//
//void PrototypeParser::parseAttributesFromXML(TileSetPrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    parseAttributesFromXML(std::static_pointer_cast<Prototype>(pProto), attributes);
//
//    UInt32Point size(attributes.value("columns").toUInt(), attributes.value("rows").toUInt());
//    pProto->resizeTiles(size);
//    if (attributes.hasAttribute("tiles"))
//    {
//        QStringList tileList = attributes.value("tiles").toString().split(",");
//        for (int32 i = 0; i < tileList.size(); ++i)
//            pProto->setTileID(UInt32Point(i/size.x, i%size.y), tileList.at(i).toUInt());
//    }
//}
//
//void PrototypeParser::parseAttributesFromXML(AutoTilePrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    parseAttributesFromXML(std::static_pointer_cast<Prototype>(pProto), attributes);
//
//    if (attributes.hasAttribute("tiles"))
//    {
//        QStringList tileList = attributes.value("tiles").toString().split(",");
//        for (int32 i = 0; i < tileList.size(); ++i)
//            pProto->setTileID(static_cast<AUTO_TILE::AUTO_TILE_INDEX>(i), tileList.at(i).toUInt());
//    }
//}
//
//void PrototypeParser::parseAttributesFromXML(MAP_STRUCTURE::MapPrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    parseAttributesFromXML(std::static_pointer_cast<Prototype>(pProto), attributes);
//
//     setup map size
//    pProto->setSize(UInt32Point(attributes.value("sizeX").toUInt(), attributes.value("sizeY").toUInt()),
//        attributes.value("foreLayer").toUInt(), attributes.value("backLayer").toUInt());
//
//    pProto->setParentID(attributes.value("parentID").toUInt());
//    pProto->setScriptName(attributes.value("script").toString());
//    pProto->setFileName(attributes.value("file").toString());
//}
//
//void PrototypeParser::parseAttributesFromXML(WorldObjectPrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    parseAttributesFromXML(std::static_pointer_cast<Prototype>(pProto), attributes);
//
//     bounding rect
//    pProto->setBoundingX(attributes.value("boundingX").toInt());
//    pProto->setBoundingY(attributes.value("boundingY").toInt());
//    pProto->setBoundingWidth(attributes.value("boundingWidth").toUInt());
//    pProto->setBoundingHeight(attributes.value("boundingHeight").toUInt());
//
//    pProto->setAnimationSpeed(attributes.value("animationSpeed").toUShort());
//    pProto->setScriptName(attributes.value("script").toString());
//}
//
//void PrototypeParser::parseAttributesFromXML(DynamicObjectPrototypePtr pProto, QXmlStreamAttributes& attributes)
//{
//    parseAttributesFromXML(std::static_pointer_cast<MAP_OBJECT::WorldObjectPrototype>(pProto), attributes);
//    pProto->setMovementSpeed(attributes.value("movementSpeed").toUShort());
//}
//
///*#####
//# reading: children
//#####*/
//void PrototypeParser::parseChildrenFromXML(AnimationPrototypePtr pProto, const QXmlStreamReader& reader)
//{
//    if (reader.name() == "frame")
//    {
//        AnimationPrototype::Frame newFrame;
//        QXmlStreamAttributes attributes = reader.attributes();
//        newFrame.setTimeInMsec(attributes.value("time").toUInt());
//        pProto->setFrame(pProto->getFrameCount(), newFrame);
//    }
//    else if (reader.name() == "sprite")
//    {
//        AnimationPrototype::Frame lastFrame;
//        if (pProto->getFrame(pProto->getFrameCount()-1, lastFrame))
//        {
//            AnimationPrototype::Sprite newSprite;
//            QXmlStreamAttributes attributes = reader.attributes();
//            newSprite.m_uiSpriteID = attributes.value("ID").toUInt();
//            newSprite.m_Pos.x = attributes.value("x").toInt();
//            newSprite.m_Pos.y = attributes.value("y").toInt();
//            newSprite.m_uiScale = attributes.value("scale").toUInt();
//            newSprite.m_uiRotation = attributes.value("rotation").toUInt();
//            newSprite.m_uiOpacity = attributes.value("opacity").toUShort();
//            lastFrame.addSprite(newSprite);
//            pProto->setFrame(pProto->getFrameCount()-1, lastFrame);
//        }
//    }
//}
//
//void PrototypeParser::parseChildrenFromXML(LocalisationPrototypePtr pProto, const QXmlStreamReader& reader)
//{
//    if (reader.name() == "local")
//    {
//        QXmlStreamAttributes attributes = reader.attributes();
//        if (!attributes.hasAttribute("entry"))
//            return;
//
//        pProto->setLocalisation(attributes.value("entry").toUInt(), attributes.value("text").toString());
//    }
//}
//
//void PrototypeParser::parseChildrenFromXML(WorldObjectPrototypePtr pProto, const QXmlStreamReader& reader)
//{
//    if (reader.name() == "animation")
//    {
//        QXmlStreamAttributes attributes = reader.attributes();
//        if (!attributes.hasAttribute("entry"))
//            return;
//
//        pProto->setAnimationInfo(attributes.value("entry").toUInt(),
//            MAP_OBJECT::AnimationInfo(attributes.value("ID").toUInt(), attributes.value("typeID").toUInt()));
//    }
//}