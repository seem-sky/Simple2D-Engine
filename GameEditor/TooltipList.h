#pragma once

#include <QtWidgets/QTreeView>
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>
#include <Global.h>

class TooltipList : public QTreeView
{
    Q_OBJECT    
private:
    void _endTracking();
    virtual QWidget* _setupTooltipWidget(uint32 uiPrototypeID) { return new QWidget(this); }

protected:
    virtual void leaveEvent(QEvent* pEvent);
    virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous);

public:
    TooltipList(QWidget* pParent = nullptr);

    // tooltip
    inline void setMaximumTooltipSize(const QSize& size) { m_MaximumTooltipSize = size; }
    inline QSize getMaximumTooltipSize() const { return m_MaximumTooltipSize; }

    void setShowTooltip(bool show = true);
    inline bool showTooltip() const { return m_ShowTooltip; }

    enum class ToolTipPosition
    {
        Left,
        Right
    };
    void setToolTipPosition(ToolTipPosition pos) { m_ToolTipPos = pos; }

    uint32 getSelectedID() const;

private slots:
    void _showTooltip();
    void _onItemEntered(const QModelIndex& index);

signals:
    void changeIndex(int32 index);

private:
    QWidget* m_pToolTip = nullptr;
    int32 m_CurrentRow = -1;
    QTimer m_Timer;
    QSize m_MaximumTooltipSize;
    ToolTipPosition m_ToolTipPos; 
    bool m_ShowTooltip;
};
