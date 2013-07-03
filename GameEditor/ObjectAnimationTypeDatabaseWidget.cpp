#include "ObjectAnimationTypeDatabaseWidget.h"
#include "moc_ObjectAnimationTypeDatabaseWidget.h"
#include "PrototypeTreeWidgetItem.h"

using namespace DATABASE;

ObjectAnimationTypeDatabaseWidget::ObjectAnimationTypeDatabaseWidget(QWidget *pParent) : DatabaseWidget(pParent)
{
    connectWidgets();
}

bool ObjectAnimationTypeDatabaseWidget::setWidgetsFromPrototype(const DATABASE::ObjectAnimationTypePrototypePtr &proto)
{
    if (!DatabaseWidget::setWidgetsFromPrototype(proto))
        return false;
    // prevent editing basic object animations
    m_pName->setReadOnly(proto->getID() <= MIN_DYNAMIC_OBJECT_POSE);
    return true;
}

void ObjectAnimationTypeDatabaseWidget::resizeDatabase(uint32 uiSize)
{
    // prevent editing basic object animations
    if (uiSize < MIN_DYNAMIC_OBJECT_POSE)
        uiSize = MIN_DYNAMIC_OBJECT_POSE;
    DatabaseWidget::resizeDatabase(uiSize);
}

void ObjectAnimationTypeDatabaseWidget::fillListWidget(const UInt32StringMap &sStrings)
{
    m_pListCount->setValue(0);
    m_pList->clear();
    m_pListCount->setValue(sStrings.size());
    for (UInt32StringMap::const_iterator itr = sStrings.begin(); itr != sStrings.end(); ++itr)
    {
        QStringList stringList;
        stringList.push_back(QString::number(itr->first));
        stringList.push_back(itr->second);
        PrototypeTreeWidgetItem *pItem = new PrototypeTreeWidgetItem(stringList);
        // if standard entry, set blue color
        if (itr->first <= MIN_DYNAMIC_OBJECT_POSE)
            pItem->setForeground(1, QBrush(QColor(0, 0, 255)));
        m_pList->addTopLevelItem(pItem);
    }

    // select topLevelItem
    if (QTreeWidgetItem *pItem = m_pList->topLevelItem(0))
        m_pList->setCurrentItem(pItem);
}