#ifndef INDEX_DIALOG_H
#define INDEX_DIALOG_H

#include <QtWidgets/QDialog>
#include "ui/UI_IndexDialog.h"
#include "Global.h"

class IndexDialog : public QDialog, Ui_IndexDialog
{
public:
    IndexDialog(uint32 p_uiIndex);

    inline uint32 GetCurrentIndexValue() const { return m_pIndex->value(); }
};
#endif