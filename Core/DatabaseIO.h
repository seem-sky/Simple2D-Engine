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
    protected:
        virtual bool getAttributeFromXML(TilePrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        TileDatabaseXMLReader(const TileDatabasePtr &pDB) : TextureDatabaseReader(pDB) {}
    };

    class TileDatabaseXMLWriter : public TextureDatabaseWriter<TilePrototype>
    {
    protected:
        virtual void getXMLFromAttributes(TilePrototypePtr proto, QXmlStreamWriter &writer);

    public:
        TileDatabaseXMLWriter(const TileDatabasePtr &pDB) : TextureDatabaseWriter(pDB) {}
    };

    /*####
    # TileSetDatabase
    ####*/
    class TileSetDatabaseXMLReader : public DatabaseReader<TILE_SET::TileSetPrototype>
    {
    protected:
        virtual bool getAttributeFromXML(TileSetPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        TileSetDatabaseXMLReader(const TileSetDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class TileSetDatabaseXMLWriter : public DatabaseWriter<TILE_SET::TileSetPrototype>
    {
    protected:
        virtual void getXMLFromAttributes(TileSetPrototypePtr proto, QXmlStreamWriter &writer);

    public:
        TileSetDatabaseXMLWriter(const TileSetDatabasePtr &pDB) : DatabaseWriter(pDB) {}
    };

    /*####
    # AutoTileDatabase
    ####*/
    class AutoTileDatabaseXMLReader : public DatabaseReader<AUTO_TILE::AutoTilePrototype>
    {
    protected:
        virtual bool getAttributeFromXML(AutoTilePrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        AutoTileDatabaseXMLReader(const AutoTileDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class AutoTileDatabaseXMLWriter : public DatabaseWriter<AUTO_TILE::AutoTilePrototype>
    {
    protected:
        virtual void getXMLFromAttributes(AutoTilePrototypePtr proto, QXmlStreamWriter &writer);

    public:
        AutoTileDatabaseXMLWriter(const AutoTileDatabasePtr &pDB) : DatabaseWriter(pDB) {}
    };

    /*####
    # SpriteDatabase
    ####*/
    typedef TextureDatabaseReader<SpritePrototype> SpriteDatabaseXMLReader;
    typedef TextureDatabaseWriter<SpritePrototype> SpriteDatabaseXMLWriter;

    /*####
    # AnimationDatabase
    ####*/
    class AnimationDatabaseXMLReader : public DatabaseReader<AnimationPrototype>
    {
    private:
        bool _getFrameAttribute(DATABASE::AnimationPrototype::Frame &frame, const QString &nodeName, const QString &nodeValue);
        bool _getSpriteAttribute(DATABASE::AnimationPrototype::Sprite &sprite, const QString &nodeName, const QString &nodeValue);

    protected:
        virtual bool getChildrenFromXML(const QDomNode &node, AnimationPrototypePtr proto, const QString &childName);

    public:
        AnimationDatabaseXMLReader(const AnimationDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class AnimationDatabaseXMLWriter : public DatabaseWriter<AnimationPrototype>
    {
    protected:
        virtual void getXMLFromAttributes(AnimationPrototypePtr proto, QXmlStreamWriter &writer);

    public:
        AnimationDatabaseXMLWriter(const AnimationDatabasePtr &pDB) : DatabaseWriter(pDB) {}
    };

    /*####
    # MapDatabase
    ####*/
    class MapDatabaseXMLReader : public DatabaseReader<MAP::MapPrototype>
    {
    protected:
        virtual bool getAttributeFromXML(MAP::MapPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        MapDatabaseXMLReader(const MapDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class MapDatabaseXMLWriter : public DatabaseWriter<MAP::MapPrototype>
    {
    protected:
        virtual void getXMLFromAttributes(MAP::MapPrototypePtr proto, QXmlStreamWriter &writer);

    public:
        MapDatabaseXMLWriter(const MapDatabasePtr &pDB) : DatabaseWriter(pDB) {}
    };

    /*####
    # LocalsDatabase
    ####*/
    class LocalsDatabaseXMLReader : public DatabaseReader<LocalisationPrototype>
    {
    protected:
        virtual bool getAttributeFromXML(LocalisationPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        LocalsDatabaseXMLReader(const LocalsDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class LocalsDatabaseXMLWriter : public DatabaseWriter<LocalisationPrototype>
    {
    protected:
        virtual void getXMLFromAttributes(LocalisationPrototypePtr proto, QXmlStreamWriter &writer);

    public:
        LocalsDatabaseXMLWriter(const LocalsDatabasePtr &pDB) : DatabaseWriter(pDB) {}
    };

    /*####
    # WorldObjectDatabase
    ####*/
    class WorldObjectDatabaseXMLReader : public DatabaseReader<WorldObjectPrototype>
    {
    protected:
        virtual bool getChildrenFromXML(const QDomNode &node, WorldObjectPrototypePtr proto, const QString &childName);
        virtual bool getAttributeFromXML(WorldObjectPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        WorldObjectDatabaseXMLReader(const WorldObjectDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class WorldObjectDatabaseXMLWriter : public DatabaseWriter<WorldObjectPrototype>
    {
    protected:
        virtual void getXMLFromAttributes(WorldObjectPrototypePtr proto, QXmlStreamWriter &writer);

    public:
        WorldObjectDatabaseXMLWriter(const WorldObjectDatabasePtr &pDB) : DatabaseWriter(pDB) {}
    };

    /*####
    # ObjectAnimationTypeDatabase
    ####*/
    typedef DatabaseReader<ObjectAnimationTypePrototype> ObjectAnimationTypeDatabaseXMLReader;
    typedef DatabaseWriter<ObjectAnimationTypePrototype> ObjectAnimationTypeDatabaseXMLWriter;
}
#endif