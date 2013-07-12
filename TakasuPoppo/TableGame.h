//
//  TableGame.h
//  TakasuPoppo
//
//  Created by MinhNT on 13/07/08.
//
//

#ifndef TakasuPoppo_TableGame_h
#define TakasuPoppo_TableGame_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace std;
using namespace cocos2d;
class TableGame : public cocos2d::CCObject
{
private:
	int with;
    int height;
    int rows;
    int cols;
    
    CCPoint point;
	
    CCArray *arraySquare;
	
public:
	TableGame();
	TableGame(CCPoint p, int  with, int height, int rows, int cols);// ham tao co doi so
    
    int getWith();
    void setWith(int w);
    int getHeight();
    void setHeight(int h);
    int getRows();
    void setRows(int r);
    int getCols();
    void setCols(int c);
    CCPoint getPoint();
    void setPoint(CCPoint p);
    
    CCArray * getArraySquare();
    void setArraySquare(CCArray * array);

};


#endif
