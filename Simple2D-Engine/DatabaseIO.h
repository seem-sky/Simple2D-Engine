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
    class MapDatabaseXMLReader : public DatabaseReader<MAP_STRUCTURE::MapPrototype>
    {
    protected:
        virtual bool getAttributeFromXML(MAP_STRUCTURE::MapPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        MapDatabaseXMLReader(const MapDatabasePtr &pDB) : DatabaseReader(pDB) {}
    };

    class MapDatabaseXMLWriter : public DatabaseWriter<MAP_STRUCTURE::MapPrototype>
    {
    protected:
        virtual void getXMLFromAttributes(MAP_STRUCTURE::MapPrototypePtr proto, QXmlStreamWriter &writer);

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
    # ObjectDatabase
    ####*/
    template <class T>
    class ObjectDatabaseXMLReader : public DatabaseReader<T>
    {
    protected:
        virtual bool getChildrenFromXML(const QDomNode &node, std::shared_ptr<T> proto, const QString &childName)
        {
            if (node.isNull() || !proto || childName.isEmpty())
                return false;

            if (DatabaseReader::getChildrenFromXML(node, proto, childName))
                return true;

            // checkout animation info
            if (childName == "AnimationInfo")
            {
                MAP_OBJECT::AnimationInfo newAnimationInfo;
                QDomNamedNodeMap attributeList = node.attributes();
                // get attributes
                for (int32 i = 0; i < attributeList.length(); ++i)
                {
                    QDomNode tempNode = attributeList.item(i);
                    if (tempNode.nodeName() == "animationID")
                        newAnimationInfo.m_uiAnimationID = tempNode.nodeValue().toUInt();
                    else if (tempNode.nodeName() == "animationTypeID")
                        newAnimationInfo.m_uiObjectAnimationTypeID = tempNode.nodeValue().toUInt();
                }
                proto->setAnimationInfo(newAnimationInfo.m_uiObjectAnimationTypeID, newAnimationInfo);
                return true;
            }
            return false;
        }

        virtual bool getAttributeFromXML(std::shared_ptr<T> proto, const QString &attributeName, const QString &attributeValue)
        {
            if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
                return false;
            if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
                return true;

            if (attributeName == "boundingX")
            {
                proto->setBoundingX(attributeValue.toUInt());
                return true;
            }
            else if (attributeName == "boundingY")
            {
                proto->setBoundingY(attributeValue.toUInt());
                return true;
            }
            else if (attributeName == "boundingWidth")
            {
                proto->setBoundingWidth(attributeValue.toUInt());
                return true;
            }
            else if (attributeName == "boundingHeight")
            {
                proto->setBoundingHeight(attributeValue.toUInt());
                return true;
            }
            else if (attributeName == "animationSpeed")
            {
                proto->setAnimationSpeed(attributeValue.toUShort());
                return true;
            }
            else if (attributeName == "scriptName")
            {
                proto->setScriptName(attributeValue);
                return true;
            }
            return false;
        }

    public:
        ObjectDatabaseXMLReader(const std::shared_ptr<Database<T>> &pDB) : DatabaseReader(pDB) {}
    };

    template <class T>
    class ObjectDatabaseXMLWriter : public DatabaseWriter<T>
    {
    protected:
        virtual void getXMLFromAttributes(std::shared_ptr<T> proto , QXmlStreamWriter &writer)
        {
            if (!proto)
                return;
            DatabaseWriter::getXMLFromAttributes(proto, writer);
            writer.writeAttribute("boundingX", QString::number(proto->getBoundingX()));
            writer.writeAttribute("boundingY", QString::number(proto->getBoundingY()));
            writer.writeAttribute("boundingWidth", QString::number(proto->getBoundingWidth()));
            writer.writeAttribute("boundingHeight", QString::number(proto->getBoundingHeight()));
            writer.writeAttribute("animationSpeed", QString::number(proto->getAnimationSpeed()));
            writer.writeAttribute("scriptName", proto->getScriptName());

            // store animation infos
            for (uint32 i = 1; i <= proto->getAnimationCount(); ++i)
            {
                MAP_OBJECT::AnimationInfo animationInfo = proto->getAnimationInfo(i);
                if (animationInfo.m_uiAnimationID == 0 && animationInfo.m_uiObjectAnimationTypeID == 0)
                    continue;
                writer.writeEmptyElement("AnimationInfo");
                writer.writeAttribute("animationID", QString::number(animationInfo.m_uiAnimationID));
                writer.writeAttribute("animationTypeID", QString::number(animationInfo.m_uiObjectAnimationTypeID));
            }
        }

    public:
        ObjectDatabaseXMLWriter(const std::shared_ptr<Database<T>> &pDB) : DatabaseWriter(pDB) {}
    };

    /*####
    # WorldObjectDatabase
    ####*/
    typedef DatabaseReader<MAP_OBJECT::WorldObjectPrototype> WorldObjectDatabaseXMLReader;
    typedef DatabaseWriter<MAP_OBJECT::WorldObjectPrototype> WorldObjectDatabaseXMLWriter;

    /*####
    # DynamicObjectDatabase
    ####*/
    class DynamicObjectDatabaseXMLReader : public ObjectDatabaseXMLReader<MAP_OBJECT::DynamicObjectPrototype>
    {
    protected:
        virtual bool getAttributeFromXML(DynamicObjectPrototypePtr proto, const QString &attributeName, const QString &attributeValue);

    public:
        DynamicObjectDatabaseXMLReader(const DynamicObjectDatabasePtr &pDB) : ObjectDatabaseXMLReader(pDB) {}
    };

    class DynamicObjectDatabaseXMLWriter : public ObjectDatabaseXMLWriter<MAP_OBJECT::DynamicObjectPrototype>
    {
    protected:
        virtual void getXMLFromAttributes(DynamicObjectPrototypePtr proto, QXmlStreamWriter &writer);

    public:
        DynamicObjectDatabaseXMLWriter(const DynamicObjectDatabasePtr &pDB) : ObjectDatabaseXMLWriter(pDB) {}
    };

    /*####
    # ObjectAnimationTypeDatabase
    ####*/
    typedef DatabaseReader<ObjectAnimationTypePrototype> ObjectAnimationTypeDatabaseXMLReader;
    typedef DatabaseWriter<ObjectAnimationTypePrototype> ObjectAnimationTypeDatabaseXMLWriter;
}
#endif