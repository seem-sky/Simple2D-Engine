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

        void Brush::start(const UInt32Point& pos)
        {
            setPosition(pos);
            return start();
        }

        void Brush::start()
        {
            _checkValidation();
            MapTileInfoVec tiles, borderTiles;
            m_pArea->start(tiles, borderTiles);
            m_pType->setTiles(tiles);

            // check border tiles
            m_pType->setBorderTiles(borderTiles);

            // init RevertInfo
            
            m_RevertInfo.addTiles(tiles);
            m_RevertInfo.addTiles(borderTiles);
        }

        void Brush::setPosition(const UInt32Point& pos)
        {
            _checkValidation();
            m_pArea->setStartPosition(pos);
        }

        REVERT::BrushRevert Brush::getBrushRevert() const
        {
            return m_RevertInfo;
        }

        void Brush::resetRevertInfo()
        {
            m_RevertInfo.clear();
        }
    }
}
