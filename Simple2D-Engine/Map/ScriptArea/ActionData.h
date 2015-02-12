#ifndef MAP_SCRIPT_AREA_ACTION_DATA_H
#define MAP_SCRIPT_AREA_ACTION_DATA_H

#include <QtCore/QString>
#include <Global.h>
#include <geometry/Point.h>

namespace MAP
{
    namespace SCRIPT_AREA
    {
        namespace ACTION
        {
            enum class Type
            {
                none,
                teleport,
                custom_script
            };

            class Data
            {
            public:
                Data() = default;
                Data(const QString& scriptName);
                Data(uint32 mapID, const GEOMETRY::Point<int32>& pos);

                Type getType() const;

                uint32 getMapID() const;
                const GEOMETRY::Point<int32>& getPosition() const;

                QString getScriptName() const;

            private:
                Type m_Type = Type::none;

                // type teleport
                uint32 m_MapID = 0;
                GEOMETRY::Point<int32> m_Position;

                // type custom script
                QString m_ScriptName;
            };
        }
    }
}
#endif
