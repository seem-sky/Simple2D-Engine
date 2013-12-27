#include "DatabaseWidgetAnimationType.h"

using namespace DATABASE;

DatabaseWidgetAnimationType::DatabaseWidgetAnimationType(QWidget* pParent) : DatabaseWidgetBase(pParent)
{
}

void DatabaseWidgetAnimationType::setDatabaseMgr(DatabaseMgr& DBMgr)
{
    m_pModuleList->setDatabaseModel(new DatabaseModel(DBMgr, DatabaseType::ANIMATION_TYPE_DATABASE));
}
