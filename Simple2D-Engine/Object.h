#ifndef OBJECT_H
#define OBJECT_H

#include "Global.h"
#include "TransformationHolder.h"

namespace ENTITY
{
    class Object
    {
    protected:
        inline void setID(uint32 uiID) { m_uiID = uiID;}

    public:
        Object(uint32 GUID, uint32 uiID = 0, const std::string &name = "");

        inline Int32Point getPosition() const { return m_Position; }
        inline void setPosition(const Int32Point &newPos) { m_Position = newPos; }
        void move(const Int32Point &range, uint32 time);

        inline void setScript(const std::string &script) { m_Script = script; }
        inline std::string getScript() const { return m_Script; }

        virtual void update(uint32 uiDiff);

        inline void setName(const std::string &name) { m_Name = name; }

        inline uint32 getGUID() const { return m_uiGUID; }

    private:
        uint32 m_uiID;
        std::string m_Name;
        std::string m_Script;
        Int32Point m_Position;
        uint32 m_uiGUID;
        TRANSFORMATION::TransformationHolder m_TransformationHolder;
    };
}
#endif