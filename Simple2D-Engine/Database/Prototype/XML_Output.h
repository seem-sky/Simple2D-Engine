#pragma once

#include <QtCore/QXmlStreamReader>
#include "Derived.h"

namespace database
{
    namespace prototype
    {
        class XML_Output
        {
        public:
            XML_Output(QXmlStreamWriter& writer);

            void next(const Tile& obj);
            void next(const AutoTile& obj);
            void next(const TileSet& obj);

            void next(const Sprite& obj);

            void next(const AnimationAdditionType& obj);
            void next(const AnimationAdditionPoint& obj);
            void next(const AnimationAdditionRect& obj);
            void next(const Animation& obj);

            void next(const WorldObject& obj);

            void next(const Map& obj);

        private:
            QXmlStreamWriter& m_Writer;
        };
    }
}