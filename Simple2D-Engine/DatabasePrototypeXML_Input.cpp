#include "Database/Prototype/XML_Input.h"
#include <QtCore/QStringList>

using namespace database::prototype;

// free functions
void read(Base& obj, const QXmlStreamAttributes& attributes)
{
    obj.setName(attributes.value("name").toString());
}

void read(Resource& obj, const QXmlStreamAttributes& attributes)
{
    obj.setFileName(attributes.value("file").toString());
    obj.setPath(attributes.value("path").toString());
    read(static_cast<Base&>(obj), attributes);
}

void read(Texture& obj, const QXmlStreamAttributes& attributes)
{
    if (attributes.hasAttribute("tRed") && attributes.hasAttribute("tGreen") && attributes.hasAttribute("tBlue"))
    {
        Color color(attributes.value("tRed").toShort(), attributes.value("tGreen").toShort(), attributes.value("tBlue").toShort());
        obj.setTransparencyColor(color);
    }
    read(static_cast<Resource&>(obj), attributes);
}

void read(Tile& obj, const QXmlStreamAttributes& attributes)
{
    auto flagMgr = obj.getPassability();
    flagMgr.setFlag(attributes.value("passability").toUInt());
    obj.setPassability(flagMgr);
    read(static_cast<Texture&>(obj), attributes);
}

void read(AutoTile& obj, const QXmlStreamAttributes& attributes)
{
    if (attributes.hasAttribute("tiles"))
    {
        QStringList tileList = attributes.value("tiles").toString().split(",");
        for (int32 i = 0; i < tileList.size(); ++i)
            obj.setTileID(static_cast<AutoTile::Index>(i), tileList.at(i).toUInt());
    }
    read(static_cast<Base&>(obj), attributes);
}

void read(TileSet& obj, const QXmlStreamAttributes& attributes)
{
    GEOMETRY::Size<uint32> size(attributes.value("columns").toUInt(), attributes.value("rows").toUInt());
    obj.resize(size);
    if (attributes.hasAttribute("tiles"))
    {
        QStringList tileList = attributes.value("tiles").toString().split(",");
        for (int32 i = 0; i < tileList.size(); ++i)
            obj.setTileID(GEOMETRY::Point<uint32>(i % size.getWidth(), i / size.getWidth()), tileList.at(i).toUInt());
    }
    read(static_cast<Base&>(obj), attributes);
}

void read(AnimationAddition& obj, const QXmlStreamAttributes& attributes)
{
    read(static_cast<Base&>(obj), attributes);
}

void read(AnimationAdditionPoint& obj, const QXmlStreamAttributes& attributes)
{
    read(static_cast<AnimationAddition&>(obj), attributes);
}

void read(AnimationAdditionRect& obj, const QXmlStreamAttributes& attributes)
{
    read(static_cast<AnimationAddition&>(obj), attributes);
}

void read(WorldObject& obj, const QXmlStreamAttributes& attributes)
{
    // bounding rect
    obj.setBoundingRect(GEOMETRY::Rectangle<int32>(attributes.value("boundingX").toInt(), attributes.value("boundingY").toInt(),
        attributes.value("boundingWidth").toUInt(), attributes.value("boundingHeight").toUInt()));

    obj.setAnimationSpeed(attributes.value("animationSpeed").toUShort());
    obj.setScriptName(attributes.value("script").toString());

    read(static_cast<Base&>(obj), attributes);
}

void read(Map& obj, const QXmlStreamAttributes& attributes)
{
    // setup map size
    obj.resize(attributes.value("width").toUInt(), attributes.value("height").toUInt());
    obj.setLayerCount(attributes.value("layerF").toUInt(), MAP::LayerType::LAYER_FOREGROUND);
    obj.setLayerCount(attributes.value("layerB").toUInt(), MAP::LayerType::LAYER_BACKGROUND);

    obj.setParentID(attributes.value("parentID").toUInt());
    obj.setScriptName(attributes.value("script").toString());
    read(static_cast<Resource&>(obj), attributes);
}

// template functions
template <class T>
void startReading(T& obj, QXmlStreamReader& reader)
{
    if (reader.tokenType() != QXmlStreamReader::StartElement)
        throw std::ios::failure("Bad tokenType.");

    auto attributes = reader.attributes();
    if (!attributes.hasAttribute("ID"))
        throw std::ios::failure("Bad tokenType.");
    obj = T(attributes.value("ID").toUInt());
    read(obj, attributes);
}

template <class T>
void readChildren(T& obj, QXmlStreamReader& reader)
{
    int32 count = 0;
    do
    {
        switch (reader.readNext())
        {
        case QXmlStreamReader::StartElement:
            ++count;
            readChildren(obj, reader);
            break;
        case QXmlStreamReader::EndElement:
            --count;
            break;
        }
    } while (count > 0);
}

/*#####
# XML_Input
#####*/
XML_Input::XML_Input(QXmlStreamReader& reader)
    : m_Reader(reader)
{}

void XML_Input::next(Tile& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(AutoTile& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(TileSet& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(Sprite& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(AnimationAdditionType& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(AnimationAdditionPoint& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(AnimationAdditionRect& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(Animation& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(WorldObject& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}

void XML_Input::next(Map& obj)
{
    startReading(obj, m_Reader);
    readChildren(obj, m_Reader);
}
