#pragma once

#include <QtCore/QXmlStreamWriter>
#include <QtCore/QFile>
#include "Prototype/XML_Output.h"

namespace database
{
    class XML_Output
    {
    public:
        template <class DATABASE>
        void write(const DATABASE& db, const QString& pathName)
        {
            QFile file(pathName + ".xml");
            file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
            QXmlStreamWriter writer(&file);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();
            writer.writeStartElement("DB");
            writer.writeAttribute("size", QString::number(db.getSize()));
            prototype::XML_Output protoWriter(writer);
            for (uint32 i = 1; i <= db.getSize(); ++i)
            {
                auto pPrototype = db.getPrototype(i);
                if (pPrototype && !pPrototype->isEmpty())
                    protoWriter.next(*pPrototype);
            }

            writer.writeEndElement();
            writer.writeEndDocument();
        }
    };
}
