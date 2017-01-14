//
//  pkScene.cpp
//  tiegao
//
//  Created by mac on 17-1-6.
//
//

#include "pkScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "NetManager.h"
#include "AudioManager.h"

#include "PkLayer.h"
#include "RankListScene.h"
#include "ClothesScene.h"
#include "MainScene.h"

#include "AppUtil.h"
#include "Loading2.h"
#include "PromptLayer.h"

pkScene::pkScene(){
    
}
pkScene::~pkScene(){
    
}

bool pkScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    num_child = 0;
    
    xingBool1_1 = false;
    xingBool1_2 = false;
    xingBool1_3 = false;
    xingBool2_1 = false;
    xingBool2_2 = false;
    xingBool2_3 = false;
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    themeInfo = DATA->getCompetition()->getTheme();
    selfItem = DATA->getCompetition()->getSelf();
    
    this->creatAnimation();
    this->creat_view();
    
    this->creat_Man1();
    
    
    return true;
}
CCScene* pkScene::scene(){
    CCScene* scene = CCScene::create();
    pkScene* layer = pkScene::create();
    scene->addChild(layer);
    
    return scene;
}

void pkScene::onEnter(){
    BaseScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    nc->addObserver(this, SEL_CallFuncO(&pkScene::_823Callback), "HTTP_FINISHED_823", NULL);
    nc->addObserver(this, SEL_CallFuncO(&pkScene::_821Callback), "HTTP_FINISHED_821", NULL);
    nc->addObserver(this, SEL_CallFuncO(&pkScene::_825Callback), "HTTP_FINISHED_825", NULL);
    nc->addObserver(this, SEL_CallFuncO(&pkScene::_827Callback), "HTTP_FINISHED_827", NULL);
    
    
    this->scheduleOnce(SEL_SCHEDULE(&pkScene::keyBackStatus), .8f);
}
void pkScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void pkScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    BaseScene::onExit();
}
void pkScene::keyBackClicked(){
    num_child++;
    CCLog("===== pkScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    if (DATA->current_guide_step() == 0) {
        this->backCallBack(NULL);
    }
}

void pkScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
//    if (button_type == AH_BUTTON_TYPE_YESNO) {
//        if (button_tag == AH_BUTTON_TAG_YES) {
//            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_BUY_ENERGY");
//        }else if (button_tag == AH_BUTTON_TAG_NO){
//            PromptLayer* layer = PromptLayer::create();
//            layer->show_prompt(this->getScene(), "据说体力藏在活动里~!去看看活动吧.");
//        }
//    }
}

void pkScene::creat_view(){
    bgSpr = CCSprite::create("res/pic/pk/pk_bg.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(pkScene::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 20);
    
    // 搭配对决
    CCSprite* dapeiSpr = CCSprite::create("res/pic/pk/pk_dapei.png");
    dapeiSpr->setPosition(ccp(bgSpr->getContentSize().width* .5f, bgSpr->getContentSize().height* .9f));
    bgSpr->addChild(dapeiSpr, 4);
    this->creatGuang(dapeiSpr);//
    
    
    // 得分规则
    CCSprite* tishiKuangSpr = CCSprite::create("res/pic/pk/pk_kuang3.png");
    tishiKuangSpr->setAnchorPoint(ccp(1.f, .5f));
    tishiKuangSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 5.f, DISPLAY->ScreenHeight()* .78f));
    this->addChild(tishiKuangSpr, 5);
    this->creatScore(tishiKuangSpr);//
    
    
    // 皇冠
    CCSprite* huangguanSpr = CCSprite::create("res/pic/pk/pk_huangguan1.png");
    huangguanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .78f));
    this->addChild(huangguanSpr, 5);
    this->creatHuangguan(huangguanSpr);//
    
    
    // 匹配
    CCSprite* tishiSpr1 = CCSprite::create("res/pic/pk/pk_tishi1.png");
    tishiSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .67f));
    this->addChild(tishiSpr1, 10);
    
    
    // 人物框
    this->creatRenwuKuang();
    
    // 对决
    this->creatDuijue();
    
    // 主题
    this->creatZhuti();
    
    
    // 换装
    CCSprite* hzSpr1 = CCSprite::create("res/pic/pk/pk_button4.png");
    CCSprite* hzSpr2 = CCSprite::create("res/pic/pk/pk_button4.png");
    hzSpr2->setScale(1.02f);
    hzItem = CCMenuItemSprite::create(hzSpr1, hzSpr2, this, menu_selector(pkScene::huanzhuangCallback));
    hzItem->setPosition(ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .14f));
    
    // 换好
    CCSprite* hhSpr1 = CCSprite::create("res/pic/pk/pk_button2.png");
    CCSprite* hhSpr2 = CCSprite::create("res/pic/pk/pk_button2.png");
    hhSpr2->setScale(1.02f);
    hhItem = CCMenuItemSprite::create(hhSpr1, hhSpr2, this, menu_selector(pkScene::huanhaoCallback));
    hhItem->setPosition(ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .14f));
    
    // 开始
    CCSprite* startSpr1 = CCSprite::create("res/pic/pk/pk_button3.png");
    CCSprite* startSpr2 = CCSprite::create("res/pic/pk/pk_button3.png");
    startSpr2->setScale(1.02f);
    startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(pkScene::startCallback));
    startItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .14f));
    this->creatStart();
    if (selfItem->getStartTimes() >= themeInfo->getStartLimit()) {
        startItem->setEnabled(false);
        startItem->setColor(ccGRAY);
    }
    
    
    
    // 祝福
    CCSprite* zhufuSpr1 = CCSprite::create("res/pic/pk/pk_zhufu.png");
    CCSprite* zhufuSpr2 = CCSprite::create("res/pic/pk/pk_zhufu.png");
    zhufuSpr2->setScale(1.02f);
    zhufuItem = CCMenuItemSprite::create(zhufuSpr1, zhufuSpr2, this, menu_selector(pkScene::zhufuCallback));
    zhufuItem->setPosition(ccp(DISPLAY->ScreenWidth() - 100, DISPLAY->ScreenHeight()* .1f));
    this->creatZhufu(zhufuItem);
    
    CCMenu* buttonMenu = CCMenu::create(hzItem, hhItem, startItem, zhufuItem, NULL);
    buttonMenu->setPosition(CCPointZero);
    this->addChild(buttonMenu, 10);
    
    // 祝福动态效果
    CCSprite* zfAction = AppUtil::get_self_sprite("pic/pk/pk_zhufu.png");
    zhufuSpr1->addChild(zfAction);
    zfAction->runAction(CCRepeatForever::create(AppUtil::action_expand_fade_out(1.5f, 0.5f, 1.8f)));
    
    //
    hzItem->setVisible(true);
    hhItem->setVisible(true);
    startItem->setVisible(false);
    zhufuItem->setVisible(false);
}
void pkScene::creatScore(CCSprite* tishiKuangSpr){
    CCArray* ruleArr = themeInfo->getRule();
    
    CCLabelTTF* tishiLabel1 = CCLabelTTF::create("得分规则", DISPLAY->fangzhengFont(), 24, CCSizeMake(tishiKuangSpr->getContentSize().width* .9f, 24), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    tishiLabel1->setPosition(ccp(tishiKuangSpr->getContentSize().width* .5f, tishiKuangSpr->getContentSize().height* .83f));
    tishiLabel1->setColor(ccc3(173, 109, 93));
    tishiKuangSpr->addChild(tishiLabel1);
    

    CCLabelTTF* slLabel = CCLabelTTF::create("胜利:", DISPLAY->fangzhengFont(), 22);
    slLabel->setAnchorPoint(ccp(0.f, .5f));
    slLabel->setPosition(ccp(7.f, tishiKuangSpr->getContentSize().height* .56f));
    slLabel->setColor(ccc3(201, 128, 110));
    tishiKuangSpr->addChild(slLabel);
    
    
    CCDictionary* slDic = (CCDictionary* )ruleArr->objectAtIndex(0);
    CCInteger* slScoreInteger = (CCInteger* )slDic->objectForKey("score");
    CCInteger* slNumInteger = (CCInteger* )slDic->objectForKey("num");
    CCString* slStr2 = (CCString* )slDic->objectForKey("type");
    CCString* slStr = CCString::createWithFormat("%d分%d", slScoreInteger->getValue(), slNumInteger->getValue());
    slScore = CCLabelTTF::create(slStr->getCString(), DISPLAY->fangzhengFont(), 22);
    slScore->setPosition(ccp(tishiKuangSpr->getContentSize().width* .65f, tishiKuangSpr->getContentSize().height* .56f));
    slScore->setColor(ccc3(201, 128, 110));
    tishiKuangSpr->addChild(slScore);
    if (strcmp(slStr2->getCString(), "diam") == 0) {// 钻石
        CCSprite* goldSpr = CCSprite::create("res/pic/pk/pk_gold.png");
        goldSpr->setScale(.9f);
        goldSpr->setPosition(ccp(slScore->getContentSize().width + 10, slScore->getContentSize().height* .5f));
        slScore->addChild(goldSpr);
    }else if (strcmp(slStr2->getCString(), "coin") == 0){// 金币
        CCSprite* coinSpr = CCSprite::create("res/pic/clothesScene/gj_coin.png");
        coinSpr->setScale(.6f);
        coinSpr->setPosition(ccp(slScore->getContentSize().width + 10, slScore->getContentSize().height* .52f));
        slScore->addChild(coinSpr);
    }else if (strcmp(slStr2->getCString(), "piece") == 0){// 碎片
        CCSprite* debrisSpr = CCSprite::create("res/pic/clothesScene/gj_debris.png");
        debrisSpr->setScale(.6f);
        debrisSpr->setPosition(ccp(slScore->getContentSize().width + 10, slScore->getContentSize().height* .52f));
        slScore->addChild(debrisSpr);
    }
    
    
    CCLabelTTF* pjLabel = CCLabelTTF::create("平局:", DISPLAY->fangzhengFont(), 22);
    pjLabel->setAnchorPoint(ccp(0.f, .5f));
    pjLabel->setPosition(ccp(7.f, tishiKuangSpr->getContentSize().height* .34f));
    pjLabel->setColor(ccc3(201, 128, 110));
    tishiKuangSpr->addChild(pjLabel);
    
    CCDictionary* pjDic = (CCDictionary* )ruleArr->objectAtIndex(1);
    CCInteger* pjScoreInteger = (CCInteger* )pjDic->objectForKey("score");
    CCString* pjStr = CCString::createWithFormat("%d分", pjScoreInteger->getValue());
    pjScore = CCLabelTTF::create(pjStr->getCString(), DISPLAY->fangzhengFont(), 22);
    pjScore->setPosition(ccp(tishiKuangSpr->getContentSize().width* .65f, tishiKuangSpr->getContentSize().height* .34f));
    pjScore->setColor(ccc3(201, 128, 110));
    tishiKuangSpr->addChild(pjScore);
    
    
    CCLabelTTF* sbLabel = CCLabelTTF::create("失败:", DISPLAY->fangzhengFont(), 22);
    sbLabel->setAnchorPoint(ccp(0.f, .5f));
    sbLabel->setPosition(ccp(7.f, tishiKuangSpr->getContentSize().height* .12f));
    sbLabel->setColor(ccc3(201, 128, 110));
    tishiKuangSpr->addChild(sbLabel);
    
    CCDictionary* sbDic = (CCDictionary* )ruleArr->objectAtIndex(2);
    CCInteger* sbScoreInteger = (CCInteger* )sbDic->objectForKey("score");
    CCString* sbStr = CCString::createWithFormat("%d分", sbScoreInteger->getValue());
    sbScore = CCLabelTTF::create(sbStr->getCString(), DISPLAY->fangzhengFont(), 22);
    sbScore->setPosition(ccp(tishiKuangSpr->getContentSize().width* .65f, tishiKuangSpr->getContentSize().height* .12f));
    sbScore->setColor(ccc3(201, 128, 110));
    tishiKuangSpr->addChild(sbScore);
}
void pkScene::creatGuang(CCSprite* dapeiSpr){
    CCSprite* guangSpr = CCSprite::create("res/pic/pk/pk_guang.png");
    guangSpr->setAnchorPoint(ccp(.5f, 1));
    guangSpr->setPosition(ccp(dapeiSpr->getContentSize().width* .5f, dapeiSpr->getContentSize().height* .2f));
    dapeiSpr->addChild(guangSpr);
    CCRotateBy* action1 = CCRotateBy::create(.5f, -5);
    CCRotateBy* action2 = CCRotateBy::create(.5f, 5);
    CCRotateBy* action3 = CCRotateBy::create(.5f, 5);
    CCRotateBy* action4 = CCRotateBy::create(.5f, -5);
    CCSequence* seq1 = CCSequence::create(action1, action2, action3, action4, NULL);
    
    CCFadeOut* fade1 = CCFadeOut::create(.5f);
    CCFadeIn* fade2 = CCFadeIn::create(.5f);
    CCFadeOut* fade3 = CCFadeOut::create(.5f);
    CCFadeIn* fade4 = CCFadeIn::create(.5f);
    CCSequence* seq2 = CCSequence::create(fade1, fade2, fade3, fade4, NULL);
    
    CCSpawn* seq3 = CCSpawn::create(seq1, seq2, NULL);
    guangSpr->runAction(CCRepeatForever::create(seq3));
}
void pkScene::creatHuangguan(CCSprite* huangguanSpr){
    CCSprite* huangguanSpr1 = CCSprite::create("res/pic/pk/pk_huangguan2.png");
    huangguanSpr1->setPosition(ccp(huangguanSpr->getContentSize().width* .5f, huangguanSpr->getContentSize().height* .33f));
    huangguanSpr->addChild(huangguanSpr1);
    CCRotateBy* action = CCRotateBy::create(10.f, 360);
    huangguanSpr1->runAction(CCRepeatForever::create(CCRepeatForever::create(action)));
    
    CCSprite* huangguanSpr2 = CCSprite::create("res/pic/pk/pk_huangguan3.png");
    huangguanSpr2->setPosition(ccp(huangguanSpr->getContentSize().width* .503f, huangguanSpr->getContentSize().height* .1f));
    huangguanSpr->addChild(huangguanSpr2, 5);
    
    int paimingIndex = selfItem->getLastRank();// 排名
    int paimingFloat = 0;// 字体大小
    CCString* paimingStr;
    if (paimingIndex > 0) {
        paimingStr = CCString::createWithFormat("%d", paimingIndex);
        paimingFloat = 60;
    }else{
        paimingStr = CCString::createWithFormat("%s", "未上榜");
        paimingFloat = 40;
    }
    CCLabelTTF* paimingLabel = CCLabelTTF::create(paimingStr->getCString(), DISPLAY->fangzhengFont(), paimingFloat, CCSizeMake(huangguanSpr->getContentSize().width* .9f, paimingFloat), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    paimingLabel->setPosition(ccp(huangguanSpr->getContentSize().width* .5f, huangguanSpr->getContentSize().height* .32f));
    paimingLabel->setColor(ccc3(241, 104, 149));
    huangguanSpr->addChild(paimingLabel, 3);
}
void pkScene::creatRenwuKuang(){
//    // 1
//    CCSprite* kuangSpr1_1 = CCSprite::create("res/pic/pk/pk_kuang5.png");
//    kuangSpr1_1->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .52f));
//    this->addChild(kuangSpr1_1, 10);
//    CCRotateBy* kuangAction1 = CCRotateBy::create(25.f, 360);
//    kuangSpr1_1->runAction(CCRepeatForever::create(CCRepeatForever::create(kuangAction1)));
//    CCSprite* kuangSpr1_2 = CCSprite::create("res/pic/pk/pk_kuang4.png");
//    kuangSpr1_2->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .52f));
//    this->addChild(kuangSpr1_2, 11);
//    
//    kuangSpr1_3 = CCSprite::create("res/pic/pk/pk_kuang1.png");
//    kuangSpr1_3->setPosition(ccp(kuangSpr1_2->getContentSize().width* .5f, -kuangSpr1_3->getContentSize().height* .53f));
//    kuangSpr1_2->addChild(kuangSpr1_3, 11);
//    this->creatName1(kuangSpr1_3);
//    
//    xingSpr1_1 = CCSprite::create("res/pic/pk/pk_xing1.png");
//    xingSpr1_1->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .52f));
//    xingIndex1_1 = 50+ rand()%205;
//    if (xingIndex1_1 > 50 && xingIndex1_1< 255) {
//        xingBool1_1 = false;
//    }else{
//        xingBool1_1 = true;
//    }
//    xingSpr1_1->setOpacity(xingIndex1_1);
//    this->addChild(xingSpr1_1, 12);
//    
//    xingSpr1_2 = CCSprite::create("res/pic/pk/pk_xing2.png");
//    xingSpr1_2->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .52f));
//    xingIndex1_2 = 50+ rand()%205;
//    if (xingIndex1_2 > 50 && xingIndex1_2< 255) {
//        xingBool1_2 = false;
//    }else{
//        xingBool1_2 = true;
//    }
//    xingSpr1_2->setOpacity(xingIndex1_2);
//    this->addChild(xingSpr1_2, 12);
//    
//    xingSpr1_3 = CCSprite::create("res/pic/pk/pk_xing3.png");
//    xingSpr1_3->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .52f));
//    xingIndex1_3 = 50+ rand()%205;
//    if (xingIndex1_3 > 50 && xingIndex1_3< 255) {
//        xingBool1_3 = false;
//    }else{
//        xingBool1_3 = true;
//    }
//    xingSpr1_3->setOpacity(xingIndex1_3);
//    this->addChild(xingSpr1_3, 12);
//    
//    // 2
//    CCSprite* kuangSpr2_1 = CCSprite::create("res/pic/pk/pk_kuang5.png");
//    kuangSpr2_1->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .52f));
//    this->addChild(kuangSpr2_1, 10);
//    CCRotateBy* kuangAction2 = CCRotateBy::create(25.f, -360);
//    kuangSpr2_1->runAction(CCRepeatForever::create(CCRepeatForever::create(kuangAction2)));
//    CCSprite* kuangSpr2_2 = CCSprite::create("res/pic/pk/pk_kuang4.png");
//    kuangSpr2_2->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .52f));
//    this->addChild(kuangSpr2_2, 11);
//    
//    kuangSpr2_3 = CCSprite::create("res/pic/pk/pk_kuang1.png");
//    kuangSpr2_3->setPosition(ccp(kuangSpr2_2->getContentSize().width* .5f, -kuangSpr2_3->getContentSize().height* .53f));
//    kuangSpr2_2->addChild(kuangSpr2_3, 11);
//    this->creatName2(kuangSpr2_3, NULL);
//    
//    
//    xingSpr2_1 = CCSprite::create("res/pic/pk/pk_xing1.png");
//    xingSpr2_1->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .52f));
//    xingIndex2_1 = 50+ rand()%205;
//    if (xingIndex2_1 > 50 && xingIndex2_1< 255) {
//        xingBool2_1 = false;
//    }else{
//        xingBool2_1 = true;
//    }
//    xingSpr2_1->setOpacity(xingIndex2_1);
//    this->addChild(xingSpr2_1, 12);
//    
//    xingSpr2_2 = CCSprite::create("res/pic/pk/pk_xing2.png");
//    xingSpr2_2->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .52f));
//    xingIndex2_2 = 50+ rand()%205;
//    if (xingIndex2_2 > 50 && xingIndex2_2< 255) {
//        xingBool2_2 = false;
//    }else{
//        xingBool2_2 = true;
//    }
//    xingSpr2_2->setOpacity(xingIndex2_2);
//    this->addChild(xingSpr2_2, 12);
//    
//    xingSpr2_3 = CCSprite::create("res/pic/pk/pk_xing3.png");
//    xingSpr2_3->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .52f));
//    xingIndex2_3 = 50+ rand()%205;
//    if (xingIndex2_3 > 50 && xingIndex2_3< 255) {
//        xingBool2_3 = false;
//    }else{
//        xingBool2_3 = true;
//    }
//    xingSpr2_3->setOpacity(xingIndex2_3);
//    this->addChild(xingSpr2_3, 12);
    
    // 1
    CCSprite* kuangSpr1_1 = CCSprite::create("res/pic/pk/pk_kuang5.png");
    kuangSpr1_1->setScale(1.15f);
    kuangSpr1_1->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(kuangSpr1_1, 10);
    CCRotateBy* kuangAction1 = CCRotateBy::create(25.f, 360);
    kuangSpr1_1->runAction(CCRepeatForever::create(CCRepeatForever::create(kuangAction1)));
    CCSprite* kuangSpr1_2 = CCSprite::create("res/pic/pk/pk_kuang4.png");
    kuangSpr1_2->setScale(1.15f);
    kuangSpr1_2->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(kuangSpr1_2, 11);
    
    kuangSpr1_3 = CCSprite::create("res/pic/pk/pk_kuang1.png");
    kuangSpr1_3->setPosition(ccp(kuangSpr1_2->getContentSize().width* .5f, -kuangSpr1_3->getContentSize().height* .53f));
    kuangSpr1_2->addChild(kuangSpr1_3, 11);
    this->creatName1(kuangSpr1_3);
    
    xingSpr1_1 = CCSprite::create("res/pic/pk/pk_xing1.png");
    xingSpr1_1->setScale(1.15f);
    xingSpr1_1->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .53f));
    xingIndex1_1 = 50+ rand()%205;
    if (xingIndex1_1 > 50 && xingIndex1_1< 255) {
        xingBool1_1 = false;
    }else{
        xingBool1_1 = true;
    }
    xingSpr1_1->setOpacity(xingIndex1_1);
    this->addChild(xingSpr1_1, 12);
    
    xingSpr1_2 = CCSprite::create("res/pic/pk/pk_xing2.png");
    xingSpr1_2->setScale(1.15f);
    xingSpr1_2->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .53f));
    xingIndex1_2 = 50+ rand()%205;
    if (xingIndex1_2 > 50 && xingIndex1_2< 255) {
        xingBool1_2 = false;
    }else{
        xingBool1_2 = true;
    }
    xingSpr1_2->setOpacity(xingIndex1_2);
    this->addChild(xingSpr1_2, 12);
    
    xingSpr1_3 = CCSprite::create("res/pic/pk/pk_xing3.png");
    xingSpr1_3->setScale(1.15f);
    xingSpr1_3->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .53f));
    xingIndex1_3 = 50+ rand()%205;
    if (xingIndex1_3 > 50 && xingIndex1_3< 255) {
        xingBool1_3 = false;
    }else{
        xingBool1_3 = true;
    }
    xingSpr1_3->setOpacity(xingIndex1_3);
    this->addChild(xingSpr1_3, 12);
    
    // 2
    CCSprite* kuangSpr2_1 = CCSprite::create("res/pic/pk/pk_kuang5.png");
    kuangSpr2_1->setScale(1.15f);
    kuangSpr2_1->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(kuangSpr2_1, 10);
    CCRotateBy* kuangAction2 = CCRotateBy::create(25.f, -360);
    kuangSpr2_1->runAction(CCRepeatForever::create(CCRepeatForever::create(kuangAction2)));
    CCSprite* kuangSpr2_2 = CCSprite::create("res/pic/pk/pk_kuang4.png");
    kuangSpr2_2->setScale(1.15f);
    kuangSpr2_2->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(kuangSpr2_2, 11);
    
    kuangSpr2_3 = CCSprite::create("res/pic/pk/pk_kuang1.png");
    kuangSpr2_3->setPosition(ccp(kuangSpr2_2->getContentSize().width* .5f, -kuangSpr2_3->getContentSize().height* .53f));
    kuangSpr2_2->addChild(kuangSpr2_3, 11);
    this->creatName2(kuangSpr2_3, NULL);
    
    
    xingSpr2_1 = CCSprite::create("res/pic/pk/pk_xing1.png");
    xingSpr2_1->setScale(1.15f);
    xingSpr2_1->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .53f));
    xingIndex2_1 = 50+ rand()%205;
    if (xingIndex2_1 > 50 && xingIndex2_1< 255) {
        xingBool2_1 = false;
    }else{
        xingBool2_1 = true;
    }
    xingSpr2_1->setOpacity(xingIndex2_1);
    this->addChild(xingSpr2_1, 12);
    
    xingSpr2_2 = CCSprite::create("res/pic/pk/pk_xing2.png");
    xingSpr2_2->setScale(1.15f);
    xingSpr2_2->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .53f));
    xingIndex2_2 = 50+ rand()%205;
    if (xingIndex2_2 > 50 && xingIndex2_2< 255) {
        xingBool2_2 = false;
    }else{
        xingBool2_2 = true;
    }
    xingSpr2_2->setOpacity(xingIndex2_2);
    this->addChild(xingSpr2_2, 12);
    
    xingSpr2_3 = CCSprite::create("res/pic/pk/pk_xing3.png");
    xingSpr2_3->setScale(1.15f);
    xingSpr2_3->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .53f));
    xingIndex2_3 = 50+ rand()%205;
    if (xingIndex2_3 > 50 && xingIndex2_3< 255) {
        xingBool2_3 = false;
    }else{
        xingBool2_3 = true;
    }
    xingSpr2_3->setOpacity(xingIndex2_3);
    this->addChild(xingSpr2_3, 12);
    
    
    this->schedule(SEL_SCHEDULE(&pkScene::updateOpacity), .1f);
}
void pkScene::creatName1(CCSprite* kuangSpr){
    CCString* nicknameStr = CCString::createWithFormat("%s", selfItem->getNickname().c_str());
    CCString* nameStr1 = CCString::createWithFormat("%s", nicknameStr->getCString());
    CCLabelTTF* nameLabel1 = CCLabelTTF::create(nameStr1->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(kuangSpr->getContentSize().width* .9f, 20), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    nameLabel1->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .73f));
    nameLabel1->setColor(ccc3(255, 255, 255));
    kuangSpr->addChild(nameLabel1);
    
    
    int collected = DATA->getShow()->collected();
    CCString* nameStr2 = CCString::createWithFormat("服装收集:  %d", collected);
    CCLabelTTF* nameLabel2 = CCLabelTTF::create(nameStr2->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(kuangSpr->getContentSize().width* .9f, 20), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .55f, kuangSpr->getContentSize().height* .4f));
    nameLabel2->setColor(ccc3(166, 103, 87));
    kuangSpr->addChild(nameLabel2);
    
    int score = selfItem->getScore();
    CCString* nameStr3 = CCString::createWithFormat("累计得分:  %d", score);
    CCLabelTTF* nameLabel3 = CCLabelTTF::create(nameStr3->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(kuangSpr->getContentSize().width* .9f, 20), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel3->setPosition(ccp(kuangSpr->getContentSize().width* .55f, kuangSpr->getContentSize().height* .15f));
    nameLabel3->setColor(ccc3(166, 103, 87));
    kuangSpr->addChild(nameLabel3);
}
void pkScene::creatName2(CCSprite* kuangSpr, CompetitionItem* item){
    CCString* nameStr;
    if (item == NULL) {
        nameStr = CCString::createWithFormat("%s", "?????");
    }else{
        CCString* nicknameStr = CCString::createWithFormat("%s", item->getNickname().c_str());
        nameStr = CCString::createWithFormat("%s", nicknameStr->getCString());
    }
    CCLabelTTF* nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(kuangSpr->getContentSize().width* .9f, 20), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    nameLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .73f));
    nameLabel->setColor(ccc3(255, 255, 255));
    kuangSpr->addChild(nameLabel);
    
    
    CCString* nameStr2;
    if (item == NULL) {
        nameStr2 = CCString::createWithFormat("服装收集:  %s", "?????");
    }else{
        int collected = item->getCollected();
        nameStr2 = CCString::createWithFormat("服装收集:  %d", collected);
    }
    CCLabelTTF* nameLabel2 = CCLabelTTF::create(nameStr2->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(kuangSpr->getContentSize().width* .9f, 20), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .55f, kuangSpr->getContentSize().height* .4f));
    nameLabel2->setColor(ccc3(166, 103, 87));
    kuangSpr->addChild(nameLabel2);
    
    
    CCString* nameStr3;
    if (item == NULL) {
        nameStr3 = CCString::createWithFormat("累计得分:  %s", "?????");
    }else{
        int score = item->getScore();
        nameStr3 = CCString::createWithFormat("累计得分:  %d", score);
    }
    CCLabelTTF* nameLabel3 = CCLabelTTF::create(nameStr3->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(kuangSpr->getContentSize().width* .9f, 20), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel3->setPosition(ccp(kuangSpr->getContentSize().width* .55f, kuangSpr->getContentSize().height* .15f));
    nameLabel3->setColor(ccc3(166, 103, 87));
    kuangSpr->addChild(nameLabel3);
}
void pkScene::creatKuangButton(){
    CCSprite* kuangSpr = CCSprite::create("res/pic/pk/pk_moban2.png");
    kuangSpr->setScale(1.15f);
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .53f));
    kuangSpr->setTag(0x99999);
    this->addChild(kuangSpr, 30);
    
    CCSprite* buttonSpr1 = CCSprite::create("res/pic/pk/pk_button1.png");
    buttonSpr1->setScale(1.1f);
    CCSprite* buttonSpr2 = CCSprite::create("res/pic/pk/pk_button1.png");
    buttonSpr2->setScale(1.12f);
    CCMenuItem* item = CCMenuItemSprite::create(buttonSpr1, buttonSpr2, this, menu_selector(pkScene::buttonCallBack));
    item->setPosition(ccp(kuangSpr->getContentSize().width* .5f, 5));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu);
    
    
    int searchIndex = selfItem->getSearchTimes();
    CCString* startStr;
    if (searchIndex < themeInfo->getSearchFreeCount()) {
        startStr = CCString::createWithFormat("%d/%d", searchIndex, themeInfo->getSearchFreeCount());
        
        CCLabelTTF* searchLabel = CCLabelTTF::create(startStr->getCString(), DISPLAY->fangzhengFont(), 16);
        searchLabel->setPosition(ccp(item->getContentSize().width* .5f, item->getContentSize().height* .3f));
        searchLabel->setColor(ccc3(219, 50, 8));
        item->addChild(searchLabel);
    }else{
        startStr = CCString::createWithFormat("%d", selfItem->getSearchCost());
        
        CCLabelTTF* searchLabel = CCLabelTTF::create(startStr->getCString(), DISPLAY->fangzhengFont(), 16);
        searchLabel->setPosition(ccp(item->getContentSize().width* .42f, item->getContentSize().height* .3f));
        searchLabel->setColor(ccc3(219, 50, 8));
        item->addChild(searchLabel);
        
        CCSprite* goldSpr = CCSprite::create("res/pic/pk/pk_gold.png");
        goldSpr->setScale(.7f);
        goldSpr->setPosition(ccp(item->getContentSize().width* .58f, item->getContentSize().height* .31f));
        item->addChild(goldSpr);
    }
}
void pkScene::buttonCallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    LOADING->show_loading();
    NET->competition_search_opponent_821();
}
void pkScene::_821Callback(CCObject *pObj){
    LOADING->remove();
    
    if (this->getChildByTag(0x99999) != NULL) {
        CCNode* node = this->getChildByTag(0x99999);
        node->removeAllChildren();
        this->removeChildByTag(0x99999);
    }
    if (this->getChildByTag(0x88888) != NULL) {
        this->removeChildByTag(0x88888);
    }
    
    selfItem = DATA->getCompetition()->getSelf();
    opponentItem = DATA->getCompetition()->getOpponent();
    kuangSpr2_3->removeAllChildren();
    
    hzItem->setVisible(false);
    hhItem->setVisible(false);
    startItem->setVisible(true);
    zhufuItem->setVisible(true);
    
    this->creatName2(kuangSpr2_3, opponentItem);
    
    this->creat_Man2();
    this->creatKuangButton();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}

void pkScene::creatDuijue(){
    CCSprite* duijueSpr1 = CCSprite::create("res/pic/pk/pk_duijue1.png");
    duijueSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(duijueSpr1, 15);
    CCSprite* duijueGuangSpr1 = CCSprite::create("res/pic/pk/pk_guang2.png");
    duijueGuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(duijueGuangSpr1, 16);
    CCRotateBy* duijueGuangAction = CCRotateBy::create(6.f, 360);
    duijueGuangSpr1->runAction(CCRepeatForever::create(CCRepeatForever::create(duijueGuangAction)));
    
    CCSprite* quanSpr = CCSprite::create("res/pic/pk/pk_quan1.png");
    quanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(quanSpr, 17);
    CCAnimation* quanAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("quanStr");
    CCAnimate* quanAnimate = CCAnimate::create(quanAnimation);
    quanSpr->runAction(CCRepeatForever::create(quanAnimate));
    
    CCSprite* duijueSpr2 = CCSprite::create("res/pic/pk/pk_duijue2.png");
    duijueSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(duijueSpr2, 18);
    CCSprite* duijueGuangSpr2 = CCSprite::create("res/pic/pk/pk_guang3.png");
    duijueGuangSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .53f));
    this->addChild(duijueGuangSpr2, 19);
    CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 1.1f);
    CCFadeOut* fadeOut1 = CCFadeOut::create(.2f);
    CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, 1.f);
    CCFadeIn* fadeOut2 = CCFadeIn::create(.1f);
    CCSpawn* duijueGuangSpawn = CCSpawn::create(scaleTo2, fadeOut2, NULL);
    CCSequence* duijueGuangSeq = CCSequence::create(scaleTo1, CCDelayTime::create(.3f), fadeOut1, duijueGuangSpawn, NULL);
    duijueGuangSpr2->runAction(CCRepeatForever::create(duijueGuangSeq));
}
void pkScene::creatZhuti(){
    CCArray* tagArr = themeInfo->getTags();
    
    // 主题
    CCSprite* zhutiSpr = CCSprite::create("res/pic/pk/pk_zhuti.png");
    zhutiSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .26f));
    this->addChild(zhutiSpr, 10);
    
    if (tagArr->count() > 0) {
        int biaoqianTag1 = ((CCInteger* )tagArr->objectAtIndex(0))->getValue();
        CCString* biaoqianStr1 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", biaoqianTag1);
        CCSprite* biaoqianSpr1 = CCSprite::create(biaoqianStr1->getCString());
        biaoqianSpr1->setPosition(ccp(zhutiSpr->getContentSize().width* .58f, zhutiSpr->getContentSize().height* .4f));
        zhutiSpr->addChild(biaoqianSpr1);
        
        int biaoqianTag2 = ((CCInteger* )tagArr->objectAtIndex(1))->getValue();
        CCString* biaoqianStr2 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", biaoqianTag2);
        CCSprite* biaoqianSpr2 = CCSprite::create(biaoqianStr2->getCString());
        biaoqianSpr2->setPosition(ccp(zhutiSpr->getContentSize().width* .82f, zhutiSpr->getContentSize().height* .4f));
        zhutiSpr->addChild(biaoqianSpr2);
    }else{
        CCLabelTTF* biaoqianLabel = CCLabelTTF::create("没有主题", DISPLAY->fangzhengFont(), 30);
        biaoqianLabel->setPosition(ccp(zhutiSpr->getContentSize().width* .68f, zhutiSpr->getContentSize().height* .4f));
        biaoqianLabel->setColor(ccc3(255, 37, 37));
        zhutiSpr->addChild(biaoqianLabel);
    }

    // 天
    CCSprite* diSpr = CCSprite::create("res/pic/pk/pk_di1.png");
    diSpr->setPosition(ccp(zhutiSpr->getContentSize().width* .5f, -diSpr->getContentSize().height* .5f));
    zhutiSpr->addChild(diSpr);
    CCString* dayStr = CCString::createWithFormat("%d", themeInfo->getDay() + 1);
    CCLabelTTF* dayLabel = CCLabelTTF::create(dayStr->getCString(), DISPLAY->fangzhengFont(), 24);
    dayLabel->setPosition(ccp(diSpr->getContentSize().width* .83f, diSpr->getContentSize().height* .5f));
    dayLabel->setColor(ccc3(255, 37, 37));
    diSpr->addChild(dayLabel);
}
void pkScene::creatStart(){
    int startIndex = selfItem->getStartTimes();
    CCString* startStr;
    if (startIndex < themeInfo->getStartFreeCount()) {
        startStr = CCString::createWithFormat("%d/%d", startIndex, themeInfo->getStartFreeCount());
        
        CCLabelTTF* startLabel = CCLabelTTF::create(startStr->getCString(), DISPLAY->fangzhengFont(), 23);
        startLabel->setPosition(ccp(startItem->getContentSize().width* .5f, startItem->getContentSize().height* .24f));
        startLabel->setColor(ccc3(180, 37, 8));
        startItem->addChild(startLabel);
    }else{
        startStr = CCString::createWithFormat("%d", selfItem->getStartCost());
        
        CCLabelTTF* startLabel = CCLabelTTF::create(startStr->getCString(), DISPLAY->fangzhengFont(), 23);
        startLabel->setPosition(ccp(startItem->getContentSize().width* .43f, startItem->getContentSize().height* .24f));
        startLabel->setColor(ccc3(180, 37, 8));
        startItem->addChild(startLabel);
        
        CCSprite* goldSpr = CCSprite::create("res/pic/pk/pk_gold.png");
        goldSpr->setPosition(ccp(startItem->getContentSize().width* .57f, startItem->getContentSize().height* .25f));
        startItem->addChild(goldSpr);
    }
}
void pkScene::creatZhufu(CCMenuItem *item){
    CCSprite* zhufuKuangSpr = CCSprite::create("res/pic/pk/pk_kuang2.png");
    zhufuKuangSpr->setPosition(ccp(item->getContentSize().width* .5f, -zhufuKuangSpr->getContentSize().height* .5f));
    item->addChild(zhufuKuangSpr);
    
    CCString* zhufuStr = CCString::createWithFormat("无");
    zhufuLabel = CCLabelTTF::create(zhufuStr->getCString(), DISPLAY->fangzhengFont(), 24, CCSizeMake(zhufuKuangSpr->getContentSize().width, 24), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    zhufuLabel->setPosition(ccp(zhufuKuangSpr->getContentSize().width* .5f, zhufuKuangSpr->getContentSize().height* .5f));
    zhufuLabel->setColor(ccc3(151, 58, 26));
    zhufuKuangSpr->addChild(zhufuLabel);
}



void pkScene::huanzhuangCallback(CCObject* pSender){
    AUDIO->common_effect();
        
    CCLayer* layer = ClothesScene::create_with_type(5, 0, 0);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void pkScene::huanhaoCallback(CCObject* pSender){
    AUDIO->common_effect();
    
    LOADING->show_loading();
    NET->competition_prepare_827();
}
void pkScene::_827Callback(CCObject *pObj){
    LOADING->remove();
    
    opponentItem = DATA->getCompetition()->getOpponent();
    selfItem = DATA->getCompetition()->getSelf();
    kuangSpr2_3->removeAllChildren();
    if (this->getChildByTag(0x88888) != NULL) {
        this->removeChildByTag(0x88888);
    }
    if (this->getChildByTag(0x99999) != NULL) {
        this->removeChildByTag(0x99999);
    }
    
    
    hzItem->setVisible(false);
    hhItem->setVisible(false);
    startItem->setVisible(true);
    zhufuItem->setVisible(true);
    
    this->creatName2(kuangSpr2_3, opponentItem);
    
    this->creat_Man2();
    this->creatKuangButton();
}
void pkScene::startCallback(CCObject* pSender){
    LOADING->show_loading();
    AUDIO->common_effect();
    
    temSelfIndex1 = selfItem->getBuffId()/100;
    temSelfIndex2 = selfItem->getBuffId()%100;
    temOpponentIndex1 = opponentItem->getBuffedId()/100;
    temOpponentIndex2 = opponentItem->getBuffedId()%100;
    
    NET->competition_start_825();
}
void pkScene::_825Callback(CCObject *pObj){
    LOADING->remove();
    
    CCScene* pScene = CCScene::create();
    PkLayer* layer = PkLayer::create_with_Layer(temSelfIndex1, temSelfIndex2, temOpponentIndex1, temOpponentIndex2);
    pScene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void pkScene::zhufuCallback(CCObject* pSender){
    
    AUDIO->common_effect();
    LOADING->show_loading();
    NET->competition_bless_823();
}
void pkScene::_823Callback(CCObject *pObj){
    LOADING->remove();
    
    selfItem = DATA->getCompetition()->getSelf();
    if (selfItem->getBuffId() != 0) {
        CCString* zhufuStr = CCString::createWithFormat("%s", selfItem->getBuffDesc().c_str());
        zhufuLabel->setString(zhufuStr->getCString());
    }else{
        CCString* zhufuStr = CCString::createWithFormat("无");
        zhufuLabel->setString(zhufuStr->getCString());
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}

void pkScene::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    
    num_child = 0;
    
    if (DATA->getPkBool()) {
        RankListScene* layer = RankListScene::create();
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else{
        CCScene* scene = MainScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}


void pkScene::creatAnimation(){
    CCArray* arrowAnimations = CCArray::createWithCapacity(10);
    char arrowStr[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(arrowStr, "res/pic/pk/pk_quan%d.png", i);
        CCSpriteFrame* arrowFrame = CCSpriteFrame::create(arrowStr, CCRectMake(0, 0, 356, 356));
        arrowAnimations->addObject(arrowFrame);
    }
    CCAnimation* arrowAnimation = CCAnimation::createWithSpriteFrames(arrowAnimations, .4f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation, "quanStr");
}
void pkScene::updateOpacity(float dt){
    if (xingBool1_1) {
        if (xingIndex1_1 > 50 && xingIndex1_1 <= 255) {
            xingIndex1_1 -= 20;
        }else{
            xingBool1_1 = false;
            xingIndex1_1 += 20;
            if (xingIndex1_1 >= 255) {
                xingIndex1_1 = 255;
            }
        }
    }else{
        if (xingIndex1_1 > 50 && xingIndex1_1 < 255) {
            xingIndex1_1 += 20;
            if (xingIndex1_1 >= 255) {
                xingIndex1_1 = 255;
            }
        }else{
            xingBool1_1 = true;
            xingIndex1_1 -= 20;
        }
    }
    xingSpr1_1->setOpacity(xingIndex1_1);
    
    if (xingBool1_2) {
        if (xingIndex1_2 > 50 && xingIndex1_2 <= 255) {
            xingIndex1_2 -= 20;
        }else{
            xingBool1_2 = false;
            xingIndex1_2 += 20;
            if (xingIndex1_2 >= 255) {
                xingIndex1_2 = 255;
            }
        }
    }else{
        if (xingIndex1_2 > 50 && xingIndex1_2 < 255) {
            xingIndex1_2 += 20;
            if (xingIndex1_2 >= 255) {
                xingIndex1_2 = 255;
            }
        }else{
            xingBool1_2 = true;
            xingIndex1_2 -= 20;
        }
    }
    xingSpr1_2->setOpacity(xingIndex1_2);
    
    if (xingBool1_3) {
        if (xingIndex1_3 > 50 && xingIndex1_3 <= 255) {
            xingIndex1_3 -= 20;
        }else{
            xingBool1_3 = false;
            xingIndex1_3 += 20;
            if (xingIndex1_3 >= 255) {
                xingIndex1_3 = 255;
            }
        }
    }else{
        if (xingIndex1_3 > 50 && xingIndex1_3 < 255) {
            xingIndex1_3 += 20;
            if (xingIndex1_3 >= 255) {
                xingIndex1_3 = 255;
            }
        }else{
            xingBool1_3 = true;
            xingIndex1_3 -= 20;
        }
    }
    xingSpr1_3->setOpacity(xingIndex1_3);
    
    if (xingBool2_1) {
        if (xingIndex2_1 > 50 && xingIndex2_1 <= 255) {
            xingIndex2_1 -= 20;
        }else{
            xingBool2_1 = false;
            xingIndex2_1 += 20;
            if (xingIndex2_1 >= 255) {
                xingIndex2_1 = 255;
            }
        }
    }else{
        if (xingIndex2_1 > 50 && xingIndex2_1 < 255) {
            xingIndex2_1 += 20;
            if (xingIndex2_1 >= 255) {
                xingIndex2_1 = 255;
            }
        }else{
            xingBool2_1 = true;
            xingIndex2_1 -= 20;
        }
    }
    xingSpr2_1->setOpacity(xingIndex2_1);
    
    if (xingBool2_2) {
        if (xingIndex2_2 > 50 && xingIndex2_2 <= 255) {
            xingIndex2_2 -= 20;
        }else{
            xingBool2_2 = false;
            xingIndex2_2 += 20;
            if (xingIndex2_2 >= 255) {
                xingIndex2_2 = 255;
            }
        }
    }else{
        if (xingIndex2_2 > 50 && xingIndex2_2 < 255) {
            xingIndex2_2 += 20;
            if (xingIndex2_2 >= 255) {
                xingIndex2_2 = 255;
            }
        }else{
            xingBool2_2 = true;
            xingIndex2_2 -= 20;
        }
    }
    xingSpr2_2->setOpacity(xingIndex2_2);
    
    if (xingBool2_3) {
        if (xingIndex2_3 > 50 && xingIndex2_3 <= 255) {
            xingIndex2_3 -= 20;
        }else{
            xingBool2_3 = false;
            xingIndex2_3 += 20;
            if (xingIndex2_3 >= 255) {
                xingIndex2_3 = 255;
            }
        }
    }else{
        if (xingIndex2_3 > 50 && xingIndex2_3 < 255) {
            xingIndex2_3 += 20;
            if (xingIndex2_3 >= 255) {
                xingIndex2_3 = 255;
            }
        }else{
            xingBool2_3 = true;
            xingIndex2_3 -= 20;
        }
    }
    xingSpr2_3->setOpacity(xingIndex2_3);
}
void pkScene::creat_Man1()
{
    float widthFolt = .32f;
    float heightFloat = .34f;
    float scaleFloat = .82f;
    bool flipxBool = false;
    
    // 裁剪节点
    CCClippingNode *holesClipper = CCClippingNode::create();
    holesClipper->setInverted(false);
    holesClipper->setAlphaThreshold( 0.05f );
    
    
    // 创建遮罩模板图片，并设置到裁剪节点
    CCSprite *holeStencil = CCSprite::create("res/pic/pk/pk_moban.png");  //孔图
    holeStencil->setScale(1.15f);
    holeStencil->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .53f));
    holesClipper->setStencil( holeStencil);
    // 裁剪节点加载到背景图
    this->addChild(holesClipper, 20);
    
    
    // 创建裁剪content图片，并加载到裁剪节点上
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(holeStencil->getPosition().x + 87, DISPLAY->ScreenHeight()* heightFloat));
    holesClipper->addChild(manSpr, 200);
    
    CCSprite* touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    touSpr->setScale(scaleFloat);
    touSpr->setFlipX(flipxBool);
    touSpr->setPosition(ccp(holeStencil->getPosition().x + 87, DISPLAY->ScreenHeight()* heightFloat));
    holesClipper->addChild(touSpr, 210);
    
    
    CCDictionary* clothesDic = DATA->getCompetition()->getSelf()->getOndress();
    this->initClothes(holesClipper, holeStencil->getPosition().x + 82, DISPLAY->ScreenHeight()* heightFloat, scaleFloat, flipxBool, clothesDic);
}



void pkScene::creat_Man2(){
    float widthFolt = .92f;
    float heightFloat = .34f;
    float scaleFloat = .82f;
    bool flipxBool = false;
    
    // 裁剪节点
    CCClippingNode *holesClipper = CCClippingNode::create();
    holesClipper->setInverted(false);
    holesClipper->setAlphaThreshold( 0.05f );
    holesClipper->setTag(0x88888);
    
    
    // 创建遮罩模板图片，并设置到裁剪节点
    CCSprite *holeStencil = CCSprite::create("res/pic/pk/pk_moban.png");  //孔图
    holeStencil->setScale(1.15f);
    holeStencil->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .53f));
    holesClipper->setStencil( holeStencil);
    // 裁剪节点加载到背景图
    this->addChild(holesClipper, 20);
    
    
    // 创建裁剪content图片，并加载到裁剪节点上
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(holeStencil->getPosition().x + 87, DISPLAY->ScreenHeight()* heightFloat));
    holesClipper->addChild(manSpr, 200);
    
    CCSprite* touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    touSpr->setScale(scaleFloat);
    touSpr->setFlipX(flipxBool);
    touSpr->setPosition(ccp(holeStencil->getPosition().x + 87, DISPLAY->ScreenHeight()* heightFloat));
    holesClipper->addChild(touSpr, 210);
    
    
    CCDictionary* clothesDic = opponentItem->getOndress(); // 男宠衣着
    this->initClothes(holesClipper, holeStencil->getPosition().x + 87, DISPLAY->ScreenHeight()* heightFloat, scaleFloat, flipxBool, clothesDic);
}
void pkScene::initClothes(CCClippingNode * _ManSpr, float widthFolt, float heightFloat, float scaleFloat, bool flipxBool, CCDictionary* myClothesTemp){//穿衣服
    
    int sub_part = 0;
    
    for (int i = Tag_PK_TouFa; i <= Tag_PK_ZhuangRong; i++) {
        if (i == Tag_PK_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                CCSprite* _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(widthFolt, heightFloat));
                _tfSpr1->setTag(Tag_PK_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                CCSprite* _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(widthFolt, heightFloat));
                _tfSpr2->setTag(Tag_PK_TouFa2);
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
                            CCSprite* _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr1->setTag(Tag_PK_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr2->setTag(Tag_PK_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr3->setTag(Tag_PK_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PK_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                CCSprite* _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(widthFolt, heightFloat));
                _wtSpr1->setTag(Tag_PK_WaiTao1);
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
                            CCSprite* _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr1->setTag(Tag_PK_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr2->setTag(Tag_PK_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr3->setTag(Tag_PK_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PK_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                CCSprite* _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(widthFolt, heightFloat));
                _sySpr1->setTag(Tag_PK_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
                
//                CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
//                xingSpr1->setFlipX(true);
//                xingSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight()* .488f));
//                _ManSpr->addChild(xingSpr1, 420);
//
//                CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
//                xingSpr2->setFlipX(true);
//                xingSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .385f, DISPLAY->ScreenHeight()* .473f));
//                _ManSpr->addChild(xingSpr2, 420);
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
                            CCSprite* _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr1->setTag(Tag_PK_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr2->setTag(Tag_PK_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr3->setTag(Tag_PK_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PK_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    CCSprite* _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                    _kzSpr1->setTag(Tag_PK_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    CCSprite* _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                    _kzSpr1->setTag(Tag_PK_KuZi1);
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
                            CCSprite* _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr1->setTag(Tag_PK_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr2->setTag(Tag_PK_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr3->setTag(Tag_PK_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PK_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                CCSprite* _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(widthFolt, heightFloat));
                _wzSpr1->setTag(Tag_PK_WaZi1);
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
                            CCSprite* _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr1->setTag(Tag_PK_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr2->setTag(Tag_PK_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr3->setTag(Tag_PK_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PK_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                CCSprite* _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(widthFolt, heightFloat));
                _xzSpr1->setTag(Tag_PK_XieZi1);
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
                            CCSprite* _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr1->setTag(Tag_PK_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr2->setTag(Tag_PK_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr3->setTag(Tag_PK_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PK_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(widthFolt, heightFloat));
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
                                _spSpr1->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_PK_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                CCSprite* _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(widthFolt, heightFloat));
                _bSpr1->setTag(Tag_PK_Bao1);
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
                            CCSprite* _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr1->setTag(Tag_PK_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr2->setTag(Tag_PK_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr3->setTag(Tag_PK_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_PK_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                CCSprite* _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(widthFolt, heightFloat));
                _zrSpr1->setTag(Tag_PK_ZhuangRong1);
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
                            CCSprite* _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _zrSpr1->setTag(Tag_PK_ZhuangRong1);
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







