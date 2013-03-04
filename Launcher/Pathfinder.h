#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "point.h"
#include "windows.h"
#include <map>
#include "Global.h"

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
    PathNode(Point<uint32> uiPos, PathDir pathDir, uint32 uiGPrice, uint32 uiHCalc, PathNode *LastNode) : m_pLastNode(LastNode), m_uiPos(uiPos), m_uiGPrice(uiGPrice),
    m_uiHCalc(uiHCalc), m_PathDir(pathDir)
    {}

    PathDir m_PathDir;
    PathNode *m_pLastNode;
    Point<uint32> m_uiPos;
    uint32 m_uiGPrice;
    uint32 m_uiHCalc;
};
typedef std::map<Point<uint32>, PathNode*> NodeList;

class Pathfinder
{
public:
    Pathfinder(Unit *pOwner);
    ~Pathfinder(void);

    bool GetValidPath(Point<uint32> uiStartPos, Point<uint32> uiEndPos);
    Point<uint32> GetRangeBetween(Point<uint32> start, Point<uint32> end);
    Point<uint32> GetStoredEndPoint() { return m_uiStoredEndPoint; }

private:
    bool IsStoredInClosedList(PathNode *Node);
    bool IsStoredInOpenList(PathNode *Node);
    void ResetPath();
    PathNode* GetStoredPathNode(Point<uint32> uiTile);

    NodeList m_OpenNodes;
    NodeList m_ClosedNodes;

    Unit *m_pOwner;

    Point<uint32> m_uiStoredEndPoint;
};
#endif;