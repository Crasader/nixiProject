//
//  JiesuanLayer.cpp
//  tiegao
//
//  Created by mac on 17-1-11.
//
//

#include "JiesuanLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "AudioManager.h"
#include "pkScene.h"
#include "JNIController.h"


JiesuanLayer::JiesuanLayer(){
    
}
JiesuanLayer::~JiesuanLayer(){
    
}

bool JiesuanLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    themeInfo = DATA->getCompetition()->getTheme();
    selfItem = DATA->getCompetition()->getSelf();
    opponentItem = DATA->getCompetition()->getOpponent();
    
    
    return true;
}

CCScene* JiesuanLayer::scene(){
    CCScene* scene = CCScene::create();
    
    JiesuanLayer* layer = JiesuanLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}
JiesuanLayer* JiesuanLayer::create_with_Layer(int selfTempScore, int opponentTempScore, int index){
    JiesuanLayer* rtn = JiesuanLayer::create();
    rtn->init_with_Layer(selfTempScore, opponentTempScore, index);
    
    return rtn;
}


void JiesuanLayer::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&JiesuanLayer::_321CallBack), "HTTP_FINISHED_321", NULL);
    
    
    this->scheduleOnce(SEL_SCHEDULE(&JiesuanLayer::keyBackStatus), .8f);
}
void JiesuanLayer::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeAllObservers(this);
    
    CCLayer::onExit();
}

void JiesuanLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void JiesuanLayer::keyBackClicked(){
    num_child++;
//    CCLog("===== StoryScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    
    this->setKeypadEnabled(false);
//    this->fhCallBack(this);
}



void JiesuanLayer::init_with_Layer(int selfTempScore, int opponentTempScore, int index){
    selfScore = selfTempScore;
    opponentScore = opponentTempScore;
    bgIndex = index;
    
    this->creatAnimation();
    this->creat_view();
    this->creat_nameKuang();
    
    this->creat_Man1();
    this->creat_Man2();
    
    this->creat_jiesuan();
}

void JiesuanLayer::creatAnimation(){
    CCArray* arrowAnimations1 = CCArray::createWithCapacity(10);
    char arrowStr1[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(arrowStr1, "res/pic/pk/animation/pk_jiesuanXing%d.png", i);
        CCSpriteFrame* arrowFrame1 = CCSpriteFrame::create(arrowStr1, CCRectMake(0, 0, 377, 335));
        arrowAnimations1->addObject(arrowFrame1);
    }
    CCAnimation* arrowAnimation1 = CCAnimation::createWithSpriteFrames(arrowAnimations1, .2f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation1, "jiesuanXingStr");
    
    
    CCArray* arrowAnimations2 = CCArray::createWithCapacity(10);
    char arrowStr2[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(arrowStr2, "res/pic/pk/animation/pk_leftSui%d.png", i);
        CCSpriteFrame* arrowFrame2 = CCSpriteFrame::create(arrowStr2, CCRectMake(0, 0, 74, 55));
        arrowAnimations2->addObject(arrowFrame2);
    }
    CCAnimation* arrowAnimation2 = CCAnimation::createWithSpriteFrames(arrowAnimations2, .4f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation2, "leftSuiStr");
    
    
    CCArray* arrowAnimations3 = CCArray::createWithCapacity(10);
    char arrowStr3[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(arrowStr3, "res/pic/pk/animation/pk_rightSui%d.png", i);
        CCSpriteFrame* arrowFrame3 = CCSpriteFrame::create(arrowStr3, CCRectMake(0, 0, 74, 55));
        arrowAnimations3->addObject(arrowFrame3);
    }
    CCAnimation* arrowAnimation3 = CCAnimation::createWithSpriteFrames(arrowAnimations3, .4f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation3, "rightSuiStr");
}
void JiesuanLayer::creat_view(){
    
    CCSprite* huangguanSpr = CCSprite::create("res/pic/pk/pk_huangguan4.png");
    huangguanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(huangguanSpr, 5);
    
    CCString* selfScoreStr = CCString::createWithFormat("%d", selfScore);
    scoreLabel1 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", selfScoreStr->getCString(), 0, .01f);
    scoreLabel1->setAnchorPoint(ccp(.5f, .5f));
    scoreLabel1->set_delegate(NULL); // 设置代理
    scoreLabel1->setPosition(ccp(huangguanSpr->getContentSize().width* .28f, huangguanSpr->getContentSize().height* .908f));
    huangguanSpr->addChild(scoreLabel1, 1);
    
    CCString* opponentScoreStr = CCString::createWithFormat("%d", opponentScore);
    scoreLabel2 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", opponentScoreStr->getCString(), 0, .01f);
    scoreLabel2->setAnchorPoint(ccp(.5f, .5f));
    scoreLabel2->set_delegate(NULL); // 设置代理
    scoreLabel2->setPosition(ccp(huangguanSpr->getContentSize().width* .72f, huangguanSpr->getContentSize().height* .908f));
    huangguanSpr->addChild(scoreLabel2, 1);
    
//    CCSprite* ditiaoSpr = CCSprite::create("res/pic/pk/pk_ditiao.png");
//    ditiaoSpr->setAnchorPoint(ccp(.5f, 0));
//    ditiaoSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, 5));
//    this->addChild(ditiaoSpr, 10);
}
void JiesuanLayer::creat_nameKuang(){
    CCSprite* nameKuangSpr1 = CCSprite::create("res/pic/pk/pk_kuang7.png");
    nameKuangSpr1->setAnchorPoint(ccp(.5f, 1.f));
    nameKuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight() - 15.f));
    this->addChild(nameKuangSpr1, 5);
    
    CCString* nameStr1 = CCString::createWithFormat("%s", selfItem->getNickname().c_str());
    CCLabelTTF* nameLabel1 = CCLabelTTF::create(nameStr1->getCString(), DISPLAY->fangzhengFont(), 27, CCSizeMake(nameKuangSpr1->getContentSize().width* .8f, 27), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel1->setPosition(ccp(nameKuangSpr1->getContentSize().width* .56f, nameKuangSpr1->getContentSize().height* .5f));
    nameLabel1->setColor(ccc3(191, 71, 99));
    nameKuangSpr1->addChild(nameLabel1);
    
    
    CCSprite* nameKuangSpr2 = CCSprite::create("res/pic/pk/pk_kuang7.png");
    nameKuangSpr2->setFlipX(true);
    nameKuangSpr2->setAnchorPoint(ccp(.5f, 1.f));
    nameKuangSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .78f, DISPLAY->ScreenHeight() - 15.f));
    this->addChild(nameKuangSpr2, 5);
    
    CCString* nameStr2 = CCString::createWithFormat("%s", opponentItem->getNickname().c_str());
    CCLabelTTF* nameLabel2 = CCLabelTTF::create(nameStr2->getCString(), DISPLAY->fangzhengFont(), 27, CCSizeMake(nameKuangSpr1->getContentSize().width* .8f, 27), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel2->setPosition(ccp(nameKuangSpr2->getContentSize().width* .45f, nameKuangSpr2->getContentSize().height* .5f));
    nameLabel2->setColor(ccc3(191, 71, 99));
    nameKuangSpr2->addChild(nameLabel2);
}

void JiesuanLayer::creat_jiesuan(){
    CCSprite* jiesuanSpr1 = CCSprite::create("res/pic/pk/jiesuan/pk_diban.png");
    jiesuanSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .52f, DISPLAY->ScreenHeight()* .65f));
    this->addChild(jiesuanSpr1, 15);
    CCSprite* jiesuanGuangSpr1 = CCSprite::create("res/pic/pk/pk_guang2.png");
    jiesuanGuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
    this->addChild(jiesuanGuangSpr1, 16);
    CCRotateBy* jiesuanGuangAction = CCRotateBy::create(6.f, 360);
    jiesuanGuangSpr1->runAction(CCRepeatForever::create(CCRepeatForever::create(jiesuanGuangAction)));
    
    
    
    CCSprite* lingquSpr1 = CCSprite::create("res/pic/pk/jiesuan/pk_lingqu.png");
    CCSprite* lingquSpr2 = CCSprite::create("res/pic/pk/jiesuan/pk_lingqu.png");
    lingquSpr2->setScale(1.02f);
    CCMenuItem* lingquItem = CCMenuItemSprite::create(lingquSpr1, lingquSpr2, this, menu_selector(JiesuanLayer::lingquCallback));
    lingquItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .25f));
    CCMenu* menu = CCMenu::create(lingquItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 20);
    
    
    
    if (selfScore > opponentScore) {
        AUDIO->pk_shengli_effect();
        
        CCSprite* jiesuanSpr2 = CCSprite::create("res/pic/pk/jiesuan/pk_shengli.png");
        jiesuanSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .47f, DISPLAY->ScreenHeight()* .65f));
        this->addChild(jiesuanSpr2, 18);
        
        CCSprite* jiesuanXingSpr = CCSprite::create("res/pic/pk/animation/pk_jiesuanXing1.png");
        jiesuanXingSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
        this->addChild(jiesuanXingSpr, 17);
        CCAnimation* jiesuanXingAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("jiesuanXingStr");
        CCAnimate* jiesuanXingAnimate = CCAnimate::create(jiesuanXingAnimation);
        jiesuanXingSpr->runAction(CCRepeatForever::create(jiesuanXingAnimate));
        
        
        CCSprite* shareSpr1;
        CCSprite* shareSpr2;
        if (DATA->getNews()->dailyShareCount == 0) {
            shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
            shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
            shareSpr2->setScale(1.02f);
        }else{
            shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
            shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
            shareSpr2->setScale(1.02f);
        }
        CCMenuItem* shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(JiesuanLayer::shareCallBack));
        shareItem->setAnchorPoint(ccp(0, .5f));
        shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
        CCMenu* shareMenu = CCMenu::create(shareItem, NULL);
        shareMenu->setPosition(CCPointZero);
        shareMenu->setTag(0x334455);
        this->addChild(shareMenu, 20);
        
        this->creat_lingqu(lingquItem, 1);
    }else if (selfScore < opponentScore){
        AUDIO->pk_shibai_effect();
        
        CCSprite* jiesuanSpr2 = CCSprite::create("res/pic/pk/jiesuan/pk_shibai.png");
        jiesuanSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .53f, DISPLAY->ScreenHeight()* .65f));
        this->addChild(jiesuanSpr2, 18);
        
        
        CCSprite* leftSuiSpr = CCSprite::create("res/pic/pk/animation/pk_leftSui1.png");
        leftSuiSpr->setPosition(ccp(10, jiesuanSpr2->getContentSize().height* .95f));
        jiesuanSpr2->addChild(leftSuiSpr, 17);
        CCAnimation* leftSuiAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("leftSuiStr");
        CCAnimate* leftSuiAnimate = CCAnimate::create(leftSuiAnimation);
        leftSuiSpr->runAction(CCRepeatForever::create(leftSuiAnimate));
        
        CCSprite* rightSuiSpr = CCSprite::create("res/pic/pk/animation/pk_rightSui1.png");
        rightSuiSpr->setPosition(ccp(jiesuanSpr2->getContentSize().width* .7f, -20));
        jiesuanSpr2->addChild(rightSuiSpr, 17);
        CCAnimation* rightSuiAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("rightSuiStr");
        CCAnimate* rightSuiAnimate = CCAnimate::create(rightSuiAnimation);
        rightSuiSpr->runAction(CCRepeatForever::create(rightSuiAnimate));
        
        this->creat_lingqu(lingquItem, 2);
    }else if (selfScore == opponentScore){
        AUDIO->pk_shengli_effect();
        
        CCSprite* jiesuanSpr2 = CCSprite::create("res/pic/pk/jiesuan/pk_pingju.png");
        jiesuanSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
        this->addChild(jiesuanSpr2, 18);
        
        CCSprite* jiesuanXingSpr = CCSprite::create("res/pic/pk/animation/pk_jiesuanXing1.png");
        jiesuanXingSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
        this->addChild(jiesuanXingSpr, 17);
        CCAnimation* jiesuanXingAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("jiesuanXingStr");
        CCAnimate* jiesuanXingAnimate = CCAnimate::create(jiesuanXingAnimation);
        jiesuanXingSpr->runAction(CCRepeatForever::create(jiesuanXingAnimate));
        
        this->creat_lingqu(lingquItem, 0);
    }
}
void JiesuanLayer::shareCallBack(CCObject* pSender){
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
    
    JNIController::showShare(4, 0);
    
    this->schedule(SEL_SCHEDULE(&JiesuanLayer::shareStatus), .1f);
#endif
}
void JiesuanLayer::shareStatus(float dt){
    if (JNIController::getShareStatus() == 1) {
        JNIController::shareText();
        JNIController::setShareStatus(0);
        
        LOADING->show_loading();
        NET->daily_share_321();
        
        this->unschedule(SEL_SCHEDULE(&JiesuanLayer::shareStatus));
    }else if (JNIController::getShareStatus() == 2 || JNIController::getShareStatus() == 3){
        JNIController::shareText();
        JNIController::setShareStatus(0);
        this->unschedule(SEL_SCHEDULE(&JiesuanLayer::shareStatus));
    }
}
void JiesuanLayer::_321CallBack(CCObject* pSender){
    LOADING->remove();
    
    if (this->getChildByTag(0x334455) != NULL) {
        this->removeChildByTag(0x334455);
    }
    CCSprite* shareSpr1;
    CCSprite* shareSpr2;
    if (DATA->getNews()->dailyShareCount == 0) {
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2->setScale(1.02f);
    }else{
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2->setScale(1.02f);
    }
    CCMenuItem* shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(JiesuanLayer::shareCallBack));
    shareItem->setAnchorPoint(ccp(0, .5f));
    shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
    CCMenu* shareMenu = CCMenu::create(shareItem, NULL);
    shareMenu->setPosition(CCPointZero);
    shareMenu->setTag(0x334455);
    this->addChild(shareMenu, 20);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}


void JiesuanLayer::lingquCallback(CCObject* pSender){
    AUDIO->common_effect();
    
    LOADING->show_loading();
    this->scheduleOnce(SEL_SCHEDULE(&JiesuanLayer::lingquCallback2), .5f);
}
void JiesuanLayer::lingquCallback2(float dt){
    LOADING->remove();
    
    
    CCScene* scene = pkScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void JiesuanLayer::creat_Man1(){
    
    // 裁剪节点
    CCClippingNode *holesClipper = CCClippingNode::create();
    holesClipper->setInverted(false);
    holesClipper->setAlphaThreshold( 0.05f );
    
    
    // 创建遮罩模板图片，并设置到裁剪节点
    CCSprite *holeStencil = CCSprite::create("res/pic/pk/pk_moban4.png");  //孔图
    holeStencil->setAnchorPoint(ccp(1.f, .5f));
    holeStencil->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->setStencil( holeStencil);
    // 裁剪节点加载到背景图
    this->addChild(holesClipper);
    
    
    CCSprite* bgSpr1 = CCSprite::create("res/pic/house/house_1.png");
    bgSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->addChild(bgSpr1, 2);
    
    CCSprite* bgSpr2 = CCSprite::create("res/pic/pk/pk_moban6.png");
    bgSpr2->setAnchorPoint(ccp(1.f, .5f));
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->addChild(bgSpr2, 3);
    
    
    float widthFolt = .4f;
    float heightFloat = .54f;
    float scaleFloat = 1.05f;
    bool flipxBool = false;
    if (selfScore > opponentScore) {
        // 放大
//        widthFolt = .45f;
//        heightFloat = .45f;
//        scaleFloat = 1.15f;
//        flipxBool = false;
        
        // 正常
        widthFolt = .4f;
        heightFloat = .54f;
        scaleFloat = 1.05f;
        flipxBool = false;
    }else if (selfScore < opponentScore){
        //
        widthFolt = .4f;
        heightFloat = .54f;
        scaleFloat = 1.05f;
        flipxBool = false;
        
        //
        CCSprite* heimengbanSpr = CCSprite::create("res/pic/pk/jiesuan/pk_heimengban.png");
        heimengbanSpr->setAnchorPoint(ccp(1.f, .5f));
        heimengbanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        holesClipper->addChild(heimengbanSpr, 1000);
    }else if (selfScore == opponentScore){
        //
        widthFolt = .4f;
        heightFloat = .54f;
        scaleFloat = 1.05f;
        flipxBool = false;
    }
    
    // 创建裁剪content图片，并加载到裁剪节点上
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat));
    holesClipper->addChild(manSpr, 200);
    
    CCSprite* touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    touSpr->setScale(scaleFloat);
    touSpr->setFlipX(flipxBool);
    touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat));
    holesClipper->addChild(touSpr, 210);
    
    CCDictionary* clothesDic = DATA->getCompetition()->getSelf()->getOndress();
    this->initClothes(holesClipper, DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat, scaleFloat, flipxBool, clothesDic);
}
void JiesuanLayer::creat_Man2(){
    
    // 裁剪节点
    CCClippingNode *holesClipper = CCClippingNode::create();
    holesClipper->setInverted(false);
    holesClipper->setAlphaThreshold( 0.05f );
    holesClipper->setTag(0x88888);
    
    
    // 创建遮罩模板图片，并设置到裁剪节点
    CCSprite *holeStencil = CCSprite::create("res/pic/pk/pk_moban5.png");  //孔图
    holeStencil->setAnchorPoint(ccp(0, .5f));
    holeStencil->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->setStencil( holeStencil);
    // 裁剪节点加载到背景图
    this->addChild(holesClipper);
    
    
    CCString* bgStr;
    if (bgIndex > 0 && bgIndex <= 30) {
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 1);
    }else if (bgIndex > 30 && bgIndex <= 60){
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 2);
    }else if (bgIndex > 60 && bgIndex <= 90){
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 3);
    }else{
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 4);
    }
    CCSprite* bgSpr1 = CCSprite::create(bgStr->getCString());
    bgSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->addChild(bgSpr1, 2);
    
    CCSprite* bgSpr2 = CCSprite::create("res/pic/pk/pk_moban6.png");
    bgSpr2->setAnchorPoint(ccp(0, .5f));
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->addChild(bgSpr2, 3);
    
    
    float widthFolt = .91f;
    float heightFloat = .54f;
    float scaleFloat = 1.05f;
    bool flipxBool = false;
    if (opponentScore > selfScore) {
        // 放大
//        widthFolt = .96f;
//        heightFloat = .45f;
//        scaleFloat = 1.15f;
//        flipxBool = false;
        
        // 正常
        widthFolt = .91f;
        heightFloat = .54f;
        scaleFloat = 1.05f;
        flipxBool = false;
    }else if (opponentScore < selfScore){
        //
        widthFolt = .91f;
        heightFloat = .54f;
        scaleFloat = 1.05f;
        flipxBool = false;
        //
        CCSprite* heimengbanSpr = CCSprite::create("res/pic/pk/jiesuan/pk_heimengban.png");
        heimengbanSpr->setAnchorPoint(ccp(0, .5f));
        heimengbanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        holesClipper->addChild(heimengbanSpr, 1000);
    }else if (opponentScore == selfScore){
        //
        widthFolt = .91f;
        heightFloat = .54f;
        scaleFloat = 1.05f;
        flipxBool = false;
    }
    // 创建裁剪content图片，并加载到裁剪节点上
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat));
    holesClipper->addChild(manSpr, 200);
    
    CCSprite* touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    touSpr->setScale(scaleFloat);
    touSpr->setFlipX(flipxBool);
    touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat));
    holesClipper->addChild(touSpr, 210);
    
    CCDictionary* clothesDic = opponentItem->getOndress(); // 男宠衣着
    this->initClothes(holesClipper, DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat, scaleFloat, flipxBool, clothesDic);
}

void JiesuanLayer::initClothes(CCClippingNode* _ManSpr, float widthFolt, float heightFloat, float scaleFloat, bool flipxBool, CCDictionary* myClothesTemp){//穿衣服
    
    int sub_part = 0;
    
    for (int i = Tag_JSLayer_TouFa; i <= Tag_JSLayer_ZhuangRong; i++) {
        if (i == Tag_JSLayer_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                CCSprite* _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(widthFolt, heightFloat));
                _tfSpr1->setTag(Tag_JSLayer_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                CCSprite* _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(widthFolt, heightFloat));
                _tfSpr2->setTag(Tag_JSLayer_TouFa2);
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
                            _tfSpr1->setTag(Tag_JSLayer_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr2->setTag(Tag_JSLayer_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr3->setTag(Tag_JSLayer_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_JSLayer_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                CCSprite* _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(widthFolt, heightFloat));
                _wtSpr1->setTag(Tag_JSLayer_WaiTao1);
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
                            _wtSpr1->setTag(Tag_JSLayer_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr2->setTag(Tag_JSLayer_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr3->setTag(Tag_JSLayer_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_JSLayer_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 300000);
                CCSprite* _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(widthFolt, heightFloat));
                _sySpr1->setTag(Tag_JSLayer_ShangYi1);
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
                            _sySpr1->setTag(Tag_JSLayer_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr2->setTag(Tag_JSLayer_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr3->setTag(Tag_JSLayer_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_JSLayer_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    CCSprite* _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                    _kzSpr1->setTag(Tag_JSLayer_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    //                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    CCSprite* _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                    _kzSpr1->setTag(Tag_JSLayer_KuZi1);
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
                            _kzSpr1->setTag(Tag_JSLayer_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr2->setTag(Tag_JSLayer_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr3->setTag(Tag_JSLayer_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_JSLayer_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                CCSprite* _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(widthFolt, heightFloat));
                _wzSpr1->setTag(Tag_JSLayer_WaZi1);
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
                            _wzSpr1->setTag(Tag_JSLayer_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr2->setTag(Tag_JSLayer_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr3->setTag(Tag_JSLayer_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_JSLayer_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                CCSprite* _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(widthFolt, heightFloat));
                _xzSpr1->setTag(Tag_JSLayer_XieZi1);
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
                            _xzSpr1->setTag(Tag_JSLayer_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr2->setTag(Tag_JSLayer_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr3->setTag(Tag_JSLayer_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_JSLayer_ShiPin){
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
        else if (i == Tag_JSLayer_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                CCSprite* _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(widthFolt, heightFloat));
                _bSpr1->setTag(Tag_JSLayer_Bao1);
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
                            _bSpr1->setTag(Tag_JSLayer_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr2->setTag(Tag_JSLayer_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr3->setTag(Tag_JSLayer_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_JSLayer_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                CCSprite* _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(widthFolt, heightFloat));
                _zrSpr1->setTag(Tag_JSLayer_ZhuangRong1);
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
                            _zrSpr1->setTag(Tag_JSLayer_ZhuangRong1);
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




void JiesuanLayer::creat_lingqu(CCMenuItem* item, int type){
    CCArray* ruleArr = themeInfo->getRule();
    
    if (type == 1) {
        CCDictionary* slDic = (CCDictionary* )ruleArr->objectAtIndex(0);
        CCInteger* slScoreInteger = (CCInteger* )slDic->objectForKey("score");
        CCInteger* slNumInteger = (CCInteger* )slDic->objectForKey("num");
        CCString* slStr2 = (CCString* )slDic->objectForKey("type");
        CCString* slStr = CCString::createWithFormat("%d分%d", slScoreInteger->getValue(), slNumInteger->getValue());
        CCLabelTTF* slScore = CCLabelTTF::create(slStr->getCString(), DISPLAY->fangzhengFont(), 22);
        slScore->setPosition(ccp(item->getContentSize().width* .45f, item->getContentSize().height* .35f));
        slScore->setColor(ccc3(201, 128, 110));
        item->addChild(slScore);
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
    }else if (type == 2){
        CCDictionary* pjDic = (CCDictionary* )ruleArr->objectAtIndex(2);
        CCInteger* pjScoreInteger = (CCInteger* )pjDic->objectForKey("score");
        CCString* pjStr = CCString::createWithFormat("%d分", pjScoreInteger->getValue());
        CCLabelTTF* pjScore = CCLabelTTF::create(pjStr->getCString(), DISPLAY->fangzhengFont(), 22);
        pjScore->setPosition(ccp(item->getContentSize().width* .5f, item->getContentSize().height* .35f));
        pjScore->setColor(ccc3(201, 128, 110));
        item->addChild(pjScore);
    }else if (type == 0){
        CCDictionary* sbDic = (CCDictionary* )ruleArr->objectAtIndex(1);
        CCInteger* sbScoreInteger = (CCInteger* )sbDic->objectForKey("score");
        CCString* sbStr = CCString::createWithFormat("%d分", sbScoreInteger->getValue());
        CCLabelTTF* sbScore = CCLabelTTF::create(sbStr->getCString(), DISPLAY->fangzhengFont(), 22);
        sbScore->setPosition(ccp(item->getContentSize().width* .5f, item->getContentSize().height* .35f));
        sbScore->setColor(ccc3(201, 128, 110));
        item->addChild(sbScore);
    }
}







