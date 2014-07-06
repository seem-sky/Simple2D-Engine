#include "DatabasePrototypes.h"
#include <QtCore/QStringList>
#include "PrototypeAnimationModule.h"

using namespace DATABASE::PROTOTYPE;

/*#####
# Prototype
#####*/
void Prototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("ID", QString::number(getID()));
    if (!getName().isEmpty())
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
    if (!getFileName().isEmpty())
        writer.writeAttribute("file", getFileName());
    if (!getPath().isEmpty())
        writer.writeAttribute("path", getPath());

    Prototype::toXML(writer);
}

/*#####
# TexturePrototype
#####*/
void TexturePrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    RessourcePrototype::fromXML(attributes);
    if (attributes.hasAttribute("tRed"))
        int x = 0;

    Color color(attributes.value("tRed").toShort(), attributes.value("tGreen").toShort(), attributes.value("tBlue").toShort());
    if (attributes.hasAttribute("tRed") && attributes.hasAttribute("tGreen") && attributes.hasAttribute("tBlue"))
    {
        Color color(attributes.value("tRed").toShort(), attributes.value("tGreen").toShort(), attributes.value("tBlue").toShort());
        setTransparencyColor(color);
    }
}

void TexturePrototype::toXML(QXmlStreamWriter& writer) const
{
    // if invalid, its default
    auto color = getTransparencyColor();
    if (color.isValid())
    {
        writer.writeAttribute("tRed", QString::number(color.getRed()));
        writer.writeAttribute("tGreen", QString::number(color.getGreen()));
        writer.writeAttribute("tBlue", QString::number(color.getBlue()));
    }

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
    if (getPassability())
        writer.writeAttribute("passability", QString::number(getPassability()));

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
    resizeTileSet(size);
    if (attributes.hasAttribute("tiles"))
    {
        QStringList tileList = attributes.value("tiles").toString().split(",");
        for (int32 i = 0; i < tileList.size(); ++i)
            setTileID(UInt32Point(i/size.getX(), i%size.getY()), tileList.at(i).toUInt());
    }
}

void TILE_SET::TileSetPrototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("columns", QString::number(getTileSetSize().getX()));
    writer.writeAttribute("rows", QString::number(getTileSetSize().getY()));
    Prototype::toXML(writer);

    //get tile IDs
    QString tiles;
    for (uint32 x = 0; x < getTileSetSize().getX(); ++x)
    {
        for (uint32 y = 0; y < getTileSetSize().getY(); ++y)
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
            writer.writeAttribute("x", QString::number(sprite.m_Pos.getX()));
            writer.writeAttribute("y", QString::number(sprite.m_Pos.getY()));
            if (sprite.m_Scale != 100)
                writer.writeAttribute("scale", QString::number(sprite.m_Scale));
            if (sprite.m_uiRotation)
                writer.writeAttribute("rotation", QString::number(sprite.m_uiRotation));
            if (sprite.m_Opacity != 100)
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
            newSprite.m_Pos.getX() = attributes.value("x").toInt();
            newSprite.m_Pos.getY() = attributes.value("y").toInt();
            if (attributes.hasAttribute("scale"))
                newSprite.m_Scale = attributes.value("scale").toFloat();

            newSprite.m_uiRotation = attributes.value("rotation").toUInt();

            if (attributes.hasAttribute("opacity"))
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
void WORLD_OBJECT::WorldObjectPrototype::fromXML(const QXmlStreamAttributes& attributes)
{
    Prototype::fromXML(attributes);

    // bounding rect
    setBoundingLeft(attributes.value("boundingX").toInt());
    setBoundingTop(attributes.value("boundingY").toInt());
    setBoundingRight(attributes.value("boundingWidth").toUInt());
    setBoundingBottom(attributes.value("boundingHeight").toUInt());

    setAnimationSpeed(attributes.value("animationSpeed").toUShort());
    setScriptName(attributes.value("script").toString());

	getFlagManager().setFlag(attributes.value("flags").toUInt());
}

void WORLD_OBJECT::WorldObjectPrototype::toXML(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("boundingX", QString::number(getBoundingLeft()));
    writer.writeAttribute("boundingY", QString::number(getBoundingTop()));
    writer.writeAttribute("boundingWidth", QString::number(getBoundingRight()));
    writer.writeAttribute("boundingHeight", QString::number(getBoundingBottom()));
    writer.writeAttribute("animationSpeed", QString::number(getAnimationSpeed()));
	writer.writeAttribute("flags", QString::number(getFlagManager().getFlag()));

    if (!getScriptName().isEmpty())
        writer.writeAttribute("script", getScriptName());
    Prototype::toXML(writer);

    //store animation infos
	if (getFlagManager().hasFlag(Flags::FLAG_ANIMATION))
	{
		for (uint32 i = 0; i < getAnimationCount(); ++i)
		{
			auto animationInfo = getAnimationModule().getAnimationInfo(i);
			if (animationInfo.m_ID == 0 && animationInfo.m_AnimationTypeID == 0)
				continue;
			writer.writeEmptyElement("animation");
			writer.writeAttribute("entry", QString::number(i));
			writer.writeAttribute("visualType", QString::number(static_cast<uint32>(animationInfo.m_VisualType)));
			writer.writeAttribute("ID", QString::number(animationInfo.m_ID));
			writer.writeAttribute("typeID", QString::number(animationInfo.m_AnimationTypeID));
		}
	}
}

void WORLD_OBJECT::WorldObjectPrototype::insertChildren(const QXmlStreamReader& reader)
{
    if (reader.name() == "animation")
    {
        QXmlStreamAttributes attributes = reader.attributes();
        if (!attributes.hasAttribute("entry"))
            return;

		getAnimationModule().setAnimationInfo(attributes.value("entry").toUInt(),
            MODULE::ANIMATION::AnimationInfo(
                attributes.value("ID").toUInt(),
				static_cast<MODULE::ANIMATION::VisualType>(attributes.value("visualType").toUInt()),
                attributes.value("typeID").toUInt()));
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
    writer.writeAttribute("sizeX", QString::number(getSize().getX()));
    writer.writeAttribute("sizeY", QString::number(getSize().getY()));
    writer.writeAttribute("backLayer", QString::number(getLayerSize(MAP::LayerType::LAYER_BACKGROUND)));
    writer.writeAttribute("foreLayer", QString::number(getLayerSize(MAP::LayerType::LAYER_FOREGROUND)));
    if (getParentID())
        writer.writeAttribute("parentID", QString::number(getParentID()));
    if (!getScriptName().isEmpty())
        writer.writeAttribute("script", getScriptName());
    writer.writeAttribute("file", getFileName());

    Prototype::toXML(writer);
}
