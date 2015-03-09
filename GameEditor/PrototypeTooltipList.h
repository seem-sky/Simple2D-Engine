#pragma once

#include "TooltipList.h"
#include <Database/Derived.h>
#include "DatabaseModel.h"
#include "Config.h"
#include <QtGlobal.h>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include "HelperCreatorInterface.h"

class PrototypeTooltipList : public TooltipList
{
private:
    QWidget* _setupTooltipWidget(uint32 uiPrototypeID) override;

protected:
    void startDrag(Qt::DropActions supportedActions) override;

public:
    PrototypeTooltipList(QWidget* pParent = nullptr);
    PrototypeTooltipList(const helper::CreatorInterface& creator, QWidget* pParent = nullptr);

    void setup(const helper::CreatorInterface& creator);

private:
    const helper::CreatorInterface* m_pCreator;
};
