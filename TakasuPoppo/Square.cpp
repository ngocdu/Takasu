//
//  Square.cpp
//  TakasuPoppo
//
//  Created by MinhNT on 13/07/08.
//
//
#include "GameManager.h"
#include "Square.h"
Square::Square()
{
    
}

Square::Square(int category, int row, int col)
{
    this->category = category;
    this->row = row;
    this->col = col;
}

int Square::getCategory() {
    return  this->category;
}
void Square::setCategory(int c) {
    this->category = c;
}
int Square::getRow() {
    return  this->row;
}
void Square::setRow(int r) {
    this->row = r;
}
int Square::getCol() {
    return  this->col;
}
void Square::setCol(int col) {
    this->col = col;
}
bool Square::getSelected() {
    return this->selected;
}
void Square::setSelected(bool s) {
    this->selected = s;
}
CCPoint Square::getPoint() {
    return this->point;
}
void Square::setPoint(cocos2d::CCPoint p) {
    this->point = p;
}
bool Square::getRemove() {
    return this->remove;
}
void Square::setRemove(bool s) {
    this->remove = s;
}
void Square::moveDown() {
    if (this->getCol() > 1) {
        float with = GameManager::sharedGameManager()->getTableGame()->getWith() /
          GameManager::sharedGameManager()->getTableGame()->getCols() * 1.0f;
        float height = GameManager::sharedGameManager()->getTableGame()->getHeight() /
          GameManager::sharedGameManager()->getTableGame()->getRows() * 1.0f;
        float sx = with / this->getContentSize().width;
        float sy = height / this->getContentSize().height;
        int c = this->getCol() - 1;
        this->setCol(c);
        int tag = this->getTag() - 1 ;
        this->setTag(tag);
        CCPoint p = ccp(this->getPosition().x, this->getPosition().y - this->getContentSize().height * sy);
        CCMoveTo * move = CCMoveTo::create(0.2f, p);
        this->runAction(move);
    }
}
void Square::moveTop() {
    if (this->getCol() < 7) {
        float with = GameManager::sharedGameManager()->getTableGame()->getWith() /
        GameManager::sharedGameManager()->getTableGame()->getCols() * 1.0f;
        float height = GameManager::sharedGameManager()->getTableGame()->getHeight() /
        GameManager::sharedGameManager()->getTableGame()->getRows() * 1.0f;
        float sx = with / this->getContentSize().width;
        float sy = height / this->getContentSize().height;
        int c = this->getCol() + 1;
        this->setCol(c);
        int tag = this->getTag() + 1 ;
        this->setTag(tag);
        CCPoint p = ccp(this->getPosition().x, this->getPosition().y + this->getContentSize().height * sy);
        CCMoveTo * move = CCMoveTo::create(0.2f, p);
        this->runAction(move);
    }
}
void Square::moveLeft() {
    if (this->getRow() > 1) {
        float with = GameManager::sharedGameManager()->getTableGame()->getWith() /
        GameManager::sharedGameManager()->getTableGame()->getCols() * 1.0f;
        float height = GameManager::sharedGameManager()->getTableGame()->getHeight() /
        GameManager::sharedGameManager()->getTableGame()->getRows() * 1.0f;
        float sx = with / this->getContentSize().width;
        float sy = height / this->getContentSize().height;
        int r = this->getRow() - 1;
        this->setRow(r);
        int tag = this->getTag() - 7 ;
        this->setTag(tag);

        CCPoint p = ccp(this->getPosition().x - this->getContentSize().width * sx, this->getPosition().y );
        CCMoveTo * move = CCMoveTo::create(0.2f, p);
        this->runAction(move);
    }
}
void Square::moveRight() {
    if (this->getRow() < 7) {
        float with = GameManager::sharedGameManager()->getTableGame()->getWith() /
        GameManager::sharedGameManager()->getTableGame()->getCols() * 1.0f;
        float height = GameManager::sharedGameManager()->getTableGame()->getHeight() /
        GameManager::sharedGameManager()->getTableGame()->getRows() * 1.0f;
        float sx = with / this->getContentSize().width;
        float sy = height / this->getContentSize().height;
        int r = this->getRow() + 1;
        this->setRow(r);
        int tag = this->getTag() + 7 ;
        this->setTag(tag);
        CCPoint p = ccp(this->getPosition().x + this->getContentSize().width * sx, this->getPosition().y );
        CCMoveTo * move = CCMoveTo::create(0.2f, p);
        this->runAction(move);
    }
}