#ifndef OBJECT_H
#define OBJECT_H

#include "Entity.h"
#include "TransformationHolder.h"

namespace MAP
{
    class Map;
    namespace OBJECT
    {
        class Object : public ENTITY::Entity
        {
        public:
            Object(uint32 GUID, uint32 ID = 0, const std::string &name = "", const std::string &script = "");

            inline Int32Point getPosition() const { return m_Position; }
            inline void setPosition(const Int32Point &newPos) { m_Position = newPos; }

            inline void setMap(Map *pMap) { m_pMap = pMap; }
            inline const Map* getMap() const { return m_pMap; }
            inline Map* getMap() { return m_pMap; }
            uint32 getMapGUID() const;

            virtual void update(uint32 uiDiff);

        private:
            Map *m_pMap;

        protected:
            Int32Point m_Position;
            TRANSFORMATION::TransformationHolder m_TransformationHolder;
        };
    }
}
#endif