#ifndef DATABASE_MODULE_WORLD_OBJECT_ANIMATION_H
#define DATABASE_MODULE_WORLD_OBJECT_ANIMATION_H

#include "DatabaseModuleDragDrop.h"
#include "DatabaseModuleVisualViewer.h"
#include <QtWidgets/QComboBox>

class DatabaseModuleWorldObjectAnimation : public QWidget
{
    Q_OBJECT
public:
    DatabaseModuleWorldObjectAnimation(QWidget* pParent = nullptr);

    void setSpriteDatabaseModel(DATABASE::ConstDatabaseModel* pModel);
    void setAnimationDatabaseModel(DATABASE::ConstDatabaseModel* pModel);

    uint32 getAnimationCount() const;
    void setAniamtionCount(uint32 count);

    inline VisualViewerItem* getVisualViewer(uint32 index) const { return m_pModuleVisualViewer->getVisualViewer(index); }

    void clear();

private slots:
    void _onVisualTypeChanged(int);

private:
    // source lists
    DatabaseModuleTextureDragList* m_pModuleSpriteList;
    DatabaseModuleAnimationDragList* m_pModuleAnimationList;

    DatabaseModuleVisualViewer* m_pModuleVisualViewer;

    QComboBox* m_pVisualTypeBox;
};

#endif
