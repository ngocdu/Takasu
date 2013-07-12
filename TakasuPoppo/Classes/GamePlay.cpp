//
//  HelloWorldScene.cpp
//  EziSocial
//
//  Created by Paras Mendiratta on 11/04/13.
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
#include "Square.h"
#include "GameManager.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include <cstring>
#include <math.h>
using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GamePlay::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GamePlay *layer = GamePlay::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool GamePlay::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    this->setTouchEnabled(true);
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(GamePlay::menuCloseCallback) );
    pCloseItem->setAnchorPoint(ccp(1, 0));
    
    
    
    pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width, 0));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    //------------------------------------Game----------------------------------
    srandom(time(NULL));
    arraySquareMoveDown = new CCArray();
    arraySquareRemove = new CCArray();
    tableGame = GameManager::sharedGameManager()->getTableGame();
    tableGame->setArraySquare(new CCArray());
    this->addArraySquare();
    
    this->schedule(schedule_selector(GamePlay::updateMoveDown), 0.1);
    this->schedule(schedule_selector(GamePlay::updateAdd), 0.1);
    this->schedule(schedule_selector(GamePlay::update), 1/60);
    return true;
}
void GamePlay::menuCloseCallback(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
//----------------------Game----------------------------------------------------
void GamePlay::addArraySquare() {
    srandom(time(NULL));
    float with = tableGame->getWith() / tableGame->getCols() * 1.0f;
    float height = tableGame->getHeight() / tableGame->getRows() * 1.0f;
    int rows = tableGame->getRows();
    int cols = tableGame->getCols();
    CCPoint pointTable = tableGame->getPoint();
    int dem = 0;
    int k = 1;
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            int category = rand() % 7;
            while (category == k) {
                category = rand() % 7;
                if (category == k) {
                     k = category;
                }
            }
            char tileName[7][20];
            strcpy(tileName[0], "Candy1");
            strcpy(tileName[1], "Candy2");
            strcpy(tileName[2], "Candy3");
            strcpy(tileName[3], "Candy4");
            strcpy(tileName[4], "Candy5");
            strcpy(tileName[5], "Candy6");
            strcpy(tileName[6], "Candy7");
            char spriteName[100];
            sprintf(spriteName, "%s.png", tileName[category]);
            Square *sq = new Square(category, i, j);
            sq->setSelected(false);
            sq->initWithFile(spriteName);
            float sx = with / sq->getContentSize().width;
            float sy = height / sq->getContentSize().height;
            sq->setScaleX(sx);
            sq->setScaleY(sy);
            CCPoint p = ccp(pointTable.x + (i + 0.5f) * sq->getContentSize().width * sx,
                            pointTable.y + (j + 0.5f) * sq->getContentSize().height * sy);
            sq->setPosition(p);
            sq->setPoint(p);
            dem++;
            tableGame->getArraySquare()->addObject(sq);
            this->addChild(sq, 1, dem);
            char strTime[20] = {0};
            sprintf(strTime, "%i, %i : %i", i, j, sq->getTag());
            CCLabelTTF *lable = CCLabelTTF::create(strTime, "Time New Roman", 13);
            lable->setPosition(p);
            this->addChild(lable, 10);
        }
    }
}
void GamePlay::updateMoveDown(float dt) {
    CCObject *i;
    CCARRAY_FOREACH(tableGame->getArraySquare(), i) {
        Square * sq = (Square*)i;
        int tag = sq->getTag();
        if (sq->getCol() > 1 && this->getChildByTag(tag - 1) == NULL) {
            sq->moveDown();
        }
    }
}
void GamePlay::update(float dt) {
    this->checkTable();
    CCObject *i;
    CCARRAY_FOREACH(tableGame->getArraySquare(), i) {
        Square * sq = (Square*) i;
        if (this->checkSquareMove(sq) == true) {
            arraySquareRemove->addObject(sq);
        }
    }

    if (arraySquareRemove->count() > 0) {
        CCObject *i;
        CCARRAY_FOREACH(arraySquareRemove, i) {
            Square *sq = (Square *)i;
            CCParticleFlower * flower = CCParticleFlower::create();
            flower->retain();
            flower->setPosition(sq->getPosition());
            flower->setTexture( CCTextureCache::sharedTextureCache()->addImage("stars.png") );
            flower->setDuration(0.5f);
            flower->setScale(0.5f);
            flower->setLife(0.3f);
            flower->setTotalParticles(10);
            this->addChild(flower, 10);
            tableGame->getArraySquare()->removeObject(sq);
            this->removeChild(sq, true);
        }
    }
    arraySquareRemove->removeAllObjects();
}
void GamePlay::updateRemove(float dt) {
    
    if (arraySquareRemove->count() > 0) {
        CCObject *i;
        CCARRAY_FOREACH(arraySquareRemove, i) {
            Square *sq = (Square *)i;
            tableGame->getArraySquare()->removeObject(sq);
            this->removeChild(sq, true);
        }
    }
    arraySquareRemove->removeAllObjects();
}
void GamePlay::checkRemoveAfterAdd(float dt) {
    CCObject *i;
    CCARRAY_FOREACH(tableGame->getArraySquare(), i) {
        Square * sq = (Square*) i;
        if (this->checkSquareMove(sq) == true) {
            arraySquareRemove->addObject(sq);
        }
    }
}
void GamePlay::updateAdd(float dt) {
    srandom(time(NULL));
    float with = tableGame->getWith() / tableGame->getCols() * 1.0f;
    float height = tableGame->getHeight() / tableGame->getRows() * 1.0f;
    int category = rand() % 7;
    char tileName[7][20];
    strcpy(tileName[0], "Candy1");
    strcpy(tileName[1], "Candy2");
    strcpy(tileName[2], "Candy3");
    strcpy(tileName[3], "Candy4");
    strcpy(tileName[4], "Candy5");
    strcpy(tileName[5], "Candy6");
    strcpy(tileName[6], "Candy7");
    char spriteName[100];
    CCPoint pointTable = tableGame->getPoint();
    sprintf(spriteName, "%s.png", tileName[category]);
    if (this->getChildByTag(7) == NULL) {
        Square *sq = new Square(category, 1, 8);
        sq->setSelected(false);
        sq->initWithFile(spriteName);
        float sx = with / sq->getContentSize().width;
        float sy = height / sq->getContentSize().height;
        sq->setScaleX(sx);
        sq->setScaleY(sy);
        CCPoint p = ccp(pointTable.x + (1 + 0.5f) * sq->getContentSize().width * sx,
                        pointTable.y + (8 + 0.5f) * sq->getContentSize().height * sy);

        sq->setPosition(p);
        sq->setPoint(p);
        sq->moveDown();
        tableGame->getArraySquare()->addObject(sq);
        this->addChild(sq, 1, 7);
    }else if(this->getChildByTag(14) == NULL) {
        Square *sq = new Square(category, 2, 8);
        sq->setSelected(false);
        sq->initWithFile(spriteName);
        float sx = with / sq->getContentSize().width;
        float sy = height / sq->getContentSize().height;
        sq->setScaleX(sx);
        sq->setScaleY(sy);
        CCPoint p = ccp(pointTable.x + (2 + 0.5f) * sq->getContentSize().width * sx,
                        pointTable.y + (8 + 0.5f) * sq->getContentSize().height * sy);
        sq->setPosition(p);
        sq->setPoint(p);
        sq->moveDown();
        tableGame->getArraySquare()->addObject(sq);
        this->addChild(sq, 1, 14);
    } else if (this->getChildByTag(21) == NULL) {
        Square *sq = new Square(category, 3, 8);
        sq->setSelected(false);
        sq->initWithFile(spriteName);
        float sx = with / sq->getContentSize().width;
        float sy = height / sq->getContentSize().height;
        sq->setScaleX(sx);
        sq->setScaleY(sy);
        CCPoint p = ccp(pointTable.x + (3 + 0.5f) * sq->getContentSize().width * sx,
                        pointTable.y + (8 + 0.5f) * sq->getContentSize().height * sy);
        sq->setPosition(p);
        sq->setPoint(p);
        sq->moveDown();
        tableGame->getArraySquare()->addObject(sq);
        this->addChild(sq, 1, 21);
    }else if (this->getChildByTag(28) == NULL) {
        Square *sq = new Square(category, 4, 8);
        sq->setSelected(false);
        sq->initWithFile(spriteName);
        float sx = with / sq->getContentSize().width;
        float sy = height / sq->getContentSize().height;
        sq->setScaleX(sx);
        sq->setScaleY(sy);
        CCPoint p = ccp(pointTable.x + (4 + 0.5f) * sq->getContentSize().width * sx,
                        pointTable.y + (8 + 0.5f) * sq->getContentSize().height * sy);
        sq->setPosition(p);
        sq->setPoint(p);
        sq->moveDown();
        tableGame->getArraySquare()->addObject(sq);
        this->addChild(sq, 1, 28);
    }else if (this->getChildByTag(35) == NULL) {
        Square *sq = new Square(category, 5, 8);
        sq->setSelected(false);
        sq->initWithFile(spriteName);
        float sx = with / sq->getContentSize().width;
        float sy = height / sq->getContentSize().height;
        sq->setScaleX(sx);
        sq->setScaleY(sy);
        CCPoint p = ccp(pointTable.x + (5 + 0.5f) * sq->getContentSize().width * sx,
                        pointTable.y + (8 + 0.5f) * sq->getContentSize().height * sy);
        sq->setPosition(p);
        sq->setPoint(p);
        sq->moveDown();
        tableGame->getArraySquare()->addObject(sq);
        this->addChild(sq, 1, 35);
    }else if (this->getChildByTag(42) == NULL) {
        Square *sq = new Square(category, 6, 8);
        sq->setSelected(false);
        sq->initWithFile(spriteName);
        float sx = with / sq->getContentSize().width;
        float sy = height / sq->getContentSize().height;
        sq->setScaleX(sx);
        sq->setScaleY(sy);
        CCPoint p = ccp(pointTable.x + (6 + 0.5f) * sq->getContentSize().width * sx,
                        pointTable.y + (8 + 0.5f) * sq->getContentSize().height * sy);
        sq->setPosition(p);
        sq->setPoint(p);
        sq->moveDown();
        tableGame->getArraySquare()->addObject(sq);
        this->addChild(sq, 1, 42);
    }else if (this->getChildByTag(49) == NULL) {
        Square *sq = new Square(category, 7, 8);
        sq->setSelected(false);
        sq->initWithFile(spriteName);
        float sx = with / sq->getContentSize().width;
        float sy = height / sq->getContentSize().height;
        sq->setScaleX(sx);
        sq->setScaleY(sy);
        CCPoint p = ccp(pointTable.x + (7 + 0.5f) * sq->getContentSize().width * sx,
                        pointTable.y + (8 + 0.5f) * sq->getContentSize().height * sy);
        sq->setPosition(p);
        sq->setPoint(p);
        sq->moveDown();
        tableGame->getArraySquare()->addObject(sq);
        this->addChild(sq, 1, 49);
    }
}
void GamePlay::ccTouchesBegan(cocos2d::CCSet * touch,cocos2d::CCEvent* event)
{
    touchmove = false;
	CCTouch *touch1 = (CCTouch*)(touch->anyObject());
	CCPoint p2 = touch1->getLocationInView();
	touchLocation=CCDirector::sharedDirector()->convertToGL(p2);
    Square * sq1 = (Square*)tableGame->getArraySquare()->objectAtIndex(1);
    int H = (int)(tableGame->getHeight() / 7);
    int W = (int)(tableGame->getWith() / 7);
    int px = (int)(tableGame->getPoint().x +  sq1->getContentSize().width * sq1->getScaleX());
    int py = (int)(tableGame->getPoint().y +  sq1->getContentSize().height * sq1->getScaleY());
    int row = (int)((touchLocation.x - px) / W ) + 1;
    int col = (int)((touchLocation.y - py) / H ) + 1;
    int tag = (row - 1) * 7 + col;
    if (this->getChildByTag(tag) != NULL) {
        if (sqSelected == NULL || (sqSelected != NULL && sqSelected->getSelected() == false)) {
            sqSelected = (Square*)this->getChildByTag(tag);
            sqSelected->setSelected(true);
        }else if (sqSelected2 == NULL || (sqSelected2 != NULL && sqSelected2->getSelected() == false)) {
            sqSelected2 = (Square*)this->getChildByTag(tag);
            sqSelected2->setSelected(true);

        }
        
    }
//    CCObject *i;
//    CCARRAY_FOREACH(tableGame->getArraySquare(), i) {
//        Square * sq = (Square*) i;
//        int kc = ccpDistance(touchLocation, sq->getPosition());
//        if (kc < sq->getContentSize().width/2) {
//            if (sqSelected == NULL || (sqSelected != NULL && sqSelected->getSelected() == false)) {
//                sq->setSelected(true);
//                sqSelected = sq;
//                CCSprite *star = CCSprite::create("Star.png");
//                star->setPosition(ccp(sq->getContentSize().width/2, sq->getContentSize().height/2));
//                sqSelected->addChild(star);
//                CCLog("tag1: %i, row: %i, col : %i, category: %i",sqSelected->getTag(),
//                      sqSelected->getRow(), sqSelected->getCol(), sqSelected->getCategory());
//                break;
//            }else if (sqSelected2 == NULL || (sqSelected2 != NULL && sqSelected2->getSelected() == false)){
//                sq->setSelected(true);
//                sqSelected2 = sq;
//                
//                CCSprite *star = CCSprite::create("Star.png");
//                star->setPosition(ccp(sq->getContentSize().width/2, sq->getContentSize().height/2));
//                sqSelected2->addChild(star);
//                CCLog("tag2: %i, row: %i, col : %i, category: %i",sqSelected2->getTag(),
//                      sqSelected2->getRow(), sqSelected2->getCol(), sqSelected2->getCategory());
//                break;
//            } 
//        }
//    }
}
void GamePlay::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if (sqSelected) {
        CCTouch *touch = (CCTouch*)touches->anyObject();
        CCPoint p2 = touch->getLocationInView();
        CCPoint touchpoint = CCDirector::sharedDirector()->convertToGL(p2);
        
        CCRect touchRect = CCRect(touchpoint.x, touchpoint.y, 10, 10);
        
        CCRect swipeRightRect = CCRectMake((touchLocation.x + 40), touchLocation.y, 80, 20);
        CCRect swipeLeftRect = CCRectMake((touchLocation.x - 40), touchLocation.y, 80, 20);
        CCRect swipeUpRect = CCRectMake(touchLocation.x, touchLocation.y + (40), 20, 80);
        CCRect swipeDownRect = CCRectMake(touchLocation.x, touchLocation.y - (40), 20, 80);
        int tag = sqSelected->getTag();
        if ((touchpoint.x - touchLocation.x > 10) && touchRect.intersectsRect(swipeRightRect) && touchmove == false) {
//            CCLog(" phai");
            touchmove = true ;
            if (this->getChildByTag(tag + 7) != NULL && sqSelected->getRow() < 7) {
                Square * sq = (Square *)this->getChildByTag(tag + 7);
                sqSelected->moveRight();
                sqSelected->setSelected(false);
                sq->moveLeft();
            }
        }
        else if ((touchLocation.x - touchpoint.x > 10) && touchRect.intersectsRect(swipeLeftRect) && touchmove == false) {
//            CCLog(" trai");
            touchmove = true ;
            if (this->getChildByTag(tag - 7) != NULL && sqSelected->getRow() > 1) {
                Square * sq = (Square *)this->getChildByTag(tag - 7);
                sqSelected->moveLeft();
                sqSelected->setSelected(false);
                sq->moveRight();
            }
        }
        else if ((touchpoint.y - touchLocation.y > 10) && touchRect.intersectsRect(swipeUpRect) && touchmove == false) {
//           CCLog(" tren");
           touchmove = true ;
            if (this->getChildByTag(tag + 1) != NULL && sqSelected->getCol() < 7) {
                Square * sq = (Square *)this->getChildByTag(tag + 1);
                sqSelected->moveTop();
                sqSelected->setSelected(false);
                sq->moveDown();
            }
        }
        else if ((touchLocation.y - touchpoint.y > 10) && touchRect.intersectsRect(swipeDownRect) && touchmove == false) {
//            CCLog(" duoi");
            touchmove = true ;
            if (this->getChildByTag(tag - 1) != NULL && sqSelected->getCol() > 1) {
                Square * sq = (Square *)this->getChildByTag(tag - 1);
                sqSelected->moveDown();
                sqSelected->setSelected(false);
                sq->moveTop();
            }
        }
        else if (!touchRect.intersectsRect(swipeRightRect) && !touchRect.intersectsRect(swipeLeftRect)
                 && !touchRect.intersectsRect(swipeUpRect) && !touchRect.intersectsRect(swipeDownRect)){
            
        }
            
    }
}
void GamePlay::ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event)
{
    if (touchmove == false) {
        CCTouch *touch1 = (CCTouch*)(touches->anyObject());
        CCPoint p2 = touch1->getLocationInView();
        CCPoint touchpoint = CCDirector::sharedDirector()->convertToGL(p2);
        int k1 = 0;
        int k2 = 0;
        if (sqSelected != NULL && sqSelected2 != NULL && sqSelected->getSelected() == true &&
            sqSelected2->getSelected() == true) {
            int c = asb(sqSelected->getCol() - sqSelected2->getCol());
            int r = asb(sqSelected2->getRow() - sqSelected->getRow());
            if (c == 1 && r == 0) {
                if (sqSelected->getCol() - sqSelected2->getCol() == -1) {
                    sqSelected->moveTop();
                    sqSelected2->moveDown();
                    k1 = 2;
                    k2 = 1;
                }else if (sqSelected->getCol() - sqSelected2->getCol() == 1) {
                    sqSelected2->moveTop();
                    sqSelected->moveDown();
                    k1 = 1;
                    k2 = 2;
                }
            }else if(r == 1 && c == 0) {
                if (sqSelected2->getRow() - sqSelected->getRow() == -1) {
                    sqSelected2->moveRight();
                    sqSelected->moveLeft();
                    k1 = 3;
                    k2 = 4;
                }else if (sqSelected2->getRow() - sqSelected->getRow() == 1) {
                    sqSelected->moveRight();
                    sqSelected2->moveLeft();
                    k1 = 4;
                    k2 = 3;
                }
            }
            if (this->checkSquareMove(sqSelected) == false && this->checkSquareMove(sqSelected2) == false) {
                switch (k1) {
                    case 1:
                        sqSelected->moveTop();
                        break;
                    case 2:
                        sqSelected->moveDown();
                        break;
                    case 3:
                        sqSelected->moveRight();
                        break;
                    case 4:
                        sqSelected->moveLeft();
                        break;
                    default:
                        break;
                }
                switch (k2) {
                    case 1:
                        sqSelected2->moveTop();
                        break;
                    case 2:
                        sqSelected2->moveDown();
                        break;
                    case 3:
                        sqSelected2->moveRight();
                        break;
                    case 4:
                        sqSelected2->moveLeft();
                        break;
                    default:
                        break;
                }
            }
            sqSelected2 = NULL;
            sqSelected = NULL;
            CCObject *i;
            CCARRAY_FOREACH(tableGame->getArraySquare(), i) {
                Square * sq = (Square*) i;
                if (sq->getSelected() == true) {
                    sq->setSelected(false);
                    sq->removeAllChildren();
                }
            }
        }
    }else if (touchmove == true) {
        sqSelected = NULL;
        
    }
}
void GamePlay::moveBack(cocos2d::CCNode *node, int k) {
    Square *sq = (Square*)node;
    if (arraySquareRemove->count() > 0) {
        if (k == 1) sq->moveDown();
        else if (k == 2) sq->moveTop();
        else if (k == 3) sq->moveLeft();
        else if (k == 4) sq->moveRight();
    }
}
bool GamePlay::checkSquareMove(Square *sq) {
    bool t = false;
    int category = sq->getCategory();
    int tag = sq->getTag();
    int row = sq->getRow();
    int col = sq->getCol();
    if (tag + 1 <= 49 && tag + 2 <= 49 && col < 6) {
        if (this->getChildByTag(tag + 1) != NULL && this->getChildByTag(tag + 2) != NULL) {
            Square * sq1 = (Square*) this->getChildByTag(tag + 1);
            Square * sq2 = (Square*) this->getChildByTag(tag + 2);
            if (sq1->getCategory() == category && sq2->getCategory() == category) {
                t = true;
                arraySquareRemove->addObject(sq);
            }
        }
    }
    if (tag + 1 <= 49 && tag - 1 > 0  && col != 1 && col != 7) {
        if (this->getChildByTag(tag + 1) != NULL && this->getChildByTag(tag - 1) != NULL) {
            Square * sq1 = (Square*) this->getChildByTag(tag + 1);
            Square * sq2 = (Square*) this->getChildByTag(tag - 1);
            if (sq1->getCategory() == category && sq2->getCategory() == category) {
                t = true;
                arraySquareRemove->addObject(sq);
            }
        }
    }
    if (tag - 1 > 0 && tag - 2 > 0 && col > 2) {
        if (this->getChildByTag(tag - 1) != NULL && this->getChildByTag(tag - 2) != NULL) {
            Square * sq1 = (Square*) this->getChildByTag(tag - 1);
            Square * sq2 = (Square*) this->getChildByTag(tag - 2);
            if (sq1->getCategory() == category && sq2->getCategory() == category) {
                t = true;
                arraySquareRemove->addObject(sq);
            }
        }
    }
    if (tag - 7 > 0 && tag - 14 > 0 && row > 2) {
        if (this->getChildByTag(tag - 7) != NULL && this->getChildByTag(tag - 14) != NULL) {
            Square * sq1 = (Square*) this->getChildByTag(tag - 7);
            Square * sq2 = (Square*) this->getChildByTag(tag - 14);
            if (sq1->getCategory() == category && sq2->getCategory() == category) {
                t = true;
                arraySquareRemove->addObject(sq);
            }
        }
    }
    if (tag - 7 > 0 && tag + 7 <= 49 && row != 1 && row != 7 ) {
        if (this->getChildByTag(tag - 7) != NULL && this->getChildByTag(tag + 7) != NULL) {
            Square * sq1 = (Square*) this->getChildByTag(tag - 7);
            Square * sq2 = (Square*) this->getChildByTag(tag + 7);
            if (sq1->getCategory() == category && sq2->getCategory() == category) {
                t = true;
                arraySquareRemove->addObject(sq);
            }
        }
    }
    if (tag + 7 <= 49 && tag + 14 <= 49 && row < 6) {
        if (this->getChildByTag(tag + 7) != NULL && this->getChildByTag(tag + 14) != NULL) {
            Square * sq1 = (Square*) this->getChildByTag(tag + 7);
            Square * sq2 = (Square*) this->getChildByTag(tag + 14);
            if (sq1->getCategory() == category && sq2->getCategory() == category) {
                t = true;
                arraySquareRemove->addObject(sq);
            }
        }
    }
    return t;
}
void GamePlay::checkTable() {
    int point4 = 0;
    int point3 = 0;
    int point5 = 0;
    int k1 = 1;
    int k2 = 1;
    //-----------col -------------------
    for (int i = 1; i <= 7; i ++) {
        int d = 0;
        for (int j = 1 + (k1-1) * 7; j <= k1 * 7; j++) {
            if (this->getChildByTag(j) != NULL) {
                int t = 0;
                Square *sq = (Square*)this->getChildByTag(j);
                //            CCLog("category of %i : %i", j, sq->getCategory());
                if (j > 1 + (k1-1) * 7 + 1) {
                    if (this->getChildByTag(j - 1) != NULL && this->getChildByTag(j - 2) != NULL)
                    {
                        Square *sq1 = (Square*)this->getChildByTag(j - 1);
                        Square *sq2 = (Square*)this->getChildByTag(j - 2);
                        if (sq->getCategory() == sq1->getCategory() &&
                            sq->getCategory() == sq2->getCategory()) {
                            t++;
                            sq->setRemove(true);
                        }
                    }
                }
                if (j < (k1 * 7) - 1) {
                    if (this->getChildByTag(j + 1) != NULL && this->getChildByTag(j + 2) != NULL)
                    {
                        Square *sq1 = (Square*)this->getChildByTag(j + 1);
                        Square *sq2 = (Square*)this->getChildByTag(j + 2);
                        if (sq->getCategory() == sq1->getCategory() &&
                            sq->getCategory() == sq2->getCategory()) {
                            t++;
                            sq->setRemove(true);
                        }
                    }
                }
                if (j < (k1 * 7) && j > 1 + (k1-1) * 7) {
                    if (this->getChildByTag(j + 1) != NULL && this->getChildByTag(j - 1) != NULL)
                    {
                        Square *sq1 = (Square*)this->getChildByTag(j + 1);
                        Square *sq2 = (Square*)this->getChildByTag(j - 1);
                        if (sq->getCategory() == sq1->getCategory() &&
                            sq->getCategory() == sq2->getCategory()) {
                            t++;
                            sq->setRemove(true);
                        }
                    }
                }
                if (t != 0) {
                    d++;
                    t = 0;
                }
            }
        }
        if (d == 3) point3 ++;
        else if (d == 4) {
            point4 ++;
            Square *sq = (Square*)this->getChildByTag(1 + (k1-1) * 7 +  3);
            sq->setRemove(false);
            CCSprite * sprite = CCSprite::create("ball.png");
            sprite->setScale(0.5f);
            sprite->setPosition(ccp(sq->getContentSize().width/2, sq->getContentSize().height/2));
            sq->addChild(sprite, 10);
        }else if (d == 5) {
            point5 ++;
            Square *sq = (Square*)this->getChildByTag(1 + (k1-1) * 7 +  3);
            sq->setRemove(false);
            CCSprite * sprite = CCSprite::create("ball.png");
            sprite->setScale(0.5f);
            sprite->setPosition(ccp(sq->getContentSize().width/2, sq->getContentSize().height/2));
            sq->addChild(sprite, 10);
        }

        d = 0;
        k1++;
    }
    //------------------------------- row ---------------------------
    for (int i = 1; i <= 7; i ++) {
        int d = 0;
        for (int j = i; j <= i + 42; j = j + 7) {
            if (this->getChildByTag(j) != NULL) {
                int t = 0;
                Square *sq = (Square*)this->getChildByTag(j);
                if (j >= j + 14) {
                    if (this->getChildByTag(j - 7) != NULL && this->getChildByTag(j - 14) != NULL)
                    {
                        Square *sq1 = (Square*)this->getChildByTag(j - 7);
                        Square *sq2 = (Square*)this->getChildByTag(j - 14);
                        if (sq->getCategory() == sq1->getCategory() &&
                            sq->getCategory() == sq2->getCategory()) {
                            t++;
                            sq->setRemove(true);
                        }
                    }
                }
                if (j <= i + 42 - 14) {
                    if (this->getChildByTag(j + 7) != NULL && this->getChildByTag(j + 14) != NULL)
                    {
                        Square *sq1 = (Square*)this->getChildByTag(j + 7);
                        Square *sq2 = (Square*)this->getChildByTag(j + 14);
                        if (sq->getCategory() == sq1->getCategory() &&
                            sq->getCategory() == sq2->getCategory()) {
                            t++;
                            sq->setRemove(true);
                        }
                    }
                }
                if (j <= i + 42 - 7 && j >= i + 7 ) {
                    if (this->getChildByTag(j + 7) != NULL && this->getChildByTag(j - 7) != NULL)
                    {
                        Square *sq1 = (Square*)this->getChildByTag(j + 7);
                        Square *sq2 = (Square*)this->getChildByTag(j - 7);
                        if (sq->getCategory() == sq1->getCategory() &&
                            sq->getCategory() == sq2->getCategory()) {
                            t++;
                            sq->setRemove(true);
                        }
                    }
                }
                if (t != 0) {
                    d++;
                    t = 0;
                }
            }
        }
        d++;
        if (d == 3) point3 ++;
        else if (d == 4) {
          point4 ++;
          Square *sq = (Square*)this->getChildByTag(i + 7 * 3);
          sq->setRemove(false);
          CCSprite * sprite = CCSprite::create("ball.png");
            sprite->setScale(0.5f);
            sprite->setPosition(ccp(sq->getContentSize().width/2, sq->getContentSize().height/2));
            sq->addChild(sprite, 10);
        }else if (d == 5) {
            point5 ++;
            Square *sq = (Square*)this->getChildByTag(i + 7 * 3);
            sq->setRemove(false);
            CCSprite * sprite = CCSprite::create("ball.png");
            sprite->setScale(0.5f);
            sprite->setPosition(ccp(sq->getContentSize().width/2, sq->getContentSize().height/2));
            sq->addChild(sprite, 10);
        }
        d = 0;
    }
    
//    CCObject *i;
//    CCARRAY_FOREACH(tableGame->getArraySquare(), i) {
//        Square *sq = (Square*)i;
//        if (sq->getRemove() == true) {
//            arraySquareRemove->addObject(sq);
//        }
//    }
//    
//    if (arraySquareRemove->count() > 0) {
//        CCObject *i;
//        CCARRAY_FOREACH(arraySquareRemove, i) {
//            Square *sq = (Square *)i;
//            tableGame->getArraySquare()->removeObject(sq);
//            this->removeChild(sq, true);
//        }
//        arraySquareRemove->removeAllObjects();
//    }
    
    if (point3 != 0 || point4 != 0 || point5 != 0) {
        CCLog("point col 3 - %i, 4 - %i, 5 - %i",point3, point4, point5);
    }
}
int GamePlay::asb(int a) {
    if (a > 0) {
        return a;
    }else return -a;
}