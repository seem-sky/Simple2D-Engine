#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "Global.h"
#include "Point.h"
#include <boost/smart_ptr.hpp>
#include "MapDatabase.h"
#include "XML_IO.h"

namespace MAP
{
    /*#####
    # MapReader
    #####*/
    class MapReader : public XML_IO::XMLReader
    {
    private:
        void _parseTileString(const Point<uint32> &pos, QString &tileString);
        void _loadTiles(const QDomNode &parentNode, bool &result);
        void _loadObjects(const QDomNode &parentNode, bool &result);
        bool checkoutChildren(const QDomNode &parentNode);

    public:
        MapReader(MapPrototypePtr &map) : m_pMap(map), m_State(STATE_NONE), XMLReader() {}

        //void loadMapThreaded(const QString &sFileName, bool &result);

    private:
        enum MapLoadState
        {
            STATE_NONE,
            STATE_IN_PROGRESS,
            STATE_DONE
        };

        MapLoadState m_State;

        MapPrototypePtr &m_pMap;
    };

    /*#####
    # MapWriter
    #####*/
    class MapWriter
    {
    private:
        //void _storeTiles(XML::XML_WriteData &xmlResult);
        //void _storeObjects(XML::XML_WriteData &xmlResult);

    public:
        MapWriter(MapPrototypePtr &map) : m_pMap(map){}

        void storeMapThreaded(const QString &sFileName, bool &result);

        //static void getXMLDataFromObject(const MapObjectPtr &obj, XML::XML_WriteData &xmlResult);

    private:
        MapPrototypePtr &m_pMap;
    };
};
#endif