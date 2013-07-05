#ifndef OBJECT_H
#define OBJECT_H

#include "Global.h"
#include "TransformationHolder.h"

namespace ENTITY
{
    class Object
    {
    public:
        Object(uint32 uiID, const std::string &name);

        inline Int32Point getPosition() const { return m_Position; }
        inline void setPosition(const Int32Point &newPos) { m_Position = newPos; }
        void move(const Int32Point &range, uint32 time);

        virtual void update(uint32 uiDiff);

    private:
        uint32 m_uiID;
        std::string m_Name;
        Int32Point m_Position;
        TRANSFORMATION::TransformationHolder m_TransformationHolder;
    };
}
#endif