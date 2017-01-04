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


const float totalRank_z_oder = 20.f;

CCScene* RankListScene::scene() {
    RankListScene* layer = RankListScene::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

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
    
    this->initDefaultRL();
    this->createShower();
    this->createUI();
    
    ShowComp* selfShow = DATA->getShow();
    _shower->change_shower(selfShow->ondress());
    
    return true;
}

void RankListScene::createShower() {
    _shower = ShowerView::create();
    this->addChild(_shower);
}

void RankListScene::initDefaultRL() {
    _isShowCompetition = false;
    _rlv = RankListView::create();
    _rlv->setDatasource(DATA->getRanking()->ranking());
    this->addChild(_rlv);
    
    _rlv->reload();
}

void RankListScene::switchRankList() {
    
}

void RankListScene::onEnter() {
    BaseScene::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&RankListScene::_321CallBack), "HTTP_FINISHED_321", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&RankListScene::keyBackStatus), .8f);
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
}

CCSprite* RankListScene::fetchNumSprite(int num){
    CCSprite* spr;
    switch (num) {
        case 0:
            spr =CCSprite::create("pic/haoyoupaihang/num_0.png");
            break;
        case 1:
            spr =CCSprite::create("pic/haoyoupaihang/num_1.png");
            break;
        case 2:
            spr =CCSprite::create("pic/haoyoupaihang/num_2.png");
            break;
        case 3:
            spr =CCSprite::create("pic/haoyoupaihang/num_3.png");
            break;
        case 4:
            spr =CCSprite::create("pic/haoyoupaihang/num_4.png");
            break;
        case 5:
            spr =CCSprite::create("pic/haoyoupaihang/num_5.png");
            break;
        case 6:
            spr =CCSprite::create("pic/haoyoupaihang/num_6.png");
            break;
        case 7:
            spr =CCSprite::create("pic/haoyoupaihang/num_7.png");
            break;
        case 8:
            spr =CCSprite::create("pic/haoyoupaihang/num_8.png");
            break;
        case 9:
            spr =CCSprite::create("pic/haoyoupaihang/num_9.png");
            break;
        default:
            break;
    }
    
    return spr;
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

void RankListScene::_321CallBack(CCObject* pSender){
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
    CCScene* scene = HaoyouScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void RankListScene::changeShower(ShowComp* shower) {

}

void RankListScene::addFriend(CCMenuItem *btn){
    int index = ((CCInteger*)btn->getUserObject())->getValue();
    
    CCArray* rankers = DATA->getRanking()->ranking();
    ShowComp* other = (ShowComp*)rankers->objectAtIndex(index);
    
    btn->selected();
    
    NET->send_message_803(other->getShowID().c_str(), 1);
}

