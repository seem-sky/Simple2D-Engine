#include "Database/Prototype/XML_Output.h"

using namespace database::prototype;

// free functions
void write(QXmlStreamWriter& writer, const Base& obj)
{
    writer.writeAttribute("ID", QString::number(obj.getID()));
    if (!obj.getName().isEmpty())
        writer.writeAttribute("name", obj.getName());
}

void write(QXmlStreamWriter& writer, const Resource& obj)
{
    write(writer, static_cast<const Base&>(obj));

    writer.writeAttribute("file", obj.getFileName());
    if (!obj.getPath().isEmpty())
        writer.writeAttribute("path", obj.getPath());
}

void write(QXmlStreamWriter& writer, const Texture& obj)
{
    write(writer, static_cast<const Resource&>(obj));

    if (!obj.getTransparencyColor().isEmpty())
    {
        auto color = obj.getTransparencyColor();
        writer.writeAttribute("tRed", QString::number(color.getRed()));
        writer.writeAttribute("tGreen", QString::number(color.getGreen()));
        writer.writeAttribute("tBlue", QString::number(color.getBlue()));
    }
}

void write(QXmlStreamWriter& writer, const Tile& obj)
{
    write(writer, static_cast<const Texture&>(obj));

    if (obj.getPassability().getValue())
        writer.writeAttribute("passability", QString::number(obj.getPassability().getValue()));
}

void write(QXmlStreamWriter& writer, const TileSet& obj)
{
    write(writer, static_cast<const Base&>(obj));

    writer.writeAttribute("columns", QString::number(obj.getSize().getWidth()));
    writer.writeAttribute("rows", QString::number(obj.getSize().getHeight()));

    QString tiles;
    GEOMETRY::Point<uint32> pos;
    for (pos.getY() = 0; pos.getY() < obj.getSize().getHeight(); ++pos.getY())
    {
        for (pos.getX() = 0; pos.getX() < obj.getSize().getWidth(); ++pos.getX())
        {
            if (pos != pos.minimum())
                tiles.append(",");
            tiles.append(QString::number(obj.getTileID(pos)));
        }
    }
    writer.writeAttribute("tiles", tiles);
}

void write(QXmlStreamWriter& writer, const AutoTile& obj)
{
    write(writer, static_cast<const Base&>(obj));

    QString tiles;
    for (uint32 i = 0; i < DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_SET_COUNT; ++i)
    {
        if (i)
            tiles.append(",");
        tiles.append(QString::number(obj.getTileID(static_cast<AutoTile::Index>(i))));
    }
    writer.writeAttribute("tiles", tiles);
}

void write(QXmlStreamWriter& writer, const Sprite& obj)
{
    write(writer, static_cast<const Texture&>(obj));
}

void write(QXmlStreamWriter& writer, const Animation& obj)
{
    write(writer, static_cast<const Base&>(obj));
}

void write(QXmlStreamWriter& writer, const AnimationAdditionPoint& obj)
{
    write(writer, static_cast<const AnimationAddition&>(obj));
}

void write(QXmlStreamWriter& writer, const AnimationAdditionRect& obj)
{
    write(writer, static_cast<const AnimationAddition&>(obj));
}

void write(QXmlStreamWriter& writer, const AnimationAdditionType& obj)
{
    write(writer, static_cast<const Base&>(obj));
}

void write(QXmlStreamWriter& writer, const Map& obj)
{
    write(writer, static_cast<const Resource&>(obj));

    writer.writeAttribute("width", QString::number(obj.getSize().getWidth()));
    writer.writeAttribute("height", QString::number(obj.getSize().getHeight()));
    writer.writeAttribute("layerF", QString::number(obj.getLayerCount(MAP::LayerType::LAYER_FOREGROUND)));
    writer.writeAttribute("layerB", QString::number(obj.getLayerCount(MAP::LayerType::LAYER_BACKGROUND)));

    writer.writeAttribute("parentID", QString::number(obj.getParentID()));
    writer.writeAttribute("script", obj.getScriptName());
}

void writeChildren(QXmlStreamWriter& writer, const Base& obj)
{}

template <class PROTOTYPE>
void startWriteChildren(QXmlStreamWriter& writer, const PROTOTYPE& obj)
{
    writeChildren(writer, obj);
}

template <class PROTOTYPE>
void startWriting(QXmlStreamWriter& writer, const PROTOTYPE& obj)
{
    writer.writeStartElement("p");
    write(writer, obj);
    writeChildren(writer, obj);
    writer.writeEndElement();
}

// methods
XML_Output::XML_Output(QXmlStreamWriter& writer)
    : m_Writer(writer)
{}

void XML_Output::next(const Tile& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const AutoTile& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const TileSet& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const Sprite& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const AnimationAdditionType& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const AnimationAdditionPoint& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const AnimationAdditionRect& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const Animation& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const WorldObject& obj)
{
    startWriting(m_Writer, obj);
}

void XML_Output::next(const Map& obj)
{
    startWriting(m_Writer, obj);
}
