//
//  CollectTableView.cpp
//  tiegao
//
//  Created by mac on 16-11-14.
//
//

#include "CollectTableView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "AudioManager.h"



CollectTableView::CollectTableView(){
    
}
CollectTableView::~CollectTableView(){
    
}

bool CollectTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    collectMission = DATA->getCollectSource();
    allNumber = collectMission->count();
    
    DATA->setCollectIndex(0);
    
    kuangSpr = CCSprite::create("res/pic/qingjingScene/collect/qj_collectKuang.png");
    kuangSpr->retain();
    
    
    pTableView = CCTableView::create(this, CCSizeMake(172*def_Page_Index, 106));
    pTableView->setDirection(kCCScrollViewDirectionHorizontal);
    pTableView->setAnchorPoint(CCPointZero);
    pTableView->setPosition(CCPointZero);
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    this->addChild(pTableView, 1);
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    
    return true;
}

void CollectTableView::onEnter(){
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, menu_selector(TaskTableView::taskTableCallBack), "TaskTableCallBack", NULL);
    
    CCLayer::onEnter();
}

void CollectTableView::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

//CCTableViewDelegate继承自CCScrollViewDelegate
void CollectTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    
    CCPoint contOffsetPos = pTableView->getContentOffset();
    if (allNumber > 3) {
        if (contOffsetPos.x < pTableView->minContainerOffset().x) {
            pTableView->setContentOffset(CCPoint(pTableView->minContainerOffset().x, contOffsetPos.y ));
        }else if (contOffsetPos.x > pTableView->maxContainerOffset().x){
            pTableView->setContentOffset(CCPoint(pTableView->maxContainerOffset().x, contOffsetPos.y));
        }
    }
}

//点击哪个cell
void CollectTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    AUDIO->common_effect();
    
    if (DATA->getCollectIndex() != cell->getIdx() && cell->getIdx() != allNumber - 1) {
        DATA->setCollectIndex(cell->getIdx());
        
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        kuangSpr = CCSprite::create("res/pic/qingjingScene/collect/qj_collectKuang.png");
        kuangSpr->setAnchorPoint(CCPointZero);
        kuangSpr->setPosition(CCPointZero);
        cell->addChild(kuangSpr);
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification("CollectBgMethods");
    }
}

//每个cell的size
cocos2d::CCSize CollectTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(172, 106);
}

//生成cell
cocos2d::extension::CCTableViewCell* CollectTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    
    CCString* tiaoStr;
    if (idx != allNumber - 1) {
        tiaoStr = CCString::createWithFormat("res/pic/qingjingScene/collect/qj_collectTiao1.png");
    }else{
        tiaoStr = CCString::createWithFormat("res/pic/qingjingScene/collect/qj_collectTiao2.png");
    }
    CCSprite* tiaoSpr = CCSprite::create(tiaoStr->getCString());
    tiaoSpr->setAnchorPoint(CCPointZero);
    tiaoSpr->setPosition(ccp(8.5f, 9.f));
    pCell->addChild(tiaoSpr, 5);
    
    if (DATA->getCollectIndex() == idx) {
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        kuangSpr = CCSprite::create("res/pic/qingjingScene/collect/qj_collectKuang.png");
        kuangSpr->setAnchorPoint(CCPointZero);
        kuangSpr->setPosition(CCPointZero);
        pCell->addChild(kuangSpr);
    }
    
    creatName(tiaoSpr, idx);
    
    return pCell;
}
void CollectTableView::creatName(CCSprite* tiaoSpr, int index){
    CCString* nameStr;
    if (index != allNumber - 1) {
        CCInteger* integer = (CCInteger* )collectMission->objectAtIndex(index);
        if (integer->getValue() == 101) {
            nameStr = CCString::createWithFormat("云诗彤");
        }else if (integer->getValue() == 1002){
            nameStr = CCString::createWithFormat("段飞");
        }else if (integer->getValue() == 1003){
            nameStr = CCString::createWithFormat("秦雪");
        }else if (integer->getValue() == 1004){
            nameStr = CCString::createWithFormat("何岚");
        }else if (integer->getValue() == 1005){
            nameStr = CCString::createWithFormat("韩志明");
        }else if (integer->getValue() == 1006){
            nameStr = CCString::createWithFormat("韩元仓");
        }else if (integer->getValue() == 1007){
            nameStr = CCString::createWithFormat("云鼎");
        }else if (integer->getValue() == 1008){
            nameStr = CCString::createWithFormat("叶芷晴");
        }else if (integer->getValue() == 1009){
            nameStr = CCString::createWithFormat("小酒");
        }else if (integer->getValue() == 1010){
            nameStr = CCString::createWithFormat("苏小雅");
        }else if (integer->getValue() == 1011){
            nameStr = CCString::createWithFormat("柳风");
        }else if (integer->getValue() == 1012){
            nameStr = CCString::createWithFormat("高进");
        }else if (integer->getValue() == 1013){
            nameStr = CCString::createWithFormat("魏天浪");
        }else if (integer->getValue() == 1014){
            nameStr = CCString::createWithFormat("郑成强");
        }
    }else{
        nameStr = CCString::createWithFormat("?");
    }
    CCLabelTTF* nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 26);
    nameLabel->setPosition(ccp(tiaoSpr->getContentSize().width* .7f, tiaoSpr->getContentSize().height* .5f));
    nameLabel->setColor(ccc3(150, 82, 79));
    tiaoSpr->addChild(nameLabel);
}

//cell的数量
unsigned int CollectTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void CollectTableView::tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

//松开的时候，取消高亮状态
void CollectTableView::tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}































