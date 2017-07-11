//
//  NXClothesShopTableview.cpp
//  tiegao
//
//  Created by mac on 17-7-8.
//
//

#include "NXClothesShopTableview.h"
#include "DataManager.h"
#include "NXClothesScene.h"
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "StringUtil.h"
#include "MMMenu.h"


NXClothesShopTableview::NXClothesShopTableview(){
    
}
NXClothesShopTableview::~NXClothesShopTableview(){
    
}

bool NXClothesShopTableview::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    clothesPage = 0;// 衣服总行数
    clothesType = 0;// 衣服类型
    clothes_selected = 0;// 选中的衣服
    
    this->clothesType = Tag_NXCL_ShangYi;
    
    kuangSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_kuangSelected.png");
    kuangSpr->retain();
    
    pTableView = CCTableView::create(this, CCSizeMake(640, 160*def_Shop_Index));
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
void NXClothesShopTableview::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    CCArray* arr = DATA->getDataSource();
    
    CCPoint contOffsetPos = pTableView->getContentOffset();
    if (arr->count()/def_Shop_Index2 > 3) {
        if (contOffsetPos.y < pTableView->minContainerOffset().y) {
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->minContainerOffset().y));
        }else if (contOffsetPos.y > pTableView->maxContainerOffset().y){
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->maxContainerOffset().y));
        }
    }
}


//点击哪个cell
void NXClothesShopTableview::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

//每个cell的size
cocos2d::CCSize NXClothesShopTableview::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(640, 160);
}

//生成cell
cocos2d::extension::CCTableViewCell* NXClothesShopTableview::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    
    CCSprite* dikuangSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_dikuang22222.png");
    dikuangSpr->setAnchorPoint(CCPointZero);
    dikuangSpr->setPosition(ccp(0, 0));
    dikuangSpr->setTag(idx);
    spr->addChild(dikuangSpr);
    
    CCArray* arr = DATA->getDataSource();
    if (arr->count()%def_Shop_Index2 != 0) {
        
        if (idx+1 == clothesPage){
            for (int i = 0; i < arr->count()%def_Shop_Index2; i++) {
                this->creat_kuang(dikuangSpr, arr, idx, i);
                
            }
        }else{
            for (int i = 0; i < def_Shop_Index2; i++) {
                this->creat_kuang(dikuangSpr, arr, idx, i);
                
            }
        }
    }else{
        for (int i = 0; i < def_Shop_Index2; i++) {
            this->creat_kuang(dikuangSpr, arr, idx, i);
            
        }
    }
    
    
    spr->setTag(idx);
    pCell->addChild(spr);
    return pCell;
}
void NXClothesShopTableview::creat_kuang(CCSprite* dikuangSpr, CCArray* arr, int idx, int i){
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(idx*def_Shop_Index2 + i);
    int cloth_id = dic->valueForKey("id")->intValue(); // 男宠当前所穿上衣
    
    CCString* iconStr;
    CCString* dikuangStr;
    if (clothesType == Tag_NXCL_TouFa) {
        dikuangStr = CCString::createWithFormat("res/nxpic/NXClothes/NXClothes_kuang%d.png", 6);
        iconStr = CCString::createWithFormat("res/nxpic/NXClothes/icon/1toufa/icon%d.png", cloth_id);
    }else if (clothesType == Tag_NXCL_WaiTao) {
        dikuangStr = CCString::createWithFormat("res/nxpic/NXClothes/NXClothes_kuang%d.png", 3);
        iconStr = CCString::createWithFormat("res/nxpic/NXClothes/icon/2waitao/icon%d.png", cloth_id);
    }else if (clothesType == Tag_NXCL_ShangYi){
        dikuangStr = CCString::createWithFormat("res/nxpic/NXClothes/NXClothes_kuang%d.png", 1);
        iconStr = CCString::createWithFormat("res/nxpic/NXClothes/icon/3shangyi/icon%d.png", cloth_id);
    }else if (clothesType == Tag_NXCL_KuZi){
        dikuangStr = CCString::createWithFormat("res/nxpic/NXClothes/NXClothes_kuang%d.png", 2);
        iconStr = CCString::createWithFormat("res/nxpic/NXClothes/icon/4kuzi/icon%d.png", cloth_id);
    }else if (clothesType == Tag_NXCL_XieZi){
        dikuangStr = CCString::createWithFormat("res/nxpic/NXClothes/NXClothes_kuang%d.png", 4);
        iconStr = CCString::createWithFormat("res/nxpic/NXClothes/icon/6xiezi/icon%d.png", cloth_id);
    }else if (clothesType == Tag_NXCL_TeXiao){
        dikuangStr = CCString::createWithFormat("res/nxpic/NXClothes/NXClothes_kuang%d.png", 5);
        iconStr = CCString::createWithFormat("res/nxpic/NXClothes/icon/5texiao/icon%d.png", cloth_id);
    }else if (clothesType == Tag_NXCL_ShiPin){
        dikuangStr = CCString::createWithFormat("res/nxpic/NXClothes/NXClothes_kuang%d.png", 7);
        iconStr = CCString::createWithFormat("res/nxpic/NXClothes/icon/7shipin/icon%d.png", cloth_id);
    }else if (clothesType == Tag_NXCL_ZhuangRong){
        dikuangStr = CCString::createWithFormat("res/nxpic/NXClothes/NXClothes_kuang%d.png", 8);
        iconStr = CCString::createWithFormat("res/nxpic/NXClothes/icon/9zhuangrong/icon%d.png", cloth_id);
    }
    CCSprite* spr1 =  CCSprite::create(dikuangStr->getCString());
    CCSprite* spr2 =  CCSprite::create(dikuangStr->getCString());
    CCMenuItem* kuangItem = CCMenuItemSprite::create(spr1, spr2, this, menu_selector(NXClothesShopTableview::clothesButtonCallBack));
    kuangItem->setAnchorPoint(CCPointZero);
    kuangItem->setPosition(ccp(0 + 160*i, 0));
    kuangItem->setTag(idx*def_Shop_Index2 + i);
    MMMenu* menu = MMMenu::create(kuangItem, NULL);
    menu->setTag(idx*def_Shop_Index2 + i);
    menu->setPosition(CCPointZero);
    menu->setTouchesBool(false);
    menu->setCheckScissor(true);
    dikuangSpr->addChild(menu, 3);
    if (idx*def_Shop_Index2 + i == clothes_selected) {
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        
        kuangSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_kuangSelected.png");
        kuangSpr->setAnchorPoint(CCPointZero);
        kuangSpr->setPosition(ccp(0 + 160*i, 0));
        dikuangSpr->addChild(kuangSpr, 5);
        
        int cloth_id = dic->valueForKey("id")->intValue(); // 男宠当前所穿上衣
        CCNotificationCenter::sharedNotificationCenter()->postNotification("creat_clothes", (CCObject* )cloth_id);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("creat_clothesInfo", (CCObject* )clothes_selected);
    }
    
    CCSprite* iconSpr = CCSprite::create(iconStr->getCString());
    iconSpr->setScale(.8f);
    iconSpr->setPosition(ccp(kuangItem->getContentSize().width* .5f, kuangItem->getContentSize().height* .5f));
    kuangItem->addChild(iconSpr);
    
    // 金币 钻石 碎片
    CCSprite* diSpr = CCSprite::create("res/nxpic/NXClothes/NXClothes_xingdi.png");
    diSpr->setPosition(ccp(kuangItem->getContentSize().width* .5f, kuangItem->getContentSize().height* .3f));
    kuangItem->addChild(diSpr, 10);
    int cloth_type = dic->valueForKey("type")->intValue(); // 男宠当前所穿上衣
    if (cloth_type == 1) {
        CCSprite* costSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_coin2.png");
        costSpr->setPosition(ccp(diSpr->getContentSize().width* .2f, diSpr->getContentSize().height* .5f));
        diSpr->addChild(costSpr, 10);
        const CCString* costStr = dic->valueForKey("cost");
//        CCString* costStr = CCString::createWithFormat("%d", 123456789);
        CCLabelTTF* costLabel = CCLabelTTF::create(costStr->getCString(), DISPLAY->fangzhengFont(), 17, CCSizeMake(80, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        costLabel->setPosition(ccp(diSpr->getContentSize().width* .55f, diSpr->getContentSize().height* .48f));
        costLabel->setColor(ccc3(80, 63, 68));
        diSpr->addChild(costLabel);
    }else if (cloth_type == 2){
        CCSprite* costSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_gold2.png");
        costSpr->setPosition(ccp(diSpr->getContentSize().width* .2f, diSpr->getContentSize().height* .5f));
        diSpr->addChild(costSpr, 10);
        const CCString* costStr = dic->valueForKey("cost");
//        CCString* costStr = CCString::createWithFormat("%d", 123456789);
        CCLabelTTF* costLabel = CCLabelTTF::create(costStr->getCString(), DISPLAY->fangzhengFont(), 17, CCSizeMake(80, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        costLabel->setPosition(ccp(diSpr->getContentSize().width* .55f, diSpr->getContentSize().height* .48f));
        costLabel->setColor(ccc3(80, 63, 68));
        diSpr->addChild(costLabel);
    }else if (cloth_type == 3 || cloth_type == 10){
        CCSprite* costSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_xiyou.png");
        costSpr->setPosition(ccp(diSpr->getContentSize().width* .5f, diSpr->getContentSize().height* .5f));
        costSpr->setScale(.75f);
        diSpr->addChild(costSpr, 10);
    }else{
        CCSprite* costSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_xiyou.png");
        costSpr->setPosition(ccp(diSpr->getContentSize().width* .5f, diSpr->getContentSize().height* .5f));
        costSpr->setScale(.75f);
        diSpr->addChild(costSpr, 10);
    }
}
void NXClothesShopTableview::clothesButtonCallBack(CCObject* pSender){
    
    CCMenuItem* item = (CCMenuItem* )pSender;
    CCLog("item->gettag === %d", item->getTag());
    
    int now_Page = item->getTag()%def_Shop_Index2;
    if (item->getTag() != clothes_selected) {
        clothes_selected = item->getTag();
        if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
            kuangSpr->removeFromParentAndCleanup(true);
            kuangSpr = NULL;
        }
        
        CCNode* node = item->getParent()->getParent();
        kuangSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_kuangSelected.png");
        kuangSpr->setAnchorPoint(CCPointZero);
        kuangSpr->setPosition(ccp(0 + 160*now_Page, 0));
        node->addChild(kuangSpr, 5);
        
        CCArray* arr = DATA->getDataSource();
        CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(item->getTag());
        int cloth_id = dic->valueForKey("id")->intValue(); // 男宠当前所穿上衣
        CCNotificationCenter::sharedNotificationCenter()->postNotification("creat_clothes", (CCObject* )cloth_id);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("creat_clothesInfo", (CCObject* )item->getTag());
    }
}

//cell的数量
unsigned int NXClothesShopTableview::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    CCArray* arr = DATA->getDataSource();
    clothesPage = arr->count()/4;
    if (arr->count()%4 > 0) {
        clothesPage++;
    }
//    CCLog("_dataArr->count ======= %d", arr->count());
    return clothesPage;
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void NXClothesShopTableview::tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

//松开的时候，取消高亮状态
void NXClothesShopTableview::tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

void NXClothesShopTableview::onEnter(){
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, menu_selector(NXClothesTableView::clothesCallback), "ClothesCallback", NULL);
    
    
    CCLayer::onEnter();
}

void NXClothesShopTableview::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void NXClothesShopTableview::updateTableView(int type){
    clothesType = type;
    
    if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
        kuangSpr->removeFromParentAndCleanup(true);
        kuangSpr = NULL;
    }
    clothes_selected = 0;
    pTableView->reloadData();
}
int NXClothesShopTableview::updataClothes(int type){
    
}
void NXClothesShopTableview::updateTableCell(){
    
}
void NXClothesShopTableview::clothesUpdateTableCell(){
    
}






