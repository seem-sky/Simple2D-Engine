#ifndef TEXT_DATABASE_WIDGET_H
#define TEXT_DATABASE_WIDGET_H

#include "DatabaseWidget.h"
#include "ui/UI_TextDatabaseWidget.h"

class TextDatabaseWidget : public DatabaseWidget<DATABASE::LocalisationPrototype, DATABASE::LOCALISATION_INDEX>, Ui_TextDatabaseWidget
{
    Q_OBJECT
private:
    void _addCellTextEdit(QPoint pos, QString sText = "");

protected:
    void clearWidgets();
    bool getItemFromWidgets(DATABASE::LocalisationPrototypePtr &proto);
    bool setWidgetsFromPrototype(const DATABASE::LocalisationPrototypePtr &proto);

public:
    TextDatabaseWidget(QWidget *pParent = NULL);

private slots:
    void _localChanged();

    DATABASE_WIDGET_SLOTS
};
#endif