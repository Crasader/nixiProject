//
//  HomeTableView.cpp
//  tiegao
//
//  Created by mac on 16-8-5.
//
//

#include "HomeTableView.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "DisplayManager.h"



HomeTableView::HomeTableView(){
    
}
HomeTableView::~HomeTableView(){
    
}

bool HomeTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
    touchHouse = 0;
    
    allHomeArr = DATA->getHome()->getHouseTemplate();
    allNumber = allHomeArr->count();
    nowHouse = DATA->getHome()->getCurHouse().c_str();
    haveHomeArr = DATA->getHome()->getHouseUser();
    
    kuangSpr = CCSprite::create("res/pic/qingjingScene/qj_right.png");
    kuangSpr->retain();
    
    pTableView = CCTableView::create(this, CCSizeMake(157, 189* def_HomePage_Index));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setAnchorPoint(CCPointZero);
    pTableView->setPosition(CCPointZero);
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    this->addChild(pTableView, 1);
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    
    
    return true;
}

//CCTableViewDelegate继承自CCScrollViewDelegate
void HomeTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    CCPoint contOffsetPos = pTableView->getContentOffset();
    if (allNumber > 10) {
        if (contOffsetPos.y < pTableView->minContainerOffset().y) {
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->minContainerOffset().y));
        }else if (contOffsetPos.y > pTableView->maxContainerOffset().y){
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->maxContainerOffset().y));
        }
    }
}

//点击哪个cell
void HomeTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    if (atoi(nowHouse.c_str()) != cell->getIdx() + 1) {
        CCString* str = CCString::createWithFormat("%d", cell->getIdx() + 1);
        DATA->getHome()->setCurHouse(str->getCString());
    }
}

//每个cell的size
cocos2d::CCSize HomeTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(157, 189);
}

//生成cell
cocos2d::extension::CCTableViewCell* HomeTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    
    CCString* bgStr = CCString::createWithFormat("res/pic/house/icon_%d.png", idx + 1);
    CCSprite* bgKuangSpr = CCSprite::create(bgStr->getCString());
    bgKuangSpr->setAnchorPoint(CCPointZero);
    bgKuangSpr->setPosition(CCPointZero);
    spr->addChild(bgKuangSpr);
    
    if (atoi(nowHouse.c_str()) == idx + 1) {
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        kuangSpr = CCSprite::create("res/pic/qingjingScene/qj_right.png");
        kuangSpr->setPosition(ccp(bgKuangSpr->getContentSize().width* .8f, bgKuangSpr->getContentSize().height* .9f));
        kuangSpr->setTag(idx);
        bgKuangSpr->addChild(kuangSpr, 5);
    }
    
    
    
    pCell->addChild(spr);
    
    return pCell;
}

//cell的数量
unsigned int HomeTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void HomeTableView::tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

//松开的时候，取消高亮状态
void HomeTableView::tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

void HomeTableView::onEnter(){
    CCLayer::onEnter();
    
    
}

void HomeTableView::onExit(){
    
    
    CCLayer::onExit();
}












