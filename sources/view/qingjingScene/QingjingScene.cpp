//
//  QingjingScene.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "QingjingScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "MainScene.h"
#include "StoryScene.h"
#include "ConfigManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "AudioManager.h"
#include "GuideLayer.h"
#include "WSManager.h"
#include "ChatPanel.h"



QingjingScene::QingjingScene(){
    
}
QingjingScene::~QingjingScene(){
    
}

bool QingjingScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    num_child = 0;
    
    storyIndex = 0;
    renwuIndex = 0;
    allNumber = 0;
    theEndBool = false;
    
    selectedIndex = DATA->getChapterNumber();
    
    DATA->setChapterNumber(0);
    
    CSJson::Value taskNumberData = AppUtil::read_json_file("res/story/storyNumber");
    CCDictionary* taskNumberDic = AppUtil::dictionary_with_json(taskNumberData);
    allNumber = taskNumberDic->count();
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    this->creat_view();
//    this->creat_Man();
//    this->initClothes();
    
    
    if (DATA->current_guide_step() == 9){
        if (DATA->_guideBool9[2] && !DATA->_guideBool9[3]){
            GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
            layer->setTag(0x445566);
            this->addChild(layer, 500);
        }else if (DATA->_guideBool9[5] && !DATA->_guideBool9[6]){
            GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
            layer->setTag(0x445566);
            this->addChild(layer, 500);
        }
    }
    
    return true;
}
CCScene* QingjingScene::scene(){
    CCScene* scene = CCScene::create();
    QingjingScene* layer = QingjingScene::create();
    scene->addChild(layer);
    
    return scene;
}

void QingjingScene::onEnter(){
    BaseScene::onEnter();
    
    AUDIO->play_story_bgm();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&QingjingScene::_501CallBack), "HTTP_FINISHED_501", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&QingjingScene::updataButton), "Qingjing_UpdataButton", NULL);
    nc->addObserver(this, SEL_CallFuncO(&QingjingScene::updataMan), "Qingjing_UpdataMan", NULL);
    
    
    nc->addObserver(this, SEL_CallFuncO(&QingjingScene::guide_StartCallBack), "Guide_StartCallBack", NULL);
    
//    nc->addObserver(this, SEL_CallFuncO(&QingjingScene::displayChatItem), "CLOSE_CHATPANEL", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&QingjingScene::keyBackStatus), .8f);
}

void QingjingScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void QingjingScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
//    DATA->setChatOut(true);
//    DATA->getChat()->setItems(CCArray::create());
    
    BaseScene::onExit();
}

void QingjingScene::keyBackClicked(){
    num_child++;
    CCLog("===== QingjingScene  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    if (DATA->current_guide_step() == 0) {
        this->backCallBack(NULL);
    }
}

void QingjingScene::creat_view(){
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(QingjingScene::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 20);
    
    // 聊天
//    CCSprite* qipao = CCSprite::create("res/pic/panel/chat/qipao.png");
//    CCSprite* qipao2 = CCSprite::create("res/pic/panel/chat/qipao.png");
//    qipao2->setScale(1.02f);
//    item_chat = CCMenuItemSprite::create(qipao, qipao2, this, menu_selector(QingjingScene::openChat));
//    item_chat->setPosition(ccp(DISPLAY->ScreenWidth()* .075f, DISPLAY->ScreenHeight()* .32f));
//    CCMenu* menu_chat = CCMenu::create(item_chat, NULL);
//    menu_chat->setPosition(CCPointZero);
//    this->addChild(menu_chat, 20);
    
    
//    qingjingKuang = CCSprite::create("res/pic/qingjingScene/qj_kuang1.png");
//    qingjingKuang->setAnchorPoint(ccp(1, .5f));
//    qingjingKuang->setPosition(ccp(DISPLAY->ScreenWidth()+7, DISPLAY->ScreenHeight()* .61f));
//    this->addChild(qingjingKuang, 20);
//    
//    QingjingTableView* tabLayer = QingjingTableView::create();
//    tabLayer->setPosition(ccp(20, 35));
//    tabLayer->setTag(0x77777);
//    qingjingKuang->addChild(tabLayer, 5);
    
    CCSprite* tempKuangSpr = CCSprite::create("res/pic/qingjingScene/qj_dikuang.png");
    CCSprite* jiantouSpr1_1 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr1_1->setFlipX(true);
    CCSprite* jiantouSpr1_2 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr1_2->setFlipX(true);
    jiantouSpr1_2->setScale(1.02f);
    jiantouItem1 = CCMenuItemSprite::create(jiantouSpr1_1, jiantouSpr1_2, this, menu_selector(QingjingScene::jiantou1CallBack));
    jiantouItem1->setPosition(ccp(DISPLAY->ScreenWidth()* .5 - tempKuangSpr->getContentSize().width* .52f, DISPLAY->ScreenHeight()* .2f));
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .5 - tempKuangSpr->getContentSize().width* .52f - 15, DISPLAY->ScreenHeight()* .2f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .5 - tempKuangSpr->getContentSize().width* .52f, DISPLAY->ScreenHeight()* .2f));
    CCSequence* seq1 = CCSequence::create(moveTo1_1, moveTo1_2, NULL);
    jiantouItem1->runAction(CCRepeatForever::create(seq1));
    
    CCSprite* jiantouSpr2_1 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    CCSprite* jiantouSpr2_2 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr2_2->setScale(1.02f);
    jiantouItem2 = CCMenuItemSprite::create(jiantouSpr2_1, jiantouSpr2_2, this, menu_selector(QingjingScene::jiantou2CallBack));
    jiantouItem2->setPosition(ccp(DISPLAY->ScreenWidth()* .5 + tempKuangSpr->getContentSize().width* .52f, DISPLAY->ScreenHeight()* .2f));
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .5 + tempKuangSpr->getContentSize().width* .52f + 15, DISPLAY->ScreenHeight()* .2f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .5 + tempKuangSpr->getContentSize().width* .52f, DISPLAY->ScreenHeight()* .2f));
    CCSequence* seq2 = CCSequence::create(moveTo2_1, moveTo2_2, NULL);
    jiantouItem2->runAction(CCRepeatForever::create(seq2));
    
    CCMenu* jiantouMenu = CCMenu::create(jiantouItem1, jiantouItem2, NULL);
    jiantouMenu->setPosition(CCPointZero);
    this->addChild(jiantouMenu, 20);
    jiantouItem1->setColor(ccGRAY);
    jiantouItem1->setEnabled(false);
    
    
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
    CCString* taskConditionsKeyStr = CCString::createWithFormat("101_80100_%d", 0);
    CCArray* taskConditionsAchievemArr = (CCArray* )taskConditionsDic->objectForKey(taskConditionsKeyStr->getCString());
    std::string bgStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(3))->getCString();
    std::string renwuIndexStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(2))->getCString();
    renwuIndex = atoi(renwuIndexStr.c_str());
    
    
    CCString* roomStr = CCString::createWithFormat("res/pic/qingjingScene/bgimage/%s", bgStr.c_str());
    roomSpr = CCSprite::create(roomStr->getCString());
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    roomSpr->setTag(0x99999);
    this->addChild(roomSpr);
    
    // 人
    std::string manStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(0))->getCString();
    std::string lianStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(1))->getCString();
    CCString* manStr = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", manStd.c_str());
    manSpr = CCSprite::create(manStr->getCString());
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    manSpr->setTag(0x11111);
    this->addChild(manSpr, 10);
    CCString* lianStr = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", lianStd.c_str());
    lianSpr = CCSprite::create(lianStr->getCString());
    lianSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    lianSpr->setTag(0x22222);
    this->addChild(lianSpr, 11);
    
    
    CCRect swRect = CCRectMake(DISPLAY->ScreenWidth()* .5f - 517/2, DISPLAY->ScreenHeight()* .1f, 517, 210);
    CCSize slSize = CCSizeMake(517, 210);
    float disDistance = 517;
    float disScale = .25f;
    qingjingCoverView = QingjingCoverView::create(swRect, slSize, disDistance, disScale);
    
    for (int i = 0; i < allNumber; i++) {        
        //
        taskConditionsKeyStr = CCString::createWithFormat("101_80100_%d", i);
        taskConditionsAchievemArr = (CCArray* )taskConditionsDic->objectForKey(taskConditionsKeyStr->getCString());
        std::string renwuIndexStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(2))->getCString();
        renwuIndex = atoi(renwuIndexStr.c_str());
        std::string phaseIndexStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(5))->getCString();
        phaseIndex = atoi(phaseIndexStr.c_str());
        
        CCSprite* kuangSpr = CCSprite::create("res/pic/qingjingScene/qj_dikuang.png");
        kuangSpr->setPosition(CCPointZero);
        qingjingCoverView->addCard(kuangSpr, i, i);
        
        CCSprite* kuangSpr1 = CCSprite::create("res/pic/qingjingScene/qj_dikuang1.png");
        kuangSpr1->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height - 30));
        kuangSpr->addChild(kuangSpr1);
        
        CCString* labelStr;
        if (i == 0) {
            labelStr = CCString::createWithFormat("序章. %s", DISPLAY->GetOffTheName(i)->getCString());
        }else{
            labelStr = CCString::createWithFormat("%s", DISPLAY->GetOffTheName(i)->getCString());
        }
        CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 25);
        label->setPosition(ccp(kuangSpr1->getContentSize().width* .5f, kuangSpr1->getContentSize().height* .5f));
        label->setColor(ccWHITE);
        kuangSpr1->addChild(label, 2);
        
        // 结局成就
        CCLabelTTF* titleLabel = CCLabelTTF::create("结局成就", DISPLAY->fangzhengFont(), 22, CCSizeMake(385, 23), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
        titleLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
        titleLabel->setColor(ccc3(80, 63, 68));
//        titleLabel->enableStroke(ccc3(80, 63, 68), .4f);
        kuangSpr->addChild(titleLabel);
        // 显示的结局
        CSJson::Value data = AppUtil::read_json_file("res/story/storyAchievementArr");
        CCDictionary* dic = AppUtil::dictionary_with_json(data);
        CCString* keyStr = CCString::createWithFormat("101_80100_%d", i);
        CCArray* achievemArr = (CCArray* )dic->objectForKey(keyStr->getCString());
        
        
        CSJson::Value storyData = AppUtil::read_json_file("story/storyAchievement");
        CCDictionary* storyDic = AppUtil::dictionary_with_json(storyData);
        
        CCString* story_index = CCString::createWithFormat("%d", i);
        CCArray* storyArr = DATA->getStory()->story_achievments(story_index->getCString());
        bool tongguanBool = false;
        for (int j = 0; j < achievemArr->count(); j++) {
            bool achiBool = false;
            CCString* str = (CCString* )achievemArr->objectAtIndex(j);
            if (storyArr != NULL) {
                for (int k = 0; k < storyArr->count(); k++) {
                    CCString* storyStr = (CCString* )storyArr->objectAtIndex(k);
                    CCString* tongguanStr1 = CCString::createWithFormat("-1");
                    CCLog("j == %d", j);
                    CCLog("k == %d", k);
                    CCLog("keyStr == %s", keyStr->getCString());
                    CCLog("storyStr == %s", storyStr->getCString());
                    CCLog("str == %s", str->getCString());
                    if (strcmp(str->getCString(), storyStr->getCString()) == 0) {
                        achiBool = true;
                        CCLog("11111");
                    }else{
                        CCLog("22222");
                    }
                    CCString* tongguanStr2 = (CCString* )storyArr->objectAtIndex(k);
                    if (strcmp(tongguanStr1->getCString(), tongguanStr2->getCString()) == 0) {
                        tongguanBool = true;
                        CCLog("等于-1");
                    }else{
                        CCLog("不等于-1");
                    }
                }
            }
            
            
            CCString* str1 = (CCString* )storyDic->objectForKey(str->getCString());
            CCLabelTTF* label = CCLabelTTF::create(str1->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(200, 28), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            if (j == 0){
                label->setPosition(ccp(166, kuangSpr->getContentSize().height* .36f));
            }else if (j == 1){
                label->setPosition(ccp(kuangSpr->getContentSize().width* .5f + 100.5f, kuangSpr->getContentSize().height* .36f));
            }else if (j == 2){
                label->setPosition(ccp(166, kuangSpr->getContentSize().height* .23f));
            }else if (j == 3){
                label->setPosition(ccp(kuangSpr->getContentSize().width* .5f + 100.5f, kuangSpr->getContentSize().height* .23f));
            }
            if (achiBool) {
                label->setColor(ccc3(80, 63, 68));
            }else{
                label->setColor(ccc3(139, 140, 164));
            }
            kuangSpr->addChild(label);
            
            
            if (achiBool) {
                CCSprite* gouSpr1 = CCSprite::create("res/pic/qingjingScene/qj_right.png");
                gouSpr1->setScale(1.f);
                if (j == 0){
                    gouSpr1->setPosition(ccp(label->getContentSize().width* .56f, label->getContentSize().height* .5f));
                }else if (j == 1){
                    gouSpr1->setPosition(ccp(label->getContentSize().width* .56f, label->getContentSize().height* .5f));
                }else if (j == 2){
                    gouSpr1->setPosition(ccp(label->getContentSize().width* .56f, label->getContentSize().height* .5f));
                }else if (j == 3){
                    gouSpr1->setPosition(ccp(label->getContentSize().width* .56f, label->getContentSize().height* .5f));
                }
                label->addChild(gouSpr1, 1);
            }
        }
        
        
        // 开始故事
        CCSprite* startSpr1;
        CCSprite* startSpr2;
        CCMenuItem* startItem;
        
        std::string tishiStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(4))->getCString();
        CCLabelTTF* tishiLabel = CCLabelTTF::create(tishiStr.c_str(), DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .6f, 25), kCCTextAlignmentRight, kCCVerticalTextAlignmentTop);
        tishiLabel->setPosition(ccp(kuangSpr->getContentSize().width* .35f, kuangSpr->getContentSize().height* .66f));
        tishiLabel->setColor(ccc3(80, 63, 68));
        kuangSpr->addChild(tishiLabel);
        
        if (tongguanBool) {
            CCSprite* tongguanSpr = CCSprite::create("res/pic/qingjingScene/qj_tongguan.png");
            tongguanSpr->setScale(.7f);
            tongguanSpr->setAnchorPoint(ccp(0, .5f));
            tongguanSpr->setPosition(ccp(tishiLabel->getContentSize().width* 1.07f, tishiLabel->getContentSize().height* .5f));
            tishiLabel->addChild(tongguanSpr);
            
            startSpr1 = CCSprite::create("res/pic/common/btn_startstory.png");
            startSpr2 = CCSprite::create("res/pic/common/btn_startstory.png");
            startSpr2->setScale(1.02f);
            startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(QingjingScene::startCallBack));
            startItem->setPosition(ccp(kuangSpr->getContentSize().width* .86f, kuangSpr->getContentSize().height* .26f));
            startItem->setTag(i);
        }else{
            if (renwuIndex <= DATA->getPlayer()->ratings(phaseIndex)) {
                CCSprite* jiesuoSpr = CCSprite::create("res/pic/qingjingScene/qj_yijiesuo.png");
                jiesuoSpr->setScale(.7f);
                jiesuoSpr->setAnchorPoint(ccp(0, .5f));
                jiesuoSpr->setPosition(ccp(tishiLabel->getContentSize().width* 1.07f, tishiLabel->getContentSize().height* .5f));
                tishiLabel->addChild(jiesuoSpr);
                
                startSpr1 = CCSprite::create("res/pic/common/btn_startstory.png");
                startSpr2 = CCSprite::create("res/pic/common/btn_startstory.png");
                startSpr2->setScale(1.02f);
                startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(QingjingScene::startCallBack));
                startItem->setPosition(ccp(kuangSpr->getContentSize().width* .86f, kuangSpr->getContentSize().height* .26f));
                startItem->setTag(i);
            }else{
                CCSprite* jiesuoSpr = CCSprite::create("res/pic/qingjingScene/qj_weijiesuo.png");
                jiesuoSpr->setScale(.7f);
                jiesuoSpr->setAnchorPoint(ccp(0, .5f));
                jiesuoSpr->setPosition(ccp(tishiLabel->getContentSize().width* 1.07f, tishiLabel->getContentSize().height* .5f));
                tishiLabel->addChild(jiesuoSpr);
                
                startSpr1 = CCSprite::create("res/pic/common/btn_startstory.png");
                startSpr2 = CCSprite::create("res/pic/common/btn_startstory.png");
                startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, NULL);
                startItem->setPosition(ccp(kuangSpr->getContentSize().width* .86f, kuangSpr->getContentSize().height* .26f));
                startItem->setColor(ccGRAY);
                startItem->setTag(i);
            }
        }
        CCMenu* startMenu = CCMenu::create(startItem, NULL);
        startMenu->setPosition(CCPointZero);
        kuangSpr->addChild(startMenu);
    }
    
    qingjingCoverView->setPosition(swRect.origin);
    qingjingCoverView->setTag(0x77777);
    this->addChild(qingjingCoverView, 50);
    
    this->qingjingStatus();
}
void QingjingScene::qingjingStatus(){
    if (this->getChildByTag(0x99999) != NULL) {
        this->removeChildByTag(0x99999);
    }
    if (this->getChildByTag(0x11111) != NULL) {
        this->removeChildByTag(0x11111);
    }
    if (this->getChildByTag(0x22222) != NULL) {
        this->removeChildByTag(0x22222);
    }
    
    int now_task_index = -1;
    // 显示的任务的结局
    CSJson::Value taskConditionsData = AppUtil::read_json_file("res/story/taskConditions");
    CCDictionary* taskConditionsDic = AppUtil::dictionary_with_json(taskConditionsData);
    int allTask = taskConditionsDic->count();
    
//    for (int i = 0; i < allTask; i++) {
//        CCString* story_index = CCString::createWithFormat("%d", i);
//        CCArray* storyArr = DATA->getStory()->story_achievments(story_index->getCString());
//        if (storyArr != NULL) {
//            continue;
//        }else{
//            now_task_index = i;
//            break;
//        }
//    }
    
    bool tongguanBool = false;
    for (int i = 0; i < allTask; i++) {
        CCString* story_index = CCString::createWithFormat("%d", i);
        CCArray* storyArr = DATA->getStory()->story_achievments(story_index->getCString());
        if (storyArr != NULL) {
            for (int k = 0; k < storyArr->count(); k++) {
                CCString* tongguanStr1 = CCString::createWithFormat("-1");
                CCString* tongguanStr2 = (CCString* )storyArr->objectAtIndex(k);
                if (strcmp(tongguanStr1->getCString(), tongguanStr2->getCString()) == 0) {
                    tongguanBool = true;
                    CCLog("等于-1");
                }else{
                    CCLog("不等于-1");
                }
            }
            
            if (tongguanBool) {
                tongguanBool = false;
            }else{
                now_task_index = i;
                break;
            }
        }else{
            now_task_index = i;
            break;
        }
    }
    
    if (now_task_index == -1) {
        now_task_index = allTask - 1;
    }
    DATA->setChapterNumber(now_task_index);
    qingjingCoverView->scrollStatus(now_task_index);
    this->updataButton();
    
    CCString* taskConditionsKeyStr = CCString::createWithFormat("101_80100_%d", now_task_index);
    CCArray* taskConditionsAchievemArr = (CCArray* )taskConditionsDic->objectForKey(taskConditionsKeyStr->getCString());
    std::string bgStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(3))->getCString();
    // 人
    std::string manStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(0))->getCString();
    std::string lianStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(1))->getCString();
    
    CCString* roomStr = CCString::createWithFormat("res/pic/qingjingScene/bgimage/%s", bgStr.c_str());
    roomSpr = CCSprite::create(roomStr->getCString());
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    roomSpr->setTag(0x99999);
    this->addChild(roomSpr);
    
    CCString* manStr = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", manStd.c_str());
    manSpr = CCSprite::create(manStr->getCString());
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    manSpr->setTag(0x11111);
    this->addChild(manSpr, 10);
    CCString* lianStr = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", lianStd.c_str());
    lianSpr = CCSprite::create(lianStr->getCString());
    lianSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    lianSpr->setTag(0x22222);
    this->addChild(lianSpr, 11);
}

void QingjingScene::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    
    CCScene* scene = MainScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

//void QingjingScene::openChat() {
//    // talkingData
//    DATA->onEvent("点击事件", "情景界面", "点击聊天");
//    
//    AUDIO->comfirm_effect();
//    DATA->setChatOut(false);
//    if (WS->isConnected()) {
//        ChatPanel* panel = ChatPanel::create();
//        CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
//    }else{
//        WS->connect();
//    }
//}
//
//void QingjingScene::displayChatItem(){
//    if (item_chat->isVisible()) {
//        item_chat->setVisible(false);
//    }else{
//        item_chat->setVisible(true);
//    }
//    
//}


void QingjingScene::startCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "情景界面", "点击开始");
    
    CCMenuItem* item = (CCMenuItem* )pSender;
    storyIndex = item->getTag();
    
    if (storyIndex > 0) {
        bool tongguanBool = false;
        CCString* story_index = CCString::createWithFormat("%d", storyIndex-1);
        CCArray* storyArr = DATA->getStory()->story_achievments(story_index->getCString());
        if (storyArr != NULL) {
            for (int i = 0; i < storyArr->count(); i++) {
                CCString* tongguanStr1 = CCString::createWithFormat("-1");
                CCString* tongguanStr2 = (CCString* )storyArr->objectAtIndex(i);
                if (strcmp(tongguanStr1->getCString(), tongguanStr2->getCString()) == 0) {
                    tongguanBool = true;
//                    CCLog("等于-1, 通关");
                }else{
//                    CCLog("不等于-1, 不通关");
                }
            }
        }
        if (tongguanBool) {
            if (DATA->getPlayer()->energy >= 9) {
                LOADING->show_loading();
                CCString* indexStr = CCString::createWithFormat("%d", storyIndex);
                NET->start_story_501(indexStr->getCString());
            }else{
                AHMessageBox* mb = AHMessageBox::create_with_message("体力不够,是否购买体力.", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
                mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
            }
        }else {
            PromptLayer* layer = PromptLayer::create();
            layer->show_prompt(this->getScene(), "亲!前面章节,没通关.");
        }
    }else{
        if (DATA->getPlayer()->energy >= 9) {
            LOADING->show_loading();
            CCString* indexStr = CCString::createWithFormat("%d", storyIndex);
            NET->start_story_501(indexStr->getCString());
        }else{
            AHMessageBox* mb = AHMessageBox::create_with_message("体力不够,是否购买体力.", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
            mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
        }
    }
}
void QingjingScene::guide_StartCallBack(){
    LOADING->show_loading();
    CCString* indexStr = CCString::createWithFormat("%d", storyIndex);
    NET->start_story_501(indexStr->getCString());
}
void QingjingScene::_501CallBack(CCObject* pSender){
    CCScene* pScene = CCScene::create();
    StoryScene* layer = StoryScene::create_with_story_id(storyIndex);
    pScene->addChild(layer);
    CCTransitionScene* trans = CCTransitionFade::create(.3f, pScene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void QingjingScene::creat_Man(){
    float widthFolt = .75f;
    float heightFloat = .3f;
    float scaleFloat = 1.5f;
    bool flipxBool = false;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setFlipX(flipxBool);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void QingjingScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = .75f;
    float heightFloat = .3f;
    float scaleFloat = 1.5f;
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_QJ_TouFa; i <= Tag_QJ_ZhuangRong; i++) {
        if (i == Tag_QJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_QJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_QJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_QJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_QJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_QJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_QJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_QJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_QJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_QJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_QJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_QJ_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_QJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_QJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_QJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr1->setTag(Tag_QJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_QJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_QJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_QJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_QJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_QJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShiPin){
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
        else if (i == Tag_QJ_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_QJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_QJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_QJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_QJ_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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
                            _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
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


void QingjingScene::jiantou1CallBack(CCObject* pSender){
    jiantouItem1->setEnabled(false);
    jiantouItem2->setEnabled(false);
    
    selectedIndex--;
    DATA->setChapterNumberBool(true);
    if (selectedIndex >= 0) {
//        if (selectedIndex == 0) {
//            jiantouItem1->setColor(ccGRAY);
//            jiantouItem1->setEnabled(false);
//        }
        DATA->setChapterNumber(selectedIndex);
        qingjingCoverView->leftMethods();
//        jiantouItem2->setColor(ccWHITE);
//        jiantouItem2->setEnabled(true);
    }
    this->scheduleOnce(SEL_SCHEDULE(&QingjingScene::updataButton), .3f);
}
void QingjingScene::jiantou2CallBack(CCObject* pSender){
    if (!theEndBool) {
        jiantouItem1->setEnabled(false);
        jiantouItem2->setEnabled(false);
        
        selectedIndex++;
        DATA->setChapterNumberBool(true);
        if (selectedIndex < allNumber) {
            if (selectedIndex == allNumber-1) {
                selectedIndex = allNumber-1;
//                jiantouItem2->setColor(ccGRAY);
//                jiantouItem2->setEnabled(false);
            }
            DATA->setChapterNumber(selectedIndex);
            qingjingCoverView->rightMethods();
//            jiantouItem1->setColor(ccWHITE);
//            jiantouItem1->setEnabled(true);
        }
        this->scheduleOnce(SEL_SCHEDULE(&QingjingScene::updataButton), .3f);
    }else{
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(this->getScene(), "敬请期待!");
    }
}
void QingjingScene::closeButton(){
    jiantouItem1->setEnabled(false);
    jiantouItem2->setEnabled(false);
}
void QingjingScene::updataButton(){
    selectedIndex = DATA->getChapterNumber();
    if (selectedIndex == allNumber-1) {
        theEndBool = true;
        jiantouItem2->setColor(ccGRAY);
        jiantouItem2->setEnabled(true);
        jiantouItem1->setColor(ccWHITE);
        jiantouItem1->setEnabled(true);
    }else if (selectedIndex == 0){
        theEndBool = false;
        jiantouItem1->setColor(ccGRAY);
        jiantouItem1->setEnabled(false);
        jiantouItem2->setColor(ccWHITE);
        jiantouItem2->setEnabled(true);
    }else{
        theEndBool = false;
        jiantouItem1->setColor(ccWHITE);
        jiantouItem1->setEnabled(true);
        jiantouItem2->setColor(ccWHITE);
        jiantouItem2->setEnabled(true);
    }
}
void QingjingScene::updataMan(){
    if (this->getChildByTag(0x99999) != NULL) {
        this->removeChildByTag(0x99999);
    }
    if (this->getChildByTag(0x11111) != NULL) {
        this->removeChildByTag(0x11111);
    }
    if (this->getChildByTag(0x22222) != NULL) {
        this->removeChildByTag(0x22222);
    }
    
    // 显示的任务的结局
    CSJson::Value taskConditionsData = AppUtil::read_json_file("res/story/taskConditions");
    CCDictionary* taskConditionsDic = AppUtil::dictionary_with_json(taskConditionsData);
    CCString* taskConditionsKeyStr = CCString::createWithFormat("101_80100_%d", DATA->getChapterNumber());
    CCArray* taskConditionsAchievemArr = (CCArray* )taskConditionsDic->objectForKey(taskConditionsKeyStr->getCString());
    
    std::string manStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(0))->getCString();
    std::string lianStd = ((CCString* )taskConditionsAchievemArr->objectAtIndex(1))->getCString();
    std::string bgStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(3))->getCString();
    
    CCString* roomStr = CCString::createWithFormat("res/pic/qingjingScene/bgimage/%s", bgStr.c_str());
    roomSpr = CCSprite::create(roomStr->getCString());
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    roomSpr->setTag(0x99999);
    this->addChild(roomSpr);
    
    CCString* manStr = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", manStd.c_str());
    manSpr = CCSprite::create(manStr->getCString());
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    manSpr->setTag(0x11111);
    this->addChild(manSpr, 10);
    CCString* lianStr = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", lianStd.c_str());
    lianSpr = CCSprite::create(lianStr->getCString());
    lianSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    lianSpr->setTag(0x22222);
    this->addChild(lianSpr, 11);
}




void QingjingScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    box->animation_out();
    
    if (button_type == AH_BUTTON_TYPE_YESNO) {
        if (button_tag == AH_BUTTON_TAG_YES) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_BUY_ENERGY");
        }else if (button_tag == AH_BUTTON_TAG_NO){
            PromptLayer* layer = PromptLayer::create();
            layer->show_prompt(this->getScene(), "据说体力藏在活动里~!去看看活动吧.");
        }
    }
}

