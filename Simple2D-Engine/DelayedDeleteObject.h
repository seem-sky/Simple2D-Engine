#ifndef DELAYED_DELETE_OBJECT_H
#define DELAYED_DELETE_OBJECT_H

#include <QtCore/QObject>

// solution found on http://lists.qt.nokia.com/public/qt-interest/2009-December/016226.html

template <typename T>
class DelayedDeleteObject : public QObject
{
public:
    explicit DelayedDeleteObject(T* item) : m_item(item)
    {
        item = 0;
        deleteLater();
    }

    virtual ~DelayedDeleteObject()
    {
        delete m_item;
    }

private:
    T* m_item;
};

#endif
