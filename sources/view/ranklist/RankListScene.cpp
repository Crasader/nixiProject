//
//  RankListScene.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#include "RankListScene.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "AudioManager.h"
#include "NetManager.h"

#include "ShowerView.h"
#include "NotePanel.h"
#include "RankListView.h"

#include "HaoyouScene.h"
#include "MainScene.h"

#include "AppUtil.h"
#include "JNIController.h"
#include "Loading2.h"
#include "PromptLayer.h"

#include "pkScene.h"


const float totalRank_z_oder = 20.f;


RankListScene::~RankListScene() {

}

bool RankListScene::init() {
    if(! BaseScene::init()) {
        return false;
    }
    
    num_child = 0;
    
    CCSprite* background = CCSprite::create("pic/haoyoupaihang/rank_bg_1.png");
    background->setPosition(ccp(DISPLAY->ScreenWidth()*.5, DISPLAY->ScreenHeight()*.5));
    this->addChild(background);
    
    this->createShower();
    this->initDefaultRL();
    this->createUI();
    
    return true;
}

void RankListScene::createShower() {
    _shower = ShowerView::create();
    this->addChild(_shower);
    
//    ShowComp* selfShow = DATA->getShow();
//    _shower->change_shower(selfShow->ondress());
}

void RankListScene::initDefaultRL() {
    _rlv = RankListView::create();
    this->addChild(_rlv);
}

void RankListScene::onEnter() {
    BaseScene::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&RankListScene::afterHttp300), "HTTP_FINISHED_300", NULL);
    nc->addObserver(this, SEL_CallFuncO(&RankListScene::afterHttp321), "HTTP_FINISHED_321", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&RankListScene::changeCompetition), "NEED_CHANGE_COMPETITION", NULL);
    nc->addObserver(this, SEL_CallFuncO(&RankListScene::changeShower), "NEED_CHANGE_SHOWER", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&RankListScene::keyBackStatus), .8f);
    
    // 静默发送
    NET->ranking_list_300();
    this->_rlv->onTitleToggle(NULL);
}

void RankListScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    BaseScene::onExit();
}

void RankListScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void RankListScene::keyBackClicked(){
    num_child++;
    CCLog("===== RankListScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->btn_back_callback(NULL);
}

void RankListScene::createUI(){
    //分享
    CCSprite* shareSpr1 = CCSprite::create("pic/ranklist/rl_share.png");
    CCSprite* shareSpr2 = CCSprite::create("pic/ranklist/rl_share.png");
    shareSpr2->setScale(1.02f);
    
    CCMenuItemSprite* item_share = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(RankListScene::btn_share_callback));
    item_share->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .88f));
    CCMenu* menu_share = CCMenu::create(item_share, NULL);
    menu_share->setPosition(CCPointZero);
    menu_share->setTag(0x334455);
    this->addChild(menu_share, totalRank_z_oder);
    
    //纸条
    CCSprite* note_spr = CCSprite::create("pic/haoyoupaihang/btn_zhitiao.png");
    CCSprite* note_spr2 = CCSprite::create("pic/haoyoupaihang/btn_zhitiao.png");
    note_spr2->setScale(1.02f);
    CCMenuItemSprite* item_note = CCMenuItemSprite::create(note_spr, note_spr2, this, menu_selector(RankListScene::btn_note_callback));
    item_note->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .2f));
    CCMenu* menu_note = CCMenu::create(item_note, NULL);
    menu_note->setPosition(CCPointZero);
    this->addChild(menu_note, totalRank_z_oder);
    
    //返回
    CCSprite* back_spr = CCSprite::create("pic/common/btn_goback2.png");
    CCSprite* back_spr2 = CCSprite::create("pic/common/btn_goback2.png");
    back_spr2->setScale(1.02f);
    CCMenuItemSprite* item_back = CCMenuItemSprite::create(back_spr, back_spr2, this, menu_selector(RankListScene::btn_back_callback));
    item_back->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menu_back = CCMenu::create(item_back, NULL);
    menu_back->setPosition(CCPointZero);
    this->addChild(menu_back, totalRank_z_oder);
    
    // 比拼入口
    CCSprite* tiny = CCSprite::create("pic/ranklist/rl_tiny_0.png");
    tiny->setPosition(ccp(DISPLAY->W() - 116, DISPLAY->H() * 0.11));
    this->addChild(tiny);
    CCAnimate* anim = CCAnimate::create(AppUtil::animationWithPics("pic/ranklist/rl_tiny_%d.png", 1, 0, 0.5f));
    tiny->runAction(CCRepeatForever::create(anim));
    
    CCSprite* sptPlate1 = CCSprite::create("pic/ranklist/rl_tiny_plane.png");
    CCSprite* sptPlate2 = CCSprite::create("pic/ranklist/rl_tiny_plane.png");
    sptPlate2->setScale(1.01f);
    CCMenuItemSprite* btnPlate = CCMenuItemSprite::create(sptPlate1, sptPlate2, this, menu_selector(RankListScene::gotoPkScene));
    btnPlate->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menuPlate = CCMenu::createWithItem(btnPlate);
    menuPlate->setPosition(ccp(-4, - sptPlate1->getContentSize().height * 0.125));
    tiny->addChild(menuPlate);
    
    // 自个分数
    CCString* strScore = CCString::createWithFormat("本期分数: %d", DATA->getCompetition()->getSelf()->getScore());
    CCLabelTTF* lblScore = CCLabelTTF::create(strScore->getCString(), DISPLAY->fangzhengFont(), 22);
    lblScore->setPosition(ccp(sptPlate1->getContentSize().width * 0.5f, sptPlate1->getContentSize().height * 0.23f));
    btnPlate->addChild(lblScore);
}

void RankListScene::gotoPkScene(CCMenuItem *btn) {
    AUDIO->comfirm_effect();
    CCLOG("RankListScene::gotoPkScene() ...");
    
    CCScene* scene = pkScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void RankListScene::btn_share_callback(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "排行界面", "点击分享");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/share.png");
    
    CCLog("图片 === %s", path.c_str());
    
    rt->saveToFile(path.c_str());

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("share.png");
    
    CCLog("图片 === %s", path.c_str());
    
    JNIController::setShareImage(path.c_str());
    rt->saveToFile(path.c_str());
    
    JNIController::showShare();
    this->schedule(SEL_SCHEDULE(&RankListScene::shareStatus), .1f);
#endif
}

void RankListScene::shareStatus(float dt){
    
    if (JNIController::getShareStatus() == 1) {
        JNIController::shareText();
        JNIController::setShareStatus(0);
        
        LOADING->show_loading();
        NET->daily_share_321();
        
        this->unschedule(SEL_SCHEDULE(&RankListScene::shareStatus));
    }else if (JNIController::getShareStatus() == 2 || JNIController::getShareStatus() == 3){
        JNIController::shareText();
        JNIController::setShareStatus(0);
        this->unschedule(SEL_SCHEDULE(&RankListScene::shareStatus));
    }
}

void RankListScene::afterHttp300(CCObject* pObj) {
    // 啥都不做
}

void RankListScene::afterHttp321(CCObject* pObj) {
    LOADING->remove();
    
    if (this->getChildByTag(0x334455) != NULL) {
        this->removeChildByTag(0x334455);
    }
    CCSprite* shareSpr1;
    CCSprite* shareSpr2;
    if (DATA->getNews()->dailyShareCount == 0) {
        shareSpr1 = CCSprite::create("pic/haoyoupaihang/share1.png");
        shareSpr2 = CCSprite::create("pic/haoyoupaihang/share1.png");
        shareSpr2->setScale(1.02f);
    }else{
        shareSpr1 = CCSprite::create("pic/haoyoupaihang/share2.png");
        shareSpr2 = CCSprite::create("pic/haoyoupaihang/share2.png");
        shareSpr2->setScale(1.02f);
    }
    CCMenuItem* shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(RankListScene::btn_share_callback));
    shareItem->setAnchorPoint(ccp(0, .5f));
    shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
    CCMenu* shareMenu = CCMenu::create(shareItem, NULL);
    shareMenu->setPosition(CCPointZero);
    shareMenu->setTag(0x334455);
    this->addChild(shareMenu, 20);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}

void RankListScene::btn_note_callback(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "排行界面", "点击纸条");
    
    CCArray* arr = DATA->getRanking()->ranking();
    if (arr->count() == 0) {
        
    }else{
        ShowComp* show = (ShowComp*)arr->objectAtIndex(DATA->getSocial()->getSelectedRanker());
        if (show->getShowID().compare(DATA->getLogin()->obtain_sid()) == 0) {
            PromptLayer* layer = PromptLayer::create();
            layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "不可以给自己发纸条哦");
        }else{
            _note = NotePanel::create();
            _note->setEntranceType("ranker");
            this->addChild(_note, 10000);
        }
    }
    
}

void RankListScene::btn_back_callback(CCObject* pSender){
    AUDIO->goback_effect();
    // talkingData
    DATA->onEvent("点击事件", "排行界面", "点击返回");
    
    num_child = 0;
    CCScene* scene = NULL;
    if (this->getComeFrom().compare("main") == 0) {
        scene = MainScene::scene();
    }
    else if (this->getComeFrom().compare("social") == 0) {
        scene = HaoyouScene::scene();
    }
    
    if (scene) {
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void RankListScene::changeCompetition(CompetitionItem *item) {
    if (item) {
        _shower->change_shower(item->getOndress());
    }
    else {
        CCLOG("ERROR:: RankListScene::changeCompetition(CompetitionItem* item) - item is nil~");
    }
}

void RankListScene::changeShower(ShowComp* shower) {
    if (shower) {
        _shower->change_shower(shower->ondress());
    }
    else {
        CCLOG("ERROR:: RankListScene::changeShower(ShowComp* shower) - shower is nil~");
    }
}

void RankListScene::testPK() {
    CCLOG("测试比拼结果 ...");
    NET->competition_start_825();
}

