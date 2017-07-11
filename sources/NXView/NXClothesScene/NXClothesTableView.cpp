//
//  NXClothesTableView.cpp
//  tiegao
//
//  Created by mac on 16-6-15.
//
//

#include "NXClothesTableView.h"
#include "DataManager.h"
#include "NXClothesScene.h"
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "StringUtil.h"


NXClothesTableView::NXClothesTableView(){
    
}
NXClothesTableView::~NXClothesTableView(){
    
}

bool NXClothesTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    clothesPage = 0;
    clothesType = 0;
    now_CellIndex = 0;
    
    this->clothesType = Tag_NXCL_ShangYi;
    
    kuangSpr = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr->retain();
    kuangSpr1 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr1->retain();
    kuangSpr2 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr2->retain();
    kuangSpr3 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr3->retain();
    kuangSpr4 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr4->retain();
    kuangSpr5 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr5->retain();
    kuangSpr6 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr6->retain();
    kuangSpr7 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr7->retain();
    kuangSpr8 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr8->retain();
    kuangSpr9 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr9->retain();
    kuangSpr10 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
    kuangSpr10->retain();
    
    
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
    
    
    pTableView = CCTableView::create(this, CCSizeMake(160, 160* def_Page_Index));
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
void NXClothesTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    
    CCArray* arr = DATA->getDataSource();

    CCPoint contOffsetPos = pTableView->getContentOffset();
    if (arr->count() > 6) {
        if (contOffsetPos.y < pTableView->minContainerOffset().y) {
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->minContainerOffset().y));
        }else if (contOffsetPos.y > pTableView->maxContainerOffset().y){
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->maxContainerOffset().y));
        }
    }
}

//点击哪个cell
void NXClothesTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    if (DATA->current_guide_step() == 4) {
        if (DATA->_guideBool4[3] && !DATA->_guideBool4[4]){
            CCNotificationCenter::sharedNotificationCenter()->postNotification("CloseSwallowEnabled");
        }
    }else if (DATA->current_guide_step() == 6){
        CCNotificationCenter::sharedNotificationCenter()->postNotification("CloseSwallowEnabled");
    }
    
    
    now_CellIndex = cell->getIdx();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataSaveItemStatus");
    
    CCArray* arr = DATA->getDataSource();
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp(); // 临时数组
    
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(cell->getIdx());
    int cloth_id = dic->valueForKey("id")->intValue(); // 点击的衣服
    int sub_part = dic->valueForKey("sub_part")->intValue(); // 衣服的部位
    CCInteger* cloth_integer;
    CCInteger* clothesTemp_id;
    CCDictionary* shipinDic;
    
    std::string stdStr = CCUserDefault::sharedUserDefault()->getStringForKey("SaveClothes", "");
    std::string saveString;
    if (stdStr.empty()) {
        
    }else{
        CCArray* strList = StringUtil::sharedStrUtil()->split(stdStr.c_str(), ";");
        for (int i = 0; i < strList->count(); i++) {
            CCString* listStr = (CCString* )strList->objectAtIndex(i);
            int saveClothes = atoi(listStr->getCString());
            if (cloth_id == saveClothes) {
                CCNode* node = cell->getChildByTag(cell->getIdx());
                if (node->getChildByTag(0x12345) != NULL) {
                    node->removeChildByTag(0x12345);
                }
            }else{
                CCString* saveStr = CCString::createWithFormat("%d;", saveClothes);
                saveString.append(saveStr->getCString());
            }
        }
        CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", saveString.c_str());
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    
    
    if (clothesType != Tag_NXCL_ShiPin) {
        clothesTemp_id = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString()); // 男宠当前所穿上衣
    }else{
        shipinDic = (CCDictionary* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());// 获取所穿视频的字典
        clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", sub_part)->getCString());
    }
    
    if (cloth_id == clothesTemp_id->getValue()) {
        cloth_id = updataClothes(clothesType);
        
        cloth_integer = CCInteger::create(cloth_id);
        CCString* keyStr;
        if (clothesType != Tag_NXCL_ShiPin) {
            keyStr = CCString::createWithFormat("%d", clothesType);
            clothesTemp->setObject(cloth_integer, keyStr->getCString());
        }else{
            keyStr = CCString::createWithFormat("%d", clothesType);
            CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
            shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
            clothesTemp->setObject(shipinDic, keyStr->getCString());
        }
        
        if (sub_part == 11) {
            if (kuangSpr1 != NULL && kuangSpr1->getParent() != NULL) {
                kuangSpr1->removeFromParentAndCleanup(true);
                kuangSpr1 = NULL;
            }
        }else if (sub_part == 12){
            if (kuangSpr2 != NULL && kuangSpr2->getParent() != NULL) {
                kuangSpr2->removeFromParentAndCleanup(true);
                kuangSpr2 = NULL;
            }
        }else if (sub_part == 13){
            if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                kuangSpr3->removeFromParentAndCleanup(true);
                kuangSpr3 = NULL;
            }
        }else if (sub_part == 14){
            if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
                kuangSpr4->removeFromParentAndCleanup(true);
                kuangSpr4 = NULL;
            }
        }else if (sub_part == 15){
            if (kuangSpr5 != NULL && kuangSpr5->getParent() != NULL) {
                kuangSpr5->removeFromParentAndCleanup(true);
                kuangSpr5 = NULL;
            }
        }else if (sub_part == 16){
            if (kuangSpr6 != NULL && kuangSpr6->getParent() != NULL) {
                kuangSpr6->removeFromParentAndCleanup(true);
                kuangSpr6 = NULL;
            }
        }else if (sub_part == 17){
            if (kuangSpr7 != NULL && kuangSpr7->getParent() != NULL) {
                kuangSpr7->removeFromParentAndCleanup(true);
                kuangSpr7 = NULL;
            }
        }else if (sub_part == 18){
            if (kuangSpr8 != NULL && kuangSpr8->getParent() != NULL) {
                kuangSpr8->removeFromParentAndCleanup(true);
                kuangSpr8 = NULL;
            }
        }else if (sub_part == 19){
            if (kuangSpr9 != NULL && kuangSpr9->getParent() != NULL) {
                kuangSpr9->removeFromParentAndCleanup(true);
                kuangSpr9 = NULL;
            }
        }else if (sub_part == 20){
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
        }
        else{
            if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
                kuangSpr->removeFromParentAndCleanup(true);
                kuangSpr = NULL;
            }
        }
        
        
        if (clothesType != Tag_NXCL_ShiPin) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        }else{
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangClothesIndex", (CCObject* )clothesTemp_id->getValue());
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        }
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ButtonStatus", NULL);
    }else{
        cloth_integer = CCInteger::create(cloth_id);
        CCString* keyStr;
        if (clothesType != Tag_NXCL_ShiPin) {
            if (clothesType == Tag_NXCL_ShangYi) {
                if (sub_part == 1) {
                    CCInteger* kuziInteger = CCInteger::create(40000);
                    CCString* kuziStr = CCString::createWithFormat("%d", Tag_NXCL_KuZi);
                    clothesTemp->setObject(kuziInteger, kuziStr->getCString());
                    
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }else{
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }
            }else if (clothesType == Tag_NXCL_KuZi) {
                CCInteger* shangyiInteger = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", Tag_NXCL_ShangYi)->getCString());
                int shangyi_sub_part = 0;
                CCDictionary* shangyiDic = CONFIG->clothes();// 所有衣服
                CCArray* shangyiArr = (CCArray* )shangyiDic->objectForKey(Tag_NXCL_ShangYi);// 获得当前类型所有衣服
                for (int i = 0; i < shangyiArr->count(); i++) {
                    CCDictionary* syDic = (CCDictionary* )shangyiArr->objectAtIndex(i);
                    int shangyiId = syDic->valueForKey("id")->intValue();
                    if (shangyiId == shangyiInteger->getValue()) {
                        shangyi_sub_part = syDic->valueForKey("sub_part")->intValue();
                    }
                }
                
                if (shangyi_sub_part == 1) {
                    CCInteger* shangyiInteger = CCInteger::create(30000);
                    CCString* shangyiStr = CCString::createWithFormat("%d", Tag_NXCL_ShangYi);
                    clothesTemp->setObject(shangyiInteger, shangyiStr->getCString());
                    
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }else{
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }
                
            }else{
                keyStr = CCString::createWithFormat("%d", clothesType);
                clothesTemp->setObject(cloth_integer, keyStr->getCString());
            }
        }else{
            if (sub_part == 13 || sub_part == 14) {
                keyStr = CCString::createWithFormat("%d", clothesType);
                CCString* _20_keyStr = CCString::createWithFormat("%d", 20);
                CCInteger* _20Integer = CCInteger::create(70000);
                shipinDic->setObject(_20Integer, _20_keyStr->getCString());
                
                CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
                shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
                clothesTemp->setObject(shipinDic, keyStr->getCString());
            }else if (sub_part == 20){
                keyStr = CCString::createWithFormat("%d", clothesType);
                CCString* _13_keyStr = CCString::createWithFormat("%d", 13);
                CCInteger* _13Integer = CCInteger::create(70000);
                shipinDic->setObject(_13Integer, _13_keyStr->getCString());
                
                CCString* _14_keyStr = CCString::createWithFormat("%d", 14);
                CCInteger* _14Integer = CCInteger::create(70000);
                shipinDic->setObject(_14Integer, _14_keyStr->getCString());
                
                CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
                shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
                clothesTemp->setObject(shipinDic, keyStr->getCString());
            }else{
                keyStr = CCString::createWithFormat("%d", clothesType);
                CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
                shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
                clothesTemp->setObject(shipinDic, keyStr->getCString());
            }
        }
        
        if (sub_part == 11) {
            if (kuangSpr1 != NULL && kuangSpr1->getParent() != NULL) {
                kuangSpr1->removeFromParentAndCleanup(true);
                kuangSpr1 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr1 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr1->setPosition(ccp(22, 135));
            kuangSpr1->setTag(cell->getIdx());
            node->addChild(kuangSpr1, 5);
        }else if (sub_part == 12){
            if (kuangSpr2 != NULL && kuangSpr2->getParent() != NULL) {
                kuangSpr2->removeFromParentAndCleanup(true);
                kuangSpr2 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr2 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr2->setPosition(ccp(22, 135));
            kuangSpr2->setTag(cell->getIdx());
            node->addChild(kuangSpr2, 5);
        }else if (sub_part == 13){
            if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                kuangSpr3->removeFromParentAndCleanup(true);
                kuangSpr3 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr3 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr3->setPosition(ccp(22, 135));
            kuangSpr3->setTag(cell->getIdx());
            node->addChild(kuangSpr3, 5);
            
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
        }else if (sub_part == 14){
            if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
                kuangSpr4->removeFromParentAndCleanup(true);
                kuangSpr4 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr4 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr4->setPosition(ccp(22, 135));
            kuangSpr4->setTag(cell->getIdx());
            node->addChild(kuangSpr4, 5);
            
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
        }else if (sub_part == 15){
            if (kuangSpr5 != NULL && kuangSpr5->getParent() != NULL) {
                kuangSpr5->removeFromParentAndCleanup(true);
                kuangSpr5 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr5 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr5->setPosition(ccp(22, 135));
            kuangSpr5->setTag(cell->getIdx());
            node->addChild(kuangSpr5, 5);
        }else if (sub_part == 16){
            if (kuangSpr6 != NULL && kuangSpr6->getParent() != NULL) {
                kuangSpr6->removeFromParentAndCleanup(true);
                kuangSpr6 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr6 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr6->setPosition(ccp(22, 135));
            kuangSpr6->setTag(cell->getIdx());
            node->addChild(kuangSpr6, 5);
        }else if (sub_part == 17){
            if (kuangSpr7 != NULL && kuangSpr7->getParent() != NULL) {
                kuangSpr7->removeFromParentAndCleanup(true);
                kuangSpr7 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr7 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr7->setPosition(ccp(22, 135));
            kuangSpr7->setTag(cell->getIdx());
            node->addChild(kuangSpr7, 5);
        }else if (sub_part == 18){
            if (kuangSpr8 != NULL && kuangSpr8->getParent() != NULL) {
                kuangSpr8->removeFromParentAndCleanup(true);
                kuangSpr8 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr8 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr8->setPosition(ccp(22, 135));
            kuangSpr8->setTag(cell->getIdx());
            node->addChild(kuangSpr8, 5);
        }else if (sub_part == 19){
            if (kuangSpr9 != NULL && kuangSpr9->getParent() != NULL) {
                kuangSpr9->removeFromParentAndCleanup(true);
                kuangSpr9 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr9 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr9->setPosition(ccp(22, 135));
            kuangSpr9->setTag(cell->getIdx());
            node->addChild(kuangSpr9, 5);
        }else if (sub_part == 20){
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr10 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr10->setPosition(ccp(22, 135));
            kuangSpr10->setTag(cell->getIdx());
            node->addChild(kuangSpr10, 5);
            
            if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                kuangSpr3->removeFromParentAndCleanup(true);
                kuangSpr3 = NULL;
            }
            if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
                kuangSpr4->removeFromParentAndCleanup(true);
                kuangSpr4 = NULL;
            }
        }
        else{
            if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
                kuangSpr->removeFromParentAndCleanup(true);
                kuangSpr = NULL;
            }
            CCNode* spr = cell->getChildByTag(cell->getIdx());
            CCNode* dikuangSpr = spr->getChildByTag(cell->getIdx());
            kuangSpr = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr->setPosition(ccp(22, 135));
            kuangSpr->setTag(cell->getIdx());
            dikuangSpr->addChild(kuangSpr, 5);
        }
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangClothesIndex", (CCObject* )0);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ButtonStatus", NULL);
    }
    sliderV = pTableView->getContentOffset().y;
    this->updateTableCell();
}

//每个cell的size
cocos2d::CCSize NXClothesTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(160, 160);
}

//生成cell
cocos2d::extension::CCTableViewCell* NXClothesTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx) {
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp(); // 临时数组
    CCArray* arr = DATA->getDataSource();
    
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(idx);
    int cloth_id = dic->valueForKey("id")->intValue(); // 男宠当前所穿上衣
    int sub_part = dic->valueForKey("sub_part")->intValue(); // 衣服的部位
    CCString* iconStr;
    CCString* dikuangStr;
    if (DATA->getClothes()->is_owned(clothesType, cloth_id)) {
        // 购买了
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
        CCSprite* dikuangSpr = CCSprite::create(dikuangStr->getCString());
        dikuangSpr->setAnchorPoint(CCPointZero);
        dikuangSpr->setPosition(ccp(0, 0));
        dikuangSpr->setTag(idx);
        spr->addChild(dikuangSpr);
        
        CCSprite* clothSpr = CCSprite::create(iconStr->getCString());
        if (!clothSpr) {
            clothSpr = CCSprite::create("res/pic/unknown.png");
        }
        clothSpr->setScale(.7f);
        clothSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .5f, dikuangSpr->getContentSize().height* .5f));
        clothSpr->setTag(idx);
        dikuangSpr->addChild(clothSpr, 3);
        
        int phase = dic->valueForKey("phase")->intValue();
        CCSprite* xingdiSpr = CCSprite::create("res/nxpic/NXClothes/NXClothes_xingdi.png");
        xingdiSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .5f, dikuangSpr->getContentSize().height* .3f));
        dikuangSpr->addChild(xingdiSpr, 10);
        // 星星
        CCMenuItemImage* xingItem1 = CCMenuItemImage::create("res/nxpic/NXClothes/NXClothes_xing.png", "res/nxpic/NXClothes/NXClothes_xing.png");
        xingItem1->setPosition(CCPointZero);
        CCMenuItemImage* xingItem2 = CCMenuItemImage::create("res/nxpic/NXClothes/NXClothes_xing.png", "res/nxpic/NXClothes/NXClothes_xing.png");
        xingItem2->setPosition(CCPointZero);
        CCMenuItemImage* xingItem3 = CCMenuItemImage::create("res/nxpic/NXClothes/NXClothes_xing.png", "res/nxpic/NXClothes/NXClothes_xing.png");
        xingItem3->setPosition(CCPointZero);
        CCMenuItemImage* xingItem4 = CCMenuItemImage::create("res/nxpic/NXClothes/NXClothes_xing.png", "res/nxpic/NXClothes/NXClothes_xing.png");
        xingItem4->setPosition(CCPointZero);
        CCMenuItemImage* xingItem5 = CCMenuItemImage::create("res/nxpic/NXClothes/NXClothes_xing.png", "res/nxpic/NXClothes/NXClothes_xing.png");
        xingItem5->setPosition(CCPointZero);
        if (phase == 1) {
            CCMenu* menu = CCMenu::create(xingItem1, NULL);
            menu->alignItemsHorizontallyWithPadding(1.f);
            menu->setPosition(ccp(xingdiSpr->getContentSize().width* .5f, xingdiSpr->getContentSize().height* .5f));
            xingdiSpr->addChild(menu);
        }else if (phase == 2) {
            CCMenu* menu = CCMenu::create(xingItem1, xingItem2, NULL);
            menu->alignItemsHorizontallyWithPadding(1.f);
            menu->setPosition(ccp(xingdiSpr->getContentSize().width* .5f, xingdiSpr->getContentSize().height* .5f));
            xingdiSpr->addChild(menu);
        }else if (phase == 3) {
            CCMenu* menu = CCMenu::create(xingItem1, xingItem2, xingItem3, NULL);
            menu->alignItemsHorizontallyWithPadding(1.f);
            menu->setPosition(ccp(xingdiSpr->getContentSize().width* .5f, xingdiSpr->getContentSize().height* .5f));
            xingdiSpr->addChild(menu);
        }else if (phase == 4) {
            CCMenu* menu = CCMenu::create(xingItem1, xingItem2, xingItem3, xingItem4, NULL);
            menu->alignItemsHorizontallyWithPadding(1.f);
            menu->setPosition(ccp(xingdiSpr->getContentSize().width* .5f, xingdiSpr->getContentSize().height* .5f));
            xingdiSpr->addChild(menu);
        }else if (phase == 5) {
            CCMenu* menu = CCMenu::create(xingItem1, xingItem2, xingItem3, xingItem4, NULL);
            menu->alignItemsHorizontallyWithPadding(1.f);
            menu->setPosition(ccp(xingdiSpr->getContentSize().width* .5f, xingdiSpr->getContentSize().height* .5f));
            xingdiSpr->addChild(menu);
        }
        
        // 名字
        CCSprite* namekuangSpr = CCSprite::create("res/nxpic/NXClothes/NXClothes_namekuang.png");
        namekuangSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .5f, dikuangSpr->getContentSize().height* .16f));
        dikuangSpr->addChild(namekuangSpr, 11);
        const CCString* nameStr = dic->valueForKey("name");
        CCLabelTTF* nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 22, CCSizeMake(138, 22), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        nameLabel->setPosition(ccp(namekuangSpr->getContentSize().width* .5f, namekuangSpr->getContentSize().height* .5f));
        nameLabel->setColor(ccc3(113, 89, 102));
        namekuangSpr->addChild(nameLabel);
        
        
        // 锁
        if (phase > DATA->getPlayer()->phase) {
            CCSprite* suoSpr = CCSprite::create("res/nxpic/NXClothes/NXClothes_suo.png");
            suoSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .5f, dikuangSpr->getContentSize().height* .5f));
            dikuangSpr->addChild(suoSpr, 100);
        }
        
        // 新
        std::string stdStr = CCUserDefault::sharedUserDefault()->getStringForKey("SaveClothes", "");
        if (stdStr.empty()) {
            
        }else{
            CCArray* strList = StringUtil::sharedStrUtil()->split(stdStr.c_str(), ";");
            for (int i = 0; i < strList->count(); i++) {
                CCString* listStr = (CCString* )strList->objectAtIndex(i);
                int saveClothes = atoi(listStr->getCString());
                if (cloth_id == saveClothes) {
                    CCSprite* newSpr1 = CCSprite::create("res/nxpic/NXClothes/NXClothes_new1.png");
                    newSpr1->setPosition(ccp(22, 135));
                    newSpr1->setTag(0x12345);
                    dikuangSpr->addChild(newSpr1, 101);
                    
                    CCSprite* newSpr2 = CCSprite::create("res/nxpic/NXClothes/NXClothes_new2.png");
                    newSpr2->setPosition(ccp(newSpr1->getContentSize().width* .5f, newSpr1->getContentSize().height* .5f));
                    newSpr1->addChild(newSpr2);
                    
                    break;
                }
            }
        }
        
        
        //
        CCInteger* clothesTemp_id;
        if (clothesType != Tag_NXCL_ShiPin) {
            clothesTemp_id = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString()); // 男宠当前所穿上衣
        }else{
            CCDictionary* shipinDic = (CCDictionary* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());// 获取所穿视频的字典
            clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", sub_part)->getCString());
        }
        
        if (cloth_id == clothesTemp_id->getValue()) {
            //        MZLog("当前的已保存");
            
            if (sub_part == 11) {
                if (kuangSpr1 != NULL && kuangSpr1->getParent() != NULL) {
                    kuangSpr1->removeFromParentAndCleanup(true);
                    kuangSpr1 = NULL;
                }
                kuangSpr1 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr1->setPosition(ccp(22, 135));
                kuangSpr1->setTag(idx);
                dikuangSpr->addChild(kuangSpr1, 5);
            }else if (sub_part == 12){
                if (kuangSpr2 != NULL && kuangSpr2->getParent() != NULL) {
                    kuangSpr2->removeFromParentAndCleanup(true);
                    kuangSpr2 = NULL;
                }
                kuangSpr2 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr2->setPosition(ccp(22, 135));
                kuangSpr2->setTag(idx);
                dikuangSpr->addChild(kuangSpr2, 5);
            }else if (sub_part == 13){
                if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                    kuangSpr3->removeFromParentAndCleanup(true);
                    kuangSpr3 = NULL;
                }
                kuangSpr3 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr3->setPosition(ccp(22, 135));
                kuangSpr3->setTag(idx);
                dikuangSpr->addChild(kuangSpr3, 5);
            }else if (sub_part == 14){
                if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
                    kuangSpr4->removeFromParentAndCleanup(true);
                    kuangSpr4 = NULL;
                }
                kuangSpr4 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr4->setPosition(ccp(22, 135));
                kuangSpr4->setTag(idx);
                dikuangSpr->addChild(kuangSpr4, 5);
            }else if (sub_part == 15){
                if (kuangSpr5 != NULL && kuangSpr5->getParent() != NULL) {
                    kuangSpr5->removeFromParentAndCleanup(true);
                    kuangSpr5 = NULL;
                }
                kuangSpr5 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr5->setPosition(ccp(22, 135));
                kuangSpr5->setTag(idx);
                dikuangSpr->addChild(kuangSpr5, 5);
            }else if (sub_part == 16){
                if (kuangSpr6 != NULL && kuangSpr6->getParent() != NULL) {
                    kuangSpr6->removeFromParentAndCleanup(true);
                    kuangSpr6 = NULL;
                }
                kuangSpr6 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr6->setPosition(ccp(22, 135));
                kuangSpr6->setTag(idx);
                dikuangSpr->addChild(kuangSpr6, 5);
            }else if (sub_part == 17){
                if (kuangSpr7 != NULL && kuangSpr7->getParent() != NULL) {
                    kuangSpr7->removeFromParentAndCleanup(true);
                    kuangSpr7 = NULL;
                }
                kuangSpr7 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr7->setPosition(ccp(22, 135));
                kuangSpr7->setTag(idx);
                dikuangSpr->addChild(kuangSpr7, 5);
            }else if (sub_part == 18){
                if (kuangSpr8 != NULL && kuangSpr8->getParent() != NULL) {
                    kuangSpr8->removeFromParentAndCleanup(true);
                    kuangSpr8 = NULL;
                }
                kuangSpr8 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr8->setPosition(ccp(22, 135));
                kuangSpr8->setTag(idx);
                dikuangSpr->addChild(kuangSpr8, 5);
            }else if (sub_part == 19){
                if (kuangSpr9 != NULL && kuangSpr9->getParent() != NULL) {
                    kuangSpr9->removeFromParentAndCleanup(true);
                    kuangSpr9 = NULL;
                }
                kuangSpr9 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr9->setPosition(ccp(22, 135));
                kuangSpr9->setTag(idx);
                dikuangSpr->addChild(kuangSpr9, 5);
            }else if (sub_part == 20){
                if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                    kuangSpr10->removeFromParentAndCleanup(true);
                    kuangSpr10 = NULL;
                }
                kuangSpr10 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr10->setPosition(ccp(22, 135));
                kuangSpr10->setTag(idx);
                dikuangSpr->addChild(kuangSpr10, 5);
            }
            else{
                if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
                    kuangSpr->removeFromParentAndCleanup(true);
                    kuangSpr = NULL;
                }
                kuangSpr = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
                kuangSpr->setPosition(ccp(22, 135));
                kuangSpr->setTag(idx);
                dikuangSpr->addChild(kuangSpr, 5);
            }
        }else{
            
        }
        
        // 标签
        int tagIndex1 = dic->valueForKey("tag1")->intValue();
        if (tagIndex1 != 0) {
            CCString* tagStr1 = CCString::createWithFormat("res/pic/clothesScene/label/%d.png", tagIndex1);
            CCSprite* tagSpr1 = CCSprite::create(tagStr1->getCString());
            tagSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* .85f, dikuangSpr->getContentSize().height* .92f));
            dikuangSpr->addChild(tagSpr1);
        }
        int tagIndex2 = dic->valueForKey("tag2")->intValue();
        if (tagIndex2 != 0) {
            CCString* tagStr2 = CCString::createWithFormat("res/pic/clothesScene/label/%d.png", tagIndex2);
            CCSprite* tagSpr2 = CCSprite::create(tagStr2->getCString());
            tagSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* .85f, dikuangSpr->getContentSize().height* .77f));
            dikuangSpr->addChild(tagSpr2);
        }
        int tagIndex3 = dic->valueForKey("tag3")->intValue();
        if (tagIndex3 != 0) {
            CCString* tagStr3 = CCString::createWithFormat("res/pic/clothesScene/label/%d.png", tagIndex3);
            CCSprite* tagSpr3 = CCSprite::create(tagStr3->getCString());
            tagSpr3->setPosition(ccp(dikuangSpr->getContentSize().width* .85f, dikuangSpr->getContentSize().height* .62f));
            dikuangSpr->addChild(tagSpr3);
        }
        int tagIndex4 = dic->valueForKey("tag4")->intValue();
        if (tagIndex4 != 0) {
            CCString* tagStr4 = CCString::createWithFormat("res/pic/clothesScene/label/%d.png", tagIndex4);
            CCSprite* tagSpr4 = CCSprite::create(tagStr4->getCString());
            tagSpr4->setPosition(ccp(dikuangSpr->getContentSize().width* .85f, dikuangSpr->getContentSize().height* .47f));
            dikuangSpr->addChild(tagSpr4);
        }
    }else{
    }
    
    
    spr->setTag(idx);
    pCell->addChild(spr);
    
    return pCell;
}

//cell的数量
unsigned int NXClothesTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    CCArray* arr = DATA->getDataSource();
    clothesPage = arr->count();
//    CCLog("_dataArr->count ======= %d", arr->count());
    return clothesPage;
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void NXClothesTableView::tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

//松开的时候，取消高亮状态
void NXClothesTableView::tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
}

void NXClothesTableView::onEnter(){
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(NXClothesTableView::clothesCallback), "ClothesCallback", NULL);
    
    
    CCLayer::onEnter();
}

void NXClothesTableView::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void NXClothesTableView::updateTableView(int type){
    clothesType = type;
    
    if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
        kuangSpr->removeFromParentAndCleanup(true);
        kuangSpr = NULL;
    }
    
    if (kuangSpr1 != NULL && kuangSpr1->getParent() != NULL) {
        kuangSpr1->removeFromParentAndCleanup(true);
        kuangSpr1 = NULL;
    }
    if (kuangSpr2 != NULL && kuangSpr2->getParent() != NULL) {
        kuangSpr2->removeFromParentAndCleanup(true);
        kuangSpr2 = NULL;
    }
    if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
        kuangSpr3->removeFromParentAndCleanup(true);
        kuangSpr3 = NULL;
    }
    if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
        kuangSpr4->removeFromParentAndCleanup(true);
        kuangSpr4 = NULL;
    }
    if (kuangSpr5 != NULL && kuangSpr5->getParent() != NULL) {
        kuangSpr5->removeFromParentAndCleanup(true);
        kuangSpr5 = NULL;
    }
    if (kuangSpr6 != NULL && kuangSpr6->getParent() != NULL) {
        kuangSpr6->removeFromParentAndCleanup(true);
        kuangSpr6 = NULL;
    }
    if (kuangSpr7 != NULL && kuangSpr7->getParent() != NULL) {
        kuangSpr7->removeFromParentAndCleanup(true);
        kuangSpr7 = NULL;
    }
    if (kuangSpr8 != NULL && kuangSpr8->getParent() != NULL) {
        kuangSpr8->removeFromParentAndCleanup(true);
        kuangSpr8 = NULL;
    }
    if (kuangSpr9 != NULL && kuangSpr9->getParent() != NULL) {
        kuangSpr9->removeFromParentAndCleanup(true);
        kuangSpr9 = NULL;
    }
    if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
        kuangSpr10->removeFromParentAndCleanup(true);
        kuangSpr10 = NULL;
    }
    
    pTableView->reloadData();
}
int NXClothesTableView::updataClothes(int type){
    int index = 0;
    index = (type) * 10000;
    
    return index;
}
void NXClothesTableView::updateTableCell(){
    pTableView->reloadData();
    pTableView->setContentOffset(CCSizeMake(0, sliderV));
}

void NXClothesTableView::clothesUpdateTableCell(){
    if (now_CellIndex > 0) {
        pTableView->updateCellAtIndex(now_CellIndex-1);
        pTableView->updateCellAtIndex(now_CellIndex);
        pTableView->updateCellAtIndex(now_CellIndex+1);
    }else{
        pTableView->updateCellAtIndex(now_CellIndex);
        pTableView->updateCellAtIndex(now_CellIndex+1);
    }
}




void NXClothesTableView::clothesCallback(CCObject* pSender){
    CCTableViewCell* pCell = pTableView->cellAtIndex(0);
    
    now_CellIndex = pCell->getIdx();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataSaveItemStatus");
    
    CCArray* arr = DATA->getDataSource();
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp(); // 临时数组
    
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(pCell->getIdx());
    int cloth_id = dic->valueForKey("id")->intValue(); // 点击的衣服
    int sub_part = dic->valueForKey("sub_part")->intValue(); // 衣服的部位
    CCInteger* cloth_integer;
    CCInteger* clothesTemp_id;
    CCDictionary* shipinDic;
    
    if (clothesType != Tag_NXCL_ShiPin) {
        clothesTemp_id = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString()); // 男宠当前所穿上衣
    }else{
        shipinDic = (CCDictionary* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());// 获取所穿视频的字典
        clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", sub_part)->getCString());
    }
    
    if (cloth_id == clothesTemp_id->getValue()) {
        cloth_id = updataClothes(clothesType);
        
        cloth_integer = CCInteger::create(cloth_id);
        CCString* keyStr;
        if (clothesType != Tag_NXCL_ShiPin) {
            keyStr = CCString::createWithFormat("%d", clothesType);
            clothesTemp->setObject(cloth_integer, keyStr->getCString());
        }else{
            keyStr = CCString::createWithFormat("%d", clothesType);
            CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
            shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
            clothesTemp->setObject(shipinDic, keyStr->getCString());
        }
        
        if (sub_part == 11) {
            if (kuangSpr1 != NULL && kuangSpr1->getParent() != NULL) {
                kuangSpr1->removeFromParentAndCleanup(true);
                kuangSpr1 = NULL;
            }
        }else if (sub_part == 12){
            if (kuangSpr2 != NULL && kuangSpr2->getParent() != NULL) {
                kuangSpr2->removeFromParentAndCleanup(true);
                kuangSpr2 = NULL;
            }
        }else if (sub_part == 13){
            if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                kuangSpr3->removeFromParentAndCleanup(true);
                kuangSpr3 = NULL;
            }
        }else if (sub_part == 14){
            if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
                kuangSpr4->removeFromParentAndCleanup(true);
                kuangSpr4 = NULL;
            }
        }else if (sub_part == 15){
            if (kuangSpr5 != NULL && kuangSpr5->getParent() != NULL) {
                kuangSpr5->removeFromParentAndCleanup(true);
                kuangSpr5 = NULL;
            }
        }else if (sub_part == 16){
            if (kuangSpr6 != NULL && kuangSpr6->getParent() != NULL) {
                kuangSpr6->removeFromParentAndCleanup(true);
                kuangSpr6 = NULL;
            }
        }else if (sub_part == 17){
            if (kuangSpr7 != NULL && kuangSpr7->getParent() != NULL) {
                kuangSpr7->removeFromParentAndCleanup(true);
                kuangSpr7 = NULL;
            }
        }else if (sub_part == 18){
            if (kuangSpr8 != NULL && kuangSpr8->getParent() != NULL) {
                kuangSpr8->removeFromParentAndCleanup(true);
                kuangSpr8 = NULL;
            }
        }else if (sub_part == 19){
            if (kuangSpr9 != NULL && kuangSpr9->getParent() != NULL) {
                kuangSpr9->removeFromParentAndCleanup(true);
                kuangSpr9 = NULL;
            }
        }else if (sub_part == 20){
            if (kuangSpr10 != NULL && kuangSpr9->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
        }
        else{
            if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
                kuangSpr->removeFromParentAndCleanup(true);
                kuangSpr = NULL;
            }
        }
        
        
        if (clothesType != Tag_NXCL_ShiPin) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        }else{
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangClothesIndex", (CCObject* )clothesTemp_id->getValue());
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        }
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ButtonStatus", NULL);
    }else{
        cloth_integer = CCInteger::create(cloth_id);
        CCString* keyStr;
        if (clothesType != Tag_NXCL_ShiPin) {
            if (clothesType == Tag_NXCL_ShangYi) {
                if (sub_part == 1) {
                    CCInteger* kuziInteger = CCInteger::create(40000);
                    CCString* kuziStr = CCString::createWithFormat("%d", Tag_NXCL_KuZi);
                    clothesTemp->setObject(kuziInteger, kuziStr->getCString());
                    
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }else{
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }
            }else if (clothesType == Tag_NXCL_KuZi) {
                CCInteger* shangyiInteger = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", Tag_NXCL_ShangYi)->getCString());
                int shangyi_sub_part = 0;
                CCDictionary* shangyiDic = CONFIG->clothes();// 所有衣服
                CCArray* shangyiArr = (CCArray* )shangyiDic->objectForKey(Tag_NXCL_ShangYi);// 获得当前类型所有衣服
                for (int i = 0; i < shangyiArr->count(); i++) {
                    CCDictionary* syDic = (CCDictionary* )shangyiArr->objectAtIndex(i);
                    int shangyiId = syDic->valueForKey("id")->intValue();
                    if (shangyiId == shangyiInteger->getValue()) {
                        shangyi_sub_part = syDic->valueForKey("sub_part")->intValue();
                    }
                }
                
                if (shangyi_sub_part == 1) {
                    CCInteger* shangyiInteger = CCInteger::create(30000);
                    CCString* shangyiStr = CCString::createWithFormat("%d", Tag_NXCL_ShangYi);
                    clothesTemp->setObject(shangyiInteger, shangyiStr->getCString());
                    
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }else{
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }
                
            }else{
                keyStr = CCString::createWithFormat("%d", clothesType);
                clothesTemp->setObject(cloth_integer, keyStr->getCString());
            }
        }else{
            if (sub_part == 13 || sub_part == 14) {
                keyStr = CCString::createWithFormat("%d", clothesType);
                CCString* _20_keyStr = CCString::createWithFormat("%d", 20);
                CCInteger* _20Integer = CCInteger::create(70000);
                shipinDic->setObject(_20Integer, _20_keyStr->getCString());
                
                CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
                shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
                clothesTemp->setObject(shipinDic, keyStr->getCString());
            }else if (sub_part == 20){
                keyStr = CCString::createWithFormat("%d", clothesType);
                CCString* _13_keyStr = CCString::createWithFormat("%d", 13);
                CCInteger* _13Integer = CCInteger::create(70000);
                shipinDic->setObject(_13Integer, _13_keyStr->getCString());
                
                CCString* _14_keyStr = CCString::createWithFormat("%d", 14);
                CCInteger* _14Integer = CCInteger::create(70000);
                shipinDic->setObject(_14Integer, _14_keyStr->getCString());
                
                CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
                shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
                clothesTemp->setObject(shipinDic, keyStr->getCString());
            }else{
                keyStr = CCString::createWithFormat("%d", clothesType);
                CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
                shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
                clothesTemp->setObject(shipinDic, keyStr->getCString());
            }
        }
        
        if (sub_part == 11) {
            if (kuangSpr1 != NULL && kuangSpr1->getParent() != NULL) {
                kuangSpr1->removeFromParentAndCleanup(true);
                kuangSpr1 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr1 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr1->setPosition(ccp(22, 135));
            kuangSpr1->setTag(pCell->getIdx());
            node->addChild(kuangSpr1, 5);
        }else if (sub_part == 12){
            if (kuangSpr2 != NULL && kuangSpr2->getParent() != NULL) {
                kuangSpr2->removeFromParentAndCleanup(true);
                kuangSpr2 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr2 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr2->setPosition(ccp(22, 135));
            kuangSpr2->setTag(pCell->getIdx());
            node->addChild(kuangSpr2, 5);
        }else if (sub_part == 13){
            if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                kuangSpr3->removeFromParentAndCleanup(true);
                kuangSpr3 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr3 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr3->setPosition(ccp(22, 135));
            kuangSpr3->setTag(pCell->getIdx());
            node->addChild(kuangSpr3, 5);
            
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
        }else if (sub_part == 14){
            if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
                kuangSpr4->removeFromParentAndCleanup(true);
                kuangSpr4 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr4 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr4->setPosition(ccp(22, 135));
            kuangSpr4->setTag(pCell->getIdx());
            node->addChild(kuangSpr4, 5);
            
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
        }else if (sub_part == 15){
            if (kuangSpr5 != NULL && kuangSpr5->getParent() != NULL) {
                kuangSpr5->removeFromParentAndCleanup(true);
                kuangSpr5 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr5 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr5->setPosition(ccp(22, 135));
            kuangSpr5->setTag(pCell->getIdx());
            node->addChild(kuangSpr5, 5);
        }else if (sub_part == 16){
            if (kuangSpr6 != NULL && kuangSpr6->getParent() != NULL) {
                kuangSpr6->removeFromParentAndCleanup(true);
                kuangSpr6 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr6 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr6->setPosition(ccp(22, 135));
            kuangSpr6->setTag(pCell->getIdx());
            node->addChild(kuangSpr6, 5);
        }else if (sub_part == 17){
            if (kuangSpr7 != NULL && kuangSpr7->getParent() != NULL) {
                kuangSpr7->removeFromParentAndCleanup(true);
                kuangSpr7 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr7 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr7->setPosition(ccp(22, 135));
            kuangSpr7->setTag(pCell->getIdx());
            node->addChild(kuangSpr7, 5);
        }else if (sub_part == 18){
            if (kuangSpr8 != NULL && kuangSpr8->getParent() != NULL) {
                kuangSpr8->removeFromParentAndCleanup(true);
                kuangSpr8 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr8 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr8->setPosition(ccp(22, 135));
            kuangSpr8->setTag(pCell->getIdx());
            node->addChild(kuangSpr8, 5);
        }else if (sub_part == 19){
            if (kuangSpr9 != NULL && kuangSpr9->getParent() != NULL) {
                kuangSpr9->removeFromParentAndCleanup(true);
                kuangSpr9 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr9 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr9->setPosition(ccp(22, 135));
            kuangSpr9->setTag(pCell->getIdx());
            node->addChild(kuangSpr9, 5);
        }else if (sub_part == 20){
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr10 = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr10->setPosition(ccp(22, 135));
            kuangSpr10->setTag(pCell->getIdx());
            node->addChild(kuangSpr10, 5);
            
            if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                kuangSpr3->removeFromParentAndCleanup(true);
                kuangSpr3 = NULL;
            }
            if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
                kuangSpr4->removeFromParentAndCleanup(true);
                kuangSpr4 = NULL;
            }
        }
        else{
            if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
                kuangSpr->removeFromParentAndCleanup(true);
                kuangSpr = NULL;
            }
            CCNode* node = pCell->getChildByTag(pCell->getIdx());
            kuangSpr = CCSprite::create("res/nxpic/NXClothes/NXClothes_yichuan.png");
            kuangSpr->setPosition(ccp(22, 135));
            kuangSpr->setTag(pCell->getIdx());
            node->addChild(kuangSpr, 5);
        }
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangClothesIndex", (CCObject* )0);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ButtonStatus", NULL);
    }
    sliderV = pTableView->getContentOffset().y;
    this->updateTableCell();
}







