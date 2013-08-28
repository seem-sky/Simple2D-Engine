#ifndef PROTOTYPE_PARSER_H
#define PROTOTYPE_PARSER_H

#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#include "DatabasePrototypes.h"

namespace DATABASE
{
    class PrototypeParser
    {
    protected:
        template <typename TIndex>
        inline static void parseAttributesFromXML(std::shared_ptr<Prototype<TIndex>> pProto, QXmlStreamAttributes &attributes)
        {
            pProto->setID(attributes.value("ID").toUInt());
            pProto->setName(attributes.value("name").toString());
        }

        template <typename TIndex>
        inline static void parseAttributesFromXML(std::shared_ptr<TexturePrototype<TIndex>> pProto, QXmlStreamAttributes &attributes)
        {
            parseAttributesFromXML<TIndex>(std::static_pointer_cast<Prototype<SPRITE_INDEX>>(pProto), attributes);

            pProto->setFileName(attributes.value("file").toString());
            pProto->setPath(attributes.value("path").toString());
            pProto->setTransparencyColor(Color(attributes.value("transparency_color").toString().toStdString()));
        }

        inline static void parseAttributesFromXML(AnimationPrototypePtr pProto, QXmlStreamAttributes &attributes)
        {
            parseAttributesFromXML<ANIMATION_INDEX>(std::static_pointer_cast<Prototype<ANIMATION_INDEX>>(pProto), attributes);
        }

        inline static void parseAttributesFromXML(ObjectAnimationTypePrototypePtr pProto, QXmlStreamAttributes &attributes)
        {
            parseAttributesFromXML<ANIMATION_TYPE_INDEX>(std::static_pointer_cast<Prototype<ANIMATION_TYPE_INDEX>>(pProto), attributes);
        }

        inline static void parseAttributesFromXML(LocalisationPrototypePtr pProto, QXmlStreamAttributes &attributes)
        {
            parseAttributesFromXML<LOCALISATION_INDEX>(std::static_pointer_cast<Prototype<LOCALISATION_INDEX>>(pProto), attributes);
        }

        static void parseAttributesFromXML(SpritePrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(TilePrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(TileSetPrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(AutoTilePrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(MAP_STRUCTURE::MapPrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(WorldObjectPrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(DynamicObjectPrototypePtr pProto, QXmlStreamAttributes &attributes);


        // children
        inline static void parseChildrenFromXML(TilePrototypePtr pProto, const QXmlStreamReader &reader) {}
        inline static void parseChildrenFromXML(TileSetPrototypePtr pProto, const QXmlStreamReader &reader) {}
        inline static void parseChildrenFromXML(AutoTilePrototypePtr pProto, const QXmlStreamReader &reader) {}
        inline static void parseChildrenFromXML(SpritePrototypePtr pProto, const QXmlStreamReader &reader) {}
        inline static void parseChildrenFromXML(ObjectAnimationTypePrototypePtr pProto, const QXmlStreamReader &reader) {}
        inline static void parseChildrenFromXML(MAP_STRUCTURE::MapPrototypePtr pProto, const QXmlStreamReader &reader) {}

        static void parseChildrenFromXML(AnimationPrototypePtr pProto, const QXmlStreamReader &reader);
        static void parseChildrenFromXML(LocalisationPrototypePtr pProto, const QXmlStreamReader &reader);
        static void parseChildrenFromXML(WorldObjectPrototypePtr pProto, const QXmlStreamReader &reader);

    public:
        // writing
        template <typename TIndex>
        static void parseToXML(std::shared_ptr<Prototype<TIndex>> pProto,  QXmlStreamWriter &writer)
        {
            if (!pProto)
                return;

            writer.writeAttribute("ID", QString::number(pProto->getID()));
            writer.writeAttribute("name", pProto->getName());
        }

        template <typename TIndex>
        static void parseToXML(TexturePrototypePtr pProto, QXmlStreamWriter &writer)
        {
            if (!pProto)
                return;
            parseToXML(std::static_pointer_cast<Prototype<TIndex>>(pProto), writer);

            writer.writeAttribute("file", pProto->getFileName());
            writer.writeAttribute("path", pProto->getPath());
            writer.writeAttribute("transparency_color", QString::fromStdString(pProto->getTransparencyColor().getColorString()));
        }

        inline static void parseToXML(SpritePrototypePtr pProto, QXmlStreamWriter &writer)
        {
            parseToXML<SPRITE_INDEX>(std::static_pointer_cast<TexturePrototype<SPRITE_INDEX>>(pProto), writer);
        }

        inline static void parseToXML(ObjectAnimationTypePrototypePtr pProto, QXmlStreamWriter &writer)
        {
            parseToXML<ANIMATION_TYPE_INDEX>(std::static_pointer_cast<Prototype<ANIMATION_TYPE_INDEX>>(pProto), writer);
        }

        static void parseToXML(TilePrototypePtr pProto, QXmlStreamWriter &writer);
        static void parseToXML(TileSetPrototypePtr pProto, QXmlStreamWriter &writer);
        static void parseToXML(AutoTilePrototypePtr pProto, QXmlStreamWriter &writer);
        static void parseToXML(AnimationPrototypePtr pProto, QXmlStreamWriter &writer);
        static void parseToXML(MAP_STRUCTURE::MapPrototypePtr pProto, QXmlStreamWriter &writer);
        static void parseToXML(LocalisationPrototypePtr pProto, QXmlStreamWriter &writer);
        static void parseToXML(WorldObjectPrototypePtr pProto, QXmlStreamWriter &writer);
        static void parseToXML(DynamicObjectPrototypePtr proto, QXmlStreamWriter &writer);

        // reading
        template<class T>
        static void parseFromXML(std::shared_ptr<T> pProto, QXmlStreamReader &reader)
        {
            if (!pProto)
                return;
            parseAttributesFromXML(pProto, reader.attributes());
            
            // parse children
            uint32 uiStartCounter = 0;
            do
            {
                switch (reader.tokenType())
                {
                    case QXmlStreamReader::StartElement:
                        ++uiStartCounter;
                        parseChildrenFromXML(pProto, reader);
                        break;
                    case QXmlStreamReader::EndElement:
                        --uiStartCounter;
                        break;
                }
                reader.readNext();
            } while (uiStartCounter && !reader.hasError() && !reader.atEnd());
        }
    };
}
#endif