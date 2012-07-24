#include "Player.h"
#include "Logfile.h"
#include "DirectInput.h"
#include "Game.h"
#include "Unit.h"

Player::Player(void) : m_pControledUnit(NULL), m_uiMoveBuffer(MOVE_BUFFER_NONE), m_uiLockBuffer(MOVE_BUFFER_NONE)
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "Player : ";
    AddKeyAction(DIK_W, ACTION_MOVE_UP);
    AddKeyAction(DIK_S, ACTION_MOVE_DOWN);
    AddKeyAction(DIK_D, ACTION_MOVE_RIGHT);
    AddKeyAction(DIK_A, ACTION_MOVE_LEFT);
    AddKeyAction(DIK_ESCAPE, ACTION_ESCAPE);
    AddKeyAction(DIK_RETURN, ACTION_ENTER);
    AddKeyAction(DIK_P, ACTION_PAUSE_GAME);
}

Player::~Player(void)
{
    // release all keys
    if (!m_KeyList.empty())
    {
        for (KeyList::iterator itr = m_KeyList.begin(); itr != m_KeyList.end(); ++itr)
            delete *itr;
    }
}

void Player::UpdatePlayer(const ULONGLONG CurTime, const UINT CurElapsedTime)
{
    Point<int> x = GetControledUnit()->GetPosition();
    // iterate through stored keys
    if (!m_KeyList.empty())
    {
        if (DirectInput *pInput = DirectInput::Get())
        {
            m_uiMoveBuffer = MOVE_BUFFER_NONE;
            for (KeyList::iterator itr = m_KeyList.begin(); itr != m_KeyList.end(); ++itr)
            {
                if (!(*itr))
                    continue;

                // little hack, because at this time true(e.g. 128) is not like true (e.g. 3)
                // so we must check a little bit else.
                if ((!pInput->GetKeyStateKeyboard((*itr)->m_uiKey)) != (*itr)->m_bShouldPressed)
                    DoActionForKey(*itr, CurTime);
            }

            if (m_uiMoveBuffer)
                MovePlayerByBuffer();
        }
    }
}

void Player::SetControledUnit(Unit *pWho)
{
    // release old object
    if (m_pControledUnit)
    {
        m_pControledUnit->SetPlayerControle(false);
        m_pControledUnit = NULL;
    }

    if (pWho)
    {
        m_pControledUnit = pWho;
        pWho->SetPlayerControle();
        BASIC_LOG(m_sLogLocationName + "Changes controle to Object with GUID " + ToString(pWho->GetGUID())+".");
        // set screen center to controled unit
        if (CGame *pGame = CGame::Get())
        {
            if (GameInfo *pInfo = pGame->GetGameInfo())
            {
                Point<UINT> ScreenSize = pInfo->GetWindowSize();
                Point<int> UnitPos = pWho->GetPosition();
                Point<UINT> UnitSize;
                pWho->GetObjectSize(UnitSize.x, UnitSize.y);
                Point<int> MapPos;
                if (Map *pMap = pWho->GetMap())
                {
                    MapPos.x = ScreenSize.x / 2 - UnitPos.x - UnitSize.x / 2;
                    MapPos.y = ScreenSize.y / 2 - UnitPos.y - UnitSize.y / 2;
                    pMap->SetPosition(MapPos);
                }
            }
        }
    }
    else
        ERROR_LOG(m_sLogLocationName + "Unable to change controle. Object is not valid");
}

void Player::AddKeyAction(UINT uiKey, UINT actionID, bool press)
{
    PlayerKeyAction *pAction = new PlayerKeyAction();
    pAction->m_uiKey = uiKey;
    pAction->m_uiActionID = actionID;
    pAction->m_bShouldPressed = press;

    m_KeyList.push_back(pAction);
}

void Player::DoActionForKey(PlayerKeyAction *action, const ULONGLONG CurTime)
{
    if (!action)
        return;

    if (CGame *pGame = CGame::Get())
    {
        if (action->m_uiLastTimeActive + 250 < CurTime)
        {
            switch (action->m_uiActionID)
            {
            case ACTION_PAUSE_GAME:
                if (pGame->IsGamePaused())
                    pGame->PauseGame(false);
                else
                    pGame->PauseGame();
                break;
            default:
                break;
            }
        }

        // if textbox is shown
        if (pGame->ShowsTextbox())
        {
            if (action->m_uiLastTimeActive + 250 < CurTime)
            {
                TextBox *pBox = pGame->GetShownTextbox();
                if (!pBox)
                    return;

                switch (action->m_uiActionID)
                {
                case ACTION_MOVE_UP:
                    break;
                case ACTION_MOVE_DOWN:
                    break;
                case ACTION_MOVE_RIGHT:
                    break;
                case ACTION_MOVE_LEFT:
                    break;
                case ACTION_ENTER:
                    pBox->Use();
                    break;
                case ACTION_NONE:
                default:
                    break;
                }
                action->m_uiLastTimeActive = CurTime;
            }
        }

        // if menu is shown
        else if (pGame->ShowsMenu())
        {
            Menu *pMenu = pGame->GetShownMenu();
            if (!pMenu)
                return;

            if (action->m_uiLastTimeActive + 250 < CurTime)
            {
                switch (action->m_uiActionID)
                {
                case ACTION_NONE:
                    break;
                case ACTION_MOVE_UP:
                    pMenu->OnMenuUp();
                    break;
                case ACTION_MOVE_DOWN:
                    pMenu->OnMenuDown();
                    break;
                case ACTION_MOVE_RIGHT:
                    pMenu->OnMenuRight();
                    break;
                case ACTION_MOVE_LEFT:
                    pMenu->OnMenuLeft();
                    break;
                case ACTION_ESCAPE:
                    pGame->ShutDownMenu();                        
                    break;
                case ACTION_ENTER:
                    pMenu->OnMenuEnter();
                    break;
                default:
                    break;
                }
                action->m_uiLastTimeActive = CurTime;
            }
        }

        // if no menu and textbox is shown
        else
        {
            switch (action->m_uiActionID)
            {
            case ACTION_NONE:
                break;
            case ACTION_MOVE_UP:
                SetMoveBuffer(MOVE_BUFFER_UP);
                break;
            case ACTION_MOVE_DOWN:
                SetMoveBuffer(MOVE_BUFFER_DOWN);
                break;
            case ACTION_MOVE_RIGHT:
                SetMoveBuffer(MOVE_BUFFER_RIGHT);
                break;
            case ACTION_MOVE_LEFT:
                SetMoveBuffer(MOVE_BUFFER_LEFT);
                break;
            default:
                // check for last time activated
                if (action->m_uiLastTimeActive + 250 < CurTime)
                {
                    switch (action->m_uiActionID)
                    {
                    case ACTION_ESCAPE:
                        pGame->DisplayMenu(new MenuMainMenu(false, true));
                        break;
                    case ACTION_ENTER:
                        UseCollissionObject();
                        break;
                    default:
                        break;
                    }
                    action->m_uiLastTimeActive = CurTime;
                }
                break;
            }
        }
    }
}

void Player::MovePlayer(int XMove, int YMove, UINT uiMoveMsec)
{
    if (m_pControledUnit && !m_pControledUnit->IsMoving())
    {
        DIRECTION dir = DIRECTION_NONE;
        if (XMove > 0)
            dir = DIRECTION_RIGHT;
        else if (XMove < 0)
            dir = DIRECTION_LEFT;
        else if (YMove > 0)
            dir = DIRECTION_DOWN;
        else if (YMove < 0)
            dir = DIRECTION_UP;
        m_pControledUnit->MovePosition(XMove, YMove, dir, uiMoveMsec);
    }
}

void Player::MovePlayerByBuffer()
{
    // check lock, if not pressed, release lock
    if (!HasMoveBuffer(m_uiLockBuffer))
        m_uiLockBuffer = MOVE_BUFFER_NONE;

    // if there is a lock in a specific direction
    if(m_uiLockBuffer)
    {
        // direction left
        if(HasMoveBuffer(MOVE_BUFFER_LEFT) && m_uiLockBuffer != MOVE_BUFFER_LEFT)
        {
            MovePlayer(m_pControledUnit->GetMovementSpeed()*(-1), 0, 100);
            return;
        }
        // direction up
        if(HasMoveBuffer(MOVE_BUFFER_UP) && m_uiLockBuffer != MOVE_BUFFER_UP)
        {
            MovePlayer(0, m_pControledUnit->GetMovementSpeed()*(-1), 100);
            return;
        }
        // direction right
        if(HasMoveBuffer(MOVE_BUFFER_RIGHT) && m_uiLockBuffer != MOVE_BUFFER_RIGHT)
        {
            MovePlayer(m_pControledUnit->GetMovementSpeed(), 0, 100);
            return;
        }
        // direction down
        if(HasMoveBuffer(MOVE_BUFFER_DOWN) && m_uiLockBuffer != MOVE_BUFFER_DOWN)
        {
            MovePlayer(0, m_pControledUnit->GetMovementSpeed(), 100);
            return;
        }
    }

    // if there is no lock, or no other direction key is pressed
    //
    // direction left
    if(HasMoveBuffer(MOVE_BUFFER_LEFT))
    {
        if (m_pControledUnit)
            MovePlayer(m_pControledUnit->GetMovementSpeed()*(-1), 0, 100);
        m_uiLockBuffer = MOVE_BUFFER_LEFT;
    }
    // direction up
    else if(HasMoveBuffer(MOVE_BUFFER_UP))
    {
        MovePlayer(0, m_pControledUnit->GetMovementSpeed()*(-1), 100);
        m_uiLockBuffer = MOVE_BUFFER_UP;
    }
    // direction right
    else if(HasMoveBuffer(MOVE_BUFFER_RIGHT))
    {
        MovePlayer(m_pControledUnit->GetMovementSpeed(), 0, 100);
        m_uiLockBuffer = MOVE_BUFFER_RIGHT;
    }
    // direction down
    else if(HasMoveBuffer(MOVE_BUFFER_DOWN))
    {
        MovePlayer(0, m_pControledUnit->GetMovementSpeed(), 100);
        m_uiLockBuffer = MOVE_BUFFER_DOWN;
    }
}

void Player::UseCollissionObject()
{
    if (!m_pControledUnit)
        return;

    ObjectLayer *pLayer = m_pControledUnit->GetOwnerLayer();
    if (!pLayer)
        return;

    const WorldObjectList *pList = pLayer->GetObjectsOnLayer();
    if (!pList)
        return;

    for (WorldObjectList::const_iterator itr = pList->begin(); itr != pList->end(); ++itr)
    {
        if (*itr == m_pControledUnit)
            continue;

        if (CanUseObject(*itr))
        {
            ObjectAI* pAI = (*itr)->GetAI();
            if (!pAI)
                return;

            pAI->OnUse(this);
        }
    }
}

bool Player::CanUseObject(WorldObject *pWho)
{
    if (!m_pControledUnit || !pWho)
        return false;

    DIRECTION playerDir = m_pControledUnit->GetDirection();
    Point<int> objPos = m_pControledUnit->GetPosition();
    RECT objBounding = { 0, 0, 0, 0 };
    m_pControledUnit->GetBoundingRect(objBounding);

    Point<int> whoPos = pWho->GetPosition();
    UINT whoXSize = 0, whoYSize = 0;
    RECT whoBounding = { 0, 0, 0, 0 };
    pWho->GetBoundingRect(whoBounding);

    switch(playerDir)
    {
    case DIRECTION_UP:
        if ((whoPos.y + whoBounding.bottom) - (objPos.y + objBounding.top) == 0)
        {
            if ((whoPos.x + whoBounding.left <= objPos.x + objBounding.left && whoPos.x + whoBounding.right >= objPos.x + objBounding.left) ||
            // bottom left edge of Obj is in point
            (whoPos.x + whoBounding.left <= objPos.x + objBounding.right && whoPos.x + whoBounding.right >= objPos.x + objBounding.right) ||
            // left side without edges is in point
            (whoPos.x + whoBounding.left >= objPos.x + objBounding.left && whoPos.x + whoBounding.right <= objPos.x + objBounding.right))
                return true;
        }
        break;
    case DIRECTION_DOWN:
        if ((whoPos.y + whoBounding.top) - (objPos.y + objBounding.bottom) == 0)
        {
            if ((whoPos.x + whoBounding.left <= objPos.x + objBounding.left && whoPos.x + whoBounding.right >= objPos.x + objBounding.left) ||
            // bottom left edge of Obj is in point
            (whoPos.x + whoBounding.left <= objPos.x + objBounding.right && whoPos.x + whoBounding.right >= objPos.x + objBounding.right) ||
            // left side without edges is in point
            (whoPos.x + whoBounding.left >= objPos.x + objBounding.left && whoPos.x + whoBounding.right <= objPos.x + objBounding.right))
                return true;
        }
        break;
    case DIRECTION_RIGHT:
        if ((whoPos.x + whoBounding.left) - (objPos.x + objBounding.right) == 0)
        {
            if ((whoPos.y + whoBounding.top <= objPos.y + objBounding.top && whoPos.y + whoBounding.bottom >= objPos.y + objBounding.top) ||
            // bottom left edge of Obj is in point
            (whoPos.y + whoBounding.top<= objPos.y + objBounding.bottom && whoPos.y + whoBounding.bottom >= objPos.y + objBounding.bottom) ||
            // left side without edges is in point
            (whoPos.y + whoBounding.top >= objPos.y + objBounding.top && whoPos.y + whoBounding.bottom <= objPos.y + objBounding.bottom))
                return true;
        }
        break;
    case DIRECTION_LEFT:
        if ((whoPos.x + whoBounding.right) - (objPos.x + objBounding.left) == 0)
        {
            if ((whoPos.y + whoBounding.top <= objPos.y + objBounding.top && whoPos.y + whoBounding.bottom >= objPos.y + objBounding.top) ||
            // bottom left edge of Obj is in point
            (whoPos.y + whoBounding.top<= objPos.y + objBounding.bottom && whoPos.y + whoBounding.bottom >= objPos.y + objBounding.bottom) ||
            // left side without edges is in point
            (whoPos.y + whoBounding.top >= objPos.y + objBounding.top && whoPos.y + whoBounding.bottom <= objPos.y + objBounding.bottom))
                return true;
        }
        break;
    default:
        return false;
    }
    
    return false;
}