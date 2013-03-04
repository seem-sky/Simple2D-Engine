#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "Global.h"
#include "Point.h"
#include <boost/smart_ptr.hpp>
#include "MapDatabase.h"
#include "XML_Writer.h"

namespace MAP
{
    /*#####
    # MapReader
    #####*/
    class MapReader
    {
    private:
        void _loadTiles(bool &result, const std::string &sFileName = "", bool threaded = false);
        void _loadObjects(bool &result, const std::string &sFileName = "", bool threaded = false);

    public:
        MapReader(MapPrototypePtr &map) : m_pMap(map), m_State(STATE_NONE) {}

        void loadMapThreaded(const std::string &sFileName, bool &result);

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
        void _storeTiles(XML::XML_WriteData &xmlResult);
        void _storeObjects(XML::XML_WriteData &xmlResult);

    public:
        MapWriter(MapPrototypePtr &map) : m_pMap(map){}

        void storeMapThreaded(const std::string &sFileName, bool &result);

        static void getXMLDataFromObject(const MapObjectPtr &obj, XML::XML_WriteData &xmlResult);

    private:
        MapPrototypePtr &m_pMap;
    };
};
#endif