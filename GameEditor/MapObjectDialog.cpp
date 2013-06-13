#include "MapObjectDialog.h"
#include "moc_MapObjectDialog.h"

using namespace DATABASE;
using namespace MAP;

MapObjectDialog::MapObjectDialog(DATABASE::ConstWorldObjectDatabasePtr pWODB, DATABASE::ConstAnimationDatabasePtr pAnimationDB,
                                 DATABASE::ConstSpriteDatabasePtr pSpriteDB, MAP::MapObjectPtr pObject, QWidget *pParent)
                                 : m_pMapObject(pObject), m_pWorldObjectDB(pWODB), QDialog(pParent), Ui_MapObjectDialog()
{
    setupUi(this);
    connect(m_pDirUp, SIGNAL(toggled(bool)), this, SLOT(_directionButtonToggled(bool)));
    connect(m_pDirRight, SIGNAL(toggled(bool)), this, SLOT(_directionButtonToggled(bool)));
    connect(m_pDirDown, SIGNAL(toggled(bool)), this, SLOT(_directionButtonToggled(bool)));
    connect(m_pDirLeft, SIGNAL(toggled(bool)), this, SLOT(_directionButtonToggled(bool)));
    m_pView->setAnimationDB(pAnimationDB);
    m_pView->setSpriteDB(pSpriteDB);
    m_pView->setEditable(false);
    m_pView->setGridDraw(false);
    _getDataFromObject();
}

void MapObjectDialog::accept()
{
    _storeData();
    QDialog::accept();
}

void MapObjectDialog::_storeData()
{
    m_pMapObject->m_Layer = static_cast<MAP::MapObjectLayer>(m_pLayer->currentIndex());
    if (m_pDirDown->isChecked())
        m_pMapObject->m_Direction = DIRECTION_DOWN;
    else if (m_pDirLeft->isChecked())
        m_pMapObject->m_Direction = DIRECTION_LEFT;
    else if (m_pDirRight->isChecked())
        m_pMapObject->m_Direction = DIRECTION_RIGHT;
    else if (m_pDirUp->isChecked())
        m_pMapObject->m_Direction = DIRECTION_UP;
}

void MapObjectDialog::_directionButtonToggled(bool checked)
{
    if (checked)
        _setAnimationForDirection();
}

void MapObjectDialog::_setAnimationForDirection()
{
    if (!m_pMapObject || !m_pWorldObjectDB)
        return;

    ConstWorldObjectPrototypePtr proto;
    if (m_pWorldObjectDB->getItem(m_pMapObject->m_ObjectID, proto))
    {
        WorldObjectPrototype::AnimationInfo animationInfo;
        if (m_pDirUp->isChecked())
            animationInfo = proto->getAnimationInfo(0);
        else if (m_pDirRight->isChecked())
            animationInfo = proto->getAnimationInfo(1);
        else if (m_pDirDown->isChecked())
            animationInfo = proto->getAnimationInfo(2);
        else if (m_pDirLeft->isChecked())
            animationInfo = proto->getAnimationInfo(3);

        m_pView->setCurrentAnimation(animationInfo.m_uiAnimationID);
        m_pView->setContinuesAnimation();
        m_pView->playCurrentAnimation();
    }
}

void MapObjectDialog::_getDataFromObject()
{
    if (!m_pMapObject)
        return;

    m_pGUID->setValue(m_pMapObject->m_GUID);
    m_pID->setValue(m_pMapObject->m_ObjectID);
    m_pType->setText(DATABASE::WorldObjectPrototype::getTypeString(m_pMapObject->m_Type));
    switch (m_pMapObject->m_Direction)
    {
    case DIRECTION_UP: m_pDirUp->setChecked(true); break;
    case DIRECTION_RIGHT: m_pDirRight->setChecked(true); break;
    case DIRECTION_DOWN: m_pDirDown->setChecked(true); break;
    case DIRECTION_LEFT: m_pDirLeft->setChecked(true); break;
    }
    _setAnimationForDirection();
    m_pLayer->setCurrentIndex(m_pMapObject->m_Layer);
}