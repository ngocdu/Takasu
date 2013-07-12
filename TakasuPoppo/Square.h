//
//  Square.h
//  TakasuPoppo
//
//  Created by MinhNT on 13/07/08.
//
//

#ifndef TakasuPoppo_Square_h
#define TakasuPoppo_Square_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace std;
using namespace cocos2d;
class Square : public cocos2d::CCSprite
{
private:
	int category; //1->7
    int row;
    int col;
    bool selected;
    bool moved;
    bool movedback;
    bool remove = false;
	CCPoint point;
	
public:
	Square();
	Square(int  category, int row, int col);// ham tao co doi so
    
    int getCategory();
    void setCategory(int c);
    int getRow();
    void setRow(int r);
    int getCol();
    void setCol(int col);
    
    bool getSelected();
    void setSelected(bool s);
    
    bool getMoved();
    void setMoved(bool s);
    
    bool getRemove();
    void setRemove(bool s);
    
    bool getMoveBack();
    void setMoveBack(bool s);
    
    CCPoint getPoint();
    void setPoint(CCPoint p);
    
    void moveDown();
    void moveTop();
    void moveLeft();
    void moveRight();
};



#endif
