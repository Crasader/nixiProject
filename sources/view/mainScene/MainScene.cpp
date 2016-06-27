//
//  MainScene.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "MainScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "QingjingScene.h"
#include "TaskScene.h"
#include "ClothesScene.h"

#include "HaoyouRankLayer.h"
#include "StrangerScene.h"
#include "TotalRankScene.h"
#include "HaoyouScene.h"

#include "Loading2.h"
#include "NetManager.h"
#include "ConfigManager.h"

#include "MailPanel.h"
#include "OperationPanel.h"

MainScene::MainScene(){
    
}
MainScene::~MainScene(){
    
}

bool MainScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
//    this->setTouchSwallowEnabled(true);
//    this->setTouchMode(kCCTouchesOneByOne);
//    this->setTouchEnabled(true);
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    this->creat_view();
    this->creat_Man();
    this->initClothes();
    
    this->schedule(schedule_selector(MainScene::gengxin), .1f);
    
    return true;
}

CCScene* MainScene::scene(){
    CCScene* scene = CCScene::create();
    MainScene* layer = MainScene::create();
    scene->addChild(layer);
    
    return scene;
}


void MainScene::onEnter(){
    BaseScene::onEnter();
    this->setAccelerometerEnabled(true); // ?
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_huanzhuangCallBack), "HTTP_FINISHED_400", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_500CallBack), "HTTP_FINISHED_500", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::all_mail_callback_700), "HTTP_FINISHED_700", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_600CallBack), "HTTP_FINISHED_600", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::social_info_callback_800), "HTTP_FINISHED_800", NULL);
}

void MainScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    BaseScene::onExit();
}

void MainScene::keyBackClicked(){
    
}


void MainScene::didAccelerate( CCAcceleration* pAccelerationValue){
    float deceleration = 0 ;
    float sensitivity = 500;
    
    playerVelocity.x = playerVelocity.x * deceleration+ pAccelerationValue->x*sensitivity;
    
    playerVelocity.x = playerVelocity.x/ 10;
}

void MainScene::gengxin(float dt){
    if (roomSpr == NULL) {
        return;
    }
    
    CCPoint pos = roomSpr->getPosition();
    pos.x = DISPLAY->ScreenWidth()* 0.5 + playerVelocity.x;
    if (pos.x < DISPLAY->ScreenWidth()* 0.5 - 70) {
        pos.x = DISPLAY->ScreenWidth()* 0.5 - 70;
        playerVelocity = CCPointZero;
    }
    else if (pos.x > DISPLAY->ScreenWidth()* 0.5 + 70){
        pos.x = DISPLAY->ScreenWidth()* 0.5 + 70;
        playerVelocity = CCPointZero;
    }
    roomSpr->runAction(CCMoveTo::create(0.1f, ccp(pos.x, pos.y)));
}


bool MainScene::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent){
    
    
    return true;
}

void MainScene::creat_view(){
    
    roomSpr = CCSprite::create("res/pic/mainScene/main_bg.png");
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(roomSpr);
    
    // 首冲
    CCSprite* scSpr1 = CCSprite::create("res/pic/mainScene/main_chong.png");
    CCSprite* scSpr2 = CCSprite::create("res/pic/mainScene/main_chong.png");
    scSpr2->setScale(1.02f);
    CCMenuItem* shouchongItem = CCMenuItemSprite::create(scSpr1, scSpr2, this, menu_selector(MainScene::shouchongCallBack));
    shouchongItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .89f));
    
    // 活动
    CCSprite* hdSpr1 = CCSprite::create("res/pic/mainScene/main_huodong.png");
    CCSprite* hdSpr2 = CCSprite::create("res/pic/mainScene/main_huodong.png");
    hdSpr2->setScale(1.02f);
    CCMenuItem* huodongItem = CCMenuItemSprite::create(hdSpr1, hdSpr2, this, menu_selector(MainScene::huodongCallBack));
    huodongItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .8f));
    
    // 签到
    CCSprite* qdSpr1 = CCSprite::create("res/pic/mainScene/main_qiandao.png");
    CCSprite* qdSpr2 = CCSprite::create("res/pic/mainScene/main_qiandao.png");
    qdSpr2->setScale(1.02f);
    CCMenuItem* qiandaoItem = CCMenuItemSprite::create(qdSpr1, qdSpr2, this, menu_selector(MainScene::qiandaoCallBack));
    qiandaoItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .71f));
    
    // 邮件
    CCSprite* yjSpr1 = CCSprite::create("res/pic/mainScene/main_youjian.png");
    CCSprite* yjSpr2 = CCSprite::create("res/pic/mainScene/main_youjian.png");
    yjSpr2->setScale(1.02f);
    CCMenuItem* youjianItem = CCMenuItemSprite::create(yjSpr1, yjSpr2, this, menu_selector(MainScene::youjianCallBack));
    youjianItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .89f));
    
    // 好友
    CCSprite* hySpr1 = CCSprite::create("res/pic/mainScene/main_haoyou.png");
    CCSprite* hySpr2 = CCSprite::create("res/pic/mainScene/main_haoyou.png");
    hySpr2->setScale(1.02f);
    CCMenuItem* haoyouItem = CCMenuItemSprite::create(hySpr1, hySpr2, this, menu_selector(MainScene::haoyouCallBack));
    haoyouItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .8f));
    
    // 换装
    CCSprite* hzSpr1 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    CCSprite* hzSpr2 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    hzSpr2->setScale(1.02f);
    CCMenuItem* huanzhuangItem = CCMenuItemSprite::create(hzSpr1, hzSpr2, this, menu_selector(MainScene::huanzhuangCallBack));
    huanzhuangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .71f));
    
    // 排行
    CCSprite* phSpr1 = CCSprite::create("res/pic/mainScene/main_paihang.png");
    CCSprite* phSpr2 = CCSprite::create("res/pic/mainScene/main_paihang.png");
    phSpr2->setScale(1.02f);
    CCMenuItem* paihangItem = CCMenuItemSprite::create(phSpr1, phSpr2, this, menu_selector(MainScene::paihangCallBack));
    paihangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .62f));
    
    CCMenu* menu = CCMenu::create(shouchongItem, huodongItem, qiandaoItem, haoyouItem, youjianItem, huanzhuangItem, paihangItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    // 剧情
    CCSprite* jqSpr1 = CCSprite::create("res/pic/mainScene/main_juqing.png");
    CCSprite* jqSpr2 = CCSprite::create("res/pic/mainScene/main_juqing.png");
    jqSpr2->setScale(1.02f);
    CCMenuItem* juqingItem = CCMenuItemSprite::create(jqSpr1, jqSpr2, this, menu_selector(MainScene::juqingCallBack));
    juqingItem->setPosition(ccp(DISPLAY->ScreenWidth()* .91f, DISPLAY->ScreenHeight()* .3f));
    
    //日常
    CCSprite* rcSpr1 = CCSprite::create("res/pic/mainScene/main_richang.png");
    CCSprite* rcSpr2 = CCSprite::create("res/pic/mainScene/main_richang.png");
    rcSpr2->setScale(1.02f);
    CCMenuItem* richangItem = CCMenuItemSprite::create(rcSpr1, rcSpr2, this, menu_selector(MainScene::richangCallBack));
    richangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .91f, DISPLAY->ScreenHeight()* .2f));
    
    //设置
    CCSprite* szSpr1 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
    CCSprite* szSpr2 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
    szSpr2->setScale(1.02f);
    CCMenuItem* shezhiItem = CCMenuItemSprite::create(szSpr1, szSpr2, this, menu_selector(MainScene::shezhiCallBack));
    shezhiItem->setPosition(ccp(DISPLAY->ScreenWidth()* .09f, DISPLAY->ScreenHeight()* .05f));
    
    CCMenu* downMenu = CCMenu::create(juqingItem, richangItem, shezhiItem, NULL);
    downMenu->setPosition(CCPointZero);
    this->addChild(downMenu);
    
}

void MainScene::shouchongCallBack(CCObject* pSender){
    
}

void MainScene::huodongCallBack(CCObject* pSender){
    OperationPanel* panel = OperationPanel::create();
    panel->show_from(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .85f));
}

void MainScene::qiandaoCallBack(CCObject* pSender){
    
}

void MainScene::youjianCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->all_mails_700();
}

void MainScene::haoyouCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->social_info_800();
}

void MainScene::social_info_callback_800(CCObject* pObj) {
    LOADING->remove();
    CCScene* scene = HaoyouScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::renwuCallBack(CCObject* pSender){
    
}

void MainScene::huanzhuangCallBack(CCObject* pSender){
    if (DATA->getClothes()->has_init_clothes == true) {
        this->_huanzhuangCallBack(pSender);
    }
    else {
        LOADING->show_loading();
        NET->owned_clothes_400();
    }
}
void MainScene::_huanzhuangCallBack(CCObject* pSender){
    
    CCLayer* layer = ClothesScene::create_with_type(2, 0, 0);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void MainScene::paihangCallBack(CCObject* pSender){
    
}
void MainScene::juqingCallBack(CCObject* pSender){
    if (DATA->getStory()->has_init_story()) {
        this->_500CallBack(NULL);
    }else{
        LOADING->show_loading();
        NET->completed_story_500();
    }
}
void MainScene::_500CallBack(CCObject* pSender){
    CCScene* scene = QingjingScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void MainScene::richangCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->completed_mission_600();
}
void MainScene::_600CallBack(CCObject* pSender){
    LOADING->remove();
    
    CCScene* scene = TaskScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::shezhiCallBack(CCObject* pSender){
    
}


void MainScene::creat_Man(){
    float widthFolt = .65f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void MainScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = .65f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    
    for (int i = Tag_Rank_TouFa; i <= Tag_Rank_Bao; i++) {
        if (i == Tag_Rank_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_Rank_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_Rank_TouFa2);
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
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_Rank_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_Rank_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_Rank_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_Rank_WaiTao1);
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
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_Rank_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_Rank_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_Rank_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_Rank_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
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
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_Rank_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_Rank_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_Rank_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setScale(scaleFloat);
                _kzSpr1->setFlipX(flipxBool);
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _kzSpr1->setTag(Tag_Rank_KuZi1);
                _ManSpr->addChild(_kzSpr1, 305);
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
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_Rank_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_Rank_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_Rank_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_Rank_WaZi1);
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
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr1->setTag(Tag_Rank_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_Rank_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_Rank_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_Rank_XieZi1);
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
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_Rank_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_Rank_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_Rank_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
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
                                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_Rank_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_Rank_Bao1);
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
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_Rank_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_Rank_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_Rank_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}

        
void MainScene::all_mail_callback_700(cocos2d::CCObject *pObj) {
    LOADING->remove();
    MailPanel* panel = MailPanel::create();
    panel->show_from(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .85f));
}




