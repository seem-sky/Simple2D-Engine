#include "DatabasePrototypes.h"
#include <QtCore/QStringList>

using namespace DATABASE;

/*#####
# Prototype
#####*/
void Prototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("ID", QString::number(getID()));
    writer.writeAttribute("name", getName());
}

void Prototype::fromXML(const QXmlStreamAttributes& attributes)
{
    setID(attributes.value("ID").toUInt());
    setName(attributes.value("name").toString());
}

/*#####
# RessourcePrototype
#####*/
void RessourcePrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    Prototype::fromXML(attributes);

    setFileName(attributes.value("file").toString());
    setPath(attributes.value("path").toString());
}

void RessourcePrototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("file", getFileName());
    writer.writeAttribute("path", getPath());

    Prototype::toXML(writer);
}

/*#####
# TexturePrototype
#####*/
void TexturePrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    RessourcePrototype::fromXML(attributes);

    setTransparencyColor(Color(attributes.value("transparency_color").toString().toStdString()));
}

void TexturePrototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("transparency_color", QString::fromStdString(getTransparencyColor().getColorString()));

    RessourcePrototype::toXML(writer);
}

/*#####
# TilePrototype
#####*/
void TilePrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    TexturePrototype::fromXML(attributes);

    setTerrainType(attributes.value("terraintype").toUInt());
    addPassabilityFlag(attributes.value("passability").toUInt());
}

void TilePrototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("terraintype", QString::number(getTerrainType()));
    writer.writeAttribute("passability", QString::number(getPassability()));;

    TexturePrototype::toXML(writer);
}

/*#####
# AutoTilePrototype
#####*/
void AUTO_TILE::AutoTilePrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    Prototype::fromXML(attributes);

    if (attributes.hasAttribute("tiles"))
    {
        QStringList tileList = attributes.value("tiles").toString().split(",");
        for (int32 i = 0; i < tileList.size(); ++i)
            setTileID(static_cast<AUTO_TILE::AUTO_TILE_INDEX>(i), tileList.at(i).toUInt());
    }
}

void AUTO_TILE::AutoTilePrototype::toXML(QXmlStreamWriter& writer) const
{
    Prototype::toXML(writer);

    QString tiles;
    for (uint32 i = 0; i < AUTO_TILE::AUTO_TILE_SET_COUNT; ++i)
    {
        if (i)
            tiles.append(",");
        tiles.append(QString::number(getTileID(static_cast<AUTO_TILE::AUTO_TILE_INDEX>(i))));
    }
    if (!tiles.isEmpty())
        writer.writeAttribute("tiles", tiles);
}

/*#####
# TileSetPrototype
#####*/
void TILE_SET::TileSetPrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    Prototype::fromXML(attributes);

    UInt32Point size(attributes.value("columns").toUInt(), attributes.value("rows").toUInt());
    resizeTiles(size);
    if (attributes.hasAttribute("tiles"))
    {
        QStringList tileList = attributes.value("tiles").toString().split(",");
        for (int32 i = 0; i < tileList.size(); ++i)
            setTileID(UInt32Point(i/size.x, i%size.y), tileList.at(i).toUInt());
    }
}

void TILE_SET::TileSetPrototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("columns", QString::number(getTileCount().x));
    writer.writeAttribute("rows", QString::number(getTileCount().y));
    Prototype::toXML(writer);

    //get tile IDs
    QString tiles;
    for (uint32 x = 0; x < getTileCount().x; ++x)
    {
        for (uint32 y = 0; y < getTileCount().y; ++y)
        {
            if (x || y)
                tiles.append(",");
            tiles.append(QString::number(getTileID(UInt32Point(x, y))));
        }
    }
    if (!tiles.isEmpty())
        writer.writeAttribute("tiles", tiles);
}

/*#####
# AnimationPrototype
#####*/
void ANIMATION::AnimationPrototype::toXML(QXmlStreamWriter& writer) const
{
    Prototype::toXML(writer);

    //store frames
    for (uint32 i = 0; i < getFrameCount(); ++i)
    {
        ANIMATION::Frame frame;
        if (!getFrame(i, frame))
            continue;
        writer.writeStartElement("frame");
        writer.writeAttribute("time", QString::number(frame.getTimeInMsec()));
        //store sprites
        for (uint32 j = 0; j < frame.getSpriteCount(); ++j)
        {
            writer.writeEmptyElement("sprite");
            ANIMATION::Sprite sprite = frame.getSprite(j);
            writer.writeAttribute("ID", QString::number(sprite.m_uiSpriteID));
            writer.writeAttribute("x", QString::number(sprite.m_Pos.x));
            writer.writeAttribute("y", QString::number(sprite.m_Pos.y));
            writer.writeAttribute("scale", QString::number(sprite.m_Scale));
            writer.writeAttribute("rotation", QString::number(sprite.m_uiRotation));
            writer.writeAttribute("opacity", QString::number(sprite.m_Opacity));
        }
        writer.writeEndElement();
    }
}

void ANIMATION::AnimationPrototype::insertChildren(const QXmlStreamReader& reader)
{
    if (reader.name() == "frame")
    {
        ANIMATION::Frame newFrame;
        QXmlStreamAttributes attributes = reader.attributes();
        newFrame.setTimeInMsec(attributes.value("time").toUInt());
        setFrame(getFrameCount(), newFrame);
    }
    else if (reader.name() == "sprite")
    {
        ANIMATION::Frame lastFrame;
        if (getFrame(getFrameCount()-1, lastFrame))
        {
            ANIMATION::Sprite newSprite;
            QXmlStreamAttributes attributes = reader.attributes();
            newSprite.m_uiSpriteID = attributes.value("ID").toUInt();
            newSprite.m_Pos.x = attributes.value("x").toInt();
            newSprite.m_Pos.y = attributes.value("y").toInt();
            newSprite.m_Scale = attributes.value("scale").toFloat();
            newSprite.m_uiRotation = attributes.value("rotation").toUInt();
            newSprite.m_Opacity = attributes.value("opacity").toFloat();
            lastFrame.addSprite(newSprite);
            setFrame(getFrameCount()-1, lastFrame);
        }
    }
    else
        Prototype::insertChildren(reader);
}

/*#####
# LocalisationPrototype
#####*/
void LOCALISATION::LocalisationPrototype::toXML(QXmlStreamWriter& writer) const
{
    Prototype::toXML(writer);

    for (uint32 i = 0; i < getLocalsCount(); ++i)
    {
        writer.writeEmptyElement("local");
        writer.writeAttribute("index", QString::number(i));
        writer.writeAttribute("text", getLocalisation(i));
    }
}

void LOCALISATION::LocalisationPrototype::insertChildren(const QXmlStreamReader& reader)
{
    if (reader.name() == "local")
    {
        QXmlStreamAttributes attributes = reader.attributes();
        if (!attributes.hasAttribute("index"))
            return;
                        
        setLocalisation(attributes.value("index").toUInt(), attributes.value("text").toString());
    }
    else
        Prototype::insertChildren(reader);
}

/*#####
# WorldObjectPrototype
#####*/
void MAP_OBJECT::WorldObjectPrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    Prototype::fromXML(attributes);

    // bounding rect
    setBoundingX(attributes.value("boundingX").toInt());
    setBoundingY(attributes.value("boundingY").toInt());
    setBoundingWidth(attributes.value("boundingWidth").toUInt());
    setBoundingHeight(attributes.value("boundingHeight").toUInt());

    setAnimationSpeed(attributes.value("animationSpeed").toUShort());
    setScriptName(attributes.value("script").toString());
}

void MAP_OBJECT::WorldObjectPrototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("boundingX", QString::number(getBoundingX()));
    writer.writeAttribute("boundingY", QString::number(getBoundingY()));
    writer.writeAttribute("boundingWidth", QString::number(getBoundingWidth()));
    writer.writeAttribute("boundingHeight", QString::number(getBoundingHeight()));
    writer.writeAttribute("animationSpeed", QString::number(getAnimationSpeed()));
    writer.writeAttribute("script", getScriptName());
    Prototype::toXML(writer);

    //store animation infos
    for (uint32 i = 1; i <= getAnimationCount(); ++i)
    {
        MAP_OBJECT::AnimationInfo animationInfo = getAnimationInfo(i);
        if (animationInfo.m_uiAnimationID == 0 && animationInfo.m_uiAnimationTypeID == 0)
            continue;
        writer.writeEmptyElement("animation");
        writer.writeAttribute("entry", QString::number(i));
        writer.writeAttribute("ID", QString::number(animationInfo.m_uiAnimationID));
        writer.writeAttribute("typeID", QString::number(animationInfo.m_uiAnimationTypeID));
    }
}

void MAP_OBJECT::WorldObjectPrototype::insertChildren(const QXmlStreamReader& reader)
{
    if (reader.name() == "animation")
    {
        QXmlStreamAttributes attributes = reader.attributes();
        if (!attributes.hasAttribute("entry"))
            return;
                        
        setAnimationInfo(attributes.value("entry").toUInt(),
        MAP_OBJECT::AnimationInfo(attributes.value("ID").toUInt(), attributes.value("typeID").toUInt()));
    }
    else
        Prototype::insertChildren(reader);
}

/*#####
# MapPrototype
#####*/
void MAP_STRUCTURE::MapPrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    Prototype::fromXML(attributes);

    // setup map size
    setSize(UInt32Point(attributes.value("sizeX").toUInt(), attributes.value("sizeY").toUInt()), attributes.value("foreLayer").toUInt(),
        attributes.value("backLayer").toUInt());

    setParentID(attributes.value("parentID").toUInt());
    setScriptName(attributes.value("script").toString());
    setFileName(attributes.value("file").toString());
}

void MAP_STRUCTURE::MapPrototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("sizeX", QString::number(getSize().x));
    writer.writeAttribute("sizeY", QString::number(getSize().y));
    writer.writeAttribute("backLayer", QString::number(getLayerSize(MAP::LAYER_BACKGROUND)));
    writer.writeAttribute("foreLayer", QString::number(getLayerSize(MAP::LAYER_FOREGROUND)));
    writer.writeAttribute("parentID", QString::number(getParentID()));
    writer.writeAttribute("script", getScriptName());
    writer.writeAttribute("file", getFileName());

    Prototype::toXML(writer);
}
