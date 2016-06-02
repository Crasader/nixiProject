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
#include "QingjingTableView.h"
#include "MZResourceLoader.h"
#include "StoryScene.h"


QingjingScene::QingjingScene(){
    
}
QingjingScene::~QingjingScene(){
    
}

bool QingjingScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    this->creat_view();
    
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
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&QingjingScene::creat_Tishi), "Qingjing_Creat_Tishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&QingjingScene::EnterTheTishi), "Qingjing_EnterTheTishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&QingjingScene::ExitTishi), "Qingjing_ExitTishi", NULL);
}
void QingjingScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    BaseScene::onExit();
}

void QingjingScene::keyBackClicked(){
    
}

void QingjingScene::creat_view(){
    
    roomSpr = CCSprite::create("res/pic/qingjingScene/qj_bg.png");
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(roomSpr);
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/qingjingScene/qj_fanhui.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/qingjingScene/qj_fanhui.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(QingjingScene::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    
    qingjingKuang = CCSprite::create("res/pic/qingjingScene/qj_kuang1.png");
    qingjingKuang->setAnchorPoint(ccp(1, .5f));
    qingjingKuang->setPosition(ccp(DISPLAY->ScreenWidth()+7, DISPLAY->ScreenHeight()* .61f));
    this->addChild(qingjingKuang);
    
    QingjingTableView* tabLayer = QingjingTableView::create();
    tabLayer->setPosition(ccp(20, 35));
    tabLayer->setTag(0x77777);
    qingjingKuang->addChild(tabLayer, 5);
    
}

void QingjingScene::backCallBack(CCObject* pSender){
    CCScene* scene = MainScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}


void QingjingScene::creat_Tishi(){
    
    if (this->getChildByTag(0x88888) != NULL) {
        this->removeChildByTag(0x88888);
        kuangSpr = NULL;
    }
    
    bool tempBool = false;
    int index = DATA->getChapterNumber() + 1;
    std::string renwuStr = "洗碗";
    int renwuIndex = 64;
    int tiliIndex = 9;
    
    kuangSpr = CCSprite::create("res/pic/qingjingScene/qj_kuang2.png");
//    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth() - kuangSpr->getContentSize().width* .5f, DISPLAY->ScreenHeight()* .155f));
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .155f));
    kuangSpr->setTag(0x88888);
    this->addChild(kuangSpr);
    
    CCSprite* xianSpr1 = CCSprite::create("res/pic/qingjingScene/qj_xian.png");
    xianSpr1->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
    kuangSpr->addChild(xianSpr1);
    
    // 标题
    CCLabelTTF* btLabel1 = CCLabelTTF::create(DISPLAY->GetOffTheNumber(index)->getCString(), "Arial", 25);
    btLabel1->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .9f));
    btLabel1->setColor(ccWHITE);
    btLabel1->enableStroke(ccWHITE, 1.1f);
    kuangSpr->addChild(btLabel1, 2);
    
    CCLabelTTF* btLabel2 = CCLabelTTF::create(DISPLAY->GetOffTheNumber(index)->getCString(), "Arial", 25);
    btLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .5f + 2, kuangSpr->getContentSize().height* .9f - 2));
    btLabel2->setColor(ccGRAY);
    btLabel2->enableStroke(ccGRAY, 1.1f);
    kuangSpr->addChild(btLabel2);
    
    // 解锁条件
    CCLabelTTF* titleLabel2 = CCLabelTTF::create("解锁条件", "Arial", 22, CCSizeMake(385, 23), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    titleLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .54f, kuangSpr->getContentSize().height* .75f));
    titleLabel2->setColor(ccc3(80, 63, 68));
    titleLabel2->enableStroke(ccc3(80, 63, 68), .9f);
    kuangSpr->addChild(titleLabel2);
    
    // 解锁条件内容
    CCLabelTTF* label1 = CCLabelTTF::create("日常: ", "Arial", 20);
    label1->setAnchorPoint(CCPointZero);
    label1->setPosition(ccp(kuangSpr->getContentSize().width* .114f, kuangSpr->getContentSize().height* .565f));
    label1->setColor(ccc3(80, 63, 68));
    label1->enableStroke(ccc3(80, 63, 68), .9f);
    kuangSpr->addChild(label1);
    // 任务名
    CCString* labStr2 = CCString::createWithFormat("%s", renwuStr.c_str());
    CCLabelTTF* label2 = CCLabelTTF::create(labStr2->getCString(), "Arial", 20);
    label2->setAnchorPoint(CCPointZero);
    label2->setPosition(ccp(kuangSpr->getContentSize().width* .114f + label1->getContentSize().width, kuangSpr->getContentSize().height* .565f));
    label2->setColor(ccc3(137, 211, 117));
    label2->enableStroke(ccc3(137, 211, 117), .7f);
    kuangSpr->addChild(label2);
    // 任务数
    CCString* labStr3 = CCString::createWithFormat(" (%d)  达成", renwuIndex);
    CCLabelTTF* label3 = CCLabelTTF::create(labStr3->getCString(), "Arial", 20);
    label3->setAnchorPoint(CCPointZero);
    label3->setPosition(ccp(kuangSpr->getContentSize().width* .114f + label1->getContentSize().width + label2->getContentSize().width, kuangSpr->getContentSize().height* .565f));
    label3->setColor(ccc3(80, 63, 68));
    label3->enableStroke(ccc3(80, 63, 68), .9f);
    kuangSpr->addChild(label3);
    // 勾
    CCSprite* rightSpr = CCSprite::create("res/pic/qingjingScene/qj_right.png");
    rightSpr->setPosition(ccp(label3->getContentSize().width* .9f, label3->getContentSize().height* .15f));
    label3->addChild(rightSpr);
    
    // 结局成就
    CCLabelTTF* titleLabel3 = CCLabelTTF::create("结局成就", "Arial", 22, CCSizeMake(385, 23), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    titleLabel3->setPosition(ccp(kuangSpr->getContentSize().width* .54f, kuangSpr->getContentSize().height* .48f));
    titleLabel3->setColor(ccc3(80, 63, 68));
    titleLabel3->enableStroke(ccc3(80, 63, 68), .9f);
    kuangSpr->addChild(titleLabel3);
    
    // 显示的结局
    CSJson::Value data = MZResourceLoader::get_instance()->get_json_data_with_file("res/story/storyAchievementArr");
    CCDictionary* dic = MZResourceLoader::get_instance()->dictionary_with_json(data);
    CCString* keyStr = CCString::createWithFormat("101_80100_%d", index);
    CCArray* achievemArr = (CCArray* )dic->objectForKey(keyStr->getCString());

    CSJson::Value storyData = MZResourceLoader::get_instance()->get_json_data_with_file("story/storyAchievement");
    CCDictionary* storyDic = MZResourceLoader::get_instance()->dictionary_with_json(storyData);
    if (!tempBool) {
        for (int i = 0; i < achievemArr->count(); i++) {
            CCString* str = (CCString* )achievemArr->objectAtIndex(i);
            CCString* str1 = (CCString* )storyDic->objectForKey(str->getCString());
            CCLabelTTF* label = CCLabelTTF::create(str1->getCString(), "Arial", 20, CCSizeMake(385, 28), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(kuangSpr->getContentSize().width* .535f, kuangSpr->getContentSize().height* .38f - (28 * i)));
            label->setColor(ccc3(157, 147, 147));
            label->enableStroke(ccc3(157, 147, 147), .5f);
            kuangSpr->addChild(label);
        }
        
    }else{
        for (int i = 0; i < achievemArr->count(); i++) {
            CCString* str = (CCString* )achievemArr->objectAtIndex(i);
            bool achiBool = false;
            
            CCString* str1 = (CCString* )storyDic->objectForKey(str->getCString());
            CCLabelTTF* label = CCLabelTTF::create(str1->getCString(), "Arial", 20, CCSizeMake(385, 28), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(kuangSpr->getContentSize().width* .535f, kuangSpr->getContentSize().height* .38f - (28 * i)));
            if (achiBool) {
                label->setColor(ccc3(80, 63, 68));
                label->enableStroke(ccc3(80, 63, 68), .9f);
            }else{
                label->setColor(ccc3(157, 147, 147));
                label->enableStroke(ccc3(157, 147, 147), .5f);
            }
            kuangSpr->addChild(label);
            
            if (achiBool) {
                CCSprite* gouSpr1 = CCSprite::create("");
                gouSpr1->setScale(.3f);
                gouSpr1->setPosition(ccp(label->getContentSize().width, kuangSpr->getContentSize().height* .47f - (30 * i)));
                kuangSpr->addChild(gouSpr1, 1);
            }
        }
    }
    
    // 开始故事
    CCSprite* startSpr1 = CCSprite::create("res/pic/qingjingScene/qj_start.png");
    CCSprite* startSpr2 = CCSprite::create("res/pic/qingjingScene/qj_start.png");
    startSpr2->setScale(1.02f);
    CCMenuItem* startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(QingjingScene::startCallBack));
    startItem->setPosition(ccp(kuangSpr->getContentSize().width* .8f, kuangSpr->getContentSize().height* .27f));
    startItem->setTag(index);
    CCMenu* menu = CCMenu::create(startItem, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu);
    
    
    // 消耗体力
    CCSprite* tiliSpr = CCSprite::create("res/pic/qingjingScene/qj_tili_bar.png");
    tiliSpr->setPosition(ccp(kuangSpr->getContentSize().width* .8f, kuangSpr->getContentSize().height* .1f));
    kuangSpr->addChild(tiliSpr);
    // 体力数
    CCString* labStr4 = CCString::createWithFormat("%d", tiliIndex);
    CCLabelTTF* tiliLabel = CCLabelTTF::create(labStr4->getCString(), "Arial", 15);
    tiliLabel->setPosition(ccp(tiliSpr->getContentSize().width* .685f, tiliSpr->getContentSize().height* .45f));
    tiliLabel->setColor(ccWHITE);
    tiliLabel->enableStroke(ccWHITE, .5f);
    tiliSpr->addChild(tiliLabel);
    // 心
    CCSprite* xinSpr = CCSprite::create("res/pic/qingjingScene/qj_tili.png");
    xinSpr->setScale(.9f);
    xinSpr->setPosition(ccp(tiliSpr->getContentSize().width* .85f, tiliSpr->getContentSize().height* .49f));
    tiliSpr->addChild(xinSpr);
}

void QingjingScene::startCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    int index = item->getTag();
    
    CCScene* pScene = CCScene::create();
    StoryScene* layer = StoryScene::create_with_story_id(index);
    pScene->addChild(layer);
    CCTransitionScene* trans = CCTransitionFade::create(.3f, pScene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void QingjingScene::EnterTheTishi(){    
    CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth() - kuangSpr->getContentSize().width* .5f, DISPLAY->ScreenHeight()* .155f));
    kuangSpr->runAction(moveTo);
}
void QingjingScene::ExitTishi(){
    if (this->getChildByTag(0x88888) != NULL) {
        kuangSpr->removeAllChildren();
        this->removeChildByTag(0x88888);
        kuangSpr = NULL;
    }
}






