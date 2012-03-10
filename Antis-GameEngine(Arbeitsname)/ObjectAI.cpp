#include "ObjectAI.h"
#include "Game.h"

ObjectAI::ObjectAI()
{
}

ObjectAI::~ObjectAI(void)
{
}

void ObjectAI::OnUse(Player *pWho)
{
    if (CGame *pGame = CGame::Get())
    {
        pGame->ShowTextbox("Hallo, ich bin ein NPC und teste diesen Text,\nob er sich autoformatiert. Ich hoffe er tut es, sonst kann man das nicht alles lesen :P\nIch muss jetzt aber noch\nein wenig Stuss reden, da der\nText sonst zu kurz ist und ich nicht ordentlich testen kann. Sodele, nun sollte langsam mal eine gute Länge erreicht sein. Trotzdem mach ich weiter, damit ich noch ein wenig mehr testen kann, auch wenn es langsam ziemlich öde wird <.<!",
            4, Point<int>(20, 360), 20, 1000, false, "Comic Sans", SHOW_LETTER_INSTANT, true);
        //pGame->ShowTextbox("fhsdkjfnksdhlkdxnvlknv blahgdfigdfiughsidsdjfoidsfosdjffubduifhsdfjdskflksdnfmsdfahsuifhasnfkdsrahiksfksdnfksdnxkgnxdkfnsdkfnasmfcykmxbfmdnfyd",
        //    4, Point<int>(20, 360), 20, 1000, false, "Comic Sans", SHOW_LETTER_INSTANT, true);
    }
}