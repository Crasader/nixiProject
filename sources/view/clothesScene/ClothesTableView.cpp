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
    now_CellIndex = 0;
    
    this->clothesType = Tag_GJ_TouFa;
    
    kuangSpr = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr->retain();
    kuangSpr1 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr1->retain();
    kuangSpr2 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr2->retain();
    kuangSpr3 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr3->retain();
    kuangSpr4 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr4->retain();
    kuangSpr5 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr5->retain();
    kuangSpr6 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr6->retain();
    kuangSpr7 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr7->retain();
    kuangSpr8 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr8->retain();
    kuangSpr9 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
    kuangSpr9->retain();
    kuangSpr10 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
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
    
    CCArray* arr = DATA->getDataSource();

    CCPoint contOffsetPos = pTableView->getContentOffset();
    if (arr->count() > 5) {
        if (contOffsetPos.y < pTableView->minContainerOffset().y) {
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->minContainerOffset().y));
        }else if (contOffsetPos.y > pTableView->maxContainerOffset().y){
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->maxContainerOffset().y));
        }
    }
}

//点击哪个cell
void ClothesTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    now_CellIndex = cell->getIdx();
    
    CCArray* arr = DATA->getDataSource();
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp(); // 临时数组
    
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(cell->getIdx());
    int cloth_id = dic->valueForKey("id")->intValue(); // 点击的衣服
    int sub_part = dic->valueForKey("sub_part")->intValue(); // 衣服的部位
    CCInteger* cloth_integer;
    CCInteger* clothesTemp_id;
    CCDictionary* shipinDic;
    
    if (clothesType != Tag_GJ_ShiPin) {
        clothesTemp_id = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString()); // 男宠当前所穿上衣
    }else{
        shipinDic = (CCDictionary* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());// 获取所穿视频的字典
        clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", sub_part)->getCString());
    }
    
    if (cloth_id == clothesTemp_id->getValue()) {
        cloth_id = updataClothes(clothesType);
        
        cloth_integer = CCInteger::create(cloth_id);
        CCString* keyStr;
        if (clothesType != Tag_GJ_ShiPin) {
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
        
        
        if (clothesType != Tag_GJ_ShiPin) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        }else{
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangClothesIndex", (CCObject* )clothesTemp_id->getValue());
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        }
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ButtonStatus", NULL);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("Creat_money", NULL);
    }else{
        cloth_integer = CCInteger::create(cloth_id);
        CCString* keyStr;
        if (clothesType != Tag_GJ_ShiPin) {
            if (clothesType == Tag_GJ_ShangYi) {
                if (sub_part == 1) {
                    CCInteger* kuziInteger = CCInteger::create(40000);
                    CCString* kuziStr = CCString::createWithFormat("%d", Tag_GJ_KuZi);
                    clothesTemp->setObject(kuziInteger, kuziStr->getCString());
                    
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }else{
                    keyStr = CCString::createWithFormat("%d", clothesType);
                    clothesTemp->setObject(cloth_integer, keyStr->getCString());
                }
            }else if (clothesType == Tag_GJ_KuZi) {
                CCInteger* shangyiInteger = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", Tag_GJ_ShangYi)->getCString());
                int shangyi_sub_part = 0;
                CCDictionary* shangyiDic = CONFIG->clothes();// 所有衣服
                CCArray* shangyiArr = (CCArray* )shangyiDic->objectForKey(Tag_GJ_ShangYi);// 获得当前类型所有衣服
                for (int i = 0; i < shangyiArr->count(); i++) {
                    CCDictionary* syDic = (CCDictionary* )shangyiArr->objectAtIndex(i);
                    int shangyiId = syDic->valueForKey("id")->intValue();
                    if (shangyiId == shangyiInteger->getValue()) {
                        shangyi_sub_part = syDic->valueForKey("sub_part")->intValue();
                    }
                }
                
                if (shangyi_sub_part == 1) {
                    CCInteger* shangyiInteger = CCInteger::create(30000);
                    CCString* shangyiStr = CCString::createWithFormat("%d", Tag_GJ_ShangYi);
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
            kuangSpr1 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr1->setPosition(ccp(18, 170));
            kuangSpr1->setTag(cell->getIdx());
            node->addChild(kuangSpr1, 5);
        }else if (sub_part == 12){
            if (kuangSpr2 != NULL && kuangSpr2->getParent() != NULL) {
                kuangSpr2->removeFromParentAndCleanup(true);
                kuangSpr2 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr2 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr2->setPosition(ccp(18, 170));
            kuangSpr2->setTag(cell->getIdx());
            node->addChild(kuangSpr2, 5);
        }else if (sub_part == 13){
            if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                kuangSpr3->removeFromParentAndCleanup(true);
                kuangSpr3 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr3 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr3->setPosition(ccp(18, 170));
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
            kuangSpr4 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr4->setPosition(ccp(18, 170));
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
            kuangSpr5 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr5->setPosition(ccp(18, 170));
            kuangSpr5->setTag(cell->getIdx());
            node->addChild(kuangSpr5, 5);
        }else if (sub_part == 16){
            if (kuangSpr6 != NULL && kuangSpr6->getParent() != NULL) {
                kuangSpr6->removeFromParentAndCleanup(true);
                kuangSpr6 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr6 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr6->setPosition(ccp(18, 170));
            kuangSpr6->setTag(cell->getIdx());
            node->addChild(kuangSpr6, 5);
        }else if (sub_part == 17){
            if (kuangSpr7 != NULL && kuangSpr7->getParent() != NULL) {
                kuangSpr7->removeFromParentAndCleanup(true);
                kuangSpr7 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr7 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr7->setPosition(ccp(18, 170));
            kuangSpr7->setTag(cell->getIdx());
            node->addChild(kuangSpr7, 5);
        }else if (sub_part == 18){
            if (kuangSpr8 != NULL && kuangSpr8->getParent() != NULL) {
                kuangSpr8->removeFromParentAndCleanup(true);
                kuangSpr8 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr8 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr8->setPosition(ccp(18, 170));
            kuangSpr8->setTag(cell->getIdx());
            node->addChild(kuangSpr8, 5);
        }else if (sub_part == 19){
            if (kuangSpr9 != NULL && kuangSpr9->getParent() != NULL) {
                kuangSpr9->removeFromParentAndCleanup(true);
                kuangSpr9 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr9 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr9->setPosition(ccp(18, 170));
            kuangSpr9->setTag(cell->getIdx());
            node->addChild(kuangSpr9, 5);
        }else if (sub_part == 20){
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr10 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr10->setPosition(ccp(18, 170));
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
            CCNode* node = cell->getChildByTag(cell->getIdx());
            kuangSpr = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr->setPosition(ccp(18, 170));
            kuangSpr->setTag(cell->getIdx());
            node->addChild(kuangSpr, 5);
        }
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangClothesIndex", (CCObject* )0);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )cloth_id);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ButtonStatus", NULL);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("Creat_money", NULL);
    }
    sliderV = pTableView->getContentOffset().y;
    this->updateTableCell();
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
    int sub_part = dic->valueForKey("sub_part")->intValue(); // 衣服的部位
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
    }else if (clothesType == Tag_GJ_ZhuangRong){
        str = CCString::createWithFormat("res/pic/clothesScene/icon/9zhuangrong/icon%d.png", cloth_id);
    }
//    str = CCString::createWithFormat("res/pic/clothesScene/111.png");
    CCSprite* clothSpr = CCSprite::create(str->getCString());
    if (!clothSpr) {
        clothSpr = CCSprite::create("pic/unknown.png");
    }
    
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
            CCString* costStr = CCString::create("非卖品");
            CCLabelTTF* costLabel = CCLabelTTF::create(costStr->getCString(), DISPLAY->fangzhengFont(), 17, CCSizeMake(jgkuang->getContentSize().width* .9f, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            costLabel->setPosition(ccp(jgkuang->getContentSize().width* .5f, jgkuang->getContentSize().height* .48f));
            costLabel->setColor(ccWHITE);
            jgkuang->addChild(costLabel);
        }else{
            CCString* costStr = CCString::create("非卖品");
            CCLabelTTF* costLabel = CCLabelTTF::create(costStr->getCString(), DISPLAY->fangzhengFont(), 17, CCSizeMake(jgkuang->getContentSize().width* .9f, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            costLabel->setPosition(ccp(jgkuang->getContentSize().width* .5f, jgkuang->getContentSize().height* .48f));
            costLabel->setColor(ccWHITE);
            jgkuang->addChild(costLabel);
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
    
    
    CCInteger* clothesTemp_id;
    if (clothesType != Tag_GJ_ShiPin) {
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
            kuangSpr1 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr1->setPosition(ccp(18, 170));
            kuangSpr1->setTag(idx);
            spr->addChild(kuangSpr1, 5);
        }else if (sub_part == 12){
            if (kuangSpr2 != NULL && kuangSpr2->getParent() != NULL) {
                kuangSpr2->removeFromParentAndCleanup(true);
                kuangSpr2 = NULL;
            }
            kuangSpr2 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr2->setPosition(ccp(18, 170));
            kuangSpr2->setTag(idx);
            spr->addChild(kuangSpr2, 5);
        }else if (sub_part == 13){
            if (kuangSpr3 != NULL && kuangSpr3->getParent() != NULL) {
                kuangSpr3->removeFromParentAndCleanup(true);
                kuangSpr3 = NULL;
            }
            kuangSpr3 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr3->setPosition(ccp(18, 170));
            kuangSpr3->setTag(idx);
            spr->addChild(kuangSpr3, 5);
        }else if (sub_part == 14){
            if (kuangSpr4 != NULL && kuangSpr4->getParent() != NULL) {
                kuangSpr4->removeFromParentAndCleanup(true);
                kuangSpr4 = NULL;
            }
            kuangSpr4 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr4->setPosition(ccp(18, 170));
            kuangSpr4->setTag(idx);
            spr->addChild(kuangSpr4, 5);
        }else if (sub_part == 15){
            if (kuangSpr5 != NULL && kuangSpr5->getParent() != NULL) {
                kuangSpr5->removeFromParentAndCleanup(true);
                kuangSpr5 = NULL;
            }
            kuangSpr5 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr5->setPosition(ccp(18, 170));
            kuangSpr5->setTag(idx);
            spr->addChild(kuangSpr5, 5);
        }else if (sub_part == 16){
            if (kuangSpr6 != NULL && kuangSpr6->getParent() != NULL) {
                kuangSpr6->removeFromParentAndCleanup(true);
                kuangSpr6 = NULL;
            }
            kuangSpr6 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr6->setPosition(ccp(18, 170));
            kuangSpr6->setTag(idx);
            spr->addChild(kuangSpr6, 5);
        }else if (sub_part == 17){
            if (kuangSpr7 != NULL && kuangSpr7->getParent() != NULL) {
                kuangSpr7->removeFromParentAndCleanup(true);
                kuangSpr7 = NULL;
            }
            kuangSpr7 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr7->setPosition(ccp(18, 170));
            kuangSpr7->setTag(idx);
            spr->addChild(kuangSpr7, 5);
        }else if (sub_part == 18){
            if (kuangSpr8 != NULL && kuangSpr8->getParent() != NULL) {
                kuangSpr8->removeFromParentAndCleanup(true);
                kuangSpr8 = NULL;
            }
            kuangSpr8 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr8->setPosition(ccp(18, 170));
            kuangSpr8->setTag(idx);
            spr->addChild(kuangSpr8, 5);
        }else if (sub_part == 19){
            if (kuangSpr9 != NULL && kuangSpr9->getParent() != NULL) {
                kuangSpr9->removeFromParentAndCleanup(true);
                kuangSpr9 = NULL;
            }
            kuangSpr9 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr9->setPosition(ccp(18, 170));
            kuangSpr9->setTag(idx);
            spr->addChild(kuangSpr9, 5);
        }else if (sub_part == 20){
            if (kuangSpr10 != NULL && kuangSpr10->getParent() != NULL) {
                kuangSpr10->removeFromParentAndCleanup(true);
                kuangSpr10 = NULL;
            }
            kuangSpr10 = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr10->setPosition(ccp(18, 170));
            kuangSpr10->setTag(idx);
            spr->addChild(kuangSpr10, 5);
        }
        else{
            if (kuangSpr != NULL && kuangSpr->getParent() != NULL) {
                kuangSpr->removeFromParentAndCleanup(true);
                kuangSpr = NULL;
            }
            kuangSpr = CCSprite::create("res/pic/clothesScene/gj_yichuan1.png");
            kuangSpr->setPosition(ccp(18, 170));
            kuangSpr->setTag(idx);
            spr->addChild(kuangSpr, 5);
        }
    }
    
    int phase = dic->valueForKey("phase")->intValue();
    if (phase > DATA->getPlayer()->phase) {
        CCSprite* phaseSpr = CCSprite::create("res/pic/clothesScene/gj_suo.png");
        phaseSpr->setAnchorPoint(CCPointZero);
        phaseSpr->setPosition(CCPointZero);
        spr->addChild(phaseSpr, 100);
    }
    
    
    spr->setTag(idx);
    pCell->addChild(spr);
    
    return pCell;
}

//cell的数量
unsigned int ClothesTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    CCArray* arr = DATA->getDataSource();
    clothesPage = arr->count();
//    CCLog("_dataArr->count ======= %d", arr->count());
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
int ClothesTableView::updataClothes(int type){
    int index = 0;
    index = (type) * 10000;
    
    return index;
}
void ClothesTableView::updateTableCell(){
    pTableView->reloadData();
    pTableView->setContentOffset(CCSizeMake(0, sliderV));
}

void ClothesTableView::clothesUpdateTableCell(){
    if (now_CellIndex > 0) {
        pTableView->updateCellAtIndex(now_CellIndex-1);
        pTableView->updateCellAtIndex(now_CellIndex);
        pTableView->updateCellAtIndex(now_CellIndex+1);
    }else{
        pTableView->updateCellAtIndex(now_CellIndex);
        pTableView->updateCellAtIndex(now_CellIndex+1);
    }
}











