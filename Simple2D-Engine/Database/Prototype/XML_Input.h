#pragma once

#include <QtCore/QXmlStreamReader>
#include "Derived.h"

namespace database
{
    namespace prototype
    {
        class XML_Input
        {
        public:
            XML_Input(QXmlStreamReader& reader);

            void next(Tile& obj);
            void next(AutoTile& obj);
            void next(TileSet& obj);

            void next(Sprite& obj);

            void next(AnimationAdditionType& obj);
            void next(AnimationAdditionPoint& obj);
            void next(AnimationAdditionRect& obj);
            void next(Animation& obj);

            void next(WorldObject& obj);

            void next(Map& obj);

        private:
            QXmlStreamReader& m_Reader;
        };
    }
}