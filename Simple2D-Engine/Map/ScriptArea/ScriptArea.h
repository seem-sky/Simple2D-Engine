#ifndef MAP_SCRIPT_AREA_SCRIPT_AREA_H
#define MAP_SCRIPT_AREA_SCRIPT_AREA_H

#include <memory>
#include <Simple2D-Global.h>
#include <geometry/ComplexGeometricShape.h>
#include <Typedefs.h>
#include "ActionInterface.h"
#include "AreaInterface.h"
#include "Data.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        const uint32 MINIMUM_SIZE = TILE_SIZE/2;

        enum class TriggerType
        {
            touch,
            contain
        };

        enum class RepeatType
        {
            single,
            stepwise
        };

        class ScriptArea
        {
        public:
            ScriptArea(MAP::GUID guid);
            ScriptArea(const ScriptArea& other) = delete;
            ScriptArea& operator=(const ScriptArea& other) = delete;

            void setArea(const AREA::Data& data);
            AREA::Interface* getArea();
            const AREA::Interface* getArea() const;

            void setAction(const ACTION::Data& data);
            ACTION::Interface* getAction();
            const ACTION::Interface* getAction() const;

            Data getData() const;

            GUID getGUID() const { return m_GUID; }

        private:
            GUID m_GUID = 0;
            std::unique_ptr<AREA::Interface> m_pArea;

            RepeatType m_RepeatType;
            TriggerType m_TriggerType;
            std::unique_ptr<ACTION::Interface> m_pAction;
        };

        namespace AREA
        {
            Interface* create(const Data& data);
        }

        namespace ACTION
        {
            Interface* create(const Data& data);
        }
    }
}
#endif
