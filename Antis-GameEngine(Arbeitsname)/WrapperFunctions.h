#ifndef WRAPPER_FUNCTIONS_H
#define WRAPPER_FUNCTIONS_H

/*####
# Animation Time
####*/
enum ANIMATION_TIME
{
    ANIMATION_TIME_VERY_SLOW    = 1000,
    ANIMATION_TIME_SLOW         = 500,
    ANIMATION_TIME_NORMAL       = 250,
    ANIMATION_TIME_FAST         = 125,
    ANIMATION_TIME_VERY_FAST    = 65,
};

static ANIMATION_TIME WrapAnimationTimeID(UINT aniID)
{
    switch(aniID)
    {
    case 0:
        return ANIMATION_TIME_VERY_SLOW;
    case 1:
        return ANIMATION_TIME_SLOW;
    case 2:
        return ANIMATION_TIME_NORMAL;
    case 3:
        return ANIMATION_TIME_FAST;
    case 4:
        return ANIMATION_TIME_VERY_FAST;
    default:
        return ANIMATION_TIME_NORMAL;
    }
}

/*####
# Movement Speed
####*/
enum MOVEMENT_SPEED
{
    MOVEMENT_SPEED_VERY_SLOW    = 1,
    MOVEMENT_SPEED_SLOW         = 2,
    MOVEMENT_SPEED_NORMAL       = 4,
    MOVEMENT_SPEED_FAST         = 8,
    MOVEMENT_SPEED_VERY_FAST    = 16,
    MOVEMENT_SPEED_TEST         = 100,
};

static MOVEMENT_SPEED WrapMovementSpeedID(UINT speedID)
{
    switch(speedID)
    {
    case 0:
        return MOVEMENT_SPEED_VERY_SLOW;
    case 1:
        return MOVEMENT_SPEED_SLOW;
    case 2:
        return MOVEMENT_SPEED_NORMAL;
    case 3:
        return MOVEMENT_SPEED_FAST;
    case 4:
        return MOVEMENT_SPEED_VERY_FAST;
    case 5:
        return MOVEMENT_SPEED_TEST;
    default:
        return MOVEMENT_SPEED_NORMAL;
    }
}

/*####
# ScriptPointType
####*/
enum ScriptPointType
{
    SCRIPT_POINT_NONE,
    SCRIPT_POINT_TELEPORT,
};

static ScriptPointType WrapToScriptPointType(UINT uiType)
{
    switch(uiType)
    {
    case 1:
        return SCRIPT_POINT_TELEPORT;
    case 0:
    default:
        return SCRIPT_POINT_NONE;
    }
}

/*####
# Direction
####*/
enum DIRECTION
{
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_NONE,
};

static DIRECTION WrapToDirection(UINT uiDir)
{
    switch(uiDir)
    {
    case 0:
        return DIRECTION_UP;
    case 1:
        return DIRECTION_RIGHT;
    case 2:
        return DIRECTION_DOWN;
    case 3:
        return DIRECTION_LEFT;
    default:
        return DIRECTION_NONE;
    }
}

/*####
# Walkmode
####*/
enum WALKMODE
{
    WALKMODE_NONE,
    WALKMODE_RANDOM,
};

static WALKMODE WrapToWalkmode(UINT uiDir)
{
    switch(uiDir)
    {
    case 1:
        return WALKMODE_RANDOM;
    case 0:
    default:
        return WALKMODE_NONE;
    }
}

/*####
# Textbox: shows letters after a specific time
####*/
enum ShowLetterTime
{
    SHOW_LETTER_INSTANT     = 0,
    SHOW_LETTER_VERY_FAST   = 15,
    SHOW_LETTER_FAST        = 30,
    SHOW_LETTER_NORMAL      = 60,
    SHOW_LETTER_SLOW        = 120,
    SHOW_LETTER_VERY_SLOW   = 240,
};

static ShowLetterTime WrapToShowLetterTime(UINT uiDir)
{
    switch(uiDir)
    {
    case 1:
        return SHOW_LETTER_VERY_FAST;
    case 2:
        return SHOW_LETTER_FAST;
    case 3:
        return SHOW_LETTER_NORMAL;
    case 4:
        return SHOW_LETTER_SLOW;
    case 5:
        return SHOW_LETTER_VERY_SLOW;
    case 0:
    default:
        return SHOW_LETTER_INSTANT;
    }
}
#endif;