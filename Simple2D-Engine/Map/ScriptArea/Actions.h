#ifndef SCRIPT_AREA_ACTION_ACTIONS_H
#define SCRIPT_AREA_ACTION_ACTIONS_H

#include "ActionInterface.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        namespace ACTION
        {
            class Teleport : public Interface
            {
                Teleport() = default;
                Teleport(uint32 mapID, const GEOMETRY::Point<int32>& pos);

                void setMapID(uint32 ID);
                uint32 getMapID() const;
                void setPosition(const GEOMETRY::Point<int32>& pos);
                GEOMETRY::Point<int32> getPosition() const;

                Type getType() const;
                Data getData() const;

            private:
                uint32 m_MapID = 0;
                GEOMETRY::Point<int32> m_Position;
            };

            class CustomScript : public Interface
            {
            public:
                CustomScript() = default;
                CustomScript(const QString& scriptName);

                void setScriptName(const QString& script);
                const QString& getScriptName() const;

                Type getType() const;
                Data getData() const;

            private:
                QString m_ScriptName;
            };
        }
    }
}
#endif
