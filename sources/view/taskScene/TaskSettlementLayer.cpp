//
//  TaskSettlementLayer.cpp
//  tiegao
//
//  Created by mac on 16-6-29.
//
//

#include "TaskSettlementLayer.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "TaskScene.h"
#include "ClothesScene.h"
#include "DisplayManager.h"
#include "SpecialManager.h"


TaskSettlementLayer::~TaskSettlementLayer(){
}

TaskSettlementLayer* TaskSettlementLayer::create(int rating, int coin, bool isPhaseUP) {
    TaskSettlementLayer* rtn = new TaskSettlementLayer();
    if (rtn && rtn->init(rating, coin, isPhaseUP)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool TaskSettlementLayer::init(int rating, int coin, bool isPhaseUP){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->initAniamtion();
    
    _rating = rating;
    _coin = coin;
    _isPhaseUP = isPhaseUP;
    
    lingquBool = false;
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    this->creat_view();
    this->creat_Man();
    this->initClothes();
    
    return true;
}

void TaskSettlementLayer::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&TaskSettlementLayer::nc_coin_fly_completed), "COIN_FLY_COMPLETED", NULL);
    
    SPECIAL->showPetal2At(this, DISPLAY->center(), 1);
    
    this->scheduleOnce(SEL_SCHEDULE(&TaskSettlementLayer::keyBackStatus), .8f);
}
void TaskSettlementLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void TaskSettlementLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void TaskSettlementLayer::keyBackClicked(){
    
}

bool TaskSettlementLayer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent){
//    if (!lingquBool) {
//        // 给个提示
//    }else{
//        exit();
//    }
    
    return true;
}


void TaskSettlementLayer::creat_view(){
    CCSprite* bgSpr1 = CCSprite::create("res/pic/taskSettlement/ts_bg1.png");
    bgSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr1);
    
    CCString* renStr = CCString::createWithFormat("res/pic/taskSettlement/ts_ren%d.png", 1);
    CCSprite* renSpr = CCSprite::create(renStr->getCString());
    renSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .19f, DISPLAY->ScreenHeight()* .13f));
    this->addChild(renSpr, 15);
    
    // 下框
    CCSprite* kuangSpr1 = CCSprite::create("res/pic/taskSettlement/ts_kuang1.png");
    kuangSpr1->setPosition(ccp(renSpr->getContentSize().width + kuangSpr1->getContentSize().width* .33f, renSpr->getContentSize().height* .3f));
    renSpr->addChild(kuangSpr1);
    
    CCString* tishiStr;
    int randIndex1 = 0;
    int randIndex2 = 0;
    int randIndex3 = 0;
    int randIndex4 = 0;
    if (_rating == 5) {
        randIndex1 = 80 + rand()%20;
        randIndex2 = 80 + rand()%20;
        randIndex3 = 80 + rand()%20;
        randIndex4 = 80 + rand()%20;
        tishiStr = CCString::createWithFormat("不错不错.很是大方得体.");
    }else if (_rating == 4) {
        randIndex1 = 60 + rand()%20;
        randIndex2 = 60 + rand()%20;
        randIndex3 = 60 + rand()%20;
        randIndex4 = 60 + rand()%20;
        tishiStr = CCString::createWithFormat("还可以吧.吐槽君还是可以接受的.");
    }else if (_rating == 3) {
        randIndex1 = 40 + rand()%20;
        randIndex2 = 40 + rand()%20;
        randIndex3 = 40 + rand()%20;
        randIndex4 = 40 + rand()%20;
        tishiStr = CCString::createWithFormat("你的审美是那个星系的~!不敢恭维.");
    }else if (_rating == 2) {
        randIndex1 = 20 + rand()%20;
        randIndex2 = 20 + rand()%20;
        randIndex3 = 20 + rand()%20;
        randIndex4 = 20 + rand()%20;
        tishiStr = CCString::createWithFormat("你这样,还处的去门嘛.");
    }else if (_rating == 1) {
        randIndex1 = 2 + rand()%20;
        randIndex2 = 2 + rand()%20;
        randIndex3 = 2 + rand()%20;
        randIndex4 = 2 + rand()%20;
        tishiStr = CCString::createWithFormat("你光着屁股出门都比这一身强.");
    }
    CCLabelTTF* tishiLabel = CCLabelTTF::create(tishiStr->getCString(), DISPLAY->fangzhengFont(), 23, CCSizeMake(kuangSpr1->getContentSize().width* .88f, kuangSpr1->getContentSize().height* .5f), kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    tishiLabel->setPosition(ccp(kuangSpr1->getContentSize().width* .5f, kuangSpr1->getContentSize().height* .55f));
    tishiLabel->setColor(ccc3(80, 63, 68));
    kuangSpr1->addChild(tishiLabel);
    
    // 魅力
    CCSprite* taskSpr1 = CCSprite::create("res/pic/taskScene/task_1.png");
    taskSpr1->setPosition(ccp(kuangSpr1->getContentSize().width* .15f, kuangSpr1->getContentSize().height* .45f));
    kuangSpr1->addChild(taskSpr1, 10);
    CCString* taskStr1 = CCString::createWithFormat("%d", randIndex1);
    CCLabelTTF* taskLabel1 = CCLabelTTF::create(taskStr1->getCString(), DISPLAY->fangzhengFont(), 20);
    taskLabel1->setAnchorPoint(ccp(0, .5f));
    taskLabel1->setPosition(ccp(taskSpr1->getContentSize().width + 5, taskSpr1->getContentSize().height* .45f));
    taskLabel1->setColor(ccc3(80, 63, 68));
    taskSpr1->addChild(taskLabel1);
    
    // 沟通
    CCSprite* taskSpr2 = CCSprite::create("res/pic/taskScene/task_2.png");
    taskSpr2->setPosition(ccp(kuangSpr1->getContentSize().width* .45f, kuangSpr1->getContentSize().height* .45f));
    kuangSpr1->addChild(taskSpr2, 10);
    CCString* taskStr2 = CCString::createWithFormat("%d", randIndex2);
    CCLabelTTF* taskLabel2 = CCLabelTTF::create(taskStr2->getCString(), DISPLAY->fangzhengFont(), 20);
    taskLabel2->setAnchorPoint(ccp(0, .5f));
    taskLabel2->setPosition(ccp(taskSpr2->getContentSize().width + 5, taskSpr2->getContentSize().height* .45f));
    taskLabel2->setColor(ccc3(80, 63, 68));
    taskSpr2->addChild(taskLabel2);
    
    // 业务
    CCSprite* taskSpr3 = CCSprite::create("res/pic/taskScene/task_3.png");
    taskSpr3->setPosition(ccp(kuangSpr1->getContentSize().width* .15f, kuangSpr1->getContentSize().height* .25f));
    kuangSpr1->addChild(taskSpr3, 10);
    CCString* taskStr3 = CCString::createWithFormat("%d", randIndex3);
    CCLabelTTF* taskLabel3 = CCLabelTTF::create(taskStr3->getCString(), DISPLAY->fangzhengFont(), 20);
    taskLabel3->setAnchorPoint(ccp(0, .5f));
    taskLabel3->setPosition(ccp(taskSpr3->getContentSize().width + 5, taskSpr3->getContentSize().height* .45f));
    taskLabel3->setColor(ccc3(80, 63, 68));
    taskSpr3->addChild(taskLabel3);
    
    // 运气
    CCSprite* taskSpr4 = CCSprite::create("res/pic/taskScene/task_4.png");
    taskSpr4->setPosition(ccp(kuangSpr1->getContentSize().width* .45f, kuangSpr1->getContentSize().height* .25f));
    kuangSpr1->addChild(taskSpr4, 10);
    CCString* taskStr4 = CCString::createWithFormat("%d", randIndex4);
    CCLabelTTF* taskLabel4 = CCLabelTTF::create(taskStr4->getCString(), DISPLAY->fangzhengFont(), 20);
    taskLabel4->setAnchorPoint(ccp(0, .5f));
    taskLabel4->setPosition(ccp(taskSpr4->getContentSize().width + 5, taskSpr4->getContentSize().height* .45f));
    taskLabel4->setColor(ccc3(80, 63, 68));
    taskSpr4->addChild(taskLabel4);
    
    //
    CCSprite* coinSpr = CCSprite::create("res/pic/clothesScene/gj_coin.png");
    coinSpr->setPosition(ccp(kuangSpr1->getContentSize().width* .79f, kuangSpr1->getContentSize().height* .55f));
    kuangSpr1->addChild(coinSpr);
    CCString* coinStr = CCString::createWithFormat("x%d", _coin);
    CCLabelTTF* coinLabel = CCLabelTTF::create(coinStr->getCString(), DISPLAY->fangzhengFont(), 25);
    coinLabel->setAnchorPoint(ccp(0, .5f));
    coinLabel->setPosition(ccp(coinSpr->getContentSize().width + 5, coinSpr->getContentSize().height* .4f));
    coinLabel->setColor(ccc3(80, 63, 68));
    coinSpr->addChild(coinLabel);
    
    CCSprite* lingquSpr1 = CCSprite::create("res/pic/common/btn_take.png");
    lingquSpr1->setScale(.9f);
    CCSprite* lingquSpr2 = CCSprite::create("res/pic/common/btn_take.png");
    lingquSpr2->setScale(.92f);
    CCMenuItem* lingquItem = CCMenuItemSprite::create(lingquSpr1, lingquSpr2, this, menu_selector(TaskSettlementLayer::lingquCallBack));
    lingquItem->setPosition(ccp(kuangSpr1->getContentSize().width* .86f, kuangSpr1->getContentSize().height* .28f));
    CCMenu* menu = CCMenu::create(lingquItem, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr1->addChild(menu);
    
    // 上条
    tiaoSpr = CCSprite::create("res/pic/taskSettlement/ts_tiao.png");
    tiaoSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .86f));
    this->addChild(tiaoSpr, 15);
    
    CCSprite* done_text = CCSprite::create("res/pic/taskSettlement/ts_done_text.png");
    done_text->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .63f));
    tiaoSpr->addChild(done_text);
    
    
    CCSprite* bgSpr2 = CCSprite::create("res/pic/taskSettlement/ts_bg2.png");
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr2, 20);
    
    
    this->scheduleOnce(SEL_SCHEDULE(&TaskSettlementLayer::startAnimation), .5f);
}
void TaskSettlementLayer::startAnimation(float dt){
    if (_rating >= 5) {
        CCSprite* xingSpr = CCSprite::create("res/pic/taskScene/animation/5/xing_0.png");
        xingSpr->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .95f));
        xingSpr->setTag(0x88999);
        tiaoSpr->addChild(xingSpr, 100);
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("xingStr5");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCDirector::sharedDirector()->getActionManager()->addAction(animate, xingSpr, false);
        
    }
    else if (_rating == 4) {
        CCSprite* xingSpr = CCSprite::create("res/pic/taskScene/animation/4/xing_0.png");
        xingSpr->setPosition(ccp(tiaoSpr->getContentSize().width* .44f, tiaoSpr->getContentSize().height* .95f));
        xingSpr->setTag(0x88999);
        tiaoSpr->addChild(xingSpr, 100);
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("xingStr4");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCDirector::sharedDirector()->getActionManager()->addAction(animate, xingSpr, false);
    }
    else if (_rating == 3) {
        CCSprite* xingSpr = CCSprite::create("res/pic/taskScene/animation/3/xing_0.png");
        xingSpr->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .95f));
        xingSpr->setTag(0x88999);
        tiaoSpr->addChild(xingSpr, 100);
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("xingStr3");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCDirector::sharedDirector()->getActionManager()->addAction(animate, xingSpr, false);
    }
    else if (_rating == 2) {
        CCSprite* xingSpr = CCSprite::create("res/pic/taskScene/animation/2/xing_0.png");
        xingSpr->setPosition(ccp(tiaoSpr->getContentSize().width* .42f, tiaoSpr->getContentSize().height* .95f));
        xingSpr->setTag(0x88999);
        tiaoSpr->addChild(xingSpr, 100);
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("xingStr2");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCDirector::sharedDirector()->getActionManager()->addAction(animate, xingSpr, false);
    }
    else {
        CCSprite* xingSpr = CCSprite::create("res/pic/taskScene/animation/1/xing_0.png");
        xingSpr->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .95f));
        xingSpr->setTag(0x88999);
        tiaoSpr->addChild(xingSpr, 100);
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("xingStr1");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCDirector::sharedDirector()->getActionManager()->addAction(animate, xingSpr, false);
    }
}

void TaskSettlementLayer::lingquCallBack(CCNode* pSender){
    lingquBool = true;
    if (pSender) {
        pSender->setVisible(false);
    }
//    CCDictionary* postData = CCDictionary::create();
//    postData->setObject(CCInteger::create(_coin), "num");
//    CCPoint pos = ccp(DISPLAY->halfW() - 350, DISPLAY->H() * 0.15);
//    postData->setObject(CCString::createWithFormat("{%f,%f}", pos.x, pos.y), "from");
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_COIN_FLY", postData);
    SPECIAL->show_coin_reward(this, _coin, ccp(DISPLAY->halfW() + 200, DISPLAY->H() * 0.15), ccp(DISPLAY->halfW(), DISPLAY->H() * 0.82));
}

void TaskSettlementLayer::creat_Man(){
    float widthFolt = .62f;
    float heightFloat = .52f;
    float scaleFloat = .8f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}

void TaskSettlementLayer::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = .62f;
    float heightFloat = .52f;
    float scaleFloat = .8f;
    bool flipxBool = false;
    
    for (int i = Tag_GJ_TouFa; i <= Tag_GJ_ZhuangRong; i++) {
        if (i == Tag_GJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_GJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_GJ_TouFa2);
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
                            _tfSpr1->setTag(Tag_GJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_GJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_GJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_GJ_WaiTao1);
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
                            _wtSpr1->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_GJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_GJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
//                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
//                _sySpr1 = CCSprite::create(str->getCString());
//                _sySpr1->setScale(scaleFloat);
//                _sySpr1->setFlipX(flipxBool);
//                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
//                _sySpr1->setTag(Tag_GJ_ShangYi1);
//                _ManSpr->addChild(_sySpr1, 350);
                
                CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
                xingSpr1->setFlipX(flipxBool);
                xingSpr1->setScale(.6f);
                xingSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .386f, DISPLAY->ScreenHeight()* .623f));
                _ManSpr->addChild(xingSpr1, 420);
                
                CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
                xingSpr2->setFlipX(flipxBool);
                xingSpr2->setScale(.6f);
                xingSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .475f, DISPLAY->ScreenHeight()* .618f));
                _ManSpr->addChild(xingSpr2, 420);
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
                            _sySpr1->setTag(Tag_GJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_GJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_GJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setScale(scaleFloat);
                _kzSpr1->setFlipX(flipxBool);
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _kzSpr1->setTag(Tag_GJ_KuZi1);
                _ManSpr->addChild(_kzSpr1, 290);
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
                            _kzSpr1->setTag(Tag_GJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_GJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_GJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_GJ_WaZi1);
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
                            _wzSpr1->setTag(Tag_GJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_GJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_GJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_GJ_XieZi1);
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
                            _xzSpr1->setTag(Tag_GJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_GJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_GJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿饰品的字典
            
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
        else if (i == Tag_GJ_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_GJ_Bao1);
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
                            _bSpr1->setTag(Tag_GJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_GJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_GJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}

void TaskSettlementLayer::exit() {
    CCLayer* layer = TaskScene::create(1);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void TaskSettlementLayer::initAniamtion(){
    CCArray* animations1 = CCArray::createWithCapacity(40);
    char strPei1[100] = {};
    for (int i = 0; i <= 34; i++) {
        
        sprintf(strPei1, "res/pic/taskScene/animation/1/xing_%d.png", i);
        CCSpriteFrame *frame1 = CCSpriteFrame::create(strPei1,CCRectMake(0, 0, 200, 432));
        animations1->addObject(frame1);
    }
    CCAnimation* animation1 = CCAnimation::createWithSpriteFrames(animations1, 0.04f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "xingStr1");
    
    
    CCArray* animations2 = CCArray::createWithCapacity(50);
    char strPei2[100] = {};
    for (int i = 0; i <= 47; i++) {
        
        sprintf(strPei2, "res/pic/taskScene/animation/2/xing_%d.png", i);
        CCSpriteFrame *frame2 = CCSpriteFrame::create(strPei2,CCRectMake(0, 0, 330, 432));
        animations2->addObject(frame2);
    }
    CCAnimation* animation2 = CCAnimation::createWithSpriteFrames(animations2, 0.04f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "xingStr2");
    
    
    CCArray* animations3 = CCArray::createWithCapacity(50);
    char strPei3[100] = {};
    for (int i = 0; i <= 48; i++) {
        
        sprintf(strPei3, "res/pic/taskScene/animation/3/xing_%d.png", i);
        CCSpriteFrame *frame3 = CCSpriteFrame::create(strPei3,CCRectMake(0, 0, 499, 432));
        animations3->addObject(frame3);
    }
    CCAnimation* animation3 = CCAnimation::createWithSpriteFrames(animations3, 0.04f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation3, "xingStr3");
    
    
    CCArray* animations4 = CCArray::createWithCapacity(65);
    char strPei4[100] = {};
    for (int i = 0; i <= 59; i++) {
        
        sprintf(strPei4, "res/pic/taskScene/animation/4/xing_%d.png", i);
        CCSpriteFrame *frame4 = CCSpriteFrame::create(strPei4,CCRectMake(0, 0, 499, 432));
        animations4->addObject(frame4);
    }
    CCAnimation* animation4 = CCAnimation::createWithSpriteFrames(animations4, 0.04f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation4, "xingStr4");
    
    
    CCArray* animations5 = CCArray::createWithCapacity(65);
    char strPei5[100] = {};
    for (int i = 0; i <= 60; i++) {
        
        sprintf(strPei5, "res/pic/taskScene/animation/5/xing_%d.png", i);
        CCSpriteFrame *frame5 = CCSpriteFrame::create(strPei5,CCRectMake(0, 0, 499, 432));
        animations5->addObject(frame5);
    }
    CCAnimation* animation5 = CCAnimation::createWithSpriteFrames(animations5, 0.04f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation5, "xingStr5");
}

void TaskSettlementLayer::nc_coin_fly_completed(CCObject *pObj) {
    exit();
}

