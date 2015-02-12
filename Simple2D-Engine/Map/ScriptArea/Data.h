#ifndef MAP_SCRIPT_AREA_DATA_H
#define MAP_SCRIPT_AREA_DATA_H

#include "AreaData.h"
#include "ActionData.h"
#include "Simple2D-Global.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        class Data
        {
        public:
            Data() = default;
            Data(GUID guid, const AREA::Data& areaData, const ACTION::Data& actionData);

            const AREA::Data& getAreaData() const;
            const ACTION::Data& getActionData() const;
            GUID getGUID() const;

            bool isValid() const;

        private:
            GUID m_Guid = 0;
            AREA::Data m_AreaData;
            ACTION::Data m_ActionData;
        };
    }
}

// IO
QDataStream& operator<<(QDataStream& stream, const MAP::SCRIPT_AREA::Data& data);
QDataStream& operator>>(QDataStream& stream, MAP::SCRIPT_AREA::Data& data);

#endif
