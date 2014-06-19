#ifndef DEACTIVATED_TAB_H
#define DEACTIVATED_TAB_H

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QCheckBox>

class DeactivatedWidget : public QWidget
{
    Q_OBJECT
public:
    DeactivatedWidget(QWidget* pParent = nullptr);

	inline bool isActivated() const { return m_IsActivated; }
	void setActivated(bool activated = true);

	void setCheckBox(QCheckBox *pCheckBox);

private slots:
    void _onCheckBoxChanged(int state);
	
signals:
	void activationChanged(bool);

private:
	bool m_IsActivated = false;
	QCheckBox* m_pCheckBox = nullptr;
};

class DeactivatedTabWidget : public QTabWidget
{
protected:
    virtual void tabInserted(int index);

public:
    DeactivatedTabWidget(QWidget* pParent = nullptr);

    QTabBar* getTabBar() const { return tabBar(); }
	bool isTabActive(const DeactivatedWidget* pWidget) const;
};
#endif
