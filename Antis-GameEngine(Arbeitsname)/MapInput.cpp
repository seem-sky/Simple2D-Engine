#include "MapInput.h"
#include "xmlParser.h"

MapLoadResult MapInput::CreateMapFromFile(MapInfo &MapInfo, MapTiles &vMapTiles, std::string sMapName)
{
    // search for the file, if it exists
    FILE *Data;
    fopen_s(&Data, ("Maps/" + sMapName).c_str(), "r");
    if(!Data)
        return MAP_LOAD_NO_FILE;

    fclose(Data);

    // this open and parse the XML file:
    XMLNode xMainNode = XMLNode::openFileHelper(("Maps/" + sMapName).c_str());

    // read tiles
    XMLNode xNode = xMainNode.getChildNode("MapInfo").getChildNode("MapData");
    int n = xNode.nChildNode("MapTiles");
    unsigned int XTiles = 0;
    unsigned int YTiles = 0;
    for (int i = 0; i < n; i++)
    {
        // contiune if there are no "Tiles" set
        if (!xNode.getChildNode("MapTiles", i).isAttributeSet("Tiles"))
            continue;

        std::string sTileString = xNode.getChildNode("MapTiles", i).getAttribute("Tiles");
        std::vector<unsigned int> vTempTiles;
        while(!sTileString.empty())
        {
            // if first sign is a number
            if (sTileString.at(0) <= 57 && sTileString.at(0) >= 48)
            {
                // search string for first space sign, and substr
                std::string sRemoveString = sTileString.substr(0, sTileString.find_first_of(32)).c_str();
                sTileString.erase(sTileString.find_first_of(sRemoveString), sRemoveString.length());
                vTempTiles.push_back(atoi(sRemoveString.c_str()));
                XTiles++;
            }
            // if first sign is a not a number, delete it
            else 
                sTileString.erase(0, 1);

        }
        // if vTempTiles has Tiles stored, there is a new line
        if (vTempTiles.size())
            YTiles++;

        vMapTiles.push_back(vTempTiles);
    }
    if (!vMapTiles.size())
        return MAP_LOAD_CORRUPT_FILE;

    xNode = xMainNode.getChildNode("MapInfo");
    // read map name and map announce name
    if (xNode.isAttributeSet("MapName"))
        MapInfo.m_sMapName = xNode.getAttribute("MapName");
    else
        return MAP_LOAD_CORRUPT_FILE;

    if (xNode.isAttributeSet("MapAnnounceName"))
        MapInfo.m_sMapAnnounceName = xNode.getAttribute("MapAnnounceName");
    else
        return MAP_LOAD_CORRUPT_FILE;

    // store map size
    if (YTiles)
        MapInfo.m_uiX = XTiles / YTiles;
    MapInfo.m_uiY = YTiles;

    return MAP_LOAD_OK;
}
