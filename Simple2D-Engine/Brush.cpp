#include "Brush.h"
#include "BrushException.h"
#include "BrushRevert.h"

namespace MAP
{
    namespace BRUSH
    {
        Brush::Brush(AREA::Interface* pArea, TYPE::Interface* pType) : m_pArea(AREA::AreaPtr(pArea)), m_pType(TYPE::TypePtr(pType))
        {
        }

        void Brush::_checkValidation() const
        {
            if (!m_pArea)
                throw EXCEPTION::InvalidArea();

            if (!m_pType)
                throw EXCEPTION::InvalidType();
        }

        void Brush::setArea(AREA::Interface* pArea)
        {
            m_pArea = AREA::AreaPtr(pArea);
        }

        void Brush::setType(TYPE::Interface* pType)
        {
            m_pType = TYPE::TypePtr(pType);
        }

        void Brush::start(const GEOMETRY::Point<uint32>& pos)
        {
            setPosition(pos);
            return start();
        }

        void Brush::start()
        {
            _checkValidation();
            MapTileInfoVec tiles;
            PointVec<uint32> borderTiles;
            m_pArea->start(tiles, borderTiles);
            m_pType->setTiles(tiles, m_RevertInfo);

            // check border tiles
            m_pType->setBorderTiles(borderTiles, m_RevertInfo);
        }

        void Brush::setPosition(const GEOMETRY::Point<uint32>& pos)
        {
            _checkValidation();
            m_pArea->setStartPosition(pos);
            m_pType->setStartPosition(pos);
        }

        REVERT::BrushRevert Brush::getBrushRevert() const
        {
            return m_RevertInfo;
        }

        void Brush::resetRevertInfo()
        {
            m_RevertInfo.clear();
        }

        void Brush::setBrushSize(const GEOMETRY::Point<uint32>& size)
        {
            m_pArea->setBrushSize(size);
        }
    }
}
