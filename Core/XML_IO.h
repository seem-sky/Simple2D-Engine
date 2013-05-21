#ifndef XML_IO_H
#define XML_IO_H

#include "Global.h"
#include "Logfile.h"
#include <QtXml/QtXml>
#include <QtCore/QStringList>
#include <QtCore/QXmlStreamWriter>

namespace XML_IO
{
    class XMLReader
    {
    private:
        // if invalid nodelist, NULL node returns
        QDomNode changeRootNode(const QDomNode &parentNode, const QStringList &nodeNameList);
        virtual bool checkoutChildren(const QDomNode &parentNode) = 0;

    protected:
        QDomNode getSingleNode(const QDomNode &parentNode, const QString &nodeName);

    public:
        bool readFile(const QString &fileName, const QString &nodeName);
    };

    class XMLStreamWriter
    {
    private:
        virtual bool _writeChildren(QXmlStreamWriter &writer) = 0;

    public:
        bool writeFile(const QString &fileName, const QString &rootNode);
    };
}
#endif