#include "DatabasePrototypes.h"
#include "Core/Cache/Tiles.h"
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include "Config.h"

namespace DATABASE
{
    namespace PROTOTYPE
    {
		/*#####
		# Prototype
		#####*/

        /*#####
        # AutoTilePrototype
        #####*/
        namespace AUTO_TILE
        {
            bool hasTileCheck(uint32 uiTileCheck, AUTO_TILE_TILE_REQUIREMENTS tiles)
            {
                if ((uiTileCheck & tiles) == tiles)
                    return true;
                return false;
            }

            AUTO_TILE_INDEX getAutoTileIndexForTileCheck(uint32 uiTileCheck)
            {
                // lonely tile
                if (hasTileCheck(uiTileCheck, CIRCLE))
                    return INDEX_CIRCLE;

                // end sides
                if (hasTileCheck(uiTileCheck, LEFT_END))
                    return INDEX_SIDE_END_LEFT;
                if (hasTileCheck(uiTileCheck, RIGHT_END))
                    return INDEX_SIDE_END_RIGHT;
                if (hasTileCheck(uiTileCheck, TOP_END))
                    return INDEX_SIDE_END_TOP;
                if (hasTileCheck(uiTileCheck, BOTTOM_END))
                    return INDEX_SIDE_END_BOTTOM;

                // double sides
                if (hasTileCheck(uiTileCheck, SIDE_VERTICAL))
                    return INDEX_SIDE_VERTICAL;
                if (hasTileCheck(uiTileCheck, SIDE_HORIZONTAL))
                    return INDEX_SIDE_HORIZONTAL;

                // curves
                if (hasTileCheck(uiTileCheck, CURVE_TOP_LEFT))
                    return INDEX_CURVE_TOP_LEFT;
                if (hasTileCheck(uiTileCheck, CURVE_TOP_RIGHT))
                    return INDEX_CURVE_TOP_RIGHT;
                if (hasTileCheck(uiTileCheck, CURVE_BOTTOM_LEFT))
                    return INDEX_CURVE_BOTTOM_LEFT;
                if (hasTileCheck(uiTileCheck, CURVE_BOTTOM_RIGHT))
                    return INDEX_CURVE_BOTTOM_RIGHT;

                // T tiles
                if (hasTileCheck(uiTileCheck, T_TOP))
                    return INDEX_T_TOP;
                if (hasTileCheck(uiTileCheck, T_LEFT))
                    return INDEX_T_LEFT;
                if (hasTileCheck(uiTileCheck, T_RIGHT))
                    return INDEX_T_RIGHT;
                if (hasTileCheck(uiTileCheck, T_BOTTOM))
                    return INDEX_T_BOTTOM;

                // edges
                if (hasTileCheck(uiTileCheck, EDGE_TOP_LEFT))
                    return INDEX_TOP_LEFT;
                if (hasTileCheck(uiTileCheck, EDGE_TOP_RIGHT))
                    return INDEX_TOP_RIGHT;
                if (hasTileCheck(uiTileCheck, EDGE_BOTTOM_LEFT))
                    return INDEX_BOTTOM_LEFT;
                if (hasTileCheck(uiTileCheck, EDGE_BOTTOM_RIGHT))
                    return INDEX_BOTTOM_RIGHT;

                // Y tiles
                if (hasTileCheck(uiTileCheck, Y_TOP_BOTTOM_LEFT))
                    return INDEX_Y_TOP_BOTTOM_LEFT;
                if (hasTileCheck(uiTileCheck, Y_TOP_BOTTOM_RIGHT))
                    return INDEX_Y_TOP_BOTTOM_RIGHT;
                if (hasTileCheck(uiTileCheck, Y_LEFT_TOP_RIGHT))
                    return INDEX_Y_LEFT_TOP_RIGHT;
                if (hasTileCheck(uiTileCheck, Y_LEFT_BOTTOM_RIGHT))
                    return INDEX_Y_LEFT_BOTTOM_RIGHT;
                if (hasTileCheck(uiTileCheck, Y_BOTTOM_TOP_LEFT))
                    return INDEX_Y_BOTTOM_TOP_LEFT;
                if (hasTileCheck(uiTileCheck, Y_BOTTOM_TOP_RIGHT))
                    return INDEX_Y_BOTTOM_TOP_RIGHT;
                if (hasTileCheck(uiTileCheck, Y_RIGHT_TOP_LEFT))
                    return INDEX_Y_RIGHT_TOP_LEFT;
                if (hasTileCheck(uiTileCheck, Y_RIGHT_BOTTOM_LEFT))
                    return INDEX_Y_RIGHT_BOTTOM_LEFT;

                // like a star
                if (hasTileCheck(uiTileCheck, INNER_EDGE_CENTER))
                    return INDEX_INNER_CENTER;

                // triple inner edges
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT))
                    return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT))
                    return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT))
                    return INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT))
                    return INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT;

                // sides
                if (hasTileCheck(uiTileCheck, SIDE_TOP))
                    return INDEX_TOP;
                if (hasTileCheck(uiTileCheck, SIDE_LEFT))
                    return INDEX_LEFT;
                if (hasTileCheck(uiTileCheck, SIDE_RIGHT))
                    return INDEX_RIGHT;
                if (hasTileCheck(uiTileCheck, SIDE_BOTTOM))
                    return INDEX_BOTTOM;

                // diagonal inner edges
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT))
                    return INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT))
                    return INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT;

                // double inner edges
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_LEFT))
                    return INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT))
                    return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT))
                    return INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT))
                    return INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT;

                // single inner edges
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT))
                    return INDEX_INNER_EDGE_TOP_LEFT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_RIGHT))
                    return INDEX_INNER_EDGE_TOP_RIGHT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_BOTTOM_LEFT))
                    return INDEX_INNER_EDGE_BOTTOM_LEFT;
                if (hasTileCheck(uiTileCheck, INNER_EDGE_BOTTOM_RIGHT))
                    return INDEX_INNER_EDGE_BOTTOM_RIGHT;

                // if nothing other matches return center
                return INDEX_CENTER;
            }
        }

        /*#####
        # TileSetPrototype
        #####*/
        TILE_SET::TileSetPrototype::TileSetPrototype(uint32 uiID) : Prototype(uiID)
        {
            resizeTileSet(GEOMETRY::Point<uint32>(DEFAULT_COLUMN_COUNT, DEFAULT_ROW_COUNT));
        }

        void TILE_SET::TileSetPrototype::resizeTileSet(GEOMETRY::Point<uint32> size)
        {
            m_Tiles.resize(boost::extents[size.getX()][size.getY()]);
            m_Size = size;
        }

        void TILE_SET::TileSetPrototype::clear()
        {
            m_Tiles.resize(boost::extents[0][0]);
            m_Size = GEOMETRY::Point<uint32>(0, 0);
        }

        void TILE_SET::TileSetPrototype::setTileID(GEOMETRY::Point<uint32> pos, uint32 ID)
        {
            _resizeIfNeeded(GEOMETRY::Point<uint32>(pos.getX()+1, pos.getY()+1));
            m_Tiles[pos.getX()][pos.getY()] = ID;
        }

        uint32 TILE_SET::TileSetPrototype::getTileID(GEOMETRY::Point<uint32> pos) const
        {
            if (pos.getX() < m_Size.getX() && pos.getY() < m_Size.getY())
                return m_Tiles[pos.getX()][pos.getY()];
            return 0;
        }

        void TILE_SET::TileSetPrototype::_resizeIfNeeded(GEOMETRY::Point<uint32> size)
        {
            GEOMETRY::Point<uint32> newSize(m_Size);
            if (size.getX() > newSize.getX())
                newSize.getX() = size.getX();
            if (size.getY() > newSize.getY())
                newSize.getY() = size.getY();
            if (newSize != m_Size)
                resizeTileSet(newSize);
        }

        QPixmap TILE_SET::createPixmap(const TileSetPrototype& tileSet, CACHE::Tiles& tileCache)
        {
            QPixmap pixmap(tileSet.getTileSetSize().getX()*MAP::TILE_SIZE, tileSet.getTileSetSize().getY()*MAP::TILE_SIZE);
            pixmap.fill();
            QPainter painter(&pixmap);
            GEOMETRY::Point<uint32> pos;
            for (pos.getX() = 0; pos.getX() < tileSet.getTileSetSize().getX(); ++pos.getX())
            {
                for (pos.getY() = 0; pos.getY() < tileSet.getTileSetSize().getY(); ++pos.getY())
                {
                    auto info = tileCache.get(tileSet.getTileID(pos));
                    if (info.isValid())
                        painter.drawPixmap(pos.getX()*MAP::TILE_SIZE, pos.getY()*MAP::TILE_SIZE, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(),
                        MAP::TILE_SIZE, MAP::TILE_SIZE);
                }
            }
            return pixmap;
        }

        /*#####
        # AnimationPrototype
        #####*/
        namespace ANIMATION
        {
            bool AnimationPrototype::getFrame(uint32 uiIndex, Frame& frame) const
            {
                if (uiIndex < m_Frames.size())
                {
                    frame = m_Frames.at(uiIndex);
                    return true;
                }
                return false;
            }

            void AnimationPrototype::setFrame(uint32 uiIndex, Frame frame)
            {
                if (uiIndex >= m_Frames.size())
                    m_Frames.resize(uiIndex+1);
                m_Frames.at(uiIndex) = std::move(frame);
            }

            void AnimationPrototype::removeFrame(uint32 uiIndex)
            {
                if (uiIndex >= m_Frames.size())
                    return;
                if (uiIndex == m_Frames.size()-1)
                    m_Frames.resize(uiIndex);
                else
                    m_Frames.at(uiIndex) = Frame();
            }

            void Frame::calculateOffset()
            {
                m_FrameOffset.getX() = 0;
                m_FrameOffset.getY() = 0;
                for (auto& sprite : m_Sprites)
                {
                    if (sprite.m_Pos.getX() < m_FrameOffset.getX())
                        m_FrameOffset.getX() = sprite.m_Pos.getX();
                    if (sprite.m_Pos.getY() < m_FrameOffset.getY())
                        m_FrameOffset.getY() = sprite.m_Pos.getY();
                }
            }

            void Frame::setSprite(uint32 index, Sprite sprite)
            {
                if (index >= m_Sprites.size())
                    m_Sprites.resize(index+1);
                m_Sprites.at(index) = std::move(sprite);
                calculateOffset();
            }

            Sprite Frame::getSprite(uint32 index) const
            {
                if (index < m_Sprites.size())
                    return m_Sprites.at(index);
                return Sprite();
            }

            void Frame::addSprite(Sprite sprite)
            {
                m_Sprites.push_back(std::move(sprite));
                setOffsetIfNeeded(sprite.m_Pos);
            }

            void Frame::setOffsetIfNeeded(const GEOMETRY::Point<int32>& offset)
            {
                m_FrameOffset.getX() = std::min(m_FrameOffset.getX(), offset.getX());
                m_FrameOffset.getY() = std::min(m_FrameOffset.getY(), offset.getY());
            }
        }

        /*#####
        # WorldObjectPrototype
        #####*/

        /*#####
        # MapPrototype
        #####*/
        namespace MAP_STRUCTURE
        {
            MapPrototype::MapPrototype(uint32 ID, const QString& fileName) : Prototype(ID), m_FileName(fileName)
            {
                m_Layer.at(static_cast<uint32>(MAP::LayerType::LAYER_BACKGROUND)) = 1;
                m_Layer.at(static_cast<uint32>(MAP::LayerType::LAYER_FOREGROUND)) = 0;
            }

            bool MapPrototype::isValid()
            {
                return !m_FileName.isEmpty();
            }

            void MapPrototype::setSizeX(uint32 x)
            {
                setSize(GEOMETRY::Point<uint32>(x, getSize().getY()), getLayerSize(MAP::LayerType::LAYER_FOREGROUND), getLayerSize(MAP::LayerType::LAYER_BACKGROUND));
            }

            void MapPrototype::setSizeY(uint32 y)
            {
                setSize(GEOMETRY::Point<uint32>(getSize().getX(), y), getLayerSize(MAP::LayerType::LAYER_FOREGROUND), getLayerSize(MAP::LayerType::LAYER_BACKGROUND));
            }

            void MapPrototype::setLayerSize(uint8 size, MAP::LayerType layer)
            {
                setSize(getSize(), layer == MAP::LayerType::LAYER_FOREGROUND ? size : getLayerSize(MAP::LayerType::LAYER_FOREGROUND),
                    layer == MAP::LayerType::LAYER_BACKGROUND ? size : getLayerSize(MAP::LayerType::LAYER_BACKGROUND));
            }

            void MapPrototype::setSize(const GEOMETRY::Point<uint32>& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize)
            {
                m_Size = size;
                m_Layer.at(static_cast<uint32>(MAP::LayerType::LAYER_BACKGROUND)) = uiBackgroundLayerSize;
                m_Layer.at(static_cast<uint32>(MAP::LayerType::LAYER_FOREGROUND)) = uiForegroundLayerSize;
            }

            QString MapPrototype::getFilePath() const
            {
                return Config::get()->getProjectDirectory() + MAP_FOLDER + getFileName();
            }
        }
    }
}
