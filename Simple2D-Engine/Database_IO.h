#ifndef DATABASE__IO_H
#define DATABASE__IO_H

#include "Database.h"
#include "PrototypeParser.h"

namespace DATABASE
{
    namespace IO
    {
        template <class T>
        class DatabaseReader
        {
        public:
            DatabaseReader(std::shared_ptr<Database<T>> pDB) : m_pDB(pDB)
            {}

            void read(QString filePath)
            {
                QFile file(filePath);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return;

                QXmlStreamReader reader(&file);
                while(!reader.atEnd() && !reader.hasError())
                {
                    switch (reader.readNext())
                    {
                    case QXmlStreamReader::ProcessingInstruction: reader.skipCurrentElement(); continue;
                    case QXmlStreamReader::StartDocument: continue;
                    case QXmlStreamReader::StartElement:
                        if (reader.name() == "p")
                        {
                            std::shared_ptr<T> pProto(new T());
                            PrototypeParser::parseFromXML(pProto, reader);
                            if (pProto->getID())
                                m_pDB->setItem(pProto->getID(), pProto);
                        }
                        break;
                    }
                }
            }

        private:
            std::shared_ptr<Database<T>> m_pDB;
        };

        template <class T>
        class DatabaseWriter
        {
        public:
            DatabaseWriter(std::shared_ptr<Database<T>> pDB) : m_pDB(pDB)
            {}

            void write(QString filePath)
            {
                QFile file(filePath);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;

                QXmlStreamWriter writer(&file);
                writer.setAutoFormatting(true);
                writer.writeStartDocument();
                writer.writeStartElement("DB");
                uint32 uiElementCount = m_pDB->getSize();
                for (uint32 i = 1; i <= uiElementCount; ++i)
                {
                    std::shared_ptr<T> pProto;
                    if (m_pDB->getItem(i, pProto))
                    {
                        writer.writeStartElement("p");
                        PrototypeParser::parseToXML(pProto, writer);
                        writer.writeEndElement();
                    }
                }
                writer.writeEndElement();
                writer.writeEndDocument();
            }

        private:
            std::shared_ptr<Database<T>> m_pDB;
        };
    }
}
#endif
