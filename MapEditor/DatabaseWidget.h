#ifndef DATABASE_WIDGET_H
#define DATABASE_WIDGET_H

#include "UI/UI_DatabaseWidget.h"
#include "Global.h"
#include "DatabaseChanger.h"
#include <QtGui/QMessageBox>
#include "QtGlobal.h"
#include "ModifyObject.h"

// slots for child class, paste them after "private slots:"
//void _resizeButtonClicked() { _resizeDatabase(getListCountValue()); }
//void _currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) { currentItemChanged(current); }
//void _changeName() { changeName(); }

class DatabaseWidgetObject : public QWidget
{
public:
    DatabaseWidgetObject(QWidget *pParent) : QWidget(pParent) {}
    virtual void setFocus() {}      // this method is called when widget is selected (use e.g. to update some widgets)
};

template <class T>
class DatabaseWidget : public DatabaseWidgetObject, protected Ui_DatabaseWidget
{
private:
    void _updateWidget()
    {
        UInt32StdStringMap protoInfo;
        m_pDBChanger->getPrototypeShortInfos(protoInfo);
        fillListWidget(protoInfo);
    }

protected:
    void resizeEvent(QResizeEvent *pEvent) { m_ModObj.resizeEvent(this); }
    virtual void connectWidgets()
    {
        connect(m_pResizeList, SIGNAL(clicked()), this, SLOT(_resizeButtonClicked()));
        connect(m_pList, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(_currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
        connect(m_pName, SIGNAL(editingFinished()), this, SLOT(_changeName()));
    }

    void changeName()
    {
        updateName(m_pList->currentItem(), m_pName->text());
    }

    void setName(const QString &sName)
    {
        m_pName->setText(sName);
        changeName();
    }

    virtual void resizeDatabase(uint32 uiSize)
    {
        if (uiSize == m_pDBChanger->getDBSize())
            return;

        if (uiSize < m_pDBChanger->getDBSize())
        {
            if (QMessageBox::Yes != QMessageBox::question(0, "Delete " + QString::number(m_pDBChanger->getDBSize() - uiSize) + "entrys.",
                "Do you really want to delete entry " + QString::number(uiSize+1) + " up to end?", QMessageBox::Yes | QMessageBox::No))
                return;
        }
        m_pDBChanger->resizeDB(uiSize);

        change();
        _updateWidget();
    }

    virtual void changeCurrentItem(uint32 uiID)
    {
        boost::shared_ptr<T> proto;
        if (!m_pDBChanger->getPrototype(uiID, proto))
            return;

        setWidgetsFromPrototype(proto);
    }

    virtual void clearWidgets()
    {
        m_pID->clear();
        m_pName->clear();
    }

    virtual bool getPrototypeFromWidgets(boost::shared_ptr<T> &proto)
    {
        if (!proto)
            return false;

        proto->setID(m_pID->value());
        proto->setName(m_pName->text().toStdString());
        return true;
    }

    virtual bool setWidgetsFromPrototype(const boost::shared_ptr<T> &proto)
    {
        if (!proto)
            return false;

        m_pID->setValue(proto->getID());
        m_pName->setText(QString::fromStdString(proto->getName()));
        return true;
    }

    void currentItemChanged(QTreeWidgetItem *current)
    {
        clearWidgets();
        if (current)
            changeCurrentItem(current->text(0).toUInt());
    }

    void updateItem()
    {
        boost::shared_ptr<T> proto(new T());
        if (!getPrototypeFromWidgets(proto))
            return;
        m_pDBChanger->setPrototype(proto->getID(), proto);
        change();
    }

    virtual void fillListWidget(const UInt32StdStringMap &sStrings)
    {
        m_pListCount->setValue(0);
        m_pList->clear();
        m_pListCount->setValue(sStrings.size());
        for (UInt32StdStringMap::const_iterator itr = sStrings.begin(); itr != sStrings.end(); ++itr)
        {
            QStringList stringList;
            stringList.push_back(QString::number(itr->first));
            stringList.push_back(QString::fromStdString(itr->second));
            m_pList->addTopLevelItem(new PrototypeTreeWidgetItem(stringList));
        }

        // select topLevelItem
        if (QTreeWidgetItem *pItem = m_pList->topLevelItem(0))
            m_pList->setCurrentItem(pItem);
    }

    inline void change() { m_Changes = true; }

public:
    DatabaseWidget(QWidget *pParent) : DatabaseWidgetObject(pParent), Ui_DatabaseWidget(), m_Changes(false),
        m_pDBChanger(new DATABASE::DatabaseChanger<T>())
    {
        setupUi(this);
        m_pList->sortByColumn(0, Qt::AscendingOrder);
        m_ModObj.setWidget(m_pList, MODIFY_RESIZE, QPoint(0, m_pListCount->height()+10), MODIFY_DIRECTION_HEIGHT);
        m_ModObj.setWidget(m_pListCount, MODIFY_MOVE, QPoint(0, 5), MODIFY_DIRECTION_HEIGHT);
        m_ModObj.setWidget(m_pResizeList, MODIFY_MOVE, QPoint(0, 5), MODIFY_DIRECTION_HEIGHT);
    }

    void storeDBChanges()
    {
        m_pDBChanger->storeChanges();
    }

    inline uint32 getCurrentID() const { return m_pID->value(); }
    inline bool hasChanged() const { return m_Changes; }
    virtual void setDB(const boost::shared_ptr<DATABASE::Database<T>> &pDB) { m_pDBChanger->setDB(pDB); _updateWidget(); }
    inline uint32 getListCountValue() const { return m_pListCount->value(); }
    void updateName(QTreeWidgetItem *pItem, const QString &sName)
    {
        if (pItem)
            pItem->setText(1, sName);
        updateItem();
    }

    inline boost::shared_ptr<DATABASE::DatabaseChanger<T>> getDBChanger()
    {
        return m_pDBChanger;
    }

protected:
    boost::shared_ptr<DATABASE::DatabaseChanger<T>> m_pDBChanger;
    ModifyObject m_ModObj;

private:
    bool m_Changes;
};
#endif