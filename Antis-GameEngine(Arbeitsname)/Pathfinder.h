#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "point.h"
#include "windows.h"
#include <map>

class Unit;

enum PathDir
{
    PATH_START,
    PATH_UP,
    PATH_LEFT,
    PATH_DOWN,
    PATH_RIGHT,
    PATH_END,
};

struct PathNode
{
    PathNode(Point<UINT> uiPos, PathDir pathDir, UINT uiGPrice, UINT uiHCalc, PathNode *LastNode) : m_pLastNode(LastNode), m_uiPos(uiPos), m_uiGPrice(uiGPrice),
    m_uiHCalc(uiHCalc), m_PathDir(pathDir)
    {}

    PathDir m_PathDir;
    PathNode *m_pLastNode;
    Point<UINT> m_uiPos;
    UINT m_uiGPrice;
    UINT m_uiHCalc;
};
typedef std::map<Point<UINT>, PathNode*> NodeList;

class Pathfinder
{
public:
    Pathfinder(Unit *pOwner);
    ~Pathfinder(void);

    bool GetValidPath(Point<UINT> uiStartPos, Point<UINT> uiEndPos);
    Point<UINT> GetRangeBetween(Point<UINT> start, Point<UINT> end);
    Point<UINT> GetStoredEndPoint() { return m_uiStoredEndPoint; }

private:
    bool IsStoredInClosedList(PathNode *Node);
    bool IsStoredInOpenList(PathNode *Node);
    void ResetPath();
    PathNode* GetStoredPathNode(Point<UINT> uiTile);

    NodeList m_OpenNodes;
    NodeList m_ClosedNodes;

    Unit *m_pOwner;

    Point<UINT> m_uiStoredEndPoint;
};
#endif;