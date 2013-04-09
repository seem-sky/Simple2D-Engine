#ifndef DATABASE_IO_H
#define DATABASE_IO_H

#include "MapDatabase.h"
#include "DatabaseIOTemplates.h"

namespace DATABASE
{
    /*####
    # TileDatabase
    ####*/
    class TileDatabaseXMLReader : public TextureDatabaseReader<TilePrototype>
    {
    private:
        void _getPrototypeFromXML(const IXMLDOMNodePtr &pParentNode);

    protected:
        bool getAttributeFromXML(TilePrototypePtr proto, const std::string sAttributeName, CComVariant value);

    public:
        TileDatabaseXMLReader(const TileDatabasePtr &pDB) : TextureDatabaseReader(pDB) {}
        bool readFile(const std::string &sFileName, const bool threaded = false);
    };

    class TileDatabaseXMLWriter : public TextureDatabaseWriter<TilePrototype>
    {
    private:
        std::string _getNodeName() const { return "Tile"; }

    protected:
        virtual void getXMLFromAttributes(TilePrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode);

    public:
        TileDatabaseXMLWriter(const TileDatabasePtr &pDB) : TextureDatabaseWriter(pDB) {}
        bool writeFile(const std::string &sFileName, const bool threaded = false);
    };

    /*####
    # AutoTileDatabase
    ####*/
    class AutoTileDatabaseXMLReader : public DatabaseReader<AutoTilePrototype>
    {
    private:
        void _getPrototypeFromXML(const IXMLDOMNodePtr &pParentNode);

    protected:
        bool getAttributeFromXML(AutoTilePrototypePtr proto, const std::string sAttributeName, CComVariant value);

    public:
        AutoTileDatabaseXMLReader(const AutoTileDatabasePtr &pDB) : DatabaseReader(pDB) {}
        bool readFile(const std::string &sFileName, const bool threaded = false);
    };

    class AutoTileDatabaseXMLWriter : public DatabaseWriter<AutoTilePrototype>
    {
    private:
        std::string _getNodeName() const { return "AutoTile"; }

    protected:
        virtual void getXMLFromAttributes(AutoTilePrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode);

    public:
        AutoTileDatabaseXMLWriter(const AutoTileDatabasePtr &pDB) : DatabaseWriter(pDB) {}
        bool writeFile(const std::string &sFileName, const bool threaded = false);
    };

    /*####
    # SpriteDatabase
    ####*/
    class SpriteDatabaseXMLReader : public TextureDatabaseReader<SpritePrototype>
    {
    private:
        void _getPrototypeFromXML(const IXMLDOMNodePtr &pParentNode);

    protected:
        bool getAttributeFromXML(SpritePrototypePtr proto, const std::string sAttributeName, CComVariant value);

    public:
        SpriteDatabaseXMLReader(const SpriteDatabasePtr &pDB) : TextureDatabaseReader(pDB) {}
        bool readFile(const std::string &sFileName, const bool threaded = false);
    };

    class SpriteDatabaseXMLWriter : public TextureDatabaseWriter<SpritePrototype>
    {
    private:
        std::string _getNodeName() const { return "Sprite"; }

    protected:
        virtual void getXMLFromAttributes(SpritePrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode);

    public:
        SpriteDatabaseXMLWriter(const SpriteDatabasePtr &pDB) : TextureDatabaseWriter(pDB) {}
        bool writeFile(const std::string &sFileName, const bool threaded = false);
    };

    /*####
    # MapDatabase
    ####*/
    class MapDatabaseXMLReader : public DatabaseReader<MAP::MapPrototype>
    {
    private:
        void _getPrototypeFromXML(const IXMLDOMNodePtr &pParentNode);

    protected:
        bool getAttributeFromXML(MAP::MapPrototypePtr proto, const std::string sAttributeName, CComVariant value);

    public:
        MapDatabaseXMLReader(const MapDatabasePtr &pDB) : DatabaseReader(pDB) {}
        bool readFile(const std::string &sFileName, const bool threaded = false);
    };

    class MapDatabaseXMLWriter : public DatabaseWriter<MAP::MapPrototype>
    {
    private:
        std::string _getNodeName() const { return "Map"; }

    protected:
        virtual void getXMLFromAttributes(MAP::MapPrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode);

    public:
        MapDatabaseXMLWriter(const MapDatabasePtr &pDB) : DatabaseWriter(pDB) {}
        bool writeFile(const std::string &sFileName, const bool threaded = false);
    };

    /*####
    # TextDatabase
    ####*/
    class TextDatabaseXMLReader : public DatabaseReader<TextPrototype>
    {
    private:
        void _getPrototypeFromXML(const IXMLDOMNodePtr &pParentNode);

    protected:
        bool getAttributeFromXML(TextPrototypePtr proto, const std::string sAttributeName, CComVariant value);

    public:
        TextDatabaseXMLReader(const TextDatabasePtr &pDB) : DatabaseReader(pDB) {}
        bool readFile(const std::string &sFileName, const bool threaded = false);
    };

    class TextDatabaseXMLWriter : public DatabaseWriter<TextPrototype>
    {
    private:
        std::string _getNodeName() const { return "Text"; }

    protected:
        virtual void getXMLFromAttributes(TextPrototypePtr proto, MSXML2::IXMLDOMNodePtr &pNewNode);

    public:
        TextDatabaseXMLWriter(const TextDatabasePtr &pDB) : DatabaseWriter(pDB) {}
        bool writeFile(const std::string &sFileName, const bool threaded = false);
    };
}
#endif