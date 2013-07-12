#ifndef _GameManager_H_
#define _GameManager_H_

#include "cocos2d.h"
#include "TableGame.h"
using namespace std;

class GameManager : public cocos2d::CCObject
{
private:
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    TableGame *tableGame = new TableGame(ccp(0, 0), s.width * 3 / 6, s.width * 3 / 6, 7, 7);
    //Constructor
    GameManager();

    //Instance of the singleton
    static GameManager* m_mySingleton;

public:
    //Get instance of singleton
    static GameManager* sharedGameManager();
    
    TableGame *getTableGame();
};

#endif