#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "Global.h"
#include "Point.h"
#include <boost/smart_ptr.hpp>
#include "MapDatabase.h"
#include "XML_IO.h"

namespace DATABASE
{
    namespace MAP_STRUCTURE
    {
        /*#####
        # MapReader
        #####*/
        class MapReader : public XML_IO::XMLReader
        {
        private:
            void _parseTileString(const UInt32Point &pos, const QString &tileString, Layer layer);
            void _loadTiles(const QDomNode &parentNode, Layer layer);
            void _loadTiles(const QDomNode &parentNode, bool &result);

            void _loadObjects(const QDomNode &parentNode, bool &result);
            bool checkoutChildren(const QDomNode &parentNode);

        public:
            MapReader(MapPrototypePtr &map) : m_pMap(map), XMLReader() {}

        private:
            MapPrototypePtr &m_pMap;
        };

        /*#####
        # MapWriter
        #####*/
        class MapWriter : public XML_IO::XMLStreamWriter
        {
        private:
            bool _writeChildren(QXmlStreamWriter &writer);
            void _storeTiles(QXmlStreamWriter &writer, Layer layer);
            void _storeTiles(QXmlStreamWriter &writer);
            void _storeObjects(QXmlStreamWriter &writer);
            void _getXMLDataFromObject(ConstMapObjectPtr obj, QXmlStreamWriter &writer);

        public:
            MapWriter(MapPrototypePtr &map) : XMLStreamWriter(), m_pMap(map){}

        private:
            MapPrototypePtr &m_pMap;
        };
    }
}
#endif