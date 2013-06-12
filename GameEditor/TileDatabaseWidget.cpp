#include "TileDatabaseWidget.h"
#include "moc_TileDatabaseWidget.h"
#include "DatabaseWindow.h"

using namespace DATABASE;

const QString PASSABLE_RIGHT_ICON   = "icons/arrow-right.png";
const QString PASSABLE_LEFT_ICON    = "icons/arrow-left.png";
const QString PASSABLE_UP_ICON      = "icons/arrow-up.png";
const QString PASSABLE_DOWN_ICON    = "icons/arrow-down.png";
const QString PASSABLE_NONE_ICON    = "icons/cross.png";

TileDatabaseWidget::TileDatabaseWidget(QWidget *pParent) : TextureDatabaseWidget(pParent), Ui_TileDatabaseWidget()
{
    Ui_TileDatabaseWidget::setupUi(this);
    connectWidgets();
    connect(m_pUp, SIGNAL(clicked(bool)), this, SLOT(_passabilityButtonClicked(bool)));
    connect(m_pRight, SIGNAL(clicked(bool)), this, SLOT(_passabilityButtonClicked(bool)));
    connect(m_pLeft, SIGNAL(clicked(bool)), this, SLOT(_passabilityButtonClicked(bool)));
    connect(m_pDown, SIGNAL(clicked(bool)), this, SLOT(_passabilityButtonClicked(bool)));
    _setPassabilityIcons();
}

void TileDatabaseWidget::_passabilityButtonClicked(bool clicked)
{
    _setPassabilityIcons();
    updateItem();
}

void TileDatabaseWidget::_setPassabilityIcons()
{
    if (m_pUp->isChecked())
        m_pUp->setIcon(QIcon(PASSABLE_UP_ICON));
    else
        m_pUp->setIcon(QIcon(PASSABLE_NONE_ICON));

    if (m_pRight->isChecked())
        m_pRight->setIcon(QIcon(PASSABLE_RIGHT_ICON));
    else
        m_pRight->setIcon(QIcon(PASSABLE_NONE_ICON));

    if (m_pLeft->isChecked())
        m_pLeft->setIcon(QIcon(PASSABLE_LEFT_ICON));
    else
        m_pLeft->setIcon(QIcon(PASSABLE_NONE_ICON));

    if (m_pDown->isChecked())
        m_pDown->setIcon(QIcon(PASSABLE_DOWN_ICON));
    else
        m_pDown->setIcon(QIcon(PASSABLE_NONE_ICON));
}

void TileDatabaseWidget::clearWidgets()
{
    TextureDatabaseWidget::clearWidgets();
    m_pUp->setChecked(true);
    m_pRight->setChecked(true);
    m_pLeft->setChecked(true);
    m_pDown->setChecked(true);
    m_pPassability->setScene(new QGraphicsScene());
}

bool TileDatabaseWidget::getItemFromWidgets(TilePrototypePtr &proto)
{
    if (!TextureDatabaseWidget::getItemFromWidgets(proto))
        return false;

    // get passability
    if (m_pUp->isChecked())
        proto->addPassabilityFlag(TilePrototype::PASSABLE_UP);
    if (m_pRight->isChecked())
        proto->addPassabilityFlag(TilePrototype::PASSABLE_RIGHT);
    if (m_pDown->isChecked())
       proto->addPassabilityFlag(TilePrototype::PASSABLE_DOWN);
    if (m_pLeft->isChecked())
        proto->addPassabilityFlag(TilePrototype::PASSABLE_LEFT);

    return true;
}

bool TileDatabaseWidget::setWidgetsFromPrototype(const TilePrototypePtr &proto)
{
    if (!TextureDatabaseWidget::setWidgetsFromPrototype(proto))
        return false;

    // set passability
    m_pUp->setChecked(proto->hasPassabilityFlag(TilePrototype::PASSABLE_UP));
    m_pRight->setChecked(proto->hasPassabilityFlag(TilePrototype::PASSABLE_RIGHT));
    m_pDown->setChecked(proto->hasPassabilityFlag(TilePrototype::PASSABLE_DOWN));
    m_pLeft->setChecked(proto->hasPassabilityFlag(TilePrototype::PASSABLE_LEFT));
    _setPassabilityIcons();
    return true;
}

QPixmap TileDatabaseWidget::showPixmap(const TilePrototypePtr &proto)
{
    QPixmap pixmap = TextureDatabaseWidget::showPixmap(proto);
    if (!pixmap.isNull())
    {
        QGraphicsScene *pScene = new QGraphicsScene();
        pScene->addPixmap(pixmap);
        m_pPassability->setScene(pScene);
    }
    return pixmap;
}