#ifndef DATABASE_IO_H
#define DATABASE_IO_H

#include "Database.h"

namespace DATABASE
{
    namespace IO
    {
        template <class T>
        class DatabaseReader
        {
        public:
            DatabaseReader(T* pDB) : m_pDB(pDB)
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
                            std::unique_ptr<Prototype> pProto(m_pDB->getNewPrototype());
                            pProto->fromXML(reader.attributes());
                            // parse children
                            uint32 uiStartCounter = 0;
                            do
                            {
                                switch (reader.tokenType())
                                {
                                    case QXmlStreamReader::StartElement:
                                        ++uiStartCounter;
                                        pProto->insertChildren(reader);
                                        break;
                                    case QXmlStreamReader::EndElement:
                                        --uiStartCounter;
                                        break;
                                }
                                reader.readNext();
                            } while (uiStartCounter && !reader.hasError() && !reader.atEnd());

                            // store prototype
                            if (pProto->getID())
                                m_pDB->setPrototype(pProto.release());
                        }
                        break;
                    }
                }
            }

        private:
            T* m_pDB;
        };

        template <class T>
        class DatabaseWriter
        {
        public:
            DatabaseWriter(const T* pDB) : m_pDB(pDB)
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
                    if (auto pProto = m_pDB->getOriginalPrototype(i))
                    {
                        writer.writeStartElement("p");
                        pProto->toXML(writer);
                        writer.writeEndElement();
                    }
                }
                writer.writeEndElement();
                writer.writeEndDocument();
            }

        private:
            const T* m_pDB;
        };
    }
}
#endif
