#include "Player.h"
#include "Logfile.h"
#include "DirectInput.h"
#include "Game.h"

Player::Player(void) : m_uiMoveBuffer(MOVE_BUFFER_NONE), m_uiLockBuffer(MOVE_BUFFER_NONE)
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "Player : ";
    AddKeyAction(DIK_W, 1);
    AddKeyAction(DIK_S, 2);
    AddKeyAction(DIK_D, 3);
    AddKeyAction(DIK_A, 4);
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
    if (pWho)
    {
        m_pControledUnit = pWho;
        BASIC_LOG(m_sLogLocationName + "Changes controle to Object " + to_string(pWho->GetObjectInfo()->m_uiID));
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

        // if menu is shown
        if (pGame->ShowsMenu())
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

        // if no menu is shown
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
        m_pControledUnit->MovePosition(XMove, YMove, uiMoveMsec);
        DIRECTION dir = DIRECTION_NONE;
        if (XMove > 0)
            dir = DIRECTION_RIGHT;
        else if (XMove < 0)
            dir = DIRECTION_LEFT;
        else if (YMove > 0)
            dir = DIRECTION_DOWN;
        else if (YMove < 0)
            dir = DIRECTION_UP;

        // if another direction than before, set it.
        if (dir != m_pControledUnit->GetDirection())
            m_pControledUnit->SetDirection(dir);
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
            MovePlayer(-4,0,250);
            return;
        }
        // direction up
        if(HasMoveBuffer(MOVE_BUFFER_UP) && m_uiLockBuffer != MOVE_BUFFER_UP)
        {
            MovePlayer(0,-4,250);
            return;
        }
        // direction right
        if(HasMoveBuffer(MOVE_BUFFER_RIGHT) && m_uiLockBuffer != MOVE_BUFFER_RIGHT)
        {
            MovePlayer(4,0,250);
            return;
        }
        // direction down
        if(HasMoveBuffer(MOVE_BUFFER_DOWN) && m_uiLockBuffer != MOVE_BUFFER_DOWN)
        {
            MovePlayer(0,4,250);
            return;
        }
    }

    // if there is no lock, or no other direction key is pressed
    //
    // direction left
    if(HasMoveBuffer(MOVE_BUFFER_LEFT))
    {
        MovePlayer(-4,0,250);
        m_uiLockBuffer = MOVE_BUFFER_LEFT;
    }
    // direction up
    else if(HasMoveBuffer(MOVE_BUFFER_UP))
    {
        MovePlayer(0,-4,250);
        m_uiLockBuffer = MOVE_BUFFER_UP;
    }
    // direction right
    else if(HasMoveBuffer(MOVE_BUFFER_RIGHT))
    {
        MovePlayer(4,0,250);
        m_uiLockBuffer = MOVE_BUFFER_RIGHT;
    }
    // direction down
    else if(HasMoveBuffer(MOVE_BUFFER_DOWN))
    {
        MovePlayer(0,4,250);
        m_uiLockBuffer = MOVE_BUFFER_DOWN;
    }
}