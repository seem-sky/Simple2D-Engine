#include "DatabaseWidgetSprite.h"

using namespace DATABASE;

DatabaseWidgetSprite::DatabaseWidgetSprite(QWidget* pParent) : DatabaseWidgetTexture(pParent)
{
}

void DatabaseWidgetSprite::setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr)
{
    m_pModuleList->setDatabaseModel(new DatabaseModel(DBMgr, DatabaseType::SPRITE_DATABASE));
}
