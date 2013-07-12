//
//  TableGame.cpp
//  TakasuPoppo
//
//  Created by MinhNT on 13/07/08.
//
//

#include "TableGame.h"
#include "Square.h"
#include "GameManager.h"
TableGame::TableGame()
{
    
}

TableGame::TableGame(CCPoint p, int  with, int height, int rows, int cols)
{
    this->point = p;
    this->with = with;
    this->height = height ;
    this->rows = rows;
    this->cols = cols;
}

CCPoint TableGame::getPoint() {
    return this->point;
}
void TableGame::setPoint(cocos2d::CCPoint p) {
    this->point = p;
}
int TableGame::getWith() {
    return this->with;
}
void TableGame::setWith(int w) {
    this->with = w;
}
int TableGame::getHeight() {
    return this->height;
}
int TableGame::getRows() {
    return this->rows ;
}
void TableGame::setRows(int r) {
    this->rows = r;
}
int TableGame::getCols() {
    return  this->cols;
}
void TableGame::setCols(int c) {
    this->cols = c;
}
CCArray * TableGame::getArraySquare() {
    return this->arraySquare;
}
void TableGame::setArraySquare(cocos2d::CCArray *array) {
    this->arraySquare = array;
}