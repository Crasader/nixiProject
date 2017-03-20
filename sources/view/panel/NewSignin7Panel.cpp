//
//  NewSignin7Panel.cpp
//  tiegao
//
//  Created by mac on 17-3-17.
//
//

#include "NewSignin7Panel.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "NetManager.h"
#include "MainScene.h"
#include "Loading2.h"
#include "ClothesScene.h"
#include "QingjingScene.h"

#include "MZResourceLoader.h"
#include "AudioManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "BuildingLayer.h"
#include "TDCCAccount.h"
#include "TDCCTalkingDataGA.h"
#include "JNIController.h"


#pragma mark - Export

void NewSignin7Panel::show(CCNode* parent) {
    NewSignin7Panel* panel = NewSignin7Panel::create();
    parent->addChild(panel);
}


#pragma mark - Super
NewSignin7Panel::~NewSignin7Panel(){
    
}
bool NewSignin7Panel::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    
    CCSprite* diSpr = CCSprite::create("res/pic/panel/newSignin7/newSignin_di.png");
    diSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(diSpr);
    
    kuangSpr = CCSprite::create("res/pic/panel/newSignin7/newSignin_dikuang.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(kuangSpr);
    
    _ManSpr = CCSprite::create();
    kuangSpr->addChild(_ManSpr, 10);
    
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->creatView();
    
    return true;
}
void NewSignin7Panel::onEnter(){
    CCLayer::onEnter();
    
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(NewSignin7Panel::signin_callback_313), "HTTP_FINISHED_313", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&NewSignin7Panel::keyBackStatus), .8f);
}
void NewSignin7Panel::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("WHEN_SIGNIN7_EXIT");
    
    CCLayer::onExit();
}

void NewSignin7Panel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void NewSignin7Panel::keyBackClicked(){
    num_child++;
//    CCLog("===== GashaponScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    if (DATA->current_guide_step() == 0) {
        this->removeFromParentAndCleanup(true);
    }
}

bool NewSignin7Panel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    if (! kuangSpr->boundingBox().containsPoint(location)) {
        AUDIO->goback_effect();
        
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

void NewSignin7Panel::creatView(){
    
    CCSize panelSize = kuangSpr->boundingBox().size;
    
    CCSprite* icon_1 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_1->setPosition(ccp(panelSize.width* .2f, panelSize.height* .78f));
    icon_1->setTag(1);
    kuangSpr->addChild(icon_1, 20);
    
    CCSprite* icon_2 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_2->setPosition(ccp(panelSize.width* .45f, panelSize.height* .78f));
    icon_2->setTag(2);
    kuangSpr->addChild(icon_2, 20);
    
    CCSprite* icon_3 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_3->setPosition(ccp(panelSize.width* .2f, panelSize.height* .6f));
    icon_3->setTag(3);
    kuangSpr->addChild(icon_3, 20);
    
    CCSprite* icon_4 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_4->setPosition(ccp(panelSize.width* .45f, panelSize.height* .6f));
    icon_4->setTag(4);
    kuangSpr->addChild(icon_4, 20);
    
    CCSprite* icon_5 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_5->setPosition(ccp(panelSize.width* .2f, panelSize.height* .42f));
    icon_5->setTag(5);
    kuangSpr->addChild(icon_5, 20);
    
    CCSprite* icon_6 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_6->setPosition(ccp(panelSize.width* .45f, panelSize.height* .42f));
    icon_6->setTag(6);
    kuangSpr->addChild(icon_6, 20);
    
    CCSprite* icon_7 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_7->setPosition(ccp(panelSize.width* .2f, panelSize.height* .24f));
    icon_7->setTag(7);
    kuangSpr->addChild(icon_7, 20);
    
    CCSprite* icon_8 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_8->setPosition(ccp(panelSize.width* .45f, panelSize.height* .24f));
    icon_8->setTag(8);
    kuangSpr->addChild(icon_8, 20);
    
    
    
    CCDictionary* _signin7_infoDic = DATA->getSignin()->signin7_info();
    CCArray* keys = _signin7_infoDic->allKeys();
    AppUtil::sort_string_array(keys);
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(keys, pObj) {
        CCString* key = (CCString*)pObj;
        _signin7Day = atoi(key->getCString());
        nowPage = _signin7Day;
        _signin7Value = ((CCInteger* )_signin7_infoDic->objectForKey(key->getCString()))->getValue();
    }
    
    bool buttonBool = false;
    CCArray* _signin7Arr = DATA->getSignin()->signin7_template();
    if (_signin7Value == 1) {// 可以领
        
        CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(nowPage - 1);
        std::string type = item->valueForKey("type")->getCString();
        if (type.compare("wealth") == 0) {
            CCDictionary* rewardsDic = (CCDictionary* )item->objectForKey("rewards");
            this->creat_wealth(rewardsDic);
        }else if (type.compare("clothes") == 0){
            CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
            this->creat_clothes(rewardsArr);
        }
        
        buttonBool = true;
    }else if (_signin7Value == 2){// 签过了
        nowPage++;
        if (nowPage >= _signin7Arr->count()) {
            nowPage = _signin7Arr->count();
        }
        CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(nowPage - 1);
        std::string type = item->valueForKey("type")->getCString();
        if (type.compare("wealth") == 0) {
            CCDictionary* rewardsDic = (CCDictionary* )item->objectForKey("rewards");
            this->creat_wealth(rewardsDic);
        }else if (type.compare("clothes") == 0){
            CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
            this->creat_clothes(rewardsArr);
        }
        
        buttonBool = false;
    }
    
    if (nowPage <= 2) {
        CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(1);
        std::string type = item->valueForKey("type")->getCString();
        if (type.compare("clothes") == 0){
            CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
            this->creat_Clothes(rewardsArr);
            this->creatMan();
            this->initClothes();
        }
    }else if (nowPage > 2 && nowPage <= 7){
        CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(6);
        std::string type = item->valueForKey("type")->getCString();
        if (type.compare("clothes") == 0){
            CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
            this->creat_Clothes(rewardsArr);
            this->creatMan();
            this->initClothes();
        }
    }
    
    
    // 提示
    CCSprite* tishiSpr = CCSprite::create("res/pic/panel/newSignin7/newSignin_tishi.png");
    tishiSpr->setAnchorPoint(ccp(1.f, 1.f));
    tishiSpr->setPosition(ccp(panelSize.width - 15, panelSize.height - 3));
    kuangSpr->addChild(tishiSpr);
    CCString* tishiStr;
    if (_signin7Day <= 2) {
        tishiStr = CCString::createWithFormat("%d", 2 - _signin7Day);
    }else if (_signin7Day > 2 && _signin7Day <= 7){
        tishiStr = CCString::createWithFormat("%d", 7 - _signin7Day);
    }
    CCLabelTTF* tishiLabel = CCLabelTTF::create(tishiStr->getCString(), DISPLAY->fangzhengFont(), 30);
    tishiLabel->setPosition(ccp(tishiSpr->getContentSize().width* .5f, tishiSpr->getContentSize().height* .26f));
    tishiLabel->setColor(ccRED);
    tishiSpr->addChild(tishiLabel);
    
    
    CCSprite* signinSpr1 = CCSprite::create("res/pic/panel/newSignin7/newSignin_lingqu.png");
    CCSprite* signinSpr2 = CCSprite::create("res/pic/panel/newSignin7/newSignin_lingqu.png");
    signinSpr2->setScale(1.02f);
    CCMenuItem* signinItem;
    if (buttonBool) {
        signinItem = CCMenuItemSprite::create(signinSpr1, signinSpr2, this, menu_selector(NewSignin7Panel::signinCallback));
    }else{
        signinItem = CCMenuItemSprite::create(signinSpr1, signinSpr2);
        signinItem->setColor(ccGRAY);
    }
    signinItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .1f));
    CCMenu* menu = CCMenu::create(signinItem, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu, 50);
    
    
    // 箭头
    CCSprite* jiantouSpr1_1 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr1_1->setFlipX(true);
    CCSprite* jiantouSpr1_2 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr1_2->setFlipX(true);
    jiantouSpr1_2->setScale(1.02f);
    CCMenuItem* jiantouItem1 = CCMenuItemSprite::create(jiantouSpr1_1, jiantouSpr1_2, this, menu_selector(NewSignin7Panel::jiantou1CallBack));
    jiantouItem1->setPosition(ccp(-10, kuangSpr->getContentSize().height* .5f));
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.5f, ccp(-20, kuangSpr->getContentSize().height* .5f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.8f, ccp(-10, kuangSpr->getContentSize().height* .5f));
    CCSequence* seq1 = CCSequence::create(moveTo1_1, moveTo1_2, NULL);
    jiantouItem1->runAction(CCRepeatForever::create(seq1));
    
    CCSprite* jiantouSpr2_1 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    CCSprite* jiantouSpr2_2 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr2_2->setScale(1.02f);
    CCMenuItem* jiantouItem2 = CCMenuItemSprite::create(jiantouSpr2_1, jiantouSpr2_2, this, menu_selector(NewSignin7Panel::jiantou2CallBack));
    jiantouItem2->setPosition(ccp(kuangSpr->getContentSize().width + 10, kuangSpr->getContentSize().height* .5f));
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width + 20, kuangSpr->getContentSize().height* .5f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.8f, ccp(kuangSpr->getContentSize().width + 10, kuangSpr->getContentSize().height* .5f));
    CCSequence* seq2 = CCSequence::create(moveTo2_1, moveTo2_2, NULL);
    jiantouItem2->runAction(CCRepeatForever::create(seq2));
    
    CCMenu* jiantouMenu = CCMenu::create(jiantouItem1, jiantouItem2, NULL);
    jiantouMenu->setPosition(CCPointZero);
    kuangSpr->addChild(jiantouMenu, 20);
}

void NewSignin7Panel::creat_wealth(CCDictionary* dic){
    for (int i = dic->count() + 1; i <= 8; i++) {
        CCNode* node = kuangSpr->getChildByTag(i);
        node->setVisible(false);
    }
    
    CCSize panelSize = kuangSpr->boundingBox().size;
    CCNode* node1 = kuangSpr->getChildByTag(1);
    node1->setPosition(ccp(panelSize.width* .2f, panelSize.height* .65f));
    
    CCNode* node2 = kuangSpr->getChildByTag(2);
    node2->setPosition(ccp(panelSize.width* .45f, panelSize.height* .65f));
    
    CCNode* node3 = kuangSpr->getChildByTag(3);
    node3->setPosition(ccp(panelSize.width* .2f, panelSize.height* .45f));
    
    CCNode* node4 = kuangSpr->getChildByTag(4);
    node4->setPosition(ccp(panelSize.width* .45f, panelSize.height* .45f));
    
    
    for (int i = 1; i <= dic->count(); i++) {
        CCNode* node = kuangSpr->getChildByTag(i);
        CCString* str;
        CCSprite* spr;
        if (i == 1) {// 碎片
            str = CCString::createWithFormat("%d", ((CCInteger* )dic->objectForKey("piece"))->getValue());
            spr = CCSprite::create("pic/building/reward_piece.png");
        }else if (i == 2){// 体力
            str = CCString::createWithFormat("%d", ((CCInteger* )dic->objectForKey("energy"))->getValue());
            spr = CCSprite::create("pic/building/reward_energy.png");
        }else if (i == 3){// 钻石
            str = CCString::createWithFormat("%d", ((CCInteger* )dic->objectForKey("diam"))->getValue());
            spr = CCSprite::create("pic/building/reward_diam.png");
        }else if (i == 4){// 金币
            str = CCString::createWithFormat("%d", ((CCInteger* )dic->objectForKey("coin"))->getValue());
            spr = CCSprite::create("pic/building/reward_coin.png");
        }
        CCLabelTTF* label = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 30);
        label->setPosition(ccp(node->getContentSize().width* .5f, node->getContentSize().height* .16f));
        label->setColor(ccc3(113, 89, 102));
        node->addChild(label);
        
        if (spr != NULL) {
            spr->setScale(1.8f);
            spr->setPosition(ccp(node->getContentSize().width* .5f, node->getContentSize().height* .6f));
            node->addChild(spr);
        }
    }
}
void NewSignin7Panel::creat_clothes(CCArray* arr){
    int index = arr->count();
    for (int i = index + 1; i <= 8; i++) {
        CCNode* node = kuangSpr->getChildByTag(i);
        node->setVisible(false);
    }
    
    
    CCSize panelSize = kuangSpr->boundingBox().size;
    if (index == 4) {
        CCNode* node1 = kuangSpr->getChildByTag(1);
        node1->setPosition(ccp(panelSize.width* .2f, panelSize.height* .65f));
        
        CCNode* node2 = kuangSpr->getChildByTag(2);
        node2->setPosition(ccp(panelSize.width* .45f, panelSize.height* .65f));
        
        CCNode* node3 = kuangSpr->getChildByTag(3);
        node3->setPosition(ccp(panelSize.width* .2f, panelSize.height* .45f));
        
        CCNode* node4 = kuangSpr->getChildByTag(4);
        node4->setPosition(ccp(panelSize.width* .45f, panelSize.height* .45f));
    }else if (index == 5){
        CCNode* node1 = kuangSpr->getChildByTag(1);
        node1->setPosition(ccp(panelSize.width* .2f, panelSize.height* .7f));
        
        CCNode* node2 = kuangSpr->getChildByTag(2);
        node2->setPosition(ccp(panelSize.width* .45f, panelSize.height* .7f));
        
        CCNode* node3 = kuangSpr->getChildByTag(3);
        node3->setPosition(ccp(panelSize.width* .2f, panelSize.height* .5f));
        
        CCNode* node4 = kuangSpr->getChildByTag(4);
        node4->setPosition(ccp(panelSize.width* .45f, panelSize.height* .5f));
        
        CCNode* node5 = kuangSpr->getChildByTag(5);
        node5->setPosition(ccp(panelSize.width* .325f, panelSize.height* .3f));
    }else if (index == 6){
        CCNode* node1 = kuangSpr->getChildByTag(1);
        node1->setPosition(ccp(panelSize.width* .2f, panelSize.height* .7f));
        
        CCNode* node2 = kuangSpr->getChildByTag(2);
        node2->setPosition(ccp(panelSize.width* .45f, panelSize.height* .7f));
        
        CCNode* node3 = kuangSpr->getChildByTag(3);
        node3->setPosition(ccp(panelSize.width* .2f, panelSize.height* .5f));
        
        CCNode* node4 = kuangSpr->getChildByTag(4);
        node4->setPosition(ccp(panelSize.width* .45f, panelSize.height* .5f));
        
        CCNode* node5 = kuangSpr->getChildByTag(5);
        node5->setPosition(ccp(panelSize.width* .2f, panelSize.height* .3f));
        
        CCNode* node6 = kuangSpr->getChildByTag(6);
        node6->setPosition(ccp(panelSize.width* .45f, panelSize.height* .3f));
    }else if (index == 7){
        CCNode* node7 = kuangSpr->getChildByTag(7);
        node7->setPosition(ccp(panelSize.width* .325f, panelSize.height* .24f));
    }
    
    
    for (int i = 0; i < arr->count(); i++) {
        CCNode* node = kuangSpr->getChildByTag(i + 1);
        int index = ((CCInteger* )arr->objectAtIndex(i))->getValue();
        CCString* str;
        if (index >= 10000 && index < 20000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/1toufa/icon%d.png", index);
        }else if (index >= 20000 && index < 30000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/2waitao/icon%d.png", index);
        }else if (index >= 30000 && index < 40000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/3shangyi/icon%d.png", index);
        }else if (index >= 40000 && index < 50000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/4kuzi/icon%d.png", index);
        }else if (index >= 50000 && index < 60000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/5wazi/icon%d.png", index);
        }else if (index >= 60000 && index < 70000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/6xiezi/icon%d.png", index);
        }else if (index >= 70000 && index < 80000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/7shipin/icon%d.png", index);
        }else if (index >= 80000 && index < 90000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/8bao/icon%d.png", index);
        }else if (index >= 90000 && index < 100000){
            str = CCString::createWithFormat("res/pic/clothesScene/icon/9zhuangrong/icon%d.png", index);
        }
        CCSprite* clothSpr = CCSprite::create(str->getCString());
        if (!clothSpr) {
            clothSpr = CCSprite::create("res/pic/unknown.png");
        }
        clothSpr->setScale(.5f);
        clothSpr->setPosition(ccp(node->getContentSize().width* .5f, node->getContentSize().height* .5f));
        node->addChild(clothSpr);
    }
}
void NewSignin7Panel::signinCallback(CCObject* pSender){
    LOADING->show_loading();
    NET->perform_signin7_313();
}
void NewSignin7Panel::signin_callback_313(){
    LOADING->remove();
    nowPage++;
    if (nowPage >= 7) {
        nowPage = 7;
    }
    this->creatNewView();
}
void NewSignin7Panel::creatNewView(){
    kuangSpr->removeAllChildrenWithCleanup(true);
    _ManSpr = CCSprite::create();
    kuangSpr->addChild(_ManSpr, 10);
    
    
    CCSize panelSize = kuangSpr->boundingBox().size;
    
    CCSprite* icon_1 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_1->setPosition(ccp(panelSize.width* .2f, panelSize.height* .78f));
    icon_1->setTag(1);
    kuangSpr->addChild(icon_1, 20);
    
    CCSprite* icon_2 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_2->setPosition(ccp(panelSize.width* .45f, panelSize.height* .78f));
    icon_2->setTag(2);
    kuangSpr->addChild(icon_2, 20);
    
    CCSprite* icon_3 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_3->setPosition(ccp(panelSize.width* .2f, panelSize.height* .6f));
    icon_3->setTag(3);
    kuangSpr->addChild(icon_3, 20);
    
    CCSprite* icon_4 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_4->setPosition(ccp(panelSize.width* .45f, panelSize.height* .6f));
    icon_4->setTag(4);
    kuangSpr->addChild(icon_4, 20);
    
    CCSprite* icon_5 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_5->setPosition(ccp(panelSize.width* .2f, panelSize.height* .42f));
    icon_5->setTag(5);
    kuangSpr->addChild(icon_5, 20);
    
    CCSprite* icon_6 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_6->setPosition(ccp(panelSize.width* .45f, panelSize.height* .42f));
    icon_6->setTag(6);
    kuangSpr->addChild(icon_6, 20);
    
    CCSprite* icon_7 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_7->setPosition(ccp(panelSize.width* .2f, panelSize.height* .24f));
    icon_7->setTag(7);
    kuangSpr->addChild(icon_7, 20);
    
    CCSprite* icon_8 = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
    icon_8->setPosition(ccp(panelSize.width* .45f, panelSize.height* .24f));
    icon_8->setTag(8);
    kuangSpr->addChild(icon_8, 20);
    
    
    
    CCDictionary* _signin7_infoDic = DATA->getSignin()->signin7_info();
    CCArray* keys = _signin7_infoDic->allKeys();
    AppUtil::sort_string_array(keys);
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(keys, pObj) {
        CCString* key = (CCString*)pObj;
        _signin7Day = atoi(key->getCString());
        _signin7Value = ((CCInteger* )_signin7_infoDic->objectForKey(key->getCString()))->getValue();
    }
    
    bool buttonBool = false;
    CCArray* _signin7Arr = DATA->getSignin()->signin7_template();
    if (nowPage == _signin7Day) {
        if (_signin7Value == 1) {// 可以领
            CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(nowPage - 1);
            std::string type = item->valueForKey("type")->getCString();
            if (type.compare("wealth") == 0) {
                CCDictionary* rewardsDic = (CCDictionary* )item->objectForKey("rewards");
                this->creat_wealth(rewardsDic);
            }else if (type.compare("clothes") == 0){
                CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
                this->creat_clothes(rewardsArr);
            }
            
            buttonBool = true;
        }else if (_signin7Value == 2){// 签过了
            if (nowPage >= _signin7Arr->count()) {
                nowPage = _signin7Arr->count();
            }
            CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(nowPage - 1);
            std::string type = item->valueForKey("type")->getCString();
            if (type.compare("wealth") == 0) {
                CCDictionary* rewardsDic = (CCDictionary* )item->objectForKey("rewards");
                this->creat_wealth(rewardsDic);
            }else if (type.compare("clothes") == 0){
                CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
                this->creat_clothes(rewardsArr);
            }
            
            buttonBool = false;
        }
    }else{
        if (nowPage >= _signin7Arr->count()) {
            nowPage = _signin7Arr->count();
        }
        CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(nowPage - 1);
        std::string type = item->valueForKey("type")->getCString();
        if (type.compare("wealth") == 0) {
            CCDictionary* rewardsDic = (CCDictionary* )item->objectForKey("rewards");
            this->creat_wealth(rewardsDic);
        }else if (type.compare("clothes") == 0){
            CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
            this->creat_clothes(rewardsArr);
        }
        
        buttonBool = false;
    }
    
    
    if (nowPage <= 2) {
        CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(1);
        std::string type = item->valueForKey("type")->getCString();
        if (type.compare("clothes") == 0){
            CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
            this->creat_Clothes(rewardsArr);
            this->creatMan();
            this->initClothes();
        }
    }else if (nowPage > 2 && nowPage <= 7){
        CCDictionary* item = (CCDictionary*)_signin7Arr->objectAtIndex(6);
        std::string type = item->valueForKey("type")->getCString();
        if (type.compare("clothes") == 0){
            CCArray* rewardsArr = (CCArray* )item->objectForKey("rewards");
            this->creat_Clothes(rewardsArr);
            this->creatMan();
            this->initClothes();
        }
    }
    
    
    // 提示
    CCSprite* tishiSpr = CCSprite::create("res/pic/panel/newSignin7/newSignin_tishi.png");
    tishiSpr->setAnchorPoint(ccp(1.f, 1.f));
    tishiSpr->setPosition(ccp(panelSize.width - 15, panelSize.height - 3));
    kuangSpr->addChild(tishiSpr);
    CCString* tishiStr;
    if (_signin7Day <= 2) {
        tishiStr = CCString::createWithFormat("%d", 2 - _signin7Day);
    }else if (_signin7Day > 2 && _signin7Day <= 7){
        tishiStr = CCString::createWithFormat("%d", 7 - _signin7Day);
    }
    CCLabelTTF* tishiLabel = CCLabelTTF::create(tishiStr->getCString(), DISPLAY->fangzhengFont(), 30);
    tishiLabel->setPosition(ccp(tishiSpr->getContentSize().width* .5f, tishiSpr->getContentSize().height* .26f));
    tishiLabel->setColor(ccRED);
    tishiSpr->addChild(tishiLabel);
    
    
    CCSprite* signinSpr1 = CCSprite::create("res/pic/panel/newSignin7/newSignin_lingqu.png");
    CCSprite* signinSpr2 = CCSprite::create("res/pic/panel/newSignin7/newSignin_lingqu.png");
    signinSpr2->setScale(1.02f);
    CCMenuItem* signinItem;
    if (buttonBool) {
        signinItem = CCMenuItemSprite::create(signinSpr1, signinSpr2, this, menu_selector(NewSignin7Panel::signinCallback));
    }else{
        signinItem = CCMenuItemSprite::create(signinSpr1, signinSpr2);
        signinItem->setColor(ccGRAY);
    }
    signinItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .1f));
    CCMenu* menu = CCMenu::create(signinItem, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu, 50);
    
    
    
    
    // 箭头
    CCSprite* jiantouSpr1_1 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr1_1->setFlipX(true);
    CCSprite* jiantouSpr1_2 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr1_2->setFlipX(true);
    jiantouSpr1_2->setScale(1.02f);
    CCMenuItem* jiantouItem1 = CCMenuItemSprite::create(jiantouSpr1_1, jiantouSpr1_2, this, menu_selector(NewSignin7Panel::jiantou1CallBack));
    jiantouItem1->setPosition(ccp(-10, kuangSpr->getContentSize().height* .5f));
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.5f, ccp(-20, kuangSpr->getContentSize().height* .5f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.8f, ccp(-10, kuangSpr->getContentSize().height* .5f));
    CCSequence* seq1 = CCSequence::create(moveTo1_1, moveTo1_2, NULL);
    jiantouItem1->runAction(CCRepeatForever::create(seq1));
    
    CCSprite* jiantouSpr2_1 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    CCSprite* jiantouSpr2_2 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr2_2->setScale(1.02f);
    CCMenuItem* jiantouItem2 = CCMenuItemSprite::create(jiantouSpr2_1, jiantouSpr2_2, this, menu_selector(NewSignin7Panel::jiantou2CallBack));
    jiantouItem2->setPosition(ccp(kuangSpr->getContentSize().width + 10, kuangSpr->getContentSize().height* .5f));
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width + 20, kuangSpr->getContentSize().height* .5f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.8f, ccp(kuangSpr->getContentSize().width + 10, kuangSpr->getContentSize().height* .5f));
    CCSequence* seq2 = CCSequence::create(moveTo2_1, moveTo2_2, NULL);
    jiantouItem2->runAction(CCRepeatForever::create(seq2));
    
    CCMenu* jiantouMenu = CCMenu::create(jiantouItem1, jiantouItem2, NULL);
    jiantouMenu->setPosition(CCPointZero);
    kuangSpr->addChild(jiantouMenu, 20);
}
void NewSignin7Panel::jiantou1CallBack(CCObject* pSender){
    nowPage--;
    if (nowPage < 1) {
        nowPage = 7;
    }
    this->creatNewView();
}
void NewSignin7Panel::jiantou2CallBack(CCObject* pSender){
    nowPage++;
    if (nowPage > 7) {
        nowPage = 1;
    }
    this->creatNewView();
}


void NewSignin7Panel::creatMan(){
    // man坐标
//    panelSize.width* .72f, panelSize.height * 0.46f
    float widthFolt = .83f;
    float heightFloat = .57f;
    float scaleFloat = .63f;
    bool flipxBool = false;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setFlipX(flipxBool);
    _touSpr->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height * heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void NewSignin7Panel::initClothes(){//穿衣服
    float widthFolt = .83f;
    float heightFloat = .57f;
    float scaleFloat = .63f;
    bool flipxBool = false;
    int sub_part = 0;
    
    CCDictionary* allClothesDic = CONFIG->clothes();// 所有衣服
    
    for (int i = Tag_QJ_TouFa; i <= Tag_QJ_ZhuangRong; i++) {
        if (i == Tag_QJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _tfSpr1->setTag(Tag_QJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _tfSpr2->setTag(Tag_QJ_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _tfSpr1->setTag(Tag_QJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _tfSpr2->setTag(Tag_QJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _tfSpr3->setTag(Tag_QJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _wtSpr1->setTag(Tag_QJ_WaiTao1);
                _ManSpr->addChild(_wtSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wtSpr1->setTag(Tag_QJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wtSpr2->setTag(Tag_QJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wtSpr3->setTag(Tag_QJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _sySpr1->setTag(Tag_QJ_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        sub_part = clothDic->valueForKey("sub_part")->intValue();
                        
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _sySpr1->setTag(Tag_QJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _sySpr2->setTag(Tag_QJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _sySpr3->setTag(Tag_QJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                    _kzSpr1->setTag(Tag_QJ_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                    _kzSpr1->setTag(Tag_QJ_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _kzSpr1->setTag(Tag_QJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _kzSpr2->setTag(Tag_QJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _kzSpr3->setTag(Tag_QJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _wzSpr1->setTag(Tag_QJ_WaZi1);
                _ManSpr->addChild(_wzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wzSpr1->setTag(Tag_QJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wzSpr2->setTag(Tag_QJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wzSpr3->setTag(Tag_QJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _xzSpr1->setTag(Tag_QJ_XieZi1);
                _ManSpr->addChild(_xzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _xzSpr1->setTag(Tag_QJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _xzSpr2->setTag(Tag_QJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _xzSpr3->setTag(Tag_QJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr->addChild(_spSpr1, 50);
                }else{
                    CCDictionary* dic = CONFIG->clothes();// 所有衣服
                    CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                    for (int k = 0; k < clothesArr->count(); k++) {
                        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(k);
                        int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                        if (now_clothes_Id == cloth_id->getValue()) {
                            const CCString* layer1 =  clothDic->valueForKey("layer1");
                            const CCString* layer2 =  clothDic->valueForKey("layer2");
                            const CCString* layer3 =  clothDic->valueForKey("layer3");
                            if (layer1->compare("") != 0) {
                                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_QJ_Bao){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _bSpr1->setTag(Tag_QJ_Bao1);
                _ManSpr->addChild(_bSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _bSpr1->setTag(Tag_QJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _bSpr2->setTag(Tag_QJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _bSpr3->setTag(Tag_QJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_QJ_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _zrSpr1->setTag(Tag_QJ_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _zrSpr1->setTag(Tag_QJ_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}

void NewSignin7Panel::creat_Clothes(CCArray* suitsArr){
    CCDictionary* allClothesDic = CONFIG->clothes();// 所有衣服
    nowClothesTemp = CCDictionary::create();
    
    for (int i = Tag_CL_TouFa; i <= Tag_CL_ZhuangRong; i++) {
        CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
        bool clothesBool = false;
        if (i != Tag_CL_ShiPin) {
            CCInteger* clothesId;
            CCString* keyStr;
            for (int j = 0; j < clothesArr->count(); j++) {
                CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                int clthedIndex = clothDic->valueForKey("id")->intValue();
                //                CCLog("clthedIndex == %d", clthedIndex);
                keyStr = CCString::createWithFormat("%d", i);
                for (int k = 0; k < suitsArr->count(); k++) {
                    int nowClothesId = ((CCInteger* )suitsArr->objectAtIndex(k))->getValue();
                    if (nowClothesId == clthedIndex) {
                        clothesId = CCInteger::create(clthedIndex);
                        clothesBool = true;
                        break;
                    }
                }
                if (clothesBool) {
                    break;
                }
            }
            if (clothesBool) {
                clothesBool = false;
                nowClothesTemp->setObject(clothesId, keyStr->getCString());
            }else{
                clothesBool = false;
                CCInteger* cloth_integer = CCInteger::create(updataClothes(i));
                nowClothesTemp->setObject(cloth_integer, keyStr->getCString());
            }
        }else{
            CCDictionary* shipinDic = CCDictionary::create();
            CCString* keyStr = CCString::createWithFormat("%d", i);
            int k = 0;
            for (int n = 11; n <= 20; n++) {
                CCInteger* clothesId;
                CCString* subkeyStr;
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int sub_part = clothDic->valueForKey("sub_part")->intValue(); // 衣服的部位
                    if (sub_part == n) {
                        int clthedIndex = clothDic->valueForKey("id")->intValue();
//                        CCLog("clthedIndex == %d", clthedIndex);
                        subkeyStr = CCString::createWithFormat("%d", n);
                        for (int k = 0; k < suitsArr->count(); k++) {
                            int nowClothesId = ((CCInteger* )suitsArr->objectAtIndex(k))->getValue();
                            if (nowClothesId == clthedIndex) {
                                clothesBool = true;
                                clothesId = CCInteger::create(clthedIndex);
                                break;
                            }
                        }
                    }
                    if (clothesBool) {
                        break;
                    }
                }
                if (clothesBool) {
                    clothesBool = false;
                    shipinDic->setObject(clothesId, subkeyStr->getCString());
                }else{
                    clothesBool = false;
                    CCInteger* cloth_integer = CCInteger::create(updataClothes(i));
                    shipinDic->setObject(cloth_integer, subkeyStr->getCString());
                }
            }
            nowClothesTemp->setObject(shipinDic, keyStr->getCString());
        }
    }
    
}
int NewSignin7Panel::updataClothes(int type){
    return (type) * 10000;
}









