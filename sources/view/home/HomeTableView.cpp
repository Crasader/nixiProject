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
    touchHouse = cell->getIdx();
    DATA->setHouseIndex(touchHouse);
    
    if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
        kuangSpr->removeFromParentAndCleanup(true);
        kuangSpr = NULL;
    }
    CCSprite* spr = CCSprite::create("res/pic/house/house_kuang.png");
    kuangSpr = CCSprite::create("res/pic/qingjingScene/qj_right.png");
    kuangSpr->setPosition(ccp(spr->getContentSize().width* .8f, spr->getContentSize().height* .9f));
    kuangSpr->setTag(cell->getIdx());
    cell->addChild(kuangSpr, 5);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("HomeUpdataBg");
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
    
    CCDictionary* dic = (CCDictionary* )allHomeArr->objectAtIndex(idx);
    int id = dic->valueForKey("id")->intValue();
    
    CCString* bgStr = CCString::createWithFormat("res/pic/house/icon_%d.png", id);
    CCSprite* bgKuangSpr = CCSprite::create(bgStr->getCString());
    bgKuangSpr->setAnchorPoint(CCPointZero);
    bgKuangSpr->setPosition(CCPointZero);
    bgKuangSpr->setTag(idx);
    spr->addChild(bgKuangSpr);
    
    
    if (atoi(nowHouse.c_str()) == id) {
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        CCSprite* spr = CCSprite::create("res/pic/house/house_kuang.png");
        kuangSpr = CCSprite::create("res/pic/qingjingScene/qj_right.png");
        kuangSpr->setPosition(ccp(spr->getContentSize().width* .8f, spr->getContentSize().height* .9f));
        kuangSpr->setTag(idx);
        pCell->addChild(kuangSpr, 5);
    }
    
    bool haveHomeBool = false;
    haveHomeArr = DATA->getHome()->getHouseUser();
    for (int i = 0; i < haveHomeArr->count(); i++) {
        CCString* haveHomeStr = (CCString* )haveHomeArr->objectAtIndex(i);
        int haveHomeId = atoi(haveHomeStr->getCString());
        CCLog("haveHomeId == %d", haveHomeId);
        CCLog("id == %d", id);
        if (haveHomeId == id) {
            haveHomeBool = true;
            break;
        }
    }
    
    CCSprite* namekuang = CCSprite::create("res/pic/clothesScene/gj_dikuang1.png");
    namekuang->setPosition(ccp(bgKuangSpr->getContentSize().width* .5f, bgKuangSpr->getContentSize().height* .18f));
    bgKuangSpr->addChild(namekuang);
    CCString* nameStr;
    if (id == 1) {
        nameStr = CCString::createWithFormat("欧式风情");
    }else if (id == 2){
        nameStr = CCString::createWithFormat("换装空间");
    }else if (id == 3){
        nameStr = CCString::createWithFormat("田园风光");
    }else if (id == 4){
        nameStr = CCString::createWithFormat("罗马假日");
    }
    CCLabelTTF* nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(132, 22), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel->setPosition(ccp(namekuang->getContentSize().width* .5f, namekuang->getContentSize().height* .5f));
    nameLabel->setColor(ccc3(113, 89, 102));
    namekuang->addChild(nameLabel);
    
    
    if (haveHomeBool) {// 拥有
    }else{
        CCSprite* jgkuang = CCSprite::create("res/pic/clothesScene/gj_dikuang2.png");
        jgkuang->setPosition(ccp(bgKuangSpr->getContentSize().width* .5f, bgKuangSpr->getContentSize().height* .3f));
        bgKuangSpr->addChild(jgkuang);
        CCInteger* cloth_type = (CCInteger*)dic->objectForKey("cost_type");
        if (cloth_type->getValue() == 1) {
            CCSprite* costSpr = CCSprite::create("res/pic/clothesScene/gj_coin.png");
            costSpr->setPosition(ccp(jgkuang->getContentSize().width* .1f, jgkuang->getContentSize().height* .5f));
            costSpr->setScale(.75f);
            jgkuang->addChild(costSpr, 10);
            CCInteger* cost = (CCInteger*)dic->objectForKey("cost_value");
            CCString* costStr = CCString::createWithFormat("%d", cost->getValue());
            CCLabelTTF* costLabel = CCLabelTTF::create(costStr->getCString(), DISPLAY->fangzhengFont(), 17, CCSizeMake(80, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            costLabel->setPosition(ccp(jgkuang->getContentSize().width* .55f, jgkuang->getContentSize().height* .48f));
            costLabel->setColor(ccWHITE);
            jgkuang->addChild(costLabel);
            
        }else if (cloth_type->getValue() == 2){
            CCSprite* costSpr = CCSprite::create("res/pic/clothesScene/gj_gold.png");
            costSpr->setPosition(ccp(jgkuang->getContentSize().width* .1f, jgkuang->getContentSize().height* .5f));
            costSpr->setScale(.75f);
            jgkuang->addChild(costSpr, 10);
            CCInteger* cost = (CCInteger*)dic->objectForKey("cost_value");
            CCString* costStr = CCString::createWithFormat("%d", cost->getValue());
            CCLabelTTF* costLabel = CCLabelTTF::create(costStr->getCString(), DISPLAY->fangzhengFont(), 17, CCSizeMake(80, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            costLabel->setPosition(ccp(jgkuang->getContentSize().width* .55f, jgkuang->getContentSize().height* .48f));
            costLabel->setColor(ccWHITE);
            jgkuang->addChild(costLabel);
            
        }
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

void HomeTableView::updateTableView(){
    if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
        kuangSpr->removeFromParentAndCleanup(true);
        kuangSpr = NULL;
    }
    nowHouse = DATA->getHome()->getCurHouse().c_str();
    pTableView->reloadData();
}











