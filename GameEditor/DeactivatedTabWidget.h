#ifndef DEACTIVATED_TAB_H
#define DEACTIVATED_TAB_H

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QCheckBox>

class DeactivatedWidget : public QWidget
{
    Q_OBJECT
public:
    DeactivatedWidget(QWidget* pParent = nullptr);

private slots:
    void _onCheckBoxChanged(int state);
};

class DeactivatedTabWidget : public QTabWidget
{
protected:
    virtual void tabInserted(int index);

public:
    DeactivatedTabWidget(QWidget* pParent = nullptr);

    QTabBar* getTabBar() const { return tabBar(); }
};
#endif
