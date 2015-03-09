#include "Database/Prototype/Derived.h"

using namespace database::prototype;

/*#####
# Resource
#####*/
Resource::Resource(uint32 ID)
    : Base(ID)
{}

bool Resource::isEmpty() const
{
    return getFileName().isEmpty() || Base::isEmpty();
}

void Resource::setFileName(const QString& fileName)
{
    m_FileName = fileName;
}

QString Resource::getFileName() const
{
    return m_FileName;
}

void Resource::setPath(const QString& path)
{
    m_Path = path;
}

QString Resource::getPath() const
{
    return m_Path;
}

QString Resource::getPathName() const
{
    return m_Path + "/" + m_FileName;
}


/*#####
# Texture
#####*/
Texture::Texture(uint32 ID)
    : Resource(ID)
{}

Color Texture::getTransparencyColor() const
{
    return m_TransparencyColor;
}

void Texture::setTransparencyColor(const Color& color)
{
    m_TransparencyColor = color;
}

/*#####
# Tile
#####*/
Tile::Tile(uint32 ID)
    : Texture(ID)
{}

Tile::PassabilityFlagMgr Tile::getPassability() const
{
    return m_Passability;
}

void Tile::setPassability(const PassabilityFlagMgr& flags)
{
    m_Passability = flags;
}

QString Tile::getBaseDirectory() const
{
    return "/Textures/";
}

/*#####
# TileSet
#####*/
const uint32 DEFAULT_COLUMN_COUNT = 2, DEFAULT_ROW_COUNT = 2;
TileSet::TileSet(uint32 uiID) : Base(uiID)
{
    clear();
}

void TileSet::resize(uint32 width, uint32 height)
{
    m_Tiles.resize(width, height);
}
void TileSet::resize(const GEOMETRY::Size<uint32>& size)
{
    m_Tiles.resize(size);
}

GEOMETRY::Size<uint32> TileSet::getSize() const
{
    return m_Tiles.getSize();
}

void TileSet::clear()
{
    // ToDo: replace resize(0, 0) with fill method.
    resize(0, 0);
    resize(DEFAULT_COLUMN_COUNT, DEFAULT_ROW_COUNT);
}

void TileSet::setTileID(const GEOMETRY::Point<uint32>& pos, uint32 ID)
{
    m_Tiles.at(pos) = ID;
}

uint32 TileSet::getTileID(const GEOMETRY::Point<uint32>& pos) const
{
    return m_Tiles.at(pos);
}


/*#####
# AutoTile
#####*/
AutoTile::AutoTile(uint32 ID)
    : Base(ID)
{}

void AutoTile::setTileID(Index index, DATABASE::PROTOTYPE::AUTO_TILE_INDEX ID)
{
    m_Tiles.at(static_cast<std::size_t>(index)) = ID;
}

DATABASE::PROTOTYPE::AUTO_TILE_INDEX AutoTile::getTileID(Index index) const
{
    return m_Tiles.at(static_cast<std::size_t>(index));
}

void AutoTile::setTiles(const TileArray& tiles)
{
    m_Tiles = tiles;
}

TileArray AutoTile::getTiles() const
{
    return m_Tiles;
}

// static
enum Requirements
{
    CENTER = AutoTile::SAME_AROUND,

    SIDE_LEFT = AutoTile::OTHER_LEFT,
    SIDE_RIGHT = AutoTile::OTHER_RIGHT,
    SIDE_TOP = AutoTile::OTHER_TOP,
    SIDE_BOTTOM = AutoTile::OTHER_BOTTOM,

    SIDE_VERTICAL = SIDE_LEFT | SIDE_RIGHT,
    SIDE_HORIZONTAL = SIDE_TOP | SIDE_BOTTOM,

    EDGE_TOP_LEFT = SIDE_TOP | SIDE_LEFT,
    EDGE_TOP_RIGHT = SIDE_TOP | SIDE_RIGHT,
    EDGE_BOTTOM_LEFT = SIDE_BOTTOM | SIDE_LEFT,
    EDGE_BOTTOM_RIGHT = SIDE_BOTTOM | SIDE_RIGHT,

    LEFT_END = EDGE_TOP_LEFT | EDGE_BOTTOM_LEFT,
    RIGHT_END = EDGE_TOP_RIGHT | EDGE_BOTTOM_RIGHT,
    TOP_END = EDGE_TOP_LEFT | EDGE_TOP_RIGHT,
    BOTTOM_END = EDGE_BOTTOM_LEFT | EDGE_BOTTOM_RIGHT,

    CIRCLE = EDGE_TOP_LEFT | EDGE_BOTTOM_RIGHT,

    INNER_EDGE_TOP_LEFT = AutoTile::OTHER_TOP_LEFT,
    INNER_EDGE_TOP_RIGHT = AutoTile::OTHER_TOP_RIGHT,
    INNER_EDGE_BOTTOM_LEFT = AutoTile::OTHER_BOTTOM_LEFT,
    INNER_EDGE_BOTTOM_RIGHT = AutoTile::OTHER_BOTTOM_RIGHT,

    INNER_EDGE_TOP_LEFT_TOP_RIGHT = INNER_EDGE_TOP_LEFT | INNER_EDGE_TOP_RIGHT,
    INNER_EDGE_TOP_LEFT_BOTTOM_LEFT = INNER_EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_LEFT,
    INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT = INNER_EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_RIGHT,
    INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT = INNER_EDGE_BOTTOM_LEFT | INNER_EDGE_BOTTOM_RIGHT,

    INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT = INNER_EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_RIGHT,
    INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT = INNER_EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,

    INNER_EDGE_CENTER = INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT,

    CURVE_TOP_LEFT = EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_RIGHT,
    CURVE_TOP_RIGHT = EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,
    CURVE_BOTTOM_LEFT = EDGE_BOTTOM_LEFT | INNER_EDGE_TOP_RIGHT,
    CURVE_BOTTOM_RIGHT = EDGE_BOTTOM_RIGHT | INNER_EDGE_TOP_LEFT,

    T_TOP = SIDE_TOP | INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT,
    T_BOTTOM = SIDE_BOTTOM | INNER_EDGE_TOP_LEFT_TOP_RIGHT,
    T_LEFT = SIDE_LEFT | INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT,
    T_RIGHT = SIDE_RIGHT | INNER_EDGE_TOP_LEFT_BOTTOM_LEFT,

    Y_TOP_BOTTOM_LEFT = SIDE_TOP | INNER_EDGE_BOTTOM_LEFT,
    Y_TOP_BOTTOM_RIGHT = SIDE_TOP | INNER_EDGE_BOTTOM_RIGHT,
    Y_LEFT_TOP_RIGHT = SIDE_LEFT | INNER_EDGE_TOP_RIGHT,
    Y_LEFT_BOTTOM_RIGHT = SIDE_LEFT | INNER_EDGE_BOTTOM_RIGHT,
    Y_BOTTOM_TOP_LEFT = SIDE_BOTTOM | INNER_EDGE_TOP_LEFT,
    Y_BOTTOM_TOP_RIGHT = SIDE_BOTTOM | INNER_EDGE_TOP_RIGHT,
    Y_RIGHT_TOP_LEFT = SIDE_RIGHT | INNER_EDGE_TOP_LEFT,
    Y_RIGHT_BOTTOM_LEFT = SIDE_RIGHT | INNER_EDGE_BOTTOM_LEFT,

    INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT = INNER_EDGE_TOP_LEFT_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,
    INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT = INNER_EDGE_TOP_LEFT_TOP_RIGHT | INNER_EDGE_BOTTOM_RIGHT,
    INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT = INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_LEFT,
    INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT = INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_RIGHT
};

bool hasRequirement(uint32 tileCheck, Requirements tiles)
{
    if ((tileCheck & tiles) == tiles)
        return true;
    return false;
}

AutoTile::Index AutoTile::getAutoTileIndexForTileCheck(uint32 tileCheck)
{
    // lonely tile
    if (hasRequirement(tileCheck, CIRCLE))
        return Index::Circle;

    // end sides
    if (hasRequirement(tileCheck, LEFT_END))
        return Index::SideEndLeft;
    if (hasRequirement(tileCheck, RIGHT_END))
        return Index::SideEndRight;
    if (hasRequirement(tileCheck, TOP_END))
        return Index::SideEndTop;
    if (hasRequirement(tileCheck, BOTTOM_END))
        return Index::SideEndBottom;

    // double sides
    if (hasRequirement(tileCheck, SIDE_VERTICAL))
        return Index::SideVertical;
    if (hasRequirement(tileCheck, SIDE_HORIZONTAL))
        return Index::SideHorizontal;

    // curves
    if (hasRequirement(tileCheck, CURVE_TOP_LEFT))
        return Index::CurveTopLeft;
    if (hasRequirement(tileCheck, CURVE_TOP_RIGHT))
        return Index::CurveTopRight;
    if (hasRequirement(tileCheck, CURVE_BOTTOM_LEFT))
        return Index::CurveBottomLeft;
    if (hasRequirement(tileCheck, CURVE_BOTTOM_RIGHT))
        return Index::CurveBottomRight;

    // T tiles
    if (hasRequirement(tileCheck, T_TOP))
        return Index::T_Top;
    if (hasRequirement(tileCheck, T_LEFT))
        return Index::T_Left;
    if (hasRequirement(tileCheck, T_RIGHT))
        return Index::T_Right;
    if (hasRequirement(tileCheck, T_BOTTOM))
        return Index::T_Bottom;

    // edges
    if (hasRequirement(tileCheck, EDGE_TOP_LEFT))
        return Index::TopLeft;
    if (hasRequirement(tileCheck, EDGE_TOP_RIGHT))
        return Index::TopRight;
    if (hasRequirement(tileCheck, EDGE_BOTTOM_LEFT))
        return Index::BottomLeft;
    if (hasRequirement(tileCheck, EDGE_BOTTOM_RIGHT))
        return Index::BottomRight;

    // Y tiles
    if (hasRequirement(tileCheck, Y_TOP_BOTTOM_LEFT))
        return Index::Y_TopBottomLeft;
    if (hasRequirement(tileCheck, Y_TOP_BOTTOM_RIGHT))
        return Index::Y_TopBottomRight;
    if (hasRequirement(tileCheck, Y_LEFT_TOP_RIGHT))
        return Index::Y_LeftTopRight;
    if (hasRequirement(tileCheck, Y_LEFT_BOTTOM_RIGHT))
        return Index::Y_LeftBottomRight;
    if (hasRequirement(tileCheck, Y_BOTTOM_TOP_LEFT))
        return Index::Y_BottomTopLeft;
    if (hasRequirement(tileCheck, Y_BOTTOM_TOP_RIGHT))
        return Index::Y_BottomTopRight;
    if (hasRequirement(tileCheck, Y_RIGHT_TOP_LEFT))
        return Index::Y_RightTopLeft;
    if (hasRequirement(tileCheck, Y_RIGHT_BOTTOM_LEFT))
        return Index::Y_RightBottomLeft;

    // like a star
    if (hasRequirement(tileCheck, INNER_EDGE_CENTER))
        return Index::InnerCenter;

    // triple inner edges
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT))
        return Index::InnerEdgeTopLeftTopRightBottomLeft;
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT))
        return Index::InnerEdgeTopLeftTopRightBottomRight;
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT))
        return Index::InnerEdgeTopLeftBottomLeftBottomRight;
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT))
        return Index::InnerEdgeTopRightBottomLeftBottomRight;

    // sides
    if (hasRequirement(tileCheck, SIDE_TOP))
        return Index::Top;
    if (hasRequirement(tileCheck, SIDE_LEFT))
        return Index::Left;
    if (hasRequirement(tileCheck, SIDE_RIGHT))
        return Index::Right;
    if (hasRequirement(tileCheck, SIDE_BOTTOM))
        return Index::Bottom;

    // diagonal inner edges
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT))
        return Index::InnerEdgeTopLeftBottomRight;
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT))
        return Index::InnerEdgeTopRightBottomLeft;

    // double inner edges
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_LEFT))
        return Index::InnerEdgeTopLeftBottomLeft;
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT))
        return Index::InnerEdgeTopLeftTopRight;
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT))
        return Index::InnerEdgeTopRightBottomRight;
    if (hasRequirement(tileCheck, INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT))
        return Index::InnerEdgeBottomLeftBottomRight;

    // single inner edges
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_LEFT))
        return Index::InnerEdgeTopLeft;
    if (hasRequirement(tileCheck, INNER_EDGE_TOP_RIGHT))
        return Index::InnerEdgeTopRight;
    if (hasRequirement(tileCheck, INNER_EDGE_BOTTOM_LEFT))
        return Index::InnerEdgeBottomLeft;
    if (hasRequirement(tileCheck, INNER_EDGE_BOTTOM_RIGHT))
        return Index::InnerEdgeBottomRight;

    // if nothing other matches return center
    return Index::Center;
}

/*#####
# Sprite
#####*/
Sprite::Sprite(uint32 ID)
    : Texture(ID)
{}

QString Sprite::getBaseDirectory() const
{
    return "/Textures/";
}

/*#####
# AnimationAdditionType
#####*/
AnimationAdditionType::AnimationAdditionType(uint32 ID)
    : Base(ID)
{}

/*#####
# AnimationAddition
#####*/
AnimationAddition::AnimationAddition(uint32 ID)
    : Base(ID)
{}

uint32 AnimationAddition::getAdditionType() const
{
    return m_AdditionTypeID;
}

void AnimationAddition::setAdditionType(uint32 type)
{
    m_AdditionTypeID = type;
}

uint32 AnimationAddition::getSize() const
{
    return static_cast<uint32>(m_Sprites.size());
}

void AnimationAddition::addSprite(const animation::SpriteData& sprite)
{
    m_Sprites.push_back(sprite);
}

animation::SpriteData AnimationAddition::getSprite(uint32 index) const
{
    return m_Sprites.at(index);
}

/*#####
# AnimationAdditionPoint
#####*/
AnimationAdditionPoint::AnimationAdditionPoint(uint32 ID)
    : AnimationAddition(ID)
{}

GEOMETRY::Point<int32> AnimationAdditionPoint::getPosition() const
{
    return m_Pos;
}

void AnimationAdditionPoint::setPosition(const GEOMETRY::Point<int32>& pos)
{
    m_Pos = pos;
}

int32 AnimationAdditionPoint::getX() const
{
    return m_Pos.getX();
}

int32 AnimationAdditionPoint::getY() const
{
    return m_Pos.getY();
}

void AnimationAdditionPoint::setX(int32 x)
{
    m_Pos.getX() = x;
}

void AnimationAdditionPoint::setY(int32 y)
{
    m_Pos.getY() = y;
}

/*#####
# AnimationAdditionRect
#####*/
AnimationAdditionRect::AnimationAdditionRect(uint32 ID)
    : AnimationAddition(ID)
{}

GEOMETRY::Rectangle<int32> AnimationAdditionRect::getRectangle() const
{
    return m_Rect;
}

void AnimationAdditionRect::setRectangle(const GEOMETRY::Rectangle<int32>& rect)
{
    m_Rect = rect;
}

/*#####
# Animation
#####*/
Animation::Animation(uint32 ID)
    : Base(ID)
{}

const animation::FrameData& Animation::getFrame(uint32 index) const
{
    return m_Frames.at(index);
}

void Animation::setFrame(uint32 index, const animation::FrameData& frame)
{
    m_Frames.at(index) = frame;
}

void Animation::removeFrame(uint32 index)
{
    m_Frames.erase(m_Frames.begin() + index);
}

void Animation::setFrameCount(uint32 count)
{
    m_Frames.resize(count);
}

uint32 Animation::getFrameCount() const
{
    return static_cast<uint32>(m_Frames.size());
}

const animation::FrameVector& Animation::getFrames() const
{
    return m_Frames;
}

void Animation::setFrames(const animation::FrameVector& frames)
{
    m_Frames = frames;
}

/*#####
# WorldObject
#####*/
// AnimationInfo
WorldObject::AnimationInfo::AnimationInfo(uint32 ID, VisualType visualType, uint32 animationTypeID)
    : m_ID(ID), m_VisualType(visualType), m_AnimationTypeID(animationTypeID)
{}

uint32 WorldObject::AnimationInfo::getAnimationID() const
{
    return m_ID;
}

void WorldObject::AnimationInfo::setAnimationID(uint32 ID)
{
    m_ID = ID;
}

uint32 WorldObject::AnimationInfo::getAnimationTypeID() const
{
    return m_AnimationTypeID;
}

void WorldObject::AnimationInfo::setAnimationTypeID(uint32 ID)
{
    m_AnimationTypeID = ID;
}

WorldObject::VisualType WorldObject::AnimationInfo::getVisualType() const
{
    return m_VisualType;
}

void WorldObject::AnimationInfo::setVisualType(VisualType type)
{
    m_VisualType = type;
}

bool WorldObject::AnimationInfo::isEmpty() const
{
    return m_ID == 0 || m_AnimationTypeID == 0;
}

// WorldObject
WorldObject::WorldObject(uint32 ID)
    : Base(ID)
{}

GEOMETRY::Rectangle<int32> WorldObject::getBoundingRect() const
{
    return m_BoundingRect;
}

void WorldObject::setBoundingRect(const GEOMETRY::Rectangle<int32>& rect)
{
    m_BoundingRect = rect;
}

void WorldObject::setAnimationSpeed(uint16 speed)
{
    m_AnimationSpeed = speed;
}

uint16 WorldObject::getAnimationSpeed() const
{
    return m_AnimationSpeed;
}

void WorldObject::setScriptName(const QString& scriptName)
{
    m_ScriptName = scriptName;
}

QString WorldObject::getScriptName() const
{
    return m_ScriptName;
}

WorldObject::AnimationInfoVector& WorldObject::getAnimations()
{
    return m_AnimationInfos;
}

const WorldObject::AnimationInfoVector& WorldObject::getAnimations() const
{
    return m_AnimationInfos;
}

/*#####
# Map
#####*/
Map::Map(uint32 ID)
    : Resource(ID)
{}

QString Map::getBaseDirectory() const
{
    return "/Map/";
}

void Map::setScriptName(const QString& sScriptName)
{
    m_ScriptName = sScriptName;
}
QString Map::getScriptName() const
{
    return m_ScriptName;
}

void Map::resize(uint32 width, uint32 height)
{
    m_Size.getWidth() = width;
    m_Size.getHeight() = height;
}

void Map::resize(const GEOMETRY::Size<uint32>& size)
{
    m_Size = size;
}

GEOMETRY::Size<uint32> Map::getSize() const
{
    return m_Size;
}

void Map::setLayerCount(uint8 number, MAP::LayerType layer)
{
    m_Layer.at(static_cast<uint32>(layer)) = number;
}

uint8 Map::getLayerCount(MAP::LayerType layer) const
{
    return m_Layer.at(static_cast<uint32>(layer));
}

uint32 Map::getParentID() const
{
    return m_ParentID;
}
void Map::setParentID(uint32 parentID)
{
    m_ParentID = parentID;
}
