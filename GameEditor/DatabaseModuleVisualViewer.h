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
    VisualViewerItem(QWidget* pParent = nullptr, bool standardEntry = false);

    inline VisualViewer* getVisualViewer() const { return m_pVisualViewer; }

    inline uint32 getAnimationID() const { return m_pVisualViewer->getAnimationID(); }
    inline uint32 getAnimationTypeID() const { return m_pAnimationType->currentIndex()+1; }
    inline DATABASE::PROTOTYPE::WORLD_OBJECT::AnimationInfo::VisualType getVisualType() const { return m_pVisualViewer->getVisualType(); }

    void setAnimation(uint32 ID, DATABASE::PROTOTYPE::WORLD_OBJECT::AnimationInfo::VisualType type);
    void setAnimationType(uint32 ID);
    void setAnimationTypeModel(DATABASE::ConstDatabaseModel* pModel);

public:
    VisualViewer* m_pVisualViewer;
    QComboBox* m_pAnimationType;
    // ugly workaround; when item is created, it has no model, so we can´t change index.
    // This variable stores the given index until the combobox has an model.
    uint32 m_TypeID;
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
