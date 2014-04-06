#ifndef DATABASE_MODULE_VISUAL_VIEWER_H
#define DATABASE_MODULE_VISUAL_VIEWER_H

#include "VisualViewer.h"
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFrame>
#include <QtWidgets/QComboBox>
#include "DatabaseModel.h"

class VisualViewerItem : public QFrame
{
public:
    VisualViewerItem(QWidget* pParent = nullptr);

    inline VisualViewer* getVisualViewer() const { return m_pVisualViewer; }

    inline uint32 getAnimationID() const { return m_pVisualViewer->getAnimationID(); }
    inline DATABASE::WORLD_OBJECT::AnimationInfo::VisualType getVisualType() const { return m_pVisualViewer->getVisualType(); }

    void setAnimation(uint32 ID, DATABASE::WORLD_OBJECT::AnimationInfo::VisualType type);

    void setAnimationTypeModel(DATABASE::ConstDatabaseModel* pModel);

public:
    VisualViewer* m_pVisualViewer;
    QComboBox* m_pAnimationType;
};

class DatabaseModuleVisualViewer : public QWidget
{
    Q_OBJECT
public:
    DatabaseModuleVisualViewer(QWidget* pParent = nullptr);

    void setVisualViewerCount(uint32 count);
    uint32 getVisualViewerCount() const;

    VisualViewerItem* getVisualViewer(uint32 index) const;

    void insertVisualViewer(uint32 index);
    void removeVisualViewer(uint32 index);

    void clear();

    void setDatabaseMgr(const DATABASE::DatabaseMgr* pDBMgr);

private slots:
    void _onAddButtonClicked();
    void _onRemoveButtonClicked();

private:
    QTableWidget* m_pVisualViewerList;
    QPushButton* m_pAddButton;
    QPushButton* m_pRemoveButton;

    const DATABASE::DatabaseMgr* m_pDBMgr;

    DATABASE::ConstDatabaseModel* m_pAnimationTypeModel;
};

#endif
