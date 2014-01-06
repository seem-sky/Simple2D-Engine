#ifndef DATABASE_MODULE_VISUAL_VIEWER_H
#define DATABASE_MODULE_VISUAL_VIEWER_H

#include "VisualViewer.h"
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QAbstractItemDelegate>
#include <QtWidgets/QFrame>

class VisualViewerItem : public QFrame
{
public:
    VisualViewerItem(const DATABASE::DatabaseMgr& DBMgr, QWidget *pParent = nullptr);

public:
    VisualViewer *m_pVisualViewer;
};

class DatabaseModuleVisualViewer : public QWidget
{
public:
    DatabaseModuleVisualViewer(QWidget* pParent = nullptr);

    void setVisualViewerCount(uint32 count);
    uint32 getVisualViewerCount() const;

    void insertVisualViewer(uint32 index);
    void removeVisualViewer(uint32 index);

    void setDatabaseMgr(const DATABASE::DatabaseMgr* pDBMgr);

private:
    QTableWidget* m_pVisualViewerList;
    QPushButton* m_pAddButton;
    QPushButton* m_pRemoveButton;

    const DATABASE::DatabaseMgr* m_pDBMgr;
};

#endif
