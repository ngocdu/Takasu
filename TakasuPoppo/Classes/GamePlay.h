//
//  HelloWorldScene.h
//  EziSocial

//  Copyright @EziByte 2013 (http://www.ezibyte.com)
//
//  Version 1.2 (Dt: 30-May-2013)
//
/***
 
 This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source distribution.
 
 */

#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__
#include "TableGame.h"
#include "cocos2d.h"
#include "Square.h"
class GamePlay :  public cocos2d::CCLayer
{
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GamePlay);
    
    //---------------------Game-------------------------------------------------
    virtual void ccTouchesBegan(cocos2d::CCSet * touch,cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touch,cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
    void addArraySquare();
    void updateMoveDown(float dt);
    void moveDown();
    void updateRemove(float dt);
    void remove();
    void updateAdd(float dt);
    void add();
    void update(float dt);
    void updateTime(float dt);
    void updateMoveBack(float dt);
    void moveBack();
    void checkMove();
    void checkRemoveAfterAdd(float dt) ;
    bool checkSquareMove(Square *sq);
    bool checkSquareMoveAble(Square *sq);
    void moveBack(cocos2d::CCNode *node, int k);
    void checkTable();
    int asb(int a);
private:
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    TableGame *tableGame;
    CCPoint touchLocation;
    bool touchmove = false;
    Square *sqSelected;
    Square *sqSelected2;
    Square *sqSelected1;
    Square *sqSelected3;
    int directionMoveSqSelected; // 1 : dow, 2 :top, 3 :right, 4 :left 
    CCArray *arraySquareRemove;
    CCArray *arraySquareMoveDown;
    int arrayCategoryRemove[7];
    int _threes;
    int _fours;
    int _fives;
    int _times;
    CCLabelTTF *_labelTime;
    CCLabelTTF *_labelThrees;
    CCLabelTTF *_labelFours;
    CCLabelTTF *_labelFives;
};
#endif // __HELLOWORLD_SCENE_H__
