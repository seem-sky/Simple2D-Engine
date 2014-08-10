#ifndef ENTITY_H
#define ENTITY_H

#include <Global.h>
#include <string>

namespace ENTITY
{
    class Entity
    {
    public:
        Entity(uint32 GUID, uint32 ID = 0, const std::string& name = "", const std::string& script = "");

        inline void setScript(const std::string& script) { m_Script = script; }
        inline const std::string& getScript() const { return m_Script; }

        inline void setName(const std::string& name) { m_Name = name; }
        inline const std::string& getName() const { return m_Name; }

        inline uint32 getGUID() const { return m_uiGUID; }

        inline void setID(uint32 uiID) { m_uiID = uiID;}
        inline uint32 getID() const { return m_uiID; }

    private:
        uint32 m_uiID;
        std::string m_Name;
        std::string m_Script;
        uint32 m_uiGUID;
    };
}
#endif