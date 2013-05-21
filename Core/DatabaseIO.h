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
        bool getAttributeFromXML(TilePrototypePtr proto, const QString &attributeName, const QString &attributeValue);

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
    # AutoTileDatabase
    ####*/
    class AutoTileDatabaseXMLReader : public DatabaseReader<AutoTilePrototype>
    {
    protected:
        bool getAttributeFromXML(AutoTilePrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        AutoTileDatabaseXMLReader(const AutoTileDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class AutoTileDatabaseXMLWriter : public DatabaseWriter<AutoTilePrototype>
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
        bool getChildrenFromXML(const QDomNode &node, AnimationPrototypePtr proto, const QString &childName);

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
        bool getAttributeFromXML(MAP::MapPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

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
    # TextDatabase
    ####*/
    class TextDatabaseXMLReader : public DatabaseReader<TextPrototype>
    {
    protected:
        bool getAttributeFromXML(TextPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        TextDatabaseXMLReader(const TextDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class TextDatabaseXMLWriter : public DatabaseWriter<TextPrototype>
    {
    protected:
        virtual void getXMLFromAttributes(TextPrototypePtr proto, QXmlStreamWriter &writer);

    public:
        TextDatabaseXMLWriter(const TextDatabasePtr &pDB) : DatabaseWriter(pDB) {}
    };

    /*####
    # WorldObjectDatabase
    ####*/
    class WorldObjectDatabaseXMLReader : public DatabaseReader<WorldObjectPrototype>
    {
    protected:
        bool getChildrenFromXML(const QDomNode &node, WorldObjectPrototypePtr proto, const QString &childName);
        bool getAttributeFromXML(WorldObjectPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

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