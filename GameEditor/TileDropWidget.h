#pragma once

#include <QtWidgets/QFrame>
#include "HelperCreatorInterface.h"

class TileDropWidget : public QFrame
{
    Q_OBJECT
protected:
    void dragMoveEvent(QDragMoveEvent* pEvent);
    void dragEnterEvent(QDragEnterEvent* pEvent);
    void dropEvent(QDropEvent* pEvent);
    void paintEvent(QPaintEvent* pEvent);
    void contextMenuEvent(QContextMenuEvent* pEvent);

public:
    TileDropWidget(const helper::CreatorInterface& creator, QWidget* pParent = nullptr);

    uint32 getTileID() const;
    void setTileID(uint32 uiTileID);

private slots:
    void _onCleared();

private:
    uint32 m_TileID;
    QPixmap m_Pixmap;
    const helper::CreatorInterface& m_Creator;
};
