//
//  PhoneLayer.cpp
//  tiegao
//
//  Created by mac on 16-7-18.
//
//

#include "PhoneLayer.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "TaskScene.h"
#include "AppUtil.h"
#include "SpecialManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "QingjingScene.h"


PhoneLayer::PhoneLayer(){
    
}
PhoneLayer::~PhoneLayer(){
    
}

void PhoneLayer::onEnter(){
    BaseScene::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&PhoneLayer::_500CallBack), "HTTP_FINISHED_500", NULL);
}
void PhoneLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    BaseScene::onExit();
}

bool PhoneLayer::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    wordCount = 0;
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    CCArray* animations = CCArray::createWithCapacity(10);
    char strPei[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(strPei, "res/pic/qingjingScene/phone/qj_dian%d.png", i);
        CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 58, 5));
        animations->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .5f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "dianStr");
    
    this->creat_view();
    this->creat_man();
    
    return true;
}

void PhoneLayer::creat_view(){
    
    CCSprite* bgSpr = CCSprite::create("res/pic/taskScene/task_bg.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    CCString* str = CCString::createWithFormat("res/pic/taskScene/task_building_%d.png", DATA->getPlayer()->phase);
    CCSprite* fzSpr = CCSprite::create(str->getCString());
    fzSpr->setAnchorPoint(CCPointZero);
    fzSpr->setPosition(ccp(0, DISPLAY->H() * 0.22));
    this->addChild(fzSpr, 5);
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/taskScene/task_back.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/taskScene/task_back.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(PhoneLayer::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 20);
    
    CCSprite* tiaoSpr = CCSprite::create("res/pic/qingjingScene/phone/qj_tiao.png");
    tiaoSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .86f));
    this->addChild(tiaoSpr, 20);
    
    CCSprite* phoneSpr = CCSprite::create("res/pic/taskScene/animation/task_phone.png");
    phoneSpr->setAnchorPoint(ccp(.5f, 0));
    phoneSpr->setPosition(ccp(-phoneSpr->getContentSize().width, 0));
    tiaoSpr->addChild(phoneSpr);
    CCMoveTo* moveTo1 = CCMoveTo::create(.2f, ccp(-phoneSpr->getContentSize().width, 0 + 5));
    CCMoveTo* moveTo2 = CCMoveTo::create(.1f, ccp(-phoneSpr->getContentSize().width, 0));
    phoneSpr->runAction(CCRepeatForever::create(CCSequence::create(moveTo1, moveTo2, CCDelayTime::create(.1f), NULL)));
    
    CCLabelTTF* label = CCLabelTTF::create("来电中", DISPLAY->fangzhengFont(), 25);
    label->setAnchorPoint(ccp(0, 0));
    label->setPosition(ccp(tiaoSpr->getContentSize().width* .35f, 5));
    label->setColor(ccc3(232, 136, 174));
    tiaoSpr->addChild(label, 10);
    
    CCSprite* dianSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_act1.png");
    dianSpr->setAnchorPoint(CCPointZero);
    dianSpr->setPosition(ccp(tiaoSpr->getContentSize().width* .35f + label->getContentSize().width + 5, 10));
    tiaoSpr->addChild(dianSpr, 10);
    CCAnimation* manAnimation1 = CCAnimationCache::sharedAnimationCache()->animationByName("dianStr");
    CCAnimate* animate1 = CCAnimate::create(manAnimation1);
    CCRepeatForever* rep1 = CCRepeatForever::create(animate1);
    CCDirector::sharedDirector()->getActionManager()->addAction(rep1, dianSpr, false);
    
    
    CCSprite* kuangSpr = CCSprite::create("res/pic/qingjingScene/phone/qj_kuang.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .4f));
    this->addChild(kuangSpr, 50);
    saidLabel = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .8f, kuangSpr->getContentSize().height* .6f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    saidLabel->setAnchorPoint(CCPointZero);
    saidLabel->setPosition(ccp(kuangSpr->getContentSize().width* .1f, 25));
    saidLabel->setColor(ccc3(80, 63, 68));
    kuangSpr->addChild(saidLabel);
    
    CCSprite* buquSpr1 = CCSprite::create("res/pic/qingjingScene/phone/qj_buqu.png");
    CCSprite* buquSpr2 = CCSprite::create("res/pic/qingjingScene/phone/qj_buqu.png");
    buquSpr2->setScale(1.02f);
    CCMenuItem* buquItem = CCMenuItemSprite::create(buquSpr1, buquSpr2, this, menu_selector(PhoneLayer::buquCallBack));
    buquItem->setPosition(ccp(kuangSpr->getContentSize().width* .25f, -kuangSpr->getContentSize().height* .3f));
    
    CCSprite* quSpr1 = CCSprite::create("res/pic/qingjingScene/phone/qj_qu.png");
    CCSprite* quSpr2 = CCSprite::create("res/pic/qingjingScene/phone/qj_qu.png");
    quSpr2->setScale(1.02f);
    CCMenuItem* quItem = CCMenuItemSprite::create(quSpr1, quSpr2, this, menu_selector(PhoneLayer::quCallBack));
    quItem->setPosition(ccp(kuangSpr->getContentSize().width* .75f, -kuangSpr->getContentSize().height* .3f));
    
    CCMenu* buttonMenu = CCMenu::create(buquItem, quItem, NULL);
    buttonMenu->setPosition(CCPointZero);
    kuangSpr->addChild(buttonMenu);
}
void PhoneLayer::creat_man(){
    // 显示的任务的结局
    /**
     *   "101_80100_0"      任务id
     *   "1009_1",          人id
     *   "1009_2",          人表情
     *   "0",               需要星星数
     *   "1.png",           bg
     *   "开启—弘鼎企业",     提示语
     *   "1"                阶段
     *   "1002_1",          手机需要的人id
     *   "1002_5",          手机需要的人表情
     *   "那么一段糗事,你就不要来看了嘛.讨厌~!"     手机需要的提示语
     */
    CSJson::Value taskConditionsData = AppUtil::read_json_file("res/story/taskConditions");
    CCDictionary* taskConditionsDic = AppUtil::dictionary_with_json(taskConditionsData);
    CCString* taskConditionsKeyStr = CCString::createWithFormat("101_80100_%d", qingjingStatus());
    CCArray* taskConditionsAchievemArr = (CCArray* )taskConditionsDic->objectForKey(taskConditionsKeyStr->getCString());
    // 人
    std::string manStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(0))->getCString();
    std::string lianStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(1))->getCString();
    std::string saidStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(8))->getCString();
    
    CCString* manStr = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", manStd.c_str());
    CCSprite* manSpr = CCSprite::create(manStr->getCString());
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(manSpr, 10);
    CCString* lianStr = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", lianStd.c_str());
    CCSprite* lianSpr = CCSprite::create(lianStr->getCString());
    lianSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(lianSpr, 11);
    
    this->init(saidStd);
}

int PhoneLayer::qingjingStatus(){
    int now_task_index = -1;
    // 显示的任务的结局
    CSJson::Value taskConditionsData = AppUtil::read_json_file("res/story/taskConditions");
    CCDictionary* taskConditionsDic = AppUtil::dictionary_with_json(taskConditionsData);
    int allTask = taskConditionsDic->count();
    
    for (int i = 0; i < allTask; i++) {
        CCString* story_index = CCString::createWithFormat("%d", i);
        CCArray* storyArr = DATA->getStory()->story_achievments(story_index->getCString());
        if (storyArr != NULL) {
            continue;
        }else{
            now_task_index = i;
            break;
        }
    }
    if (now_task_index == -1) {
        now_task_index = allTask - 1;
    }
    return now_task_index;
}
void PhoneLayer::openTouch(float dt){
    logIndex = 0;
    this->setTouchEnabled(true);
}
void PhoneLayer::logic(float dt){
    openStory = false;
    
    this->unschedule(schedule_selector(PhoneLayer::logic));
    this->openTouch(0);
    
    if (wordCount > contentLength) {
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        return;
    }else{
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        this->schedule(schedule_selector(PhoneLayer::logic), .1f);
    }
}

void PhoneLayer::backCallBack(CCObject* pSender){
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void PhoneLayer::buquCallBack(CCObject* pSender){
    this->backCallBack(NULL);
}
void PhoneLayer::quCallBack(CCObject* pSender){
    if (DATA->getStory()->has_init_story()) {
        this->_500CallBack(NULL);
    }else{
        LOADING->show_loading();
        NET->completed_story_500();
    }
}
void PhoneLayer::_500CallBack(CCObject* pSender){
    CCScene* scene = QingjingScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

bool PhoneLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    SPECIAL->showSpotAt(this->getParent(), pTouch->getLocation(), 1);
    if (! openStory) {
        logIndex++;
        
        if (logIndex == 1) {
            if (wordCount < contentLength) {
                wordCount = getContentLength();
                this->setTouchEnabled(false);
            }
        }
    }
    return true;
}

void PhoneLayer::init(std::string str){
    content = str;
    CCLog("content == %s", content.c_str());
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
    
    this->schedule(schedule_selector(PhoneLayer::logic), .1f);
}
std::string PhoneLayer::getContentByLength(int length){
    if (length >= contentLength) {
        return getContent();
    }
    
    int i = 0;
    int _index = 0;
    while (_index < length) {
        char ch = getContent()[i];
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        _index++;
    }
    std::string str = content.substr(0, i);
    return str;
}
int PhoneLayer::getContentLength(){
    return contentLength;
}

