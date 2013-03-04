#ifndef OBJECT_AI_H
#define OBJECT_AI_H

class Player;
class WorldObject;

class ObjectAI
{
public:
    ObjectAI();
    virtual ~ObjectAI(void);

    // when used by player
    virtual void OnUse(Player *pWho);

    inline WorldObject* GetOwner() { return m_pOwner; }
    virtual void SetOwner(WorldObject* pOwner) { m_pOwner = pOwner; }

private:
    WorldObject *m_pOwner;
};
#endif;