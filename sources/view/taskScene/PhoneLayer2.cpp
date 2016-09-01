//
//  PhoneLayer2.cpp
//  tiegao
//
//  Created by mac on 16-7-18.
//
//

#include "PhoneLayer2.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "TaskScene.h"
#include "AppUtil.h"
#include "SpecialManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "QingjingScene.h"
#include "GuideLayer.h"


PhoneLayer2::PhoneLayer2(){
    
}
PhoneLayer2::~PhoneLayer2(){
    
}

void PhoneLayer2::onEnter(){
    BaseScene::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&PhoneLayer2::_500CallBack), "HTTP_FINISHED_500", NULL);
    nc->addObserver(this, SEL_CallFuncO(&PhoneLayer2::quCallBack), "Guide_quCallBack", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&PhoneLayer2::keyBackStatus), .8f);
}
void PhoneLayer2::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void PhoneLayer2::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    BaseScene::onExit();
}

bool PhoneLayer2::init(){
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
    
    
    if (DATA->current_guide_step() == 9) {
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }
    
    return true;
}

void PhoneLayer2::creat_view(){
    
    CCSprite* bgSpr = CCSprite::create("res/pic/taskScene/task_bg.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    CCString* str = CCString::createWithFormat("res/pic/taskScene/task_building_%d.png", DATA->getPlayer()->phase);
    CCSprite* fzSpr = CCSprite::create(str->getCString());
    fzSpr->setAnchorPoint(CCPointZero);
    fzSpr->setPosition(ccp(0, DISPLAY->H() * 0.22));
    this->addChild(fzSpr, 2);
    
    CCSprite* maskSpr = CCSprite::create("res/pic/mask.png");
    maskSpr->setOpacity(150);
    maskSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(maskSpr, 5);
    
    CCSprite* backSpr1 = CCSprite::create("pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(PhoneLayer2::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 20);
    
    CCSprite* tiaoSpr = CCSprite::create("res/pic/qingjingScene/phone/qj_tiao.png");
    tiaoSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .86f));
    this->addChild(tiaoSpr, 20);
    
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
    
    //
    CCSprite* kuangSpr = CCSprite::create("res/pic/qingjingScene/phone/qj_kuang.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .233f));
    this->addChild(kuangSpr, 50);
    saidLabel = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .8f, kuangSpr->getContentSize().height* .6f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    saidLabel->setAnchorPoint(CCPointZero);
    saidLabel->setPosition(ccp(kuangSpr->getContentSize().width* .1f, 25));
    saidLabel->setColor(ccc3(126, 78, 78));
    kuangSpr->addChild(saidLabel);
    
    CCSprite* buquSpr1 = CCSprite::create("res/pic/qingjingScene/phone/qj_buqu.png");
    CCSprite* buquSpr2 = CCSprite::create("res/pic/qingjingScene/phone/qj_buqu.png");
    buquSpr2->setScale(1.02f);
    CCMenuItem* buquItem = CCMenuItemSprite::create(buquSpr1, buquSpr2, this, menu_selector(PhoneLayer2::buquCallBack));
    buquItem->setPosition(ccp(kuangSpr->getContentSize().width* .25f, -kuangSpr->getContentSize().height* .24f));
    
    CCSprite* quSpr1 = CCSprite::create("res/pic/qingjingScene/phone/qj_qu.png");
    CCSprite* quSpr2 = CCSprite::create("res/pic/qingjingScene/phone/qj_qu.png");
    quSpr2->setScale(1.02f);
    CCMenuItem* quItem = CCMenuItemSprite::create(quSpr1, quSpr2, this, menu_selector(PhoneLayer2::quCallBack));
    quItem->setPosition(ccp(kuangSpr->getContentSize().width* .75f, -kuangSpr->getContentSize().height* .24f));
    
    CCMenu* buttonMenu = CCMenu::create(buquItem, quItem, NULL);
    buttonMenu->setPosition(CCPointZero);
    kuangSpr->addChild(buttonMenu);
    
    
    if (DATA->current_guide_step() == 9) {
        backItem->setColor(ccGRAY);
        backItem->setEnabled(false);
        buquItem->setColor(ccGRAY);
        buquItem->setEnabled(false);
    }
}
void PhoneLayer2::creat_man(){
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
    
//    CCString* manStr = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", manStd.c_str());
//    CCString* lianStr = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", lianStd.c_str());
    
    CCString* manStr = CCString::createWithFormat("res/pic/qingjingScene/zishi/1002_1.png");
    CCString* lianStr = CCString::createWithFormat("res/pic/qingjingScene/head/1002_5.png");
    
    
    CCSprite* bottom1 = CCSprite::create("res/pic/qingjingScene/phone/qj_phone1.png");
    bottom1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .567f));
    this->addChild(bottom1, 10);
    
    CCSprite* bottom2 = CCSprite::create("res/pic/qingjingScene/phone/qj_phone2.png");
    bottom2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .567f));
    this->addChild(bottom2, 12);
    
    CCSprite* top1 = CCSprite::create(manStr->getCString());
    _progress1 = CCProgressTimer::create(top1);
    _progress1->setScale(.5f);
    _progress1->setPosition(ccp(bottom1->getContentSize().width* .5f, bottom1->getContentSize().height* .4f));
    _progress1->setType(kCCProgressTimerTypeBar);
    _progress1->setMidpoint(ccp(0, 1));
    _progress1->setBarChangeRate(ccp(0, 1));
    bottom1->addChild(_progress1, 1);
    _progress1->setPercentage(80);
    
    CCSprite* top2 = CCSprite::create(lianStr->getCString());
    _progress2 = CCProgressTimer::create(top2);
    _progress2->setScale(.5f);
    _progress2->setPosition(ccp(bottom1->getContentSize().width* .5f, bottom1->getContentSize().height* .4f));
    _progress2->setType(kCCProgressTimerTypeBar);
    _progress2->setMidpoint(ccp(0, 1));
    _progress2->setBarChangeRate(ccp(0, 1));
    bottom1->addChild(_progress2, 2);
    _progress2->setPercentage(80);
    
    this->init(saidStd);
}

int PhoneLayer2::qingjingStatus(){
    int now_task_index = -1;
    // 显示的任务的结局
    CSJson::Value taskNumberData = AppUtil::read_json_file("res/story/storyNumber");
    CCDictionary* taskNumberDic = AppUtil::dictionary_with_json(taskNumberData);
    int allTask = taskNumberDic->count();
    
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
void PhoneLayer2::openTouch(float dt){
    logIndex = 0;
    this->setTouchEnabled(true);
}
void PhoneLayer2::logic(float dt){
    openStory = false;
    
    this->unschedule(schedule_selector(PhoneLayer2::logic));
    this->openTouch(0);
    
    if (wordCount > contentLength) {
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        return;
    }else{
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        this->schedule(schedule_selector(PhoneLayer2::logic), .1f);
    }
}

void PhoneLayer2::backCallBack(CCObject* pSender){
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void PhoneLayer2::buquCallBack(CCObject* pSender){
    this->backCallBack(NULL);
}
void PhoneLayer2::quCallBack(CCObject* pSender){
    if (DATA->getStory()->has_init_story()) {
        this->_500CallBack(NULL);
    }else{
        LOADING->show_loading();
        NET->completed_story_500();
    }
}
void PhoneLayer2::_500CallBack(CCObject* pSender){
    CCScene* scene = QingjingScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

bool PhoneLayer2::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
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

void PhoneLayer2::init(std::string str){
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
    
    this->schedule(schedule_selector(PhoneLayer2::logic), .1f);
}
std::string PhoneLayer2::getContentByLength(int length){
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
int PhoneLayer2::getContentLength(){
    return contentLength;
}

void PhoneLayer2::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    if (DATA->current_guide_step() != 8) {
        this->backCallBack(NULL);
    }
}

