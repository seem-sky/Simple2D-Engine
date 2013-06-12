#ifndef TEXT_DATABASE_WIDGET_H
#define TEXT_DATABASE_WIDGET_H

#include "DatabaseWidget.h"
#include "ui/UI_TextDatabaseWidget.h"

class TextDatabaseWidget : public DatabaseWidget<DATABASE::LocalisationPrototype>, Ui_TextDatabaseWidget
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

    void _resizeButtonClicked() { resizeDatabase(getListCountValue()); }
    void _currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) { currentItemChanged(current); }
    void _changeName() { changeName(); }
};
#endif