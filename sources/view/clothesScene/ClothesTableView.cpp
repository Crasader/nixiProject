//
//  ClothesTableView.cpp
//  tiegao
//
//  Created by mac on 16-6-15.
//
//

#include "ClothesTableView.h"
#include "DataManager.h"
#include "ClothesScene.h"
#include "ConfigManager.h"
#include "DisplayManager.h"


ClothesTableView::ClothesTableView(){
    
}
ClothesTableView::~ClothesTableView(){
    
}

bool ClothesTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    clothesPage = 0;
    clothesType = 0;
    
    this->clothesType = Tag_GJ_TouFa;
    
    kuangSpr = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr->retain();
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    CCArray* clothesArr = (CCArray* )dic->objectForKey(clothesType);// 获得当前类型所有衣服
    CCArray* tempArr = CCArray::create();
    
    for (int i = 0; i < clothesArr->count(); i++) {
        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
        int sale = clothDic->valueForKey("sale")->intValue();
        if (sale != 0) {
            tempArr->addObject(clothDic);
        }
    }
    DATA->setDataSource(tempArr);
    
    
    pTableView = CCTableView::create(this, CCSizeMake(184, 193* def_Page_Index));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setAnchorPoint(CCPointZero);
    pTableView->setPosition(ccp(0, 0));
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    this->addChild(pTableView, 1);
    
    offsetFloat = pTableView->getContentOffset().y;
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    
    return true;
}

//CCTableViewDelegate继承自CCScrollViewDelegate
void ClothesTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    
}

//点击哪个cell
void ClothesTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    CCArray* arr = DATA->getDataSource();
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp(); // 临时数组
    
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(cell->getIdx());
    int cloth_id = dic->valueForKey("id")->intValue(); // 点击的衣服
    CCInteger* cloth_integer;
    
    CCInteger* clothesTemp_id = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString()); // 男宠当前所穿上衣
    
    if (cloth_id == clothesTemp_id->getValue()) {
        cloth_id = updataClothes(clothesType);
        
        cloth_integer = CCInteger::create(cloth_id);
        CCString* keyStr = CCString::createWithFormat("%d", clothesType);
        clothesTemp->setObject(cloth_integer, keyStr->getCString());
        
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ButtonStatus", NULL);
    }else{
        cloth_integer = CCInteger::create(cloth_id);
        CCString* keyStr = CCString::createWithFormat("%d", clothesType);
        clothesTemp->setObject(cloth_integer, keyStr->getCString());
        
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        
        CCNode* node = cell->getChildByTag(cell->getIdx());
        kuangSpr = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
        kuangSpr->setPosition(ccp(18, 170));
        kuangSpr->setTag(cell->getIdx());
        node->addChild(kuangSpr, 5);
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ButtonStatus", NULL);
    }
    
}

//每个cell的size
cocos2d::CCSize ClothesTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(184, 193);
}

//生成cell
cocos2d::extension::CCTableViewCell* ClothesTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp(); // 临时数组
    CCArray* arr = DATA->getDataSource();
    
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(idx);
    int cloth_id = dic->valueForKey("id")->intValue(); // 男宠当前所穿上衣
    CCString* str;
    if (clothesType == Tag_GJ_TouFa) {
        str = CCString::createWithFormat("res/pic/clothesScene/icon/1toufa/icon%d.png", cloth_id);
    }else if (clothesType == Tag_GJ_WaiTao) {
        str = CCString::createWithFormat("res/pic/clothesScene/icon/2waitao/icon%d.png", cloth_id);
    }else if (clothesType == Tag_GJ_ShangYi){
        str = CCString::createWithFormat("res/pic/clothesScene/icon/3shangyi/icon%d.png", cloth_id);
    }else if (clothesType == Tag_GJ_KuZi){
        str = CCString::createWithFormat("res/pic/clothesScene/icon/4kuzi/icon%d.png", cloth_id);
    }else if (clothesType == Tag_GJ_WaZi){
        str = CCString::createWithFormat("res/pic/clothesScene/icon/5wazi/icon%d.png", cloth_id);
    }else if (clothesType == Tag_GJ_XieZi){
        str = CCString::createWithFormat("res/pic/clothesScene/icon/6xiezi/icon%d.png", cloth_id);
    }else if (clothesType == Tag_GJ_ShiPin){
        str = CCString::createWithFormat("res/pic/clothesScene/icon/7shipin/icon%d.png", cloth_id);
    }else if (clothesType == Tag_GJ_Bao){
        str = CCString::createWithFormat("res/pic/clothesScene/icon/8bao/icon%d.png", cloth_id);
    }
//    str = CCString::createWithFormat("res/pic/clothesScene/111.png");
    CCSprite* clothSpr = CCSprite::create(str->getCString());
    clothSpr->setAnchorPoint(CCPointZero);
    clothSpr->setPosition(ccp(0, 0));
    clothSpr->setTag(idx);
    spr->addChild(clothSpr, 3);
    // 名字
    CCSprite* namekuang = CCSprite::create("res/pic/clothesScene/gj_dikuang1.png");
    namekuang->setPosition(ccp(clothSpr->getContentSize().width* .5f, clothSpr->getContentSize().height* .1f));
    clothSpr->addChild(namekuang);
    const CCString* nameStr = dic->valueForKey("name");
    CCLabelTTF* nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(132, 22), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel->setPosition(ccp(namekuang->getContentSize().width* .5f, namekuang->getContentSize().height* .5f));
    nameLabel->setColor(ccc3(113, 89, 102));
    namekuang->addChild(nameLabel);
    
    // 价格
    if (DATA->getClothes()->is_owned(clothesType, cloth_id)) {
        // 购买了
    }else{
        CCSprite* jgkuang = CCSprite::create("res/pic/clothesScene/gj_dikuang2.png");
        jgkuang->setPosition(ccp(clothSpr->getContentSize().width* .5f, clothSpr->getContentSize().height* .22f));
        clothSpr->addChild(jgkuang);
        int cloth_type = dic->valueForKey("type")->intValue(); // 男宠当前所穿上衣
        if (cloth_type == 1) {
            CCSprite* costSpr = CCSprite::create("res/pic/clothesScene/gj_coin.png");
            costSpr->setPosition(ccp(jgkuang->getContentSize().width* .1f, jgkuang->getContentSize().height* .5f));
            costSpr->setScale(.75f);
            jgkuang->addChild(costSpr, 10);
            const CCString* costStr = dic->valueForKey("cost");
            CCLabelTTF* costLabel = CCLabelTTF::create(costStr->getCString(), DISPLAY->fangzhengFont(), 17, CCSizeMake(80, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            costLabel->setPosition(ccp(jgkuang->getContentSize().width* .55f, jgkuang->getContentSize().height* .48f));
            costLabel->setColor(ccWHITE);
            jgkuang->addChild(costLabel);
            
        }else if (cloth_type == 2){
            CCSprite* costSpr = CCSprite::create("res/pic/clothesScene/gj_gold.png");
            costSpr->setPosition(ccp(jgkuang->getContentSize().width* .1f, jgkuang->getContentSize().height* .5f));
            costSpr->setScale(.75f);
            jgkuang->addChild(costSpr, 10);
            const CCString* costStr = dic->valueForKey("cost");
            CCLabelTTF* costLabel = CCLabelTTF::create(costStr->getCString(), DISPLAY->fangzhengFont(), 17, CCSizeMake(80, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            costLabel->setPosition(ccp(jgkuang->getContentSize().width* .55f, jgkuang->getContentSize().height* .48f));
            costLabel->setColor(ccWHITE);
            jgkuang->addChild(costLabel);
            
        }else if (cloth_type == 10){
            int cloth_cost = dic->valueForKey("cost")->intValue();
            
        }else{
            int cloth_cost = dic->valueForKey("cost")->intValue();
            
        }
    }
    
    int tagIndex1 = dic->valueForKey("tag1")->intValue();
    if (tagIndex1 != 0) {
        CCString* tagStr1 = CCString::createWithFormat("res/pic/clothesScene/label/%d.png", tagIndex1);
        CCSprite* tagSpr1 = CCSprite::create(tagStr1->getCString());
        tagSpr1->setPosition(ccp(clothSpr->getContentSize().width* .85f, clothSpr->getContentSize().height* .25f));
        clothSpr->addChild(tagSpr1);
    }
    int tagIndex2 = dic->valueForKey("tag2")->intValue();
    if (tagIndex2 != 0) {
        CCString* tagStr2 = CCString::createWithFormat("res/pic/clothesScene/label/%d.png", tagIndex2);
        CCSprite* tagSpr2 = CCSprite::create(tagStr2->getCString());
        tagSpr2->setPosition(ccp(clothSpr->getContentSize().width* .85f, clothSpr->getContentSize().height* .4f));
        clothSpr->addChild(tagSpr2);
    }
    int tagIndex3 = dic->valueForKey("tag3")->intValue();
    if (tagIndex3 != 0) {
        CCString* tagStr3 = CCString::createWithFormat("res/pic/clothesScene/label/%d.png", tagIndex3);
        CCSprite* tagSpr3 = CCSprite::create(tagStr3->getCString());
        tagSpr3->setPosition(ccp(clothSpr->getContentSize().width* .85f, clothSpr->getContentSize().height* .55f));
        clothSpr->addChild(tagSpr3);
    }
    int tagIndex4 = dic->valueForKey("tag4")->intValue();
    if (tagIndex4 != 0) {
        CCString* tagStr4 = CCString::createWithFormat("res/pic/clothesScene/label/%d.png", tagIndex4);
        CCSprite* tagSpr4 = CCSprite::create(tagStr4->getCString());
        tagSpr4->setPosition(ccp(clothSpr->getContentSize().width* .85f, clothSpr->getContentSize().height* .7f));
        clothSpr->addChild(tagSpr4);
    }
    
    
    CCInteger* clothesTemp_id = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString()); // 男宠当前所穿上衣
    if (cloth_id == clothesTemp_id->getValue()) {
//        MZLog("当前的已保存");
        
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        
        kuangSpr = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
        kuangSpr->setPosition(ccp(18, 170));
        kuangSpr->setTag(idx);
        spr->addChild(kuangSpr, 5);
    }
    
    spr->setTag(idx);
    pCell->addChild(spr);
    
    return pCell;
}

//cell的数量
unsigned int ClothesTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    CCArray* arr = DATA->getDataSource();
    clothesPage = arr->count();
//    MZLog("_dataArr->count ======= %d", arr->count());
    return clothesPage;
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void ClothesTableView::tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

//松开的时候，取消高亮状态
void ClothesTableView::tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

void ClothesTableView::onEnter(){
    
    CCLayer::onEnter();
}

void ClothesTableView::onExit(){
    CCLayer::onExit();
    
    
}

void ClothesTableView::updateTableView(int type){
    clothesType = type;
    
    if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
        kuangSpr->removeFromParentAndCleanup(true);
        kuangSpr = NULL;
    }
    
    pTableView->reloadData();
}
int ClothesTableView::updataClothes(int type){
    int index = 0;
    index = (type) * 10000;
    
    return index;
}
void ClothesTableView::updateTableCell(){
    
}















