#pragma once

#include <QtCore/QXmlStreamReader>
#include <QtCore/QFile>
#include <memory>
#include "Prototype/XML_Input.h"
#include "Base.h"

namespace database
{
    class XML_Input
    {
    public:
        template <class T, uint32 size>
        void read(Base<T, size>& db, const QString& pathName)
        {
            QFile file(pathName + ".xml");
            if (!file.exists())
                return;
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QXmlStreamReader reader(&file);
            prototype::XML_Input objReader(reader);
            while (reader.readNext() != QXmlStreamReader::EndDocument)
            {
                switch (reader.tokenType())
                {
                case QXmlStreamReader::ProcessingInstruction: reader.skipCurrentElement(); continue;
                case QXmlStreamReader::StartElement:
                    if (reader.name() == "DB")
                        db.setSize(reader.attributes().value("size").toUInt());

                    else if (reader.name() == "p")
                    {
                        std::unique_ptr<T> pObj(new T());
                        objReader.next(*pObj.get());
                        if (!pObj->isEmpty() && db.getSize() >= pObj->getID())
                            db.setPrototype(pObj.release());
                    }
                    break;
                }
            }
        }
    };
}
