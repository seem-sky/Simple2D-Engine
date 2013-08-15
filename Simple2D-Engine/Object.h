#ifndef OBJECT_H
#define OBJECT_H

#include "Entity.h"
#include "TransformationHolder.h"

namespace MAP
{
    namespace OBJECT
    {
        class Object : public ENTITY::Entity
        {
        public:
            Object(uint32 GUID, uint32 ID = 0, const std::string &name = "", const std::string &script = "");

            inline Int32Point getPosition() const { return m_Position; }
            inline void setPosition(const Int32Point &newPos) { m_Position = newPos; }
            void move(const Int32Point &range, uint32 time);

            inline void setMapGUID(uint32 MapGUID) { m_MapGUID = MapGUID; }
            inline uint32 getMapGUID() const { return m_MapGUID; }

            virtual void update(uint32 uiDiff);

        private:
            Int32Point m_Position;
            TRANSFORMATION::TransformationHolder m_TransformationHolder;
            uint32 m_MapGUID;
        };
    }
}
#endif