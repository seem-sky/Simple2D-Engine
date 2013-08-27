#ifndef PROTOTYPE_PARSER_H
#define PROTOTYPE_PARSER_H

#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#include "DatabasePrototypes.h"

namespace DATABASE
{
    class PrototypeParser
    {
        // NOTE: Not every prototype needs this functions. Overload only if needed.
    protected:
        static void parseAttributesFromXML(PrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(TexturePrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(SpritePrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(TilePrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(TileSetPrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(AutoTilePrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(MAP_STRUCTURE::MapPrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(WorldObjectPrototypePtr pProto, QXmlStreamAttributes &attributes);
        static void parseAttributesFromXML(DynamicObjectPrototypePtr pProto, QXmlStreamAttributes &attributes);

        inline static void parseChildrenFromXML(PrototypePtr pProto, const QXmlStreamReader &reader) {}
        static void parseChildrenFromXML(AnimationPrototypePtr pProto, const QXmlStreamReader &reader);
        static void parseChildrenFromXML(LocalisationPrototypePtr pProto, const QXmlStreamReader &reader);
        static void parseChildrenFromXML(WorldObjectPrototypePtr pProto, const QXmlStreamReader &reader);

    public:
        // writing
        static void parseToXML(PrototypePtr pProto, QXmlStreamWriter &writer);
        static void parseToXML(TexturePrototypePtr pProto, QXmlStreamWriter &writer);
        inline static void parseToXML(SpritePrototypePtr pProto, QXmlStreamWriter &writer) { parseToXML(std::static_pointer_cast<TexturePrototype>(pProto), writer); }
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