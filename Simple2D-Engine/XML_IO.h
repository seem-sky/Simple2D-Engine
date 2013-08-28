#ifndef XML_IO_H
#define XML_IO_H

#include <Global.h>
#include "Logfile.h"
#include <QtXml/QtXml>
#include <QtCore/QStringList>
#include <QtCore/QXmlStreamWriter>
#include <boost/thread.hpp>

namespace XML_IO
{
    typedef std::unique_ptr<boost::thread> ThreadPtr;
    enum XML_Result
    {
        NONE,
        IN_PROGRESS,
        DONE,
        FAILED
    };

    class XML
    {
    private:
        virtual void _execThreaded(const QString &fileName, const QString &nodeName, XML_Result &result) = 0;

    public:
        XML() : m_result(NONE) {}

        bool waitForSuccess();

        inline XML_Result getXMLResult() const { return m_result; }

        void execThreaded(const QString &fileName, const QString &nodeName);

        bool isThreaded() const;

    protected:
        XML_Result m_result;
        
    private:
        ThreadPtr m_pThread;
    };

    class XMLReader : public XML
    {
    private:
        // if invalid nodelist, NULL node returns
        QDomNode changeRootNode(const QDomNode &parentNode, const QStringList &nodeNameList);
        virtual bool checkoutChildren(const QDomNode &parentNode) = 0;

        void _execThreaded(const QString &fileName, const QString &nodeName, XML_Result &result);

    protected:
        QDomNode getSingleNode(const QDomNode &parentNode, const QString &nodeName);

    public:
        XMLReader() : XML() {}

        XML_Result readFile(const QString &fileName, const QString &nodeName);
    };

    class XMLStreamWriter : public XML
    {
    private:
        virtual bool _writeChildren(QXmlStreamWriter &writer) = 0;
        void _execThreaded(const QString &fileName, const QString &nodeName, XML_Result &result);

    public:
        XMLStreamWriter() : XML() {}

        virtual XML_Result writeFile(const QString &fileName, const QString &nodeName);
    };
}
#endif