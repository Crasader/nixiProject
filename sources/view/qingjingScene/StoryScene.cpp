//
//  StoryScene.cpp
//  mm3c
//
//  Created by mac on 14-11-27.
//
//

#include "StoryScene.h"
#include "Dialogs.h"
#include "DialogItem.h"
#include "MainScene.h"
#include "DataManager.h"
#include "QingjingScene.h"
#include "DisplayManager.h"
#include "LabelColorLayer.h"
#include "Loading2.h"
#include "NetManager.h"
#include "StorySettlementOfTheAnimationLayer.h"
#include "PromptLayer.h"
#include "StringUtil.h"
#include "GuideLayer.h"
#include "AudioManager.h"


StoryScene::StoryScene(){
    
}
StoryScene::~StoryScene(){
    
}

bool StoryScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    num_child = 0;
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    m_current_story_index_id = 0;
    storyIndex = 0;
    animation77Index = 0;
    openStory = false;
    m_bIsKJSelect = false;
    m_bIsZDSelect = false;
    _dikuangBool = false;
    passersbyBool = false;
    manBool = false;
    passersbyBool1 = false;
    passersbyBool2 = false;
    dikuangBool = false;
    quanBool = false;
    introBool = false;
    logicBool1 = false;
    logicBool2 = false;
    manString1 = "";
    manString2 = "";
    
    oneBool = false;
    twoBool = false;
    
    headSpr1 = NULL;
    headSpr2 = NULL;
    
    _dkSpr = CCSprite::create();
    this->addChild(_dkSpr, 10);
    
    DATA->setStoryLabelArr(CCArray::create());
    
    DATA->setFastForward(true);
    
    CCArray* animations = CCArray::createWithCapacity(5);
    char strPei[100] = {};
    for (int i = 2; i >= 1; i--) {
    
        sprintf(strPei, "res/pic/qingjingScene/storyscene/qj_act%d.png", i);
        CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 107, 107));
        animations->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "quanStr");
    
    
//    MMDataManager::get_instance()->show_paper();
    
    return true;
}

CCScene* StoryScene::scene(){
    CCScene* scene = CCScene::create();
    
    StoryScene* layer = StoryScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

StoryScene* StoryScene::create_with_story_id(int _index){
    StoryScene* rtn = StoryScene::create();
    rtn->init_with_story_id(_index);
    
    return rtn;
}

void StoryScene::init_with_story_id(int _index){
    
    index = 0;
    tagIndex = 0;
    nowIndex = 0;
    wordCount = 0;
    contentLength = 0;
    
    buttonBool = false;
    buttonBool1 = false;
    buttonBool2 = false;
    buttonBool3 = false;
    recordBool1 = false;
    recordBool2 = false;
    recordBool3 = false;
    openButtonBool = false;
    
    m_current_story_index_id = _index;
    
    CCSprite* shangkuangSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_shangkuang.png");
    shangkuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .97f));
    this->addChild(shangkuangSpr, 50);
    CCLabelTTF* shangLabel = CCLabelTTF::create(DISPLAY->GetOffTheName(m_current_story_index_id)->getCString(), DISPLAY->fangzhengFont(), 30);
    shangLabel->setPosition(ccp(shangkuangSpr->getContentSize().width* .5f, shangkuangSpr->getContentSize().height* .5f));
    shangLabel->setColor(ccWHITE);
    shangkuangSpr->addChild(shangLabel);
    
    dialog = Dialogs::create();
    dialog->retain();
//    CCString* fileStr = CCString::createWithFormat("res/story/80100/%s", DISPLAY->GetOffTheNumber(m_current_story_index_id)->getCString());
    CCString* fileStr = CCString::createWithFormat("res/story/80100/story_80100_101_%d", 30);
//    MZLog("fileStr === %s", fileStr->getCString());
    dialog->config_with_file((char* )fileStr->getCString());
    dialogItem = (DialogItem* )dialog->getDialogs()->objectAtIndex(index);
    
    this->init(dialogItem);
    
    kuangSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_di.png");
    kuangSpr->setAnchorPoint(ccp(.5f, 0));
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5, 80 - 500));
    kuangSpr->setTag(Tag_GJ_kuang);
    _dkSpr->addChild(kuangSpr, 10);
    
    DATA->setDiKuangSize(kuangSpr->boundingBox().size);
    
    quanSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_act1.png");
    quanSpr->setPosition(ccp(kuangSpr->getContentSize().width* .96f, -12));
    quanSpr->setTag(0x88999);
    kuangSpr->addChild(quanSpr, 100);
    CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("quanStr");
    CCAnimate* animate = CCAnimate::create(manAnimation);
    CCRepeatForever* rep = CCRepeatForever::create(animate);
    CCDirector::sharedDirector()->getActionManager()->addAction(rep, quanSpr, false);
    quanSpr->setVisible(false);
    
    this->addButton();
    
    float widSize = kuangSpr->getContentSize().width;
    float heiSize = kuangSpr->getContentSize().height;
    
    nameKuang = CCSprite::create("res/pic/qingjingScene/storyscene/qj_namekuang.png");
    nameKuang->setPosition(ccp(kuangSpr->getContentSize().width* .15f, kuangSpr->getContentSize().height - 10));
    kuangSpr->addChild(nameKuang);
    nameKuang->setVisible(false);
    
    saidLabel = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 25, CCSizeMake(widSize* .85f, heiSize* .6f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    saidLabel->setAnchorPoint(CCPointZero);
    saidLabel->setPosition(ccp(widSize* .11, 25));
    saidLabel->setTag(0x222);
    saidLabel->setColor(ccc3(80, 63, 68));
    kuangSpr->addChild(saidLabel, 8);
    
    nameLab = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 27);
    nameLab->setPosition(ccp(nameKuang->getContentSize().width* .5f, nameKuang->getContentSize().height* .5f));
    nameLab->setTag(0x111);
    nameLab->setColor(ccWHITE);
    nameLab->enableStroke(ccWHITE, .4f);
    nameKuang->addChild(nameLab, 8);
    
    this->dialogueControl(dialogItem);
    
    this->schedule(schedule_selector(StoryScene::gengxin), .1f);
}

void StoryScene::dialogueControl(DialogItem* dialItem){
//    CCLog("index ==== %d", dialItem->getIndex());
//    CCLog("states ==== %d", dialItem->getStates());
//    CCLog("passersby ==== %d", dialItem->getPassersby());
//    CCLog("name ==== %s", dialItem->getName().c_str());
//    CCLog("said ==== %s", dialItem->getSaid().c_str());
//    CCLog("bg ==== %s", dialItem->getBg().c_str());
//    CCLog("bust ==== %d", dialItem->getBust());
//
//    CCLog("head_1 ==== %s", dialItem->getHead_1().c_str());
//    CCLog("figure_1 ==== %d", dialItem->getFigure_1());
//    CCLog("zishi_1 ==== %s", dialItem->getZishi_1().c_str());
//    CCLog("biaoqing_1 ==== %d", dialItem->getBiaoqing_1());
//
//    CCLog("head_2 ==== %s", dialItem->getHead_2().c_str());
//    CCLog("figure_2 ==== %d", dialItem->getFigure_2());
//    CCLog("zishi_2 ==== %s", dialItem->getZishi_2().c_str());
//    CCLog("biaoqing_2 ==== %d", dialItem->getBiaoqing_2());
//
//    CCLog("animation ==== %d", dialItem->getAnimation());
////    CCLog("achievenment ==== %d", dialItem->getAchievenment());
//    CCLog("next ==== %d", dialItem->getNext());
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    openStory = true;
    this->setTouchEnabled(false);
    logIndex = 0;
    
    if (dialItem->getNext() == -2) {
        chengjiuStr = dialItem->getSaid().c_str();
    }
    
    if (dialItem->getStates() == 0) {// ==0 没有人
        
        this->recordLabel(dialItem);
        
        this->removeMan();
        
        openStory = true;
        manString1 = "";
        manString2 = "";
        manBool = false;
        passersbyBool1 = false;
        passersbyBool2 = false;
        
        oneBool = false;
        twoBool = false;
        
        if (dialItem->getBg() != "keep") {
//            if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//                this->removeChildByTag(Tag_GJ_kuang);
//            }
            if (this->getChildByTag(0x88888) != NULL) {
                this->removeChildByTag(0x88888);
            }
            
            this->unschedule(schedule_selector(StoryScene::logic));
            this->outKuang();
            this->emptyLabel();
            this->creatBg();
            
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::addKuang), 1.2f);
            
        }else{
            
            if (_dikuangBool) {
                this->schedule(schedule_selector(StoryScene::logic), .1f);
            }else{
                this->unschedule(schedule_selector(StoryScene::logic));

                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::addKuang), 1.2f);
            }
            
            
        }
        
    }else if (dialItem->getStates() == 1){// ==1 一个人
        
        this->recordLabel(dialItem);
        
        this->unschedule(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation));
        this->unschedule(schedule_selector(StoryScene::logic));
        openStory = true;
        passersbyBool1 = false;
        passersbyBool2 = false;
        
        oneBool = true;
        twoBool = false;
        
        if (dialItem->getBg() != "keep") {
            
//            if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//                this->removeChildByTag(Tag_GJ_kuang);
//            }
            if (this->getChildByTag(0x88888) != NULL) {
                this->removeChildByTag(0x88888);
            }
            
            this->removeMan();
            
            this->outKuang();
            this->emptyLabel();
            this->creatBg();
            
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatMan), 1.2f);
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::addKuang), 1.6f);
            
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::closeEyesAnimation), 1.2f);
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation), 3.1f);
        }else{
            
            this->removeMan();
            
            this->emptyLabel();
            
            this->creatMan(0);
            
            this->schedule(schedule_selector(StoryScene::logic), .1f);
        }
        
    }else if (dialItem->getStates() == 2){// ==2 两个人
        
        this->removeMan();
        this->recordLabel(dialItem);
        
        this->unschedule(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation));
        this->unschedule(schedule_selector(StoryScene::logic));
        
        openStory = true;
        manBool = false;
        
        oneBool = false;
        twoBool = true;
        
        if (dialItem->getBg() != "keep") {
            
//            if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//                this->removeChildByTag(Tag_GJ_kuang);
//            }
            if (this->getChildByTag(0x88888) != NULL) {
                this->removeChildByTag(0x88888);
            }
            
            this->outKuang();
            this->emptyLabel();
            this->creatBg();
            
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatPassersbyMan), 1.2f);
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::addKuang), 1.6f);
            
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::closeEyesAnimation), 1.2f);
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation), 3.1f);
            
        }else{
            
            this->emptyLabel();
            this->creatPassersbyMan(0);
            
            this->schedule(schedule_selector(StoryScene::logic), .1f);
        }
        
    }else if (dialItem->getStates() == 3){// ==3 保持之前状态
        
        this->recordLabel(dialItem);
        
        openStory = true;
        
        if (dialItem->getBg() != "keep") {
            this->emptyLabel();
            
            this->creatBg();
            
            if (_dikuangBool) {
                this->schedule(schedule_selector(StoryScene::logic), .1f);
                
                if (oneBool) {
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation), .1f);
                }else if (twoBool){
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation), .1f);
                }
            }else{
                this->unschedule(schedule_selector(StoryScene::logic));

                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::addKuang), 1.2f);
                
                if (oneBool) {
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation), 1.5f);
                }else if (twoBool){
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation), 1.5f);
                }
            }
        }else{
            this->schedule(schedule_selector(StoryScene::logic), .1f);
            
            if (oneBool) {
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation), .1f);
            }else if (twoBool){
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation), .1f);
            }
        }
    }else if (dialItem->getStates() == 4){// ==4 只有背景图片
//        if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//            this->removeChildByTag(Tag_GJ_kuang);
//        }
        if (this->getChildByTag(0x88888) != NULL) {
            this->removeChildByTag(0x88888);
        }
        
        manString1 = "";
        manString2 = "";
        manBool = false;
        passersbyBool1 = false;
        passersbyBool2 = false;
        
        oneBool = false;
        twoBool = false;
        
        this->removeMan();
        this->outKuang();
        saidLabel->setString("");
        nameLab->setString("");
        nameKuang->setVisible(false);
        
        this->creatBg();
    }
}

void StoryScene::recordLabel(DialogItem* dialItem){
    if (dialItem->getName().length() != 0) {
        labStr.append(dialItem->getName().c_str());
        labStr.append("\n\t");
    }
    if (dialItem->getSaid().length() != 0) {
        labStr.append(dialogItem->getSaid().c_str());
        labStr.append("\n");
    }
    
    nowIndex++;
    if (nowIndex == 4) {
        nowIndex = 0;
//        char* labChar = strdup(labStr.c_str());
        
        CCString* str = CCString::createWithFormat("%s", labStr.c_str());
        DATA->getStoryLabelArr()->addObject(str);
        labStr = "";
    }
}

void StoryScene::creatMan(float dt){
    
    if (this->getChildByTag(Tag_GJ_man1) != NULL) {
        this->removeChildByTag(Tag_GJ_man1);
    }
    if (this->getChildByTag(Tag_GJ_head1) != NULL) {
        this->removeChildByTag(Tag_GJ_head1);
    }
    
    std::string str1 = dialogItem->getZishi_1();
    std::string str2 = dialogItem->getHead_1();
    
    if (!str1.empty() && !str2.empty()) {
        //缩小状态坐标
        CCString* zsStr1 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", dialogItem->getZishi_1().c_str());
        manSpr1 = CCSprite::create(zsStr1->getCString());
        manSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        manSpr1->setTag(Tag_GJ_man1);
//        manSpr1->setScale(.75f);
        manSpr1->setVisible(false);
        this->addChild(manSpr1, 5);
        
        CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", dialogItem->getHead_1().c_str());
        headSpr1 = CCSprite::create(tStr1->getCString());
        headSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        headSpr1->setTag(Tag_GJ_head1);
//        headSpr1->setScale(.75f);
        headSpr1->setVisible(false);
        this->addChild(headSpr1, 6);
        
        if (judge_biaoqing(dialogItem, 1)) {
            this->init_biaoqing(dialogItem, 1, headSpr1);
        }
        
        
        if (manString2.empty()) {
            if (manString1.empty()) {
                manString1 = dialogItem->getZishi_1();
                
                CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation), inTime);
            }else{
                if (manString1 == dialogItem->getZishi_1()) {
                    manSpr1->setVisible(true);
                    headSpr1->setVisible(true);
                    
                    this->creatManEyesAnimation();
                }else{
                    manString1 = dialogItem->getZishi_1();
                    
                    CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                    manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                    headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation), inTime);
                }
            }
        }else{
            manString1 = "";
            manString2 = "";
            manString1 = dialogItem->getZishi_1();
            
            CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
            CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
            manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
            headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
            
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::creatManEyesAnimation), inTime);
        }
    }
}

void StoryScene::creatPassersbyMan(float dt){
    
    this->removeMan();
    
    if (dialogItem->getPassersby() == 1) {
        
        std::string str1 = dialogItem->getZishi_1();
        std::string str2 = dialogItem->getHead_1();
        std::string str3 = dialogItem->getZishi_2();
        std::string str4 = dialogItem->getHead_2();
        if (!str1.empty() && !str2.empty() && !str3.empty() && !str4.empty()) {
            //1号 一般大的
            CCString* zsStr1 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", dialogItem->getZishi_1().c_str());
            manSpr1 = CCSprite::create(zsStr1->getCString());
            manSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 - 150, DISPLAY->ScreenHeight()* .5f));
//            manSpr1->setScale(.7f);
            manSpr1->setTag(Tag_GJ_man1);
            manSpr1->setVisible(false);
            this->addChild(manSpr1, 6);
            
            CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", dialogItem->getHead_1().c_str());
            headSpr1 = CCSprite::create(tStr1->getCString());
            headSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 - 150, DISPLAY->ScreenHeight()* .5f));
//            headSpr1->setScale(.7f);
            headSpr1->setTag(Tag_GJ_head1);
            headSpr1->setVisible(false);
            if (judge_biaoqing(dialogItem, 1)) {
                this->addChild(headSpr1, 8);
                this->init_biaoqing(dialogItem, 1, headSpr1);
            }else{
                this->addChild(headSpr1, 7);
            }
            
            
            //2号 一般大的
            CCString* zsStr2 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", dialogItem->getZishi_2().c_str());
            manSpr2 = CCSprite::create(zsStr2->getCString());
            manSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 + 200, DISPLAY->ScreenHeight()* .5f));
//            manSpr2->setScale(.7f);
            manSpr2->setTag(Tag_GJ_man2);
            manSpr2->setVisible(false);
            this->addChild(manSpr2, 6);
            
            CCString* tStr2 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", dialogItem->getHead_2().c_str());
            headSpr2 = CCSprite::create(tStr2->getCString());
            headSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 + 200, DISPLAY->ScreenHeight()* .5f));
//            headSpr2->setScale(.7f);
            headSpr2->setTag(Tag_GJ_head2);
            headSpr2->setVisible(false);
            if (judge_biaoqing(dialogItem, 2)) {
                this->addChild(headSpr2, 8);
                this->init_biaoqing(dialogItem, 2, headSpr2);
            }else{
                this->addChild(headSpr2, 7);
            }
            
            if (manString2.empty()) {
                manString1 = dialogItem->getZishi_1();
                manString2 = dialogItem->getZishi_2();
                
                CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                
                CCFadeIn* fadeIn3 = CCFadeIn::create(inTime);
                CCFadeIn* fadeIn4 = CCFadeIn::create(inTime);
                manSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn3, NULL));
                headSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn4, NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation), inTime);
                
            }else{
                
                if (manString1 == dialogItem->getZishi_1()) {
                    manSpr1->setVisible(true);
                    headSpr1->setVisible(true);
                    
                    this->creatPassersbyEyesAnimation(1);
                }else{
                    manString1 = dialogItem->getZishi_1();
                    
                    CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                    manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                    headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation1), inTime);
                }
                
                if (manString2 == dialogItem->getZishi_2()) {
                    manSpr2->setVisible(true);
                    headSpr2->setVisible(true);
                    
                    this->creatPassersbyEyesAnimation(2);
                }else{
                    manString2 = dialogItem->getZishi_2();
                    
                    CCFadeIn* fadeIn3 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn4 = CCFadeIn::create(inTime);
                    manSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn3, NULL));
                    headSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn4, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation2), inTime);
                }
            }
        }
    }else if (dialogItem->getPassersby() == 0){
        
        if (dialogItem->getHead_1().size() > 1) {
            std::string str1 = dialogItem->getZishi_1();
            std::string str2 = dialogItem->getHead_1();
            if (!str1.empty() && !str2.empty()) {
                CCString* zsStr1 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", dialogItem->getZishi_1().c_str());
                manSpr1 = CCSprite::create(zsStr1->getCString());
                manSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 - 150, DISPLAY->ScreenHeight()* .5f));
//                manSpr1->setScale(.7f);
                manSpr1->setTag(Tag_GJ_man1);
                manSpr1->setVisible(false);
                this->addChild(manSpr1, 5);
                
                CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", dialogItem->getHead_1().c_str());
                headSpr1 = CCSprite::create(tStr1->getCString());
                headSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 - 150, DISPLAY->ScreenHeight()* .5f));
//                headSpr1->setScale(.7f);
                headSpr1->setTag(Tag_GJ_head1);
                headSpr1->setVisible(false);
                
                if (judge_biaoqing(dialogItem, 1)) {
                    this->addChild(headSpr1, 8);
                    this->init_biaoqing(dialogItem, 1, headSpr1);
                }else{
                    this->addChild(headSpr1, 7);
                }
                
                if (manString1 == dialogItem->getZishi_1()) {
                    manSpr1->setVisible(true);
                    headSpr1->setVisible(true);
                    
                    this->creatPassersbyEyesAnimation(1);
                }else{
                    manString1 = dialogItem->getZishi_1();
                    
                    CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                    manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                    headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation1), inTime);
                }
            }
        }else{
            manString1 = "";
        }
        
        if (dialogItem->getHead_2().size() > 1) {
            std::string str3 = dialogItem->getZishi_2();
            std::string str4 = dialogItem->getHead_2();
            if (!str3.empty() && !str4.empty()) {
                CCString* zsStr2 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", dialogItem->getZishi_2().c_str());
                manSpr2 = CCSprite::create(zsStr2->getCString());
                manSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 + 200, DISPLAY->ScreenHeight()* .5f));
//                manSpr2->setScale(.7f);
                manSpr2->setTag(Tag_GJ_man2);
                manSpr2->setVisible(false);
                this->addChild(manSpr2, 5);
                
                CCString* tStr2 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", dialogItem->getHead_2().c_str());
                headSpr2 = CCSprite::create(tStr2->getCString());
                headSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 + 200, DISPLAY->ScreenHeight()* .5f));
//                headSpr2->setScale(.7f);
                headSpr2->setTag(Tag_GJ_head2);
                headSpr2->setVisible(false);
                
                if (judge_biaoqing(dialogItem, 2)) {
                    this->addChild(headSpr2, 8);
                    this->init_biaoqing(dialogItem, 2, headSpr2);
                }else{
                    this->addChild(headSpr2, 7);
                }
                
                if (manString2 == dialogItem->getZishi_2()) {
                    manSpr2->setVisible(true);
                    headSpr2->setVisible(true);
                    
                    this->creatPassersbyEyesAnimation(2);
                }else{
                    manString2 = dialogItem->getZishi_2();
                    
                    CCFadeIn* fadeIn3 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn4 = CCFadeIn::create(inTime);
                    manSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn3, NULL));
                    headSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn4, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::toPassersbyEyesAnimation2), inTime);
                }
            }
        }else{
            manString2 = "";
        }
    }
}
void StoryScene::creatManEyesAnimation(){
    
    if (ccs(dialogItem->getNameId())->intValue() != 0) {
        CCArray* printArray = split(dialogItem->getZishi_1().c_str(), "_");
        CCString* temStr = (CCString* )printArray->objectAtIndex(0);
        if (strcmp(temStr->getCString(), dialogItem->getNameId().c_str()) == 0) {
//            CCLog("字符串含有");
            std::string::size_type idx1 = dialogItem->getHead_1().find("_1");
            if (idx1 != string::npos) {
                manBool = true;
                
                CCArray* animations = CCArray::createWithCapacity(4);
                char strPei[100] = {};
                for (int i = 1; i <= 2; i++) {
                    
                    sprintf(strPei, "res/pic/qingjingScene/eyes/%d_%d.png", ccs(dialogItem->getNameId())->intValue(), i);
                    CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 900, 1136));
                    animations->addObject(frame);
                }
                CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .2f);
                CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "manStr");
                
                this->creatManBlinkEyes();
            }else{
                manBool = false;
            }
        }
    }
}
void StoryScene::creatManBlinkEyes(){
    CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("manStr");
    CCAnimate* animate = CCAnimate::create(manAnimation);
    CCRepeatForever* rep = CCRepeatForever::create(animate);
    
    CCDirector::sharedDirector()->getActionManager()->addAction(rep, headSpr1, false);
}
void StoryScene::toPassersbyEyesAnimation(){
    this->creatPassersbyEyesAnimation(1);
    this->creatPassersbyEyesAnimation(2);
}
void StoryScene::toPassersbyEyesAnimation1(){
    this->creatPassersbyEyesAnimation(1);
}
void StoryScene::toPassersbyEyesAnimation2(){
    this->creatPassersbyEyesAnimation(2);
}
void StoryScene::creatPassersbyEyesAnimation(int nameIndex){
    
    if (ccs(dialogItem->getNameId())->intValue() != 0) {
        if (nameIndex == 1) {
            CCArray* printArray = split(dialogItem->getZishi_1().c_str(), "_");
            CCString* temStr = (CCString* )printArray->objectAtIndex(0);
            if (strcmp(temStr->getCString(), dialogItem->getNameId().c_str()) == 0) {
                std::string::size_type idx1 = dialogItem->getHead_1().find("_1");
                if (idx1 != string::npos) {
                    passersbyBool1 = true;
                }else{
                    passersbyBool1 = false;
                }
            }
        }else if (nameIndex == 2){
            CCArray* printArray = split(dialogItem->getZishi_2().c_str(), "_");
            CCString* temStr = (CCString* )printArray->objectAtIndex(0);
            if (strcmp(temStr->getCString(), dialogItem->getNameId().c_str()) == 0) {
                std::string::size_type idx1 = dialogItem->getHead_2().find("_1");
                if (idx1 != string::npos) {
                    passersbyBool2 = true;
                }else{
                    passersbyBool2 = false;
                }
            }
        }
        CCArray* animations = CCArray::createWithCapacity(4);
        char strPei[100] = {};
        for (int i = 1; i <= 2; i++) {
            
            sprintf(strPei, "res/pic/qingjingScene/eyes/%d_%d.png", ccs(dialogItem->getNameId())->intValue(), i);
            CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 900, 1136));
            animations->addObject(frame);
        }
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .2f);
        if (passersbyBool1) {
            passersbyBool2 = false;
            CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "passersbyStr1");
            this->creatPassersbyBlinkEyes();
        }else if (passersbyBool2){
            passersbyBool1 = false;
            CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "passersbyStr2");
            this->creatPassersbyBlinkEyes();
        }
    }
}
void StoryScene::creatPassersbyBlinkEyes(){
    if (passersbyBool1) {
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("passersbyStr1");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCRepeatForever* rep = CCRepeatForever::create(animate);
        
        if (headSpr1 != NULL) {
            CCArray* printArray = split(dialogItem->getZishi_1().c_str(), "_");
            CCString* temStr = (CCString* )printArray->objectAtIndex(0);
            if (strcmp(temStr->getCString(), dialogItem->getNameId().c_str()) == 0) {
                std::string::size_type idx1 = dialogItem->getHead_1().find("_1");
                if (idx1 != string::npos) {
                    CCDirector::sharedDirector()->getActionManager()->addAction(rep, headSpr1, false);
                }
            }
            
        }
        
    }else if (passersbyBool2){
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("passersbyStr2");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCRepeatForever* rep = CCRepeatForever::create(animate);
        
        if (headSpr2 != NULL) {
            CCArray* printArray = split(dialogItem->getZishi_2().c_str(), "_");
            CCString* temStr = (CCString* )printArray->objectAtIndex(0);
            if (strcmp(temStr->getCString(), dialogItem->getNameId().c_str()) == 0) {
                std::string::size_type idx1 = dialogItem->getHead_2().find("_1");
                if (idx1 != string::npos) {
                    CCDirector::sharedDirector()->getActionManager()->addAction(rep, headSpr2, false);
                }
            }
        }
        
    }
}
void StoryScene::closeEyesAnimation(){
    
    if (manBool) {
        manBool = false;
        
        if (headSpr1 != NULL) {
            CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget(headSpr1);
            
            CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", dialogItem->getHead_1().c_str());
            CCTexture2D* aTexture = CCTextureCache::sharedTextureCache()->addImage(tStr1->getCString());
            CCRect rect = CCRectZero;
            rect.size = aTexture->getContentSize();
            headSpr1->setTexture(aTexture);
            headSpr1->setTextureRect(rect);
            headSpr1->setVisible(true);
        }
        
    }else if (passersbyBool1){
        passersbyBool1 = false;
        
        if (headSpr1 != NULL) {
            CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget(headSpr1);
            
            CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", dialogItem->getHead_1().c_str());
            CCTexture2D* aTexture = CCTextureCache::sharedTextureCache()->addImage(tStr1->getCString());
            CCRect rect = CCRectZero;
            rect.size = aTexture->getContentSize();
            headSpr1->setTexture(aTexture);
            headSpr1->setTextureRect(rect);
            headSpr1->setVisible(true);
        }
        
    }else if (passersbyBool2){
        passersbyBool2 = false;
        
        if (headSpr2 != NULL) {
            CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget(headSpr2);
            
            CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", dialogItem->getHead_2().c_str());
            CCTexture2D* aTexture = CCTextureCache::sharedTextureCache()->addImage(tStr1->getCString());
            CCRect rect = CCRectZero;
            rect.size = aTexture->getContentSize();
            headSpr2->setTexture(aTexture);
            headSpr2->setTextureRect(rect);
            headSpr2->setVisible(true);
        }
        
    }
}

void StoryScene::addKuang(float dt){
//    if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//        this->removeChildByTag(Tag_GJ_kuang);
//    }
    _dikuangBool = true;
    _dkSpr->setVisible(true);
    CCFiniteTimeAction* _actMove1 = CCMoveBy::create(.3f, ccp(0, 535));
    CCFiniteTimeAction* _actMove2 = CCMoveBy::create(.1f, ccp(0, -35));
    CCCallFuncN* complete = CCCallFuncN::create(this, callfuncN_selector(StoryScene::addComplete));
    _dkSpr->runAction(CCSequence::create(_actMove1, _actMove2, CCDelayTime::create(.3f), complete, NULL));
    
    this->schedule(schedule_selector(StoryScene::logic), 1.5f);
}
void StoryScene::addComplete(){
    dikuangBool = true;
}
void StoryScene::outKuang(){
    if (_dikuangBool) {
        _dikuangBool = false;
        _dkSpr->setVisible(false);
        CCFiniteTimeAction* _actMove = CCMoveBy::create(.3f, ccp(0, -500));
        _dkSpr->runAction(CCSequence::create(_actMove, NULL));
    }
}

void StoryScene::logic(float dt){
    
    openStory = false;
    
    this->unschedule(schedule_selector(StoryScene::logic));
    
    if (!buttonBool1 && !buttonBool2) {
        if (logicBool1 || logicBool2) {
            
        }else{
            this->openTouch(0);
        }
    }
    
    if (dialogItem->getName().length() <= 0) {
        nameKuang->setVisible(false);
    }else{
        nameKuang->setVisible(true);
    }
    nameLab->setString(dialogItem->getName().c_str());
    
    if (wordCount > contentLength) {
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        this->closeEyesAnimation();
        
//        if (!quanBool) {
//            quanBool = true;
//            
//            quanSpr->setVisible(true);
//        }
//        
//        if (buttonBool3) {
//            this->setTouchEnabled(false);
//            
//            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), 1.f);
//        }
        
        
        // 101 、1002-1014
        int nameId = atoi(dialogItem->getNameId().c_str());
        if (nameId > 0 && nameId != 1015 && nameId != 1016 && nameId != 1017 && nameId != 1020 && nameId != 1021 && nameId != 1102 && nameId != 1202 && nameId != 1017 && nameId != 1018 && nameId != 1030) {
            
            if (CCUserDefault::sharedUserDefault()->getBoolForKey(dialogItem->getNameId().c_str(), false)) {
                if (!quanBool) {
                    quanBool = true;
                    
                    quanSpr->setVisible(true);
                }
                
                if (buttonBool3) {
                    openStory = true;
                    this->setTouchEnabled(false);
                    
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), 1.f);
                }
            }else{
                openStory = true;
                
                CCUserDefault::sharedUserDefault()->setBoolForKey("openCollect", true);
                CCUserDefault::sharedUserDefault()->setBoolForKey(dialogItem->getNameId().c_str(), true);
                CCUserDefault::sharedUserDefault()->flush();
                introBool = true;
                logicBool1 = true;
                
                CCString* introStr = CCString::createWithFormat("res/pic/qingjingScene/intro/%s.png", dialogItem->getNameId().c_str());
                CCSprite* introSpr = CCSprite::create(introStr->getCString());
                introSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                introSpr->setScale(.1f);
                introSpr->setVisible(false);
                introSpr->setTag(0x551188);
                CCScaleTo* scaleTo = CCScaleTo::create(.6f, 1.f);
                CCActionInterval * rotateto = CCRotateTo::create(.6f, 360* 2);
                CCFadeIn* fadeIn = CCFadeIn::create(.4f);
                CCSpawn* spawn = CCSpawn::create(CCShow::create(), scaleTo, rotateto, fadeIn, NULL);
                introSpr->runAction(CCSequence::create(CCDelayTime::create(.3f), spawn, NULL));
                this->addChild(introSpr, 100);
                
                _dkSpr->runAction(CCSequence::create(CCDelayTime::create(.5f), CCHide::create(), NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::openTouch), 1.5f);
            }
        }else{
            if (!quanBool) {
                quanBool = true;
                
                quanSpr->setVisible(true);
            }
            
            if (buttonBool3) {
                openStory = true;
                this->setTouchEnabled(false);
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), 1.f);
            }
        }
        
        
        return;
    }else{
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        if (buttonBool1) {
            // 101 、1002-1014
            int nameId = atoi(dialogItem->getNameId().c_str());
            if (nameId > 0 && nameId != 1015 && nameId != 1016 && nameId != 1017 && nameId != 1020 && nameId != 1021 && nameId != 1102 && nameId != 1202 && nameId != 1017 && nameId != 1018 && nameId != 1030) {
                
                if (CCUserDefault::sharedUserDefault()->getBoolForKey(dialogItem->getNameId().c_str(), false)) {
                    wordCount = getContentLength();
                    saidLabel->setString(getContentByLength(wordCount).c_str());
                    this->closeEyesAnimation();
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), .1f);
                    
                    this->schedule(schedule_selector(StoryScene::logic), .1f);
                }else{
                    openStory = true;
                    
                    CCUserDefault::sharedUserDefault()->setBoolForKey("openCollect", true);
                    CCUserDefault::sharedUserDefault()->setBoolForKey(dialogItem->getNameId().c_str(), true);
                    CCUserDefault::sharedUserDefault()->flush();
                    introBool = true;
                    logicBool2 = true;
                    
                    CCString* introStr = CCString::createWithFormat("res/pic/qingjingScene/intro/%s.png", dialogItem->getNameId().c_str());
                    CCSprite* introSpr = CCSprite::create(introStr->getCString());
                    introSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                    introSpr->setScale(.1f);
                    introSpr->setVisible(false);
                    introSpr->setTag(0x551188);
                    CCScaleTo* scaleTo = CCScaleTo::create(.6f, 1.f);
                    CCActionInterval * rotateto = CCRotateTo::create(.6f, 360* 2);
                    CCFadeIn* fadeIn = CCFadeIn::create(.4f);
                    CCSpawn* spawn = CCSpawn::create(CCShow::create(), scaleTo, rotateto, fadeIn, NULL);
                    introSpr->runAction(CCSequence::create(CCDelayTime::create(.3f), spawn, NULL));
                    this->addChild(introSpr, 100);
                    
                    _dkSpr->runAction(CCSequence::create(CCDelayTime::create(.5f), CCHide::create(), NULL));
                    
                    
                    wordCount = getContentLength();
                    saidLabel->setString(getContentByLength(wordCount).c_str());
                    this->closeEyesAnimation();
                    
                    
                    buttonBool1 = false;
                    buttonBool2 = false;
                    buttonBool3 = false;
                    recordBool1 = buttonBool1;
                    recordBool2 = buttonBool2;
                    recordBool3 = buttonBool3;
                    kuaijinToggleItem->setSelectedIndex(0);
                    zidongToggleItem->setSelectedIndex(0);
                    
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::openTouch), 1.5f);
                }
            }else{
                wordCount = getContentLength();
                saidLabel->setString(getContentByLength(wordCount).c_str());
                this->closeEyesAnimation();
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), .1f);
                
                this->schedule(schedule_selector(StoryScene::logic), .1f);
            }
        }else{
            this->schedule(schedule_selector(StoryScene::logic), .1f);
        }
    }
    
    
//    string::size_type idx = dialogItem->getNameId().find("1002");
//    if (idx != string::npos) {
//        CCLog("<><><> 有有有");
//        this->creatLogic();
//    }else{
//        CCLog("<><><> 没有没有没有");
//        this->creatLogic();
//    }
}


void StoryScene::creatBg(){
    openStory = true;
    this->setTouchEnabled(false);
    
    if (this->getChildByTag(Tag_GJ_bg) != NULL) {
        this->removeChildByTag(Tag_GJ_bg);
    }
    
    std::string str = dialogItem->getBg();
    if (!str.empty()) {
        CCString * CBg = CCString::createWithFormat("res/pic/qingjingScene/bgimage/%s", dialogItem->getBg().c_str());
        bgSpr = CCSprite::create(CBg->getCString());
        bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        bgSpr->setTag(Tag_GJ_bg);
        bgSpr->setOpacity(0);
        this->addChild(bgSpr, 3);
        
        if (dialogItem->getStates() != 4) {
            //淡入
            CCFiniteTimeAction* _bgAction = CCFadeIn::create(1.5f);
            bgSpr->runAction(CCSequence::create(_bgAction, NULL));
        }else{
            
            //淡入
            CCFiniteTimeAction* _bgAction = CCFadeIn::create(.3f);
            bgSpr->runAction(CCSequence::create(_bgAction, NULL));
            
            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::openTouch), 1.f);
        }
    }
}

void StoryScene::emptyLabel(){
    
    nameLab->setString("");
    saidLabel->setString(getContentByLength(wordCount).c_str());
}

void StoryScene::addButton(){
    // 退出
    CCSprite* fhSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_tuichu.png");
    CCSprite* fhSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_tuichu.png");
    fhSpr2->setColor(ccGRAY);
    CCMenuItem* fhItem;
    fhItem = CCMenuItemSprite::create(fhSpr1, fhSpr2, this, menu_selector(StoryScene::fhCallBack));
    fhItem->setPosition(ccp(kuangSpr->boundingBox().size.width* .1f, 43 - 500));
    
    // 回顾
    CCSprite* hgSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_huikan.png");
    CCSprite* hgSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_huikan.png");
    hgSpr2->setColor(ccGRAY);
    CCMenuItem* hkButton = CCMenuItemSprite::create(hgSpr1, hgSpr2, this, menu_selector(StoryScene::button2CallBack));
    hkButton->setPosition(ccp(kuangSpr->getContentSize().width* .61, kuangSpr->getContentSize().height + 80 - 500));
    
    
    // 自动
    CCSprite* zdSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_zidong1.png");
    CCSprite* zdSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_zidong1.png");
    zdSpr2->setColor(ccGRAY);
    CCSprite* ztSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_zidong2.png");
    CCSprite* ztSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_zidong2.png");
    ztSpr2->setColor(ccGRAY);
    CCMenuItem* zidongItemOn = CCMenuItemSprite::create(zdSpr1, zdSpr1);
    CCMenuItem* zidongItemOff = CCMenuItemSprite::create(ztSpr1, ztSpr2);
    zidongToggleItem = CCMenuItemToggle::createWithTarget(this, menu_selector(StoryScene::button3CallBack), zidongItemOn,zidongItemOff,NULL);
    zidongToggleItem->setPosition(ccp(kuangSpr->getContentSize().width* .76, kuangSpr->getContentSize().height + 80 - 500));
    zidongToggleItem->setTag(Tag_zidong);
    m_bIsZDSelect = recordBool3;
    if (m_bIsZDSelect) {
        zidongToggleItem->setSelectedIndex(1);
    }else{
        zidongToggleItem->setSelectedIndex(0);
    }
    
    // 快进
    CCSprite* kjSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin1.png");
    CCSprite* kjSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin1.png");
    kjSpr2->setColor(ccGRAY);
    CCSprite* ztSpr3 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin2.png");
    CCSprite* ztSpr4 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin2.png");
    ztSpr4->setColor(ccGRAY);
    CCMenuItem* kuaijinItemOn = CCMenuItemSprite::create(kjSpr1, kjSpr2);
    CCMenuItem* kuaijinItemOff = CCMenuItemSprite::create(ztSpr3, ztSpr4);
    kuaijinToggleItem = CCMenuItemToggle::createWithTarget(this, menu_selector(StoryScene::button1CallBack), kuaijinItemOn,kuaijinItemOff,NULL);
    kuaijinToggleItem->setPosition(ccp(kuangSpr->getContentSize().width* .91, kuangSpr->getContentSize().height + 80 - 500));
    CCPoint pos = ccp(kuaijinToggleItem->getPosition().x, kuaijinToggleItem->getPosition().y);
    kuaijinToggleItem->setTag(Tag_kuaijin);
    m_bIsKJSelect = recordBool1;
    if (DATA->getFastForward()) {
        if (m_bIsKJSelect) {
            kuaijinToggleItem->setSelectedIndex(1);
        }else{
            kuaijinToggleItem->setSelectedIndex(0);
        }
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
    
    
    CCMenu* menu = CCMenu::create(fhItem, kuaijinToggleItem, hkButton, zidongToggleItem, NULL);
    menu->setPosition(CCPointZero);
    menu->setTag(0x88888);
    _dkSpr->addChild(menu, 15);
    
    if (DATA->current_guide_step() == 9) {
        if (DATA->getOpenGuideBool9()) {
            fhItem->setColor(ccGRAY);
            fhItem->setEnabled(false);
        }
    }
    
}

void StoryScene::button1CallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "剧情界面", "点击快进");
    
    if (DATA->getFastForward()) {
        
        if (!buttonBool1) {
            buttonBool1 = true;
            buttonBool2 = false;
            buttonBool3 = false;
            recordBool1 = buttonBool1;
            recordBool2 = buttonBool2;
            recordBool3 = buttonBool3;
            openStory = true;
            this->setTouchEnabled(false);
            
            kuaijinToggleItem->setSelectedIndex(1);
            zidongToggleItem->setSelectedIndex(0);
            
            
            // talkingData
//            MMDataManager::get_instance()->onEvent("点击事件", "星途界面", "开启快进");
            
            wordCount = getContentLength();
            this->unschedule(schedule_selector(StoryScene::logic));
            this->schedule(schedule_selector(StoryScene::logic), .1f);
            
        }else{
            buttonBool1 = false;
            buttonBool2 = false;
            buttonBool3 = false;
            recordBool1 = buttonBool1;
            recordBool2 = buttonBool2;
            recordBool3 = buttonBool3;
            openStory = false;
            this->setTouchEnabled(true);
            
            kuaijinToggleItem->setSelectedIndex(0);
            zidongToggleItem->setSelectedIndex(0);
        }
    }else{
//        if (MMAudioManager::get_instance()->is_effect_on()) {
//            MMAudioManager::get_instance()->play_effect(kAudio_Button_errorCommon, false);
//        }
        
//        promptLayer->promptBox(31);
        
        buttonBool1 = false;
        recordBool1 = buttonBool1;
        openStory = false;
        this->setTouchEnabled(true);
        
        kuaijinToggleItem->setSelectedIndex(0);
    }
    
}
void StoryScene::button2CallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "剧情界面", "点击回顾");
    
    if (!buttonBool2) {
        buttonBool2 = true;
        buttonBool1 = false;
        buttonBool3 = false;
        recordBool1 = buttonBool1;
        recordBool2 = buttonBool2;
        recordBool3 = buttonBool3;
        openStory = true;
        this->setTouchEnabled(false);
        
        kuaijinToggleItem->setSelectedIndex(0);
        zidongToggleItem->setSelectedIndex(0);
        
        // talkingData
//        MMDataManager::get_instance()->onEvent("点击事件", "星途界面", "开启日志");
        
        LabelColorLayer* layer = LabelColorLayer::create_with_index(labStr.c_str());
        layer->setTag(0x999999);
        this->addChild(layer, 100);
        
        
    }else{
        buttonBool1 = false;
        buttonBool2 = false;
        buttonBool3 = false;
        recordBool1 = buttonBool1;
        recordBool2 = buttonBool2;
        recordBool3 = buttonBool3;
        openStory = false;
        this->setTouchEnabled(true);
        
        kuaijinToggleItem->setSelectedIndex(0);
        zidongToggleItem->setSelectedIndex(0);
    }
}


void StoryScene::button3CallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "剧情界面", "点击自动");
    
    if (!buttonBool3) {
        buttonBool3 = true;
        buttonBool1 = false;
        buttonBool2 = false;
        recordBool1 = buttonBool1;
        recordBool2 = buttonBool2;
        recordBool3 = buttonBool3;
        
        kuaijinToggleItem->setSelectedIndex(0);
        zidongToggleItem->setSelectedIndex(1);
        
        // talkingData
//        MMDataManager::get_instance()->onEvent("点击事件", "星途界面", "开启播放");
        
        this->unschedule(schedule_selector(StoryScene::logic));
        this->schedule(schedule_selector(StoryScene::logic), .1f);
    }else{
        buttonBool1 = false;
        buttonBool2 = false;
        buttonBool3 = false;
        recordBool1 = buttonBool1;
        recordBool2 = buttonBool2;
        recordBool3 = buttonBool3;
        
        kuaijinToggleItem->setSelectedIndex(0);
        zidongToggleItem->setSelectedIndex(0);
    }
}

void StoryScene::initButton(){
    if (DATA->current_guide_step() == 9) {
        if (DATA->getOpenGuideBool9()) {
            GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
            layer->setTag(0x445566);
            this->addChild(layer, 500);
        }
    }
    
    
    CCArray* arr = dialogItem->getArray();
    int nextIndex;
    CCString* str;
    
    recordBool1 = buttonBool1;
//    MZLog("recordBool1 === %d", recordBool1);
    recordBool2 = buttonBool2;
//    MZLog("recordBool2 === %d", recordBool2);
    recordBool3 = buttonBool3;
//    MZLog("recordBool3 === %d", recordBool3);
    buttonBool1 = false;
    buttonBool2 = false;
    buttonBool3 = false;
    
    for (int i = 0; i < arr->count(); i++){
        CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(i);
        nextIndex = dic->valueForKey("next")->intValue();
        str = (CCString* )dic->objectForKey("dialog");
//        CCLog("%s == %d", str->getCString(), nextIndex);
        
        this->creatButton(i);
        this->initLabel(i);
        this->menuEffect(i);
    }
}

void StoryScene::creatButton(int dex){
    buttonBool = true;
    
    if (dex == 0) {
        // 任务条
        CCSprite* spr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCSprite* spr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCMenuItem* item1 = CCMenuItemSprite::create(spr1, spr2, this, menu_selector(StoryScene::callBackMethods));
        item1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .59f));
        item1->setTag(Tag_Item_0);
        CCMenu* menu1 = CCMenu::create(item1, NULL);
        menu1->setPosition(CCPointMake(0, 0));
        menu1->setTag(Tag_Menu_0);
        menu1->setVisible(true);
        this->addChild(menu1, 20);
        
        if (DATA->current_guide_step() == 9) {
            if (DATA->getOpenGuideBool9()) {
                item1->setEnabled(false);
            }
        }
        
        
    }else if (dex == 1){
        CCSprite* spr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCSprite* spr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCMenuItem* item2 = CCMenuItemSprite::create(spr1, spr2, this, menu_selector(StoryScene::callBackMethods));
        item2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        item2->setTag(Tag_Item_1);
        CCMenu* menu2 = CCMenu::create(item2, NULL);
        menu2->setPosition(CCPointMake(0, 0));
        menu2->setTag(Tag_Menu_1);
        menu2->setVisible(true);
        this->addChild(menu2, 20);
        
    }else if (dex == 2){
        CCSprite* spr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCSprite* spr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCMenuItem* item3 = CCMenuItemSprite::create(spr1, spr2, this, menu_selector(StoryScene::callBackMethods));
        item3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .41f));
        item3->setTag(Tag_Item_2);
        CCMenu* menu3 = CCMenu::create(item3, NULL);
        menu3->setPosition(CCPointMake(0, 0));
        menu3->setTag(Tag_Menu_2);
        menu3->setVisible(true);
        this->addChild(menu3, 20);
        
        if (DATA->current_guide_step() == 9) {
            if (DATA->getOpenGuideBool9()) {
                item3->setEnabled(false);
            }
        }
        
    }
}
void StoryScene::initLabel(int dex){
//    CCArray* arr = dialogItem->getArray();
//    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(dex);
//    dialogItem = (MMDialogItem* )dialog->getDialogs()->objectAtIndex(index);
    CCArray* arr = dialogItem->getArray();
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(dex);
    int nextIndex = dic->valueForKey("next")->intValue();
    CCString* str = (CCString* )dic->objectForKey("dialog");
//    CCLog("%s == %d", str->getCString(), nextIndex);
    
//    CCString* str = (CCString* )dic->objectForKey("dialog");
//    MZLog("====== str->length()%d ===== %d", dex, str->length());
//    CCLog("label ==== %s", str->getCString());
    
    CCLabelTTF* lab;
    if (str->length() > 75) {
        lab = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 28.f);
    }else if (str->length() > 85){
        lab = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 25.f);
    }
    else{
        lab = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 34.f);
    }
    
    if (dex == 0){
        lab->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .59f));
        lab->setTag(Tag_Label_0);
    }else if (dex == 1){
        lab->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        lab->setTag(Tag_Label_1);
    }else if (dex == 2){
        lab->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .41f));
        lab->setTag(Tag_Label_2);
    }
    lab->setColor(ccWHITE);
//    lab->enableStroke(ccWHITE, .4f);
    lab->setVisible(true);
    this->addChild(lab, 21);
}

void StoryScene::menuEffect(int dex){
    
    float actTime1 = 0.5f;
    float actTime2 = 0.6f;
    float actTime3 = 0.7f;
    if (dex == 0) {
        CCNode* node1 = this->getChildByTag(Tag_Menu_0);
        CCNode* labNode1 = this->getChildByTag(Tag_Label_0);
        if (node1 != NULL && labNode1 != NULL) {
            node1->setPositionX(-900);
            node1->runAction(CCSequence::create(CCMoveTo::create(actTime1, CCPointMake(0, 0)), NULL));
            
            labNode1->setPositionX(-900);
            labNode1->runAction(CCSequence::create(CCMoveTo::create(actTime1, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .59f)), NULL));
        }
        
    }else if (dex == 1){
        CCNode* node2 = this->getChildByTag(Tag_Menu_1);
        CCNode* labNode2 = this->getChildByTag(Tag_Label_1);
        if (node2 != NULL && labNode2 != NULL) {
            node2->setPositionX(-900);
            node2->runAction(CCSequence::create(CCMoveTo::create(actTime2, CCPointMake(0, 0)), NULL));
            
            labNode2->setPositionX(-900);
            labNode2->runAction(CCSequence::create(CCMoveTo::create(actTime2, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f)), NULL));
        }
        
    }else if (dex == 2){
        CCNode* node3 = this->getChildByTag(Tag_Menu_2);
        CCNode* labNode3 = this->getChildByTag(Tag_Label_2);
        if (node3 != NULL && labNode3 != NULL) {
            node3->setPositionX(-900);
            node3->runAction(CCSequence::create(CCMoveTo::create(actTime3, CCPointMake(0, 0)), NULL));
            
            labNode3->setPositionX(-900);
            labNode3->runAction(CCSequence::create(CCMoveTo::create(actTime3, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .41f)), NULL));
        }
    }
    
    // 添加点击引导
//    if(MMDataManager::get_instance()->getPlayer_info()->getLevel() < 1)
//    {
//        if(!this->getChildByTag(0x1234)){
//            YDStoryScene * layer = YDStoryScene::create();
//            this->addChild(layer, 10, 0x1234);
//        }
//    }
}

void StoryScene::goCallBackMethods(){
    if(this->getChildByTag(0x1234))
    {
        this->removeChildByTag(0x1234, true);
    }
    
    CCNode* node;
    CCMenuItem* menuItem;
    CCArray* arr = dialogItem->getArray();
    int numIndex = arr->count();
//    MZLog("numIndex ==== %d", numIndex);
    
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(menuItem->getTag() - 30);
    int attribute = dic->valueForKey("attribute")->intValue();
    int attributeValue = dic->valueForKey("attributeValue")->intValue();
//    MZLog("attribute === %d", attribute);
//    MZLog("attributeValue === %d", attributeValue);
    
    float actTime = 0.4f;
    if (buttonBool) {
        buttonBool = false;
        
        CCNode* node1, * node2, * node3, * labNode1, * labNode2, * labNode3;
        if (this->getChildByTag(Tag_Menu_0) != NULL) {
            node1 = this->getChildByTag(Tag_Menu_0);
            labNode1 = this->getChildByTag(Tag_Label_0);
        }
        
        if (this->getChildByTag(Tag_Menu_1) != NULL) {
            node2 = this->getChildByTag(Tag_Menu_1);
            labNode2 = this->getChildByTag(Tag_Label_1);
        }
        
        if (this->getChildByTag(Tag_Menu_2) != NULL) {
            node3 = this->getChildByTag(Tag_Menu_2);
            labNode3 = this->getChildByTag(Tag_Label_2);
        }
        
        switch (menuItem->getTag()) {
            case Tag_Item_0:{
                tagIndex = Tag_Item_0;
                
                if (numIndex >= 1 && numIndex < 3) {
                    CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                    node2->runAction(CCSequence::create(_Action2, NULL));
                    
                    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                    labNode2->runAction(CCSequence::create(_labAction2, NULL));
                }else if (numIndex >= 1 && numIndex <= 3){
                    CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                    node2->runAction(CCSequence::create(_Action2, NULL));
                    
                    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                    labNode2->runAction(CCSequence::create(_labAction2, NULL));
                    
                    CCFiniteTimeAction* _Action3 = CCFadeOut::create(actTime);
                    node3->runAction(CCSequence::create(_Action3, NULL));
                    
                    CCFiniteTimeAction* _labAction3 = CCFadeOut::create(actTime);
                    labNode3->runAction(CCSequence::create(_labAction3, NULL));
                    
                }
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::buttonAction), 0.2f);
            }
                break;
                
            case Tag_Item_1:{
                tagIndex = Tag_Item_1;
                
                if (numIndex >= 1 && numIndex < 3) {
                    CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                    node1->runAction(CCSequence::create(_Action1, NULL));
                    
                    CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                    labNode1->runAction(CCSequence::create(_labAction1, NULL));
                }else if (numIndex >= 1 && numIndex <= 3){
                    CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                    node1->runAction(CCSequence::create(_Action1, NULL));
                    
                    CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                    labNode1->runAction(CCSequence::create(_labAction1, NULL));
                    
                    CCFiniteTimeAction* _Action3 = CCFadeOut::create(actTime);
                    node3->runAction(CCSequence::create(_Action3, NULL));
                    
                    CCFiniteTimeAction* _labAction3 = CCFadeOut::create(actTime);
                    labNode3->runAction(CCSequence::create(_labAction3, NULL));
                }
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::buttonAction), 0.2f);
                
            }
                break;
                
            case Tag_Item_2:{
                tagIndex = Tag_Item_2;
                
                CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                node1->runAction(CCSequence::create(_Action1, NULL));
                
                CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                labNode1->runAction(CCSequence::create(_labAction1, NULL));
                
                CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                node2->runAction(CCSequence::create(_Action2, NULL));
                
                CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                labNode2->runAction(CCSequence::create(_labAction2, NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::buttonAction), 0.2f);
                
            }
                break;
                
            default:
                break;
        }
    }
}
void StoryScene::callBackMethods(CCObject* pSender){
    if(this->getChildByTag(0x1234))
    {
        this->removeChildByTag(0x1234, true);
    }
    
    CCMenuItemImage* menuItem = (CCMenuItemImage* )pSender;
    CCArray* arr = dialogItem->getArray();
    int numIndex = arr->count();
//    MZLog("numIndex ==== %d", numIndex);
    
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(menuItem->getTag() - 30);
    int attribute = dic->valueForKey("attribute")->intValue();
    int attributeValue = dic->valueForKey("attributeValue")->intValue();
//    MZLog("attribute === %d", attribute);
//    MZLog("attributeValue === %d", attributeValue);
    
    float actTime = 0.4f;
    if (buttonBool) {
        buttonBool = false;
        
        CCNode* node1, * node2, * node3, * labNode1, * labNode2, * labNode3;
        if (this->getChildByTag(Tag_Menu_0) != NULL) {
            node1 = this->getChildByTag(Tag_Menu_0);
            labNode1 = this->getChildByTag(Tag_Label_0);
        }
        
        if (this->getChildByTag(Tag_Menu_1) != NULL) {
            node2 = this->getChildByTag(Tag_Menu_1);
            labNode2 = this->getChildByTag(Tag_Label_1);
        }
        
        if (this->getChildByTag(Tag_Menu_2) != NULL) {
            node3 = this->getChildByTag(Tag_Menu_2);
            labNode3 = this->getChildByTag(Tag_Label_2);
        }
        
        switch (menuItem->getTag()) {
            case Tag_Item_0:{
                tagIndex = Tag_Item_0;
                
//                if (MMAudioManager::get_instance()->is_effect_on()) {
//                    MMAudioManager::get_instance()->play_effect(kAudio_Button_Common, false);
//                }
                
                if (numIndex >= 1 && numIndex < 3) {
                    CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                    node2->runAction(CCSequence::create(_Action2, NULL));
                    
                    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                    labNode2->runAction(CCSequence::create(_labAction2, NULL));
                }else if (numIndex >= 1 && numIndex <= 3){
                    CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                    node2->runAction(CCSequence::create(_Action2, NULL));
                    
                    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                    labNode2->runAction(CCSequence::create(_labAction2, NULL));
                    
                    CCFiniteTimeAction* _Action3 = CCFadeOut::create(actTime);
                    node3->runAction(CCSequence::create(_Action3, NULL));
                    
                    CCFiniteTimeAction* _labAction3 = CCFadeOut::create(actTime);
                    labNode3->runAction(CCSequence::create(_labAction3, NULL));
                    
                }
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::buttonAction), 0.2f);
            }
                break;
                
            case Tag_Item_1:{
                tagIndex = Tag_Item_1;
                
                if (numIndex >= 1 && numIndex < 3) {
                    CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                    node1->runAction(CCSequence::create(_Action1, NULL));
                    
                    CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                    labNode1->runAction(CCSequence::create(_labAction1, NULL));
                }else if (numIndex >= 1 && numIndex <= 3){
                    CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                    node1->runAction(CCSequence::create(_Action1, NULL));
                    
                    CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                    labNode1->runAction(CCSequence::create(_labAction1, NULL));
                    
                    CCFiniteTimeAction* _Action3 = CCFadeOut::create(actTime);
                    node3->runAction(CCSequence::create(_Action3, NULL));
                    
                    CCFiniteTimeAction* _labAction3 = CCFadeOut::create(actTime);
                    labNode3->runAction(CCSequence::create(_labAction3, NULL));
                }
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::buttonAction), 0.2f);
                
            }
                break;
                
            case Tag_Item_2:{
                tagIndex = Tag_Item_2;
                
                CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                node1->runAction(CCSequence::create(_Action1, NULL));
                
                CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                labNode1->runAction(CCSequence::create(_labAction1, NULL));
                
                CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                node2->runAction(CCSequence::create(_Action2, NULL));
                
                CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                labNode2->runAction(CCSequence::create(_labAction2, NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::buttonAction), 0.2f);
                
            }
                break;
                
            default:
                break;
        }
    }
}

void StoryScene::buttonAction(float dt){
    CCNode* node, * labNode;
    _bSpr = CCSprite::create();
    if (tagIndex == Tag_Item_0) {
        node = this->getChildByTag(Tag_Menu_0);
        labNode = this->getChildByTag(Tag_Label_0);
        _bSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .53f));
    }else if (tagIndex == Tag_Item_1){
        node = this->getChildByTag(Tag_Menu_1);
        labNode = this->getChildByTag(Tag_Label_1);
        _bSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .44f));
    }else if (tagIndex == Tag_Item_2){
        node = this->getChildByTag(Tag_Menu_2);
        labNode = this->getChildByTag(Tag_Label_2);
        _bSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .36f));
    }
    this->addChild(_bSpr, 9);
    
    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::buttonOut), 0.6f);
}
void StoryScene::buttonOut(float dt){
    CCNode* node, * labNode;
    if (tagIndex == Tag_Item_0) {
        node = this->getChildByTag(Tag_Menu_0);
        labNode = this->getChildByTag(Tag_Label_0);
    }else if (tagIndex == Tag_Item_1){
        node = this->getChildByTag(Tag_Menu_1);
        labNode = this->getChildByTag(Tag_Label_1);
    }else if (tagIndex == Tag_Item_2){
        node = this->getChildByTag(Tag_Menu_2);
        labNode = this->getChildByTag(Tag_Label_2);
    }
    
    CCFiniteTimeAction* _Action2 = CCFadeOut::create(.6f);
    node->runAction(CCSequence::create(_Action2, NULL));
    
    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(.6f);
    labNode->runAction(CCSequence::create(_labAction2, NULL));
    
    removeButton();
    
    if (tagIndex == Tag_Item_0) {
        this->callBack1();
    }else if (tagIndex == Tag_Item_1){
        this->callBack2();
    }else if (tagIndex == Tag_Item_2){
        this->callBack3();
    }
}

void StoryScene::removeButton(){
    CCArray* arr = dialogItem->getArray();
    int nextindex = 0;
    CCDictionary* dic;
    
    if (tagIndex == Tag_Item_0) {
        dic = (CCDictionary*)arr->objectAtIndex(0);
        index = dic->valueForKey("next")->intValue();
        CCString* str = (CCString* )dic->objectForKey("dialog");
//        CCLog("%s == %d", str->getCString(), index);
        nextindex = dic->valueForKey("next")->intValue();
    }else if (tagIndex == Tag_Item_1){
        dic = (CCDictionary*)arr->objectAtIndex(1);
        index = dic->valueForKey("next")->intValue();
        CCString* str = (CCString* )dic->objectForKey("dialog");
//        CCLog("%s == %d", str->getCString(), index);
        nextindex = dic->valueForKey("next")->intValue();
    }else if (tagIndex == Tag_Item_2){
        dic = (CCDictionary*)arr->objectAtIndex(2);
        index = dic->valueForKey("next")->intValue();
        CCString* str = (CCString* )dic->objectForKey("dialog");
//        CCLog("%s == %d", str->getCString(), index);
        nextindex = dic->valueForKey("next")->intValue();
    }
    
    
    openButtonBool = false;
    
//    dialogItem->setArray(CCArray);
    
    if (this->getChildByTag(Tag_Menu_0) != NULL) {
        this->removeChildByTag(Tag_Menu_0);
    }
    if (this->getChildByTag(Tag_Menu_1) != NULL) {
        this->removeChildByTag(Tag_Menu_1);
    }
    if (this->getChildByTag(Tag_Menu_2) != NULL) {
        this->removeChildByTag(Tag_Menu_2);
    }
    
    if (this->getChildByTag(Tag_Label_0) != NULL) {
        this->removeChildByTag(Tag_Label_0);
    }
    if (this->getChildByTag(Tag_Label_1) != NULL) {
        this->removeChildByTag(Tag_Label_1);
    }
    if (this->getChildByTag(Tag_Label_2) != NULL) {
        this->removeChildByTag(Tag_Label_2);
    }
    
    if (this->getChildByTag(Tag_Item_0) != NULL) {
        this->removeChildByTag(Tag_Item_0);
    }
    if (this->getChildByTag(Tag_Item_1) != NULL) {
        this->removeChildByTag(Tag_Item_1);
    }
    if (this->getChildByTag(Tag_Item_2) != NULL) {
        this->removeChildByTag(Tag_Item_2);
    }
}

void StoryScene::removeMan(){
    this->unschedule(SEL_SCHEDULE(&StoryScene::_70CallFunc));
    
    if (headSpr1 != NULL) {
        headSpr1->removeAllChildren();
    }
    if (headSpr2 != NULL) {
        headSpr2->removeAllChildren();
    }
    
    if (this->getChildByTag(Tag_GJ_man1) != NULL) {
        this->removeChildByTag(Tag_GJ_man1);
    }
    if (this->getChildByTag(Tag_GJ_head1) != NULL) {
        this->removeChildByTag(Tag_GJ_head1);
    }
    if (this->getChildByTag(Tag_GJ_man2) != NULL) {
        this->removeChildByTag(Tag_GJ_man2);
    }
    if (this->getChildByTag(Tag_GJ_head2) != NULL) {
        this->removeChildByTag(Tag_GJ_head2);
    }
    
    manSpr1 = NULL;
    manSpr2 = NULL;
    headSpr1 = NULL;
    headSpr2 = NULL;
}

void StoryScene::callBack1(){
    
    CCArray* arr = dialogItem->getArray();
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(0);
    
//    buttonBool1 = recordBool1;
    buttonBool1 = false;
    recordBool1 = buttonBool1;
    m_bIsKJSelect = buttonBool1;
    if (DATA->getFastForward()) {
        if (m_bIsKJSelect) {
            kuaijinToggleItem->setSelectedIndex(1);
        }else{
            kuaijinToggleItem->setSelectedIndex(0);
        }
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
//    MZLog("StoryScene::callBack1 ******* buttonBool1 === %d", buttonBool1);
    buttonBool2 = recordBool2;
//    MZLog("StoryScene::callBack1 ******* buttonBool2 === %d", buttonBool2);
    buttonBool3 = recordBool3;
//    MZLog("StoryScene::callBack1 ******* buttonBool3 === %d", buttonBool3);
    
    dialogItem = (DialogItem* )dialog->getDialogs()->objectAtIndex(dic->valueForKey("next")->intValue());
    wordCount = 0;
    this->init(dialogItem);
    this->dialogueControl(dialogItem);
}
void StoryScene::callBack2(){
    CCArray* arr = dialogItem->getArray();
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(1);
    
//    buttonBool1 = recordBool1;
    buttonBool1 = false;
    recordBool1 = buttonBool1;
    m_bIsKJSelect = buttonBool1;
    if (DATA->getFastForward()) {
        if (m_bIsKJSelect) {
            kuaijinToggleItem->setSelectedIndex(1);
        }else{
            kuaijinToggleItem->setSelectedIndex(0);
        }
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
//    MZLog("StoryScene::callBack2 ******* buttonBool1 === %d", buttonBool1);
    buttonBool2 = recordBool2;
//    MZLog("StoryScene::callBack2 ******* buttonBool2 === %d", buttonBool2);
    buttonBool3 = recordBool3;
//    MZLog("StoryScene::callBack2 ******* buttonBool3 === %d", buttonBool3);
    
    dialogItem = (DialogItem* )dialog->getDialogs()->objectAtIndex(dic->valueForKey("next")->intValue());
    wordCount = 0;
    this->init(dialogItem);
    this->dialogueControl(dialogItem);
}
void StoryScene::callBack3(){
    CCArray* arr = dialogItem->getArray();
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(2);
    
//    buttonBool1 = recordBool1;
    buttonBool1 = false;
    recordBool1 = buttonBool1;
    m_bIsKJSelect = buttonBool1;
    if (DATA->getFastForward()) {
        if (m_bIsKJSelect) {
            kuaijinToggleItem->setSelectedIndex(1);
        }else{
            kuaijinToggleItem->setSelectedIndex(0);
        }
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
//    MZLog("StoryScene::callBack3 ******* buttonBool1 === %d", buttonBool1);
    buttonBool2 = recordBool2;
//    MZLog("StoryScene::callBack3 ******* buttonBool2 === %d", buttonBool2);
    buttonBool3 = recordBool3;
//    MZLog("StoryScene::callBack3 ******* buttonBool3 === %d", buttonBool3);
    
    dialogItem = (DialogItem* )dialog->getDialogs()->objectAtIndex(dic->valueForKey("next")->intValue());
    wordCount = 0;
    this->init(dialogItem);
    this->dialogueControl(dialogItem);
}

void StoryScene::gengxin(float dt){
    
    if (bgSpr != NULL) {
        CCPoint pos = bgSpr->getPosition();
        pos.x = DISPLAY->ScreenWidth()* .5f + playerVelocity.x;
        
        bool iPhoneOriPad = CCUserDefault::sharedUserDefault()->getBoolForKey("iPhoneOriPad");
        if (!iPhoneOriPad) {
            if (pos.x < DISPLAY->ScreenWidth()* .5f - 70) {
                pos.x = DISPLAY->ScreenWidth()* .5f - 70;
                playerVelocity = CCPointZero;
            }
            else if (pos.x > DISPLAY->ScreenWidth()* .5f + 70){
                pos.x = DISPLAY->ScreenWidth()* .5f + 70;
                playerVelocity = CCPointZero;
            }
        }else{
            if (pos.x < DISPLAY->ScreenWidth()* .5f - 10) {
                pos.x = DISPLAY->ScreenWidth()* .5f - 10;
                playerVelocity = CCPointZero;
            }
            else if (pos.x > DISPLAY->ScreenWidth()* .5f + 10){
                pos.x = DISPLAY->ScreenWidth()* .5f + 10;
                playerVelocity = CCPointZero;
            }
        }
        bgSpr->runAction(CCMoveTo::create(.1f, ccp(pos.x, pos.y)));
    }
}

void StoryScene::init(DialogItem* item){
    content = item->getSaid();
    contentLength = 0;
    
    int length = content.length();
    int i = 0;
    while (i < length) {
        char ch = getContent()[i];
        
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        contentLength++;
    }
}

std::string StoryScene::getContentByLength(int length){
    if (length >= contentLength) {
        return getContent();
    }
    
    int i = 0;
    int index = 0;
    while (index < length) {
        char ch = getContent()[i];
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        index++;
    }
    std::string str = content.substr(0, i);
//    MZLog("index ======= %d", index);
    return str;
}
int StoryScene::getContentLength(){
    return contentLength;
}

void StoryScene::onEnter()
{
    this->setAccelerometerEnabled(true);
    
    BaseScene::onEnter();
    
    BaseScene::hideBaseScene();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&StoryScene::_503CallBack), "HTTP_FINISHED_503", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&StoryScene::LabelColorFhCallBack), "LabelColorFhCallBack", NULL);
    nc->addObserver(this, SEL_CallFuncO(&StoryScene::goCallBackMethods), "GoCallBackMethods", NULL);
    nc->addObserver(this, SEL_CallFuncO(&StoryScene::button1CallBack), "Button1CallBack", NULL);
    
    
    
    // 播放背景音
//    if (MMAudioManager::get_instance()->is_music_on()) {
//        if (MMAudioManager::get_instance()->isBackgroundMusicPlaying()) {
//            MMAudioManager::get_instance()->play_music(kMusic_BG_Qingjing, true);
//        }
//        else {
//            MMAudioManager::get_instance()->play_music(kMusic_BG_Qingjing, true);
//        }
//    }

    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::keyBackStatus), .8f);
}
void StoryScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void StoryScene::onExit(){
    openStory = true;
    this->setTouchEnabled(false);
    this->setAccelerometerEnabled(false);
    this->unscheduleAllSelectors();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeAllObservers(this);
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
//    if (MMAudioManager::get_instance()->isBackgroundMusicPlaying()) {
//        bool is = CCUserDefault::sharedUserDefault()->getBoolForKey("play_1", true);
//        if (is) {
//            if (MMAudioManager::get_instance()->isBackgroundMusicPlaying()) {
//                MMAudioManager::get_instance()->play_music(kMusic_BG_main2, true);
//            }
//        } else {
//            if (MMAudioManager::get_instance()->isBackgroundMusicPlaying()) {
//                MMAudioManager::get_instance()->play_music(kMusic_BG_main1, true);
//            }
//        }
//    }
    
    BaseScene::onExit();
}

//void StoryScene::registerWithTouchDispatcher(){
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
//}

bool StoryScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("StoryScene::ccTouchBegan() ...");
    static int guide_count = 0;
    if (!openStory) {
        if (introBool) {
            introBool = false;
            
            if (this->getChildByTag(0x551188) != NULL) {
                this->removeChildByTag(0x551188);
            }
            _dkSpr->runAction(CCShow::create());
            
            if (logicBool1) {
                logicBool1 = false;
                
                if (!quanBool) {
                    quanBool = true;
                    
                    quanSpr->setVisible(true);
                }
                
                if (buttonBool3) {
                    openStory = true;
                    this->setTouchEnabled(false);
                    
                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), 1.f);
                }
            }else if (logicBool2){
                logicBool2 = false;
                
                
            }
            
        }else{
            
            logIndex++;
            
            if (buttonBool3) {
                if (logIndex == 1) {
                    
                    this->unschedule(SEL_SCHEDULE(&StoryScene::getIndex));
                    
                    if (dialogItem->getStates() != 4) {
                        if (wordCount < contentLength) {
                            wordCount = getContentLength();
                            openStory = true;
                            this->setTouchEnabled(false);
                            
                            this->closeEyesAnimation();
                            
                            int nameId = atoi(dialogItem->getNameId().c_str());
                            if (nameId > 0 && nameId != 1015 && nameId != 1016 && nameId != 1017 && nameId != 1020 && nameId != 1021 && nameId != 1102 && nameId != 1202) {
                                
                                if (CCUserDefault::sharedUserDefault()->getBoolForKey(dialogItem->getNameId().c_str(), false)) {
                                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::openTouch), .5f);
                                }else{
                                    openStory = true;
                                }
                            }else{
                                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::openTouch), .5f);
                            }
                        }else{
                            openStory = true;
                            this->setTouchEnabled(false);
                            
                            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), .5f);
                            
                        }
                    }else{
                        openStory = true;
                        this->setTouchEnabled(false);
                        
                        this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), .5f);
                    }
                }
            }else{
                if (logIndex == 1) {
                    this->unschedule(SEL_SCHEDULE(&StoryScene::getIndex));
                    
                    if (dialogItem->getStates() != 4) {
                        if (wordCount < contentLength) {
                            wordCount = getContentLength();
                            openStory = true;
                            this->setTouchEnabled(false);
                            
                            this->closeEyesAnimation();
                            
                            int nameId = atoi(dialogItem->getNameId().c_str());
                            if (nameId > 0 && nameId != 1015 && nameId != 1016 && nameId != 1017 && nameId != 1020 && nameId != 1021 && nameId != 1102 && nameId != 1202) {
                                
                                if (CCUserDefault::sharedUserDefault()->getBoolForKey(dialogItem->getNameId().c_str(), false)) {
                                    this->scheduleOnce(SEL_SCHEDULE(&StoryScene::openTouch), .5f);
                                }else{
                                    openStory = true;
                                }
                            }else{
                                this->scheduleOnce(SEL_SCHEDULE(&StoryScene::openTouch), .5f);
                            }
                            
                        }else{
                            openStory = true;
                            this->setTouchEnabled(false);
                            
                            this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), .3f);
                            
                        }
                    }else{
                        openStory = true;
                        this->setTouchEnabled(false);
                        
                        this->scheduleOnce(SEL_SCHEDULE(&StoryScene::getIndex), .3f);
                    }
                }
            }
        }
    }
    
    return true;
}

void StoryScene::openTouch(float dt){
    logIndex = 0;
    openStory = false;
    this->setTouchEnabled(true);
}

void StoryScene::getIndex(float dt){
    if (quanSpr != NULL) {
        quanBool = false;
        quanSpr->setVisible(false);
    }
    
    if (dialogItem->getArray() == NULL) {
        index = dialogItem->getNext();
//        CCLOG("StoryScene::getIndex -- %d", index);
        
        CCString* _achievement = CCString::createWithFormat("%s",dialogItem->getAchievement().c_str());
        if (index == -1) {
            buttonBool1 = false;
            buttonBool2 = false;
            buttonBool3 = false;
            
            if (_dkSpr->getChildByTag(0x88888) != NULL) {
                CCNode* node = _dkSpr->getChildByTag(0x88888);
                if (node->getChildByTag(Tag_kuaijin) != NULL) {
                    kuaijinToggleItem->setSelectedIndex(0);
                }
                if (node->getChildByTag(Tag_zidong) != NULL) {
                    zidongToggleItem->setSelectedIndex(0);
                }
            }
            openStory = true;
            this->setTouchEnabled(false);
            
            storyIndex = 1;
            LOADING->show_loading();
            endingStr = CCString::createWithFormat("%s", "-1");
            endingStr->retain();
            CCString* indexStr = CCString::createWithFormat("%d", m_current_story_index_id);
            NET->commit_story_503(indexStr->getCString(), endingStr->getCString());
            
        }else if (index == -2){
            buttonBool1 = false;
            buttonBool2 = false;
            buttonBool3 = false;
            
            if (_dkSpr->getChildByTag(0x88888) != NULL) {
                CCNode* node = _dkSpr->getChildByTag(0x88888);
                if (node->getChildByTag(Tag_kuaijin) != NULL) {
                    kuaijinToggleItem->setSelectedIndex(0);
                }
                if (node->getChildByTag(Tag_zidong) != NULL) {
                    zidongToggleItem->setSelectedIndex(0);
                }
            }
            openStory = true;
            this->setTouchEnabled(false);
            endingStr = CCString::createWithFormat("%s", _achievement->getCString());
            endingStr->retain();
            
            storyIndex = 2;
            LOADING->show_loading();
            CCString* indexStr = CCString::createWithFormat("%d", m_current_story_index_id);
            NET->commit_story_503(indexStr->getCString(), endingStr->getCString());
            
        }else{
            dialogItem = (DialogItem* )dialog->getDialogs()->objectAtIndex(index);
            this->init(dialogItem);
            wordCount = 0;
            this->dialogueControl(dialogItem);
        }
        
    }else{
        if (!openButtonBool) {
            openButtonBool = true;
            this->initButton();
        }
    }
}

void StoryScene::didAccelerate( CCAcceleration* pAccelerationValue){
    float deceleration = 0 ;
    float sensitivity = 500;
    
    playerVelocity.x = playerVelocity.x * deceleration+ pAccelerationValue->x*sensitivity;
    
    playerVelocity.x = playerVelocity.x/ 10;
}

//void StoryScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
//    box->animation_out();
//    if (button_type == AH_BUTTON_TYPE_YESNO2) {
//        if (button_tag == AH_BUTTON_TAG_YES) {
//            if (MMAudioManager::get_instance()->is_music_on()) {
//                MMAudioManager::get_instance()->stopBackgroundMusic();
//            }
//            
//            if (MMAudioManager::get_instance()->is_effect_on()) {
//                MMAudioManager::get_instance()->play_effect(kAudio_Button_Back, false);
//            }
//            
//            // talkingData
//            MMDataManager::get_instance()->onEvent("点击事件", "星途界面", "中途退出剧情");
//            
//            MMDataManager::get_instance()->setInGame(false);
//            MMLoadingLayer* layer = MMLoadingLayer::create_with_type(0);
//            this->addChild(layer, 5000);
//            MMNetManager::get_instance()->http_3000_get_pet_story(MMDataManager::get_instance()->get_current_manpet_id());
//        }
//    }
//}

void StoryScene::keyBackClicked(){
    num_child++;
//    CCLog("===== StoryScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    
    this->setKeypadEnabled(false);
    this->fhCallBack(this);
}

void StoryScene::fhCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "剧情界面", "点击退出");
    
    AUDIO->goback_effect();
//    if (MMAudioManager::get_instance()->is_effect_on()) {
//        MMAudioManager::get_instance()->play_effect(kAudio_audio_AHMessageBox, false);
//    }
    
//    AHMessageBox* mb = AHMessageBox::create_with_message(def_storyScene_007, this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO2, true);
//    mb->setPosition(Center);
//    CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 3010);
    
    if (DATA->getOpenGuideBool9()) {
        DATA->setOpenGuideBool9(false);
    }
    
    if (DATA->current_guide_step() == 0 || DATA->current_guide_step() != 8) {
        num_child = 0;
        CCScene* scene = QingjingScene::scene();
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}

void StoryScene::LabelColorFhCallBack(CCObject* pSender){
    if (this->getChildByTag(0x999999) != NULL) {
        this->removeChildByTag(0x999999);
    }
    
    buttonBool1 = false;
    buttonBool2 = false;
    buttonBool3 = false;
    recordBool1 = buttonBool1;
    recordBool2 = buttonBool2;
    recordBool3 = buttonBool3;
    openStory = false;
    this->setTouchEnabled(true);
}

void StoryScene::_503CallBack(CCObject* pSender){
    LOADING->remove();
    
    CCDictionary* extra = (CCDictionary* )pSender;
    CCInteger* goldIndex = (CCInteger* )extra->objectForKey("pass_reward");
    StorySettlementOfTheAnimationLayer* layer = StorySettlementOfTheAnimationLayer::create_with_index(storyIndex, endingStr->getCString(), goldIndex->getValue());
    this->addChild(layer, 1000);
}


bool StoryScene::judge_biaoqing(DialogItem* dialItem, int index){
    if (index == 1) {
        if (dialogItem->getBiaoqing_1() == 0) {
            return false;
        }else{
            return true;
        }
    }else if (index == 2){
        if (dialogItem->getBiaoqing_2() == 0) {
            return false;
        }else{
            return true;
        }
    }else{
        return false;
    }
}
// 表情
void StoryScene::init_biaoqing(DialogItem* dialItem, int index, CCSprite* spr){
    int bqIndex = 0;
    if (index == 1) {
        bqIndex = dialItem->getBiaoqing_1();
    }else if (index == 2){
        bqIndex = dialItem->getBiaoqing_2();
    }
    
    if (bqIndex == 20) {
        this->creat_Animation20(dialItem, index, spr);
    }else if (bqIndex == 21){
        this->creat_Animation21(dialItem, index, spr);
    }else if (bqIndex == 22){
        this->creat_Animation22(dialItem, index, spr);
    }else if (bqIndex == 30){
        this->creat_Animation30(dialItem, index, spr);
    }else if (bqIndex == 50){
        this->creat_Animation50(dialItem, index, spr);
    }else if (bqIndex == 60){
        this->creat_Animation60(dialItem, index, spr);
    }else if (bqIndex == 70){
        this->creat_Animation70(dialItem, index, spr);
    }else if (bqIndex == 71){
        this->creat_Animation71(dialItem, index, spr);
    }else if (bqIndex == 72){
        this->creat_Animation72(dialItem, index, spr);
    }else if (bqIndex == 73){
        this->creat_Animation73(dialItem, index, spr);
    }else if (bqIndex == 74){
        this->creat_Animation74(dialItem, index, spr);
    }else if (bqIndex == 75){
        this->creat_Animation75(dialItem, index, spr);
    }else if (bqIndex == 76){
        this->creat_Animation76(dialItem, index, spr);
    }else if (bqIndex == 77){
        this->creat_Animation77(dialItem, index, spr);
    }
}

void StoryScene::creat_Animation20(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* qipaoSpr = CCSprite::create("res/pic/qingjingScene/biaoqing/qj_qipao.png");
    if (manIndex == 101) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .74f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .74f));
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .87f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .375f, spr->getContentSize().height* .87f));
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .678f, spr->getContentSize().height* .672f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .362f, spr->getContentSize().height* .672f));
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .75f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .346f, spr->getContentSize().height* .75f));
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .69f, spr->getContentSize().height* .8f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .8f));
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .635f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .365f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1007) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .84f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .84f));
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .672f, spr->getContentSize().height* .65f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .378f, spr->getContentSize().height* .65f));
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .685f, spr->getContentSize().height* .77f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .77f));
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .615f, spr->getContentSize().height* .665f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .32f, spr->getContentSize().height* .665f));
        }
    }else if (manIndex == 1015){
        qipaoSpr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .675f, spr->getContentSize().height* .695f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .695f));
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .625f, spr->getContentSize().height* .88f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .325f, spr->getContentSize().height* .88f));
        }
    }
    spr->addChild(qipaoSpr);
    
    if (buttonBool1) {// 快进中
        CCSprite* _20Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/20.png");
        if (index == 1) {
            _20Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .55f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _20Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_20Spr);
    }else{
        CCSprite* _20Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/20.png");
        if (index == 1) {
            _20Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .55f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _20Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        _20Spr->setScale(.2f);
        qipaoSpr->addChild(_20Spr);
        
        CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.2f);
        CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, 1.f);
        CCScaleTo* scaleTo3 = CCScaleTo::create(.2f, 1.2f);
        CCScaleTo* scaleTo4 = CCScaleTo::create(.1f, 1.f);
        CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(scaleTo1, scaleTo2, scaleTo3, scaleTo4, CCDelayTime::create(.3f), NULL));
        _20Spr->runAction(rep);
    }
}
void StoryScene::creat_Animation21(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* qipaoSpr = CCSprite::create("res/pic/qingjingScene/biaoqing/qj_qipao.png");
    if (manIndex == 101) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .74f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .74f));
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .87f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .375f, spr->getContentSize().height* .87f));
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .678f, spr->getContentSize().height* .672f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .362f, spr->getContentSize().height* .672f));
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .75f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .346f, spr->getContentSize().height* .75f));
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .69f, spr->getContentSize().height* .8f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .8f));
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .635f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .365f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1007) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .84f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .84f));
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .672f, spr->getContentSize().height* .65f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .378f, spr->getContentSize().height* .65f));
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .685f, spr->getContentSize().height* .77f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .77f));
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .615f, spr->getContentSize().height* .665f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .32f, spr->getContentSize().height* .665f));
        }
    }else if (manIndex == 1015){
        qipaoSpr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .675f, spr->getContentSize().height* .695f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .695f));
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .625f, spr->getContentSize().height* .88f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .325f, spr->getContentSize().height* .88f));
        }
    }
    spr->addChild(qipaoSpr);
    
    if (buttonBool1) {// 快进中
        CCSprite* _21Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/21.png");
        if (index == 1) {
            _21Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .55f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _21Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_21Spr);
    }else{
        CCSprite* _21Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/21.png");
        if (index == 1) {
            _21Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .55f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _21Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_21Spr);
        
        CCRotateTo* rotateTo1 = CCRotateTo::create(.2f, -20);
        CCRotateTo* rotateTo2 = CCRotateTo::create(.1f, -15);
        CCRotateTo* rotateTo3 = CCRotateTo::create(.1f, -20);
        CCRotateTo* rotateTo4 = CCRotateTo::create(.2f, 10);
        CCRotateTo* rotateTo5 = CCRotateTo::create(.1f, 5);
        CCRotateTo* rotateTo6 = CCRotateTo::create(.1f, 10);
        CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(rotateTo1, rotateTo2, rotateTo3, CCDelayTime::create(.2f), rotateTo4, rotateTo5, rotateTo6, CCDelayTime::create(.2f), NULL));
        _21Spr->runAction(rep);
    }
}
void StoryScene::creat_Animation22(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* qipaoSpr = CCSprite::create("res/pic/qingjingScene/biaoqing/qj_qipao.png");
    CCSprite* _22Spr2 = CCSprite::create("res/pic/qingjingScene/biaoqing/22.png");
    if (manIndex == 101) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .74f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .74f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-8);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .56f, spr->getContentSize().height* .78f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(8);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .45f, spr->getContentSize().height* .78f));
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .87f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .375f, spr->getContentSize().height* .87f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-10);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .58f, spr->getContentSize().height* .92f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(10);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .92f));
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .678f, spr->getContentSize().height* .672f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .362f, spr->getContentSize().height* .672f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-8);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .59f, spr->getContentSize().height* .71f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(18);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .475f, spr->getContentSize().height* .72f));
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .75f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .346f, spr->getContentSize().height* .75f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-7);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .55f, spr->getContentSize().height* .79f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(7);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .44f, spr->getContentSize().height* .79f));
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .69f, spr->getContentSize().height* .8f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .8f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-9);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .59f, spr->getContentSize().height* .85f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(9);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .85f));
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .635f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .365f, spr->getContentSize().height* .735f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-8);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .55f, spr->getContentSize().height* .76f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(23);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .762f));
        }
    }else if (manIndex == 1007){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .84f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .84f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-12);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .558f, spr->getContentSize().height* .877f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(15);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .485f, spr->getContentSize().height* .875f));
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .672f, spr->getContentSize().height* .65f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .378f, spr->getContentSize().height* .65f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-10);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .58f, spr->getContentSize().height* .68f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(12);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .68f));
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .685f, spr->getContentSize().height* .77f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .77f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-5);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .58f, spr->getContentSize().height* .8f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(14);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .475f, spr->getContentSize().height* .81f));
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .615f, spr->getContentSize().height* .665f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .32f, spr->getContentSize().height* .665f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-5);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .55f, spr->getContentSize().height* .68f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(18);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .44f, spr->getContentSize().height* .695f));
        }
    }else if (manIndex == 1015){
        qipaoSpr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
        _22Spr2->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .675f, spr->getContentSize().height* .695f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .695f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-5);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .59f, spr->getContentSize().height* .71f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(23);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .725f));
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .735f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-5);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .57f, spr->getContentSize().height* .76f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(23);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .46f, spr->getContentSize().height* .77f));
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .625f, spr->getContentSize().height* .88f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .325f, spr->getContentSize().height* .88f));
        }
        
        if (index == 1) {
            _22Spr2->setRotation(-3);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .905f));
        }else if (index == 2) {
            _22Spr2->setFlipX(true);
            _22Spr2->setRotation(15);
            _22Spr2->setPosition(ccp(spr->getContentSize().width* .43f, spr->getContentSize().height* .905f));
        }
    }
    spr->addChild(qipaoSpr);
    
    if (buttonBool1) {// 快进中
        CCSprite* _22Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/21.png");
        if (index == 1) {
            _22Spr1->setPosition(ccp(qipaoSpr->getContentSize().width* .55f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _22Spr1->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_22Spr1);
        
        spr->addChild(_22Spr2);
    }else{
        _22Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/21.png");
        if (index == 1) {
            _22Spr1->setPosition(ccp(qipaoSpr->getContentSize().width* .55f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _22Spr1->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->setVisible(false);
        qipaoSpr->addChild(_22Spr1);
        
        _22Spr2->setVisible(false);
        spr->addChild(_22Spr2);
        
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(StoryScene::_22CallFunc));
        _22Spr2->runAction(CCSequence::create(CCShow::create(), CCDelayTime::create(.1f), CCHide::create(), CCDelayTime::create(.1f), CCShow::create(), CCDelayTime::create(.1f), CCHide::create(), CCDelayTime::create(.1f), CCShow::create(), NULL));
        qipaoSpr->runAction(CCSequence::create(CCDelayTime::create(.8f), CCShow::create(), callFunc, NULL));
    }
}
void StoryScene::_22CallFunc(){
    
    CCRotateTo* rotateTo1 = CCRotateTo::create(.2f, -20);
    CCRotateTo* rotateTo2 = CCRotateTo::create(.1f, -15);
    CCRotateTo* rotateTo3 = CCRotateTo::create(.1f, -20);
    CCRotateTo* rotateTo4 = CCRotateTo::create(.2f, 10);
    CCRotateTo* rotateTo5 = CCRotateTo::create(.1f, 5);
    CCRotateTo* rotateTo6 = CCRotateTo::create(.1f, 10);
    CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(rotateTo1, rotateTo2, rotateTo3, CCDelayTime::create(.2f), rotateTo4, rotateTo5, rotateTo6, CCDelayTime::create(.2f), NULL));
    _22Spr1->runAction(rep);
}
void StoryScene::creat_Animation30(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    if (buttonBool1) {// 快进中
        CCSprite* _30Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/30_1.png");
        CCSprite* _30Spr2 = CCSprite::create("res/pic/qingjingScene/biaoqing/30_2.png");
        CCSprite* _30Spr3 = CCSprite::create("res/pic/qingjingScene/biaoqing/30_3.png");
        if (manIndex == 101) {
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .67f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .67f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .67f));
        }else if (manIndex == 1002) {
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .82f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .82f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .82f));
        }else if (manIndex == 1003){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .62f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .62f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .62f));
        }else if (manIndex == 1004){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .69f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .69f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .69f));
        }else if (manIndex == 1005){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .74f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .74f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .74f));
        }else if (manIndex == 1006){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .49f, spr->getContentSize().height* .67f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .49f, spr->getContentSize().height* .67f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .49f, spr->getContentSize().height* .67f));
        }else if (manIndex == 1007){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .77f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .77f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .77f));
        }else if (manIndex == 1008){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .58f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .58f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .58f));
        }else if (manIndex == 1009){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .7f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .7f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .7f));
        }else if (manIndex == 1010){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .58f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .58f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .58f));
        }else if (manIndex == 1015){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
        }else if (manIndex == 1020){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .61f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .61f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .61f));
        }else if (manIndex == 1021){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .65f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .65f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .65f));
        }else if (manIndex == 1102){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .465f, spr->getContentSize().height* .79f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .465f, spr->getContentSize().height* .79f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .465f, spr->getContentSize().height* .79f));
        }
        spr->addChild(_30Spr1);
        spr->addChild(_30Spr2);
        spr->addChild(_30Spr3);
    }else{
        CCSprite* _30Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/30_1.png");
        _30Spr2 = CCSprite::create("res/pic/qingjingScene/biaoqing/30_2.png");
        CCSprite* _30Spr3 = CCSprite::create("res/pic/qingjingScene/biaoqing/30_3.png");
        if (manIndex == 101) {
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .67f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .67f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .67f));
        }else if (manIndex == 1002) {
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .82f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .82f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .82f));
        }else if (manIndex == 1003){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .62f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .62f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .62f));
        }else if (manIndex == 1004){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .69f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .69f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .69f));
        }else if (manIndex == 1005){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .74f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .74f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .74f));
        }else if (manIndex == 1006){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .49f, spr->getContentSize().height* .67f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .49f, spr->getContentSize().height* .67f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .49f, spr->getContentSize().height* .67f));
        }else if (manIndex == 1007){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .77f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .77f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .77f));
        }else if (manIndex == 1008){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .58f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .58f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .58f));
        }else if (manIndex == 1009){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .7f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .7f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .7f));
        }else if (manIndex == 1010){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .58f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .58f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .58f));
        }else if (manIndex == 1015){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
        }else if (manIndex == 1020){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .61f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .61f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .61f));
        }else if (manIndex == 1021){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .65f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .65f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .65f));
        }else if (manIndex == 1102){
            _30Spr1->setPosition(ccp(spr->getContentSize().width* .465f, spr->getContentSize().height* .79f));
            _30Spr2->setPosition(ccp(spr->getContentSize().width* .465f, spr->getContentSize().height* .79f));
            _30Spr3->setPosition(ccp(spr->getContentSize().width* .465f, spr->getContentSize().height* .79f));
        }
        _30Spr1->setVisible(false);
        _30Spr2->setVisible(false);
        _30Spr3->setVisible(false);
        spr->addChild(_30Spr1);
        spr->addChild(_30Spr2);
        spr->addChild(_30Spr3);
        
        _30Spr1->runAction(CCSequence::create(CCDelayTime::create(.4f), CCShow::create(), NULL));
        _30Spr3->runAction(CCSequence::create(CCDelayTime::create(.6f), CCShow::create(), NULL));
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(StoryScene::_30CallFunc));
        _30Spr2->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCShow::create(), callFunc, NULL));
    }
}
void StoryScene::_30CallFunc(){
    CCScaleTo* scaleTo1 = CCScaleTo::create(.1f, .96f);
    CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, 1.f);
    CCScaleTo* scaleTo3 = CCScaleTo::create(.1f, .96f);
    CCScaleTo* scaleTo4 = CCScaleTo::create(.1f, 1.f);
    CCScaleTo* scaleTo5 = CCScaleTo::create(.1f, .96f);
    CCScaleTo* scaleTo6 = CCScaleTo::create(.1f, 1.f);
    CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(scaleTo1, scaleTo2, scaleTo3, scaleTo4, scaleTo5, scaleTo6, CCDelayTime::create(.6f), NULL));
    _30Spr2->runAction(rep);
}
void StoryScene::creat_Animation50(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* _50Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/50.png");
    CCSprite* _50Spr2 = CCSprite::create("res/pic/qingjingScene/biaoqing/50.png");
    if (manIndex == 101) {
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .476f, spr->getContentSize().height* .642f));
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .547f, spr->getContentSize().height* .637f));
    }else if (manIndex == 1002) {
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .481f, spr->getContentSize().height* .758f));
        _50Spr1->setRotation(2);
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .558f, spr->getContentSize().height* .753f));
        _50Spr1->setRotation(5);
    }else if (manIndex == 1003){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .587f));
        _50Spr1->setRotation(20);
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .554f, spr->getContentSize().height* .568f));
        _50Spr2->setRotation(15);
    }else if (manIndex == 1004){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .475f, spr->getContentSize().height* .65f));
        _50Spr1->setRotation(15);
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .543f, spr->getContentSize().height* .635f));
        _50Spr2->setScaleX(.5f);
    }else if (manIndex == 1005){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .537f, spr->getContentSize().height* .72f));
        _50Spr1->setRotation(15);
        _50Spr2->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1006){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .462f, spr->getContentSize().height* .62f));
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .543f, spr->getContentSize().height* .62f));
    }else if (manIndex == 1007){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .485f, spr->getContentSize().height* .74f));
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .555f, spr->getContentSize().height* .74f));
    }else if (manIndex == 1008){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .477f, spr->getContentSize().height* .555f));
        _50Spr1->setRotation(15);
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .559f, spr->getContentSize().height* .54f));
        _50Spr2->setRotation(15);
    }else if (manIndex == 1009){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .468f, spr->getContentSize().height* .668f));
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .543f, spr->getContentSize().height* .668f));
    }else if (manIndex == 1010){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .495f, spr->getContentSize().height* .538f));
        _50Spr1->setRotation(-30);
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .555f, spr->getContentSize().height* .577f));
        _50Spr2->setScaleX(.5f);
        _50Spr2->setRotation(-25);
    }else if (manIndex == 1015){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
        _50Spr2->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .454f, spr->getContentSize().height* .549f));
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .548f, spr->getContentSize().height* .55f));
    }else if (manIndex == 1021){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .455f, spr->getContentSize().height* .575f));
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .553f, spr->getContentSize().height* .575f));
    }else if (manIndex == 1102){
        _50Spr1->setPosition(ccp(spr->getContentSize().width* .475f, spr->getContentSize().height* .747f));
        _50Spr2->setPosition(ccp(spr->getContentSize().width* .542f, spr->getContentSize().height* .759f));
        _50Spr2->setScaleX(.6f);
        _50Spr2->setRotation(-20);
    }
    
    
    if (buttonBool1) {// 快进中
        _50Spr1->setOpacity(177);
        spr->addChild(_50Spr1);
        _50Spr2->setOpacity(177);
        spr->addChild(_50Spr2);
        
    }else{
        _50Spr1->setOpacity(0);
        spr->addChild(_50Spr1);
        _50Spr2->setOpacity(0);
        spr->addChild(_50Spr2);
        
        CCFadeTo* fadeTo1_1 = CCFadeTo::create(1.5f, 177);
        CCFadeTo* fadeTo2_1 = CCFadeTo::create(1.5f, 177);
        _50Spr1->runAction(CCSequence::create(CCDelayTime::create(.5f), fadeTo1_1, NULL));
        _50Spr2->runAction(CCSequence::create(CCDelayTime::create(.5f), fadeTo2_1, NULL));
    }
}
void StoryScene::creat_Animation60(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* qipaoSpr = CCSprite::create("res/pic/qingjingScene/biaoqing/qj_qipao.png");
    if (manIndex == 101) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .74f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .74f));
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .87f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .375f, spr->getContentSize().height* .87f));
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .678f, spr->getContentSize().height* .672f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .362f, spr->getContentSize().height* .672f));
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .75f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .346f, spr->getContentSize().height* .75f));
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .69f, spr->getContentSize().height* .8f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .8f));
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .635f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .365f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1007) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .84f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .84f));
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .672f, spr->getContentSize().height* .65f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .378f, spr->getContentSize().height* .65f));
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .685f, spr->getContentSize().height* .77f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .77f));
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .615f, spr->getContentSize().height* .665f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .32f, spr->getContentSize().height* .665f));
        }
    }else if (manIndex == 1015){
        qipaoSpr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .675f, spr->getContentSize().height* .695f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .695f));
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .625f, spr->getContentSize().height* .88f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .325f, spr->getContentSize().height* .88f));
        }
    }
    spr->addChild(qipaoSpr);
    
    if (buttonBool1) {// 快进中
        CCSprite* _60Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/60.png");
        if (index == 1) {
            _60Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .555f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _60Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_60Spr);
    }else{
        CCSprite* _60Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/60.png");
        if (index == 1) {
            _60Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .555f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _60Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_60Spr);
        
        CCRotateTo* rotateTo1 = CCRotateTo::create(.2f, -20);
        CCRotateTo* rotateTo2 = CCRotateTo::create(.1f, -15);
        CCRotateTo* rotateTo3 = CCRotateTo::create(.1f, -20);
        CCRotateTo* rotateTo4 = CCRotateTo::create(.2f, 10);
        CCRotateTo* rotateTo5 = CCRotateTo::create(.1f, 5);
        CCRotateTo* rotateTo6 = CCRotateTo::create(.1f, 10);
        CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(rotateTo1, rotateTo2, rotateTo3, CCDelayTime::create(.2f), rotateTo4, rotateTo5, rotateTo6, CCDelayTime::create(.2f), NULL));
        _60Spr->runAction(rep);
    }
}
void StoryScene::creat_Animation70(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        animation77Index = 1;
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        animation77Index = 2;
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* _70Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/70_2.png");
    CCSprite* _70Spr2 = CCSprite::create("res/pic/qingjingScene/biaoqing/70_3.png");
    CCProgressTimer* _70Progress = CCProgressTimer::create(_70Spr2);
    if (manIndex == 101) {
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .71f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .7f));
    }else if (manIndex == 1002) {
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .84f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .84f));
    }else if (manIndex == 1003){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .64f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .64f));
    }else if (manIndex == 1004){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .725f));
        _70Spr1->setRotation(5);
        _70Progress->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .71f));
        _70Progress->setRotation(5);
    }else if (manIndex == 1005){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .77f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .77f));
    }else if (manIndex == 1006){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .7f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .49f, spr->getContentSize().height* .69f));
    }else if (manIndex == 1007){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .8f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .8f));
    }else if (manIndex == 1008){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .61f));
        _70Spr1->setRotation(10);
        _70Progress->setPosition(ccp(spr->getContentSize().width* .535f, spr->getContentSize().height* .59f));
        _70Progress->setRotation(10);
    }else if (manIndex == 1009){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .73f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .73f));
    }else if (manIndex == 1010){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .61f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .61f));
    }else if (manIndex == 1015){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .65f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .64f));
    }else if (manIndex == 1021){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .69f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .69f));
    }else if (manIndex == 1102){
        _70Spr1->setPosition(ccp(spr->getContentSize().width* .46f, spr->getContentSize().height* .83f));
        _70Progress->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .83f));
    }
    spr->addChild(_70Spr1);
    spr->addChild(_70Progress);
    
    if (buttonBool1) {// 快进中
        
    }else{
        CCArray* animations = CCArray::createWithCapacity(5);
        char strPei[100] = {};
        for (int i = 1; i <= 2; i++) {
            
            sprintf(strPei, "res/pic/qingjingScene/biaoqing/70_%d.png", i);
            CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 182, 84));
            animations->addObject(frame);
        }
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .5f);
        CCAnimate* animate = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(animate, NULL);
        CCDirector::sharedDirector()->getActionManager()->addAction(seq, _70Spr1, false);
        
        
        //设置进度条的模式
        //kCCProgressTimerTypeBar表示条形模式
        _70Progress->setType(kCCProgressTimerTypeBar);
        //设置进度条变化的方向
        //ccp(0,1)表示沿着y轴变化
        _70Progress->setBarChangeRate(ccp(0, 1));
        //设置进度条的起始位置
        //ccp(0,0)表示下面
        _70Progress->setMidpoint(ccp(0, 1));
        //设置进度条的ID
        _70Progress->setTag(100);
        
        //创建一个定时器
        this->schedule(SEL_SCHEDULE(&StoryScene::_70CallFunc), .01f);
    }
}
void StoryScene::_70CallFunc(){
    CCNode* node;
    if (animation77Index == 1) {
        node = this->getChildByTag(Tag_GJ_head1);
    }else if (animation77Index == 2){
        node = this->getChildByTag(Tag_GJ_head2);
    }
    
    //通过进度条的ID得到进度条
    CCProgressTimer* progress = (CCProgressTimer*)node->getChildByTag(100);
    
    //设置progress的进度，每调用一次进度加一
    progress->setPercentage(progress->getPercentage() + 3);
    
    //当进度大于或者等于100时
    if(progress->getPercentage() >= 100)
    {
        //终止定时器
        this->unschedule(SEL_SCHEDULE(&StoryScene::_70CallFunc));
    }
}
void StoryScene::creat_Animation71(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* _71Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/71_1.png");
    if (manIndex == 101) {
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .63f, spr->getContentSize().height* .76f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .76f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .89f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .89f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .69f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .69f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .63f, spr->getContentSize().height* .77f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .35f, spr->getContentSize().height* .77f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .82f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .39f, spr->getContentSize().height* .82f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .61f, spr->getContentSize().height* .75f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .75f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1007){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .85f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .39f, spr->getContentSize().height* .85f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .67f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .41f, spr->getContentSize().height* .67f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .78f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .78f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .61f, spr->getContentSize().height* .67f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .335f, spr->getContentSize().height* .67f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1015){
        _71Spr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .71f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .71f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .75f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .75f));
            _71Spr->setRotation(-10);
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            _71Spr->setPosition(ccp(spr->getContentSize().width* .62f, spr->getContentSize().height* .89f));
            _71Spr->setRotation(40);
        }else if (index == 2){
            _71Spr->setPosition(ccp(spr->getContentSize().width* .34f, spr->getContentSize().height* .89f));
            _71Spr->setRotation(-10);
        }
    }
    spr->addChild(_71Spr);
    
    if (buttonBool1) {// 快进中
        CCSprite* _71Spr2 = CCSprite::create("res/pic/qingjingScene/biaoqing/71_2.png");
        _71Spr2->setAnchorPoint(CCPointZero);
        _71Spr2->setPosition(CCPointZero);
        _71Spr->addChild(_71Spr2);
    }else{
        CCSprite* _71Spr2 = CCSprite::create("res/pic/qingjingScene/biaoqing/71_2.png");
        _71Spr2->setAnchorPoint(CCPointZero);
        _71Spr2->setPosition(CCPointZero);
        _71Spr2->setVisible(false);
        _71Spr->addChild(_71Spr2);
        
        CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(CCShow::create(), CCDelayTime::create(.4f), CCHide::create(), CCDelayTime::create(.2f), NULL));
        _71Spr2->runAction(rep);
    }
}
void StoryScene::creat_Animation72(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* _72Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/72_5.png");
    if (manIndex == 101) {
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .62f, spr->getContentSize().height* .76f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .39f, spr->getContentSize().height* .76f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .89f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .4f, spr->getContentSize().height* .89f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .64f, spr->getContentSize().height* .69f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .69f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .62f, spr->getContentSize().height* .77f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .77f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .82f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .41f, spr->getContentSize().height* .82f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .6f, spr->getContentSize().height* .75f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .39f, spr->getContentSize().height* .75f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1007){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .63f, spr->getContentSize().height* .85f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .41f, spr->getContentSize().height* .85f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .63f, spr->getContentSize().height* .67f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .42f, spr->getContentSize().height* .67f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .78f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .39f, spr->getContentSize().height* .78f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .6f, spr->getContentSize().height* .67f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .35f, spr->getContentSize().height* .67f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1015){
        _72Spr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .64f, spr->getContentSize().height* .71f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .71f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .64f, spr->getContentSize().height* .75f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .39f, spr->getContentSize().height* .75f));
            _72Spr->setRotation(5);
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            _72Spr->setPosition(ccp(spr->getContentSize().width* .61f, spr->getContentSize().height* .89f));
            _72Spr->setRotation(-3);
        }else if (index == 2){
            _72Spr->setFlipX(true);
            _72Spr->setPosition(ccp(spr->getContentSize().width* .34f, spr->getContentSize().height* .89f));
            _72Spr->setRotation(5);
        }
    }
    spr->addChild(_72Spr);
    
    if (buttonBool1) {// 快进中
        
    }else{
        CCArray* animations = CCArray::createWithCapacity(10);
        char strPei[100] = {};
        for (int i = 1; i <= 6; i++) {
            
            sprintf(strPei, "res/pic/qingjingScene/biaoqing/72_%d.png", i);
            CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 54, 75));
            animations->addObject(frame);
        }
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .3f);
        CCAnimate* animate = CCAnimate::create(animation);
        CCRepeatForever* rep = CCRepeatForever::create(animate);
        CCDirector::sharedDirector()->getActionManager()->addAction(rep, _72Spr, false);
    }
}
void StoryScene::creat_Animation73(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* qipaoSpr = CCSprite::create("res/pic/qingjingScene/biaoqing/qj_qipao.png");
    if (manIndex == 101) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .74f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .74f));
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .87f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .375f, spr->getContentSize().height* .87f));
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .678f, spr->getContentSize().height* .672f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .362f, spr->getContentSize().height* .672f));
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .75f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .346f, spr->getContentSize().height* .75f));
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .69f, spr->getContentSize().height* .8f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .8f));
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .635f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .365f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1007) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .84f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .84f));
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .672f, spr->getContentSize().height* .65f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .378f, spr->getContentSize().height* .65f));
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .685f, spr->getContentSize().height* .77f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .77f));
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .615f, spr->getContentSize().height* .665f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .32f, spr->getContentSize().height* .665f));
        }
    }else if (manIndex == 1015){
        qipaoSpr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .675f, spr->getContentSize().height* .695f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .695f));
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .625f, spr->getContentSize().height* .88f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .325f, spr->getContentSize().height* .88f));
        }
    }
    spr->addChild(qipaoSpr);
    
    if (buttonBool1) {// 快进中
        CCSprite* _73Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/73_3.png");
        _73Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .555f, qipaoSpr->getContentSize().height* .5f));
        qipaoSpr->addChild(_73Spr);
    }else{
        CCSprite* _73Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/73_3.png");
        if (index == 1) {
            _73Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .555f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _73Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_73Spr);
        
        CCArray* animations = CCArray::createWithCapacity(5);
        char strPei[100] = {};
        for (int i = 1; i <= 3; i++) {
            
            sprintf(strPei, "res/pic/qingjingScene/biaoqing/73_%d.png", i);
            CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 62, 14));
            animations->addObject(frame);
        }
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .3f);
        CCAnimate* animate = CCAnimate::create(animation);
        CCRepeatForever* rep = CCRepeatForever::create(animate);
        CCDirector::sharedDirector()->getActionManager()->addAction(rep, _73Spr, false);
    }
}
void StoryScene::creat_Animation74(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* qipaoSpr = CCSprite::create("res/pic/qingjingScene/biaoqing/qj_qipao.png");
    if (manIndex == 101) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .74f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .74f));
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .87f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .375f, spr->getContentSize().height* .87f));
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .678f, spr->getContentSize().height* .672f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .362f, spr->getContentSize().height* .672f));
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .75f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .346f, spr->getContentSize().height* .75f));
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .69f, spr->getContentSize().height* .8f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .8f));
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .635f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .365f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1007) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .84f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .84f));
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .672f, spr->getContentSize().height* .65f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .378f, spr->getContentSize().height* .65f));
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .685f, spr->getContentSize().height* .77f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .77f));
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .615f, spr->getContentSize().height* .665f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .32f, spr->getContentSize().height* .665f));
        }
    }else if (manIndex == 1015){
        qipaoSpr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .675f, spr->getContentSize().height* .695f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .695f));
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .735f));
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .625f, spr->getContentSize().height* .88f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .325f, spr->getContentSize().height* .88f));
        }
    }
    spr->addChild(qipaoSpr);
    
    if (buttonBool1) {// 快进中
        CCSprite* _74Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/74.png");
        if (index == 1) {
            _74Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .555f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _74Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_74Spr);
    }else{
        CCSprite* _74Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/74.png");
        if (index == 1) {
            _74Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .555f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _74Spr->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->addChild(_74Spr);
        
        CCRotateTo* rotateTo1 = CCRotateTo::create(.2f, -35);
        CCRotateTo* rotateTo2 = CCRotateTo::create(.1f, -30);
        CCRotateTo* rotateTo3 = CCRotateTo::create(.1f, -35);
        CCRotateTo* rotateTo4 = CCRotateTo::create(.2f, 10);
        CCRotateTo* rotateTo5 = CCRotateTo::create(.1f, 5);
        CCRotateTo* rotateTo6 = CCRotateTo::create(.1f, 10);
        CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(rotateTo1, rotateTo2, rotateTo3, CCDelayTime::create(.2f), rotateTo4, rotateTo5, rotateTo6, CCDelayTime::create(.2f), NULL));
        _74Spr->runAction(rep);
    }
}
void StoryScene::creat_Animation75(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* qipaoSpr = CCSprite::create("res/pic/qingjingScene/biaoqing/qj_qipao.png");
    CCSprite* _75Spr2 = CCSprite::create("res/pic/qingjingScene/biaoqing/75_1.png");
    if (manIndex == 101) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .74f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .74f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-13);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .768f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(11);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .455f, spr->getContentSize().height* .765f));
        }
    }else if (manIndex == 1002) {
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .87f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .375f, spr->getContentSize().height* .87f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-11);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .56f, spr->getContentSize().height* .89f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(7);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .895f));
        }
    }else if (manIndex == 1003){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .678f, spr->getContentSize().height* .672f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .362f, spr->getContentSize().height* .672f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-9);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .57f, spr->getContentSize().height* .7f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(10);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .46f, spr->getContentSize().height* .702f));
        }
    }else if (manIndex == 1004){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .65f, spr->getContentSize().height* .75f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .346f, spr->getContentSize().height* .75f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-12);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .778f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(5);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .437f, spr->getContentSize().height* .772f));
        }
    }else if (manIndex == 1005){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .69f, spr->getContentSize().height* .8f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .8f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-15);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .57f, spr->getContentSize().height* .837f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(8);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .473f, spr->getContentSize().height* .829f));
        }
    }else if (manIndex == 1006){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .635f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .365f, spr->getContentSize().height* .735f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-18);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .747f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(14);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .46f, spr->getContentSize().height* .746f));
        }
    }else if (manIndex == 1007){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .66f, spr->getContentSize().height* .84f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .38f, spr->getContentSize().height* .84f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-15);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .538f, spr->getContentSize().height* .857f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(11);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .478f, spr->getContentSize().height* .857f));
        }
    }else if (manIndex == 1008){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .672f, spr->getContentSize().height* .65f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .378f, spr->getContentSize().height* .65f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-17);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .56f, spr->getContentSize().height* .67f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(10);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .665f));
        }
    }else if (manIndex == 1009){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .685f, spr->getContentSize().height* .77f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .37f, spr->getContentSize().height* .77f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-5);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .56f, spr->getContentSize().height* .79f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(6);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .791f));
        }
    }else if (manIndex == 1010){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .615f, spr->getContentSize().height* .665f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .32f, spr->getContentSize().height* .665f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-8);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .515f, spr->getContentSize().height* .67f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(11);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .43f, spr->getContentSize().height* .674f));
        }
    }else if (manIndex == 1015){
        qipaoSpr->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
        _75Spr2->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .675f, spr->getContentSize().height* .695f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .695f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-5);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .57f, spr->getContentSize().height* .7f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(13);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .713f));
        }
    }else if (manIndex == 1021){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .735f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .36f, spr->getContentSize().height* .735f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-8);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .55f, spr->getContentSize().height* .743f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(8);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .465f, spr->getContentSize().height* .74f));
        }
    }else if (manIndex == 1102){
        if (index == 1) {
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .625f, spr->getContentSize().height* .88f));
        }else if (index == 2) {
            qipaoSpr->setFlipX(true);
            qipaoSpr->setPosition(ccp(spr->getContentSize().width* .325f, spr->getContentSize().height* .88f));
        }
        
        if (index == 1) {
            _75Spr2->setRotation(-5);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .89f));
        }else if (index == 2) {
            _75Spr2->setFlipX(true);
            _75Spr2->setRotation(6);
            _75Spr2->setPosition(ccp(spr->getContentSize().width* .425f, spr->getContentSize().height* .885f));
        }
    }
    spr->addChild(qipaoSpr);
    
    if (buttonBool1) {// 快进中
        CCSprite* _75Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/75.png");
        _75Spr1->setPosition(ccp(qipaoSpr->getContentSize().width* .555f, qipaoSpr->getContentSize().height* .5f));
        qipaoSpr->addChild(_75Spr1);
        
        spr->addChild(_75Spr2);
    }else{
        _75Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/75.png");
        if (index == 1) {
            _75Spr1->setPosition(ccp(qipaoSpr->getContentSize().width* .555f, qipaoSpr->getContentSize().height* .5f));
        }else if (index == 2){
            _75Spr1->setPosition(ccp(qipaoSpr->getContentSize().width* .44f, qipaoSpr->getContentSize().height* .5f));
        }
        qipaoSpr->setVisible(false);
        qipaoSpr->addChild(_75Spr1);
        
        _75Spr2->setVisible(false);
        spr->addChild(_75Spr2);
        
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(StoryScene::_75CallFunc));
        _75Spr2->runAction(CCSequence::create(CCShow::create(), CCDelayTime::create(.1f), CCHide::create(), CCDelayTime::create(.1f), CCShow::create(), CCDelayTime::create(.1f), CCHide::create(), CCDelayTime::create(.1f), CCShow::create(), NULL));
        qipaoSpr->runAction(CCSequence::create(CCDelayTime::create(.8f), CCShow::create(), callFunc, NULL));
    }
}
void StoryScene::_75CallFunc(){
    CCRotateTo* rotateTo1 = CCRotateTo::create(.2f, -20);
    CCRotateTo* rotateTo2 = CCRotateTo::create(.1f, -15);
    CCRotateTo* rotateTo3 = CCRotateTo::create(.1f, -20);
    CCRotateTo* rotateTo4 = CCRotateTo::create(.2f, 10);
    CCRotateTo* rotateTo5 = CCRotateTo::create(.1f, 5);
    CCRotateTo* rotateTo6 = CCRotateTo::create(.1f, 10);
    CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(rotateTo1, rotateTo2, rotateTo3, CCDelayTime::create(.2f), rotateTo4, rotateTo5, rotateTo6, CCDelayTime::create(.2f), NULL));
    _75Spr1->runAction(rep);
}

void StoryScene::creat_Animation76(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* _76Spr1 = CCSprite::create("res/pic/qingjingScene/biaoqing/70_2.png");
    if (manIndex == 101) {
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .71f));
    }else if (manIndex == 1002) {
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .84f));
    }else if (manIndex == 1003){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .64f));
    }else if (manIndex == 1004){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .725f));
        _76Spr1->setRotation(5);
    }else if (manIndex == 1005){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .77f));
    }else if (manIndex == 1006){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .7f));
    }else if (manIndex == 1007){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .8f));
    }else if (manIndex == 1008){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .54f, spr->getContentSize().height* .61f));
        _76Spr1->setRotation(10);
    }else if (manIndex == 1009){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .73f));
    }else if (manIndex == 1010){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .47f, spr->getContentSize().height* .61f));
    }else if (manIndex == 1015){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .65f));
    }else if (manIndex == 1021){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .69f));
    }else if (manIndex == 1102){
        _76Spr1->setPosition(ccp(spr->getContentSize().width* .46f, spr->getContentSize().height* .83f));
    }
    spr->addChild(_76Spr1);
    
    if (buttonBool1) {// 快进中
        
    }else{
        CCArray* animations = CCArray::createWithCapacity(5);
        char strPei[100] = {};
        for (int i = 1; i <= 2; i++) {
            
            sprintf(strPei, "res/pic/qingjingScene/biaoqing/70_%d.png", i);
            CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 182, 84));
            animations->addObject(frame);
        }
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .5f);
        CCAnimate* animate = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(animate, NULL);
        CCDirector::sharedDirector()->getActionManager()->addAction(seq, _76Spr1, false);
    }
}
void StoryScene::creat_Animation77(DialogItem* dialItem, int index, CCSprite* spr){
    CCArray* strList;
    if (index == 1) {
        animation77Index = 1;
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_1().c_str(), "_");
    }else if (index == 2){
        animation77Index = 2;
        strList = StringUtil::sharedStrUtil()->split(dialItem->getHead_2().c_str(), "_");
    }
    CCString* str = (CCString* )strList->objectAtIndex(0);
    int manIndex = atoi(str->getCString());
    
    CCSprite* _77Spr = CCSprite::create("res/pic/qingjingScene/biaoqing/70_3.png");
    CCProgressTimer* _77Progress = CCProgressTimer::create(_77Spr);
    if (manIndex == 101) {
        _77Progress->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .7f));
    }else if (manIndex == 1002) {
        _77Progress->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .84f));
    }else if (manIndex == 1003){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .64f));
    }else if (manIndex == 1004){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .71f));
        _77Progress->setRotation(5);
    }else if (manIndex == 1005){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .53f, spr->getContentSize().height* .77f));
    }else if (manIndex == 1006){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .49f, spr->getContentSize().height* .69f));
    }else if (manIndex == 1007){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .8f));
    }else if (manIndex == 1008){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .535f, spr->getContentSize().height* .59f));
        _77Progress->setRotation(10);
    }else if (manIndex == 1009){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .51f, spr->getContentSize().height* .73f));
    }else if (manIndex == 1010){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .61f));
    }else if (manIndex == 1015){
        _77Progress->setPosition(ccp(spr->getContentSize().width* 3.f, spr->getContentSize().height* 3.f));
    }else if (manIndex == 1020){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .64f));
    }else if (manIndex == 1021){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .52f, spr->getContentSize().height* .69f));
    }else if (manIndex == 1102){
        _77Progress->setPosition(ccp(spr->getContentSize().width* .48f, spr->getContentSize().height* .83f));
    }
    _77Progress->setTag(77);
    spr->addChild(_77Progress);
    
    if (buttonBool1) {// 快进中
        
    }else{
        //设置进度条的模式
        //kCCProgressTimerTypeBar表示条形模式
        _77Progress->setType(kCCProgressTimerTypeBar);
        //设置进度条变化的方向
        //ccp(0,1)表示沿着y轴变化
        _77Progress->setBarChangeRate(ccp(0, 1));
        //设置进度条的起始位置
        //ccp(0,0)表示下面
        _77Progress->setMidpoint(ccp(0, 1));
        //设置进度条的ID
        _77Progress->setTag(100);
        
        //创建一个定时器
        this->schedule(SEL_SCHEDULE(&StoryScene::_70CallFunc), .01f);
    }
}



CCArray* StoryScene::split(const char* srcStr,const char* sSep)
{
    CCArray* stringArr = CCArray::create();
    
    CCString* str = CCString::create(srcStr);
    
    int size = strlen(srcStr);
    
    int istartpos = 0;
    int iendpos   = 0;
    iendpos = str->m_sString.find(sSep);
    
    CCString *strSingle = NULL;
    
    /*将分割的字符串添加到容器里面*/
    while (iendpos>0)
    {
        strSingle = CCString::create("");
        
        strSingle->m_sString = str->m_sString.substr(istartpos,iendpos);
        
        stringArr->addObject(strSingle);
        
        str->m_sString = str->m_sString.substr(iendpos+1,size);
        
        iendpos = str->m_sString.find(sSep);
        
        size = str->m_sString.size();
    }
    
    if (str->m_sString.compare("")!=0)
    {
        stringArr->addObject(CCString::create(str->m_sString));
    }  
    
    return stringArr;  
}





