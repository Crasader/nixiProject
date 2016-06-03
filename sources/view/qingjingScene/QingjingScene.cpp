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
#include "AppUtil.h"

QingjingScene::QingjingScene(){
    
}
QingjingScene::~QingjingScene(){
    
}

bool QingjingScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    this->creat_view();
    creat_Tishi();
    
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
    
}
void QingjingScene::onExit(){
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
    
    bool tempBool = false;
    int index = 1;
    
    kuangSpr = CCSprite::create("res/pic/qingjingScene/qj_kuang2.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth() - kuangSpr->getContentSize().width* .5f, DISPLAY->ScreenHeight()* .155f));
    this->addChild(kuangSpr);
    
    CCSprite* xianSpr1 = CCSprite::create("res/pic/qingjingScene/qj_xian.png");
    xianSpr1->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
    kuangSpr->addChild(xianSpr1);
    
    // 标题
    CCLabelTTF* titleLabel1 = CCLabelTTF::create(DISPLAY->GetOffTheNumber(index)->getCString(), "Arial", 22);
    titleLabel1->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .9f));
    titleLabel1->setColor(ccc3(111, 84, 105));
    titleLabel1->enableStroke(ccc3(111, 84, 105), .5f);
    kuangSpr->addChild(titleLabel1);
    
    // 解锁条件
    CCLabelTTF* titleLabel2 = CCLabelTTF::create("解锁条件", "Arial", 22, CCSizeMake(385, 23), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    titleLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .54f, kuangSpr->getContentSize().height* .75f));
    titleLabel2->setColor(ccc3(111, 84, 105));
    titleLabel2->enableStroke(ccc3(111, 84, 105), .5f);
    kuangSpr->addChild(titleLabel2);
    
    // 结局成就
    CCLabelTTF* titleLabel3 = CCLabelTTF::create("结局成就", "Arial", 22, CCSizeMake(385, 23), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    titleLabel3->setPosition(ccp(kuangSpr->getContentSize().width* .54f, kuangSpr->getContentSize().height* .48f));
    titleLabel3->setColor(ccc3(111, 84, 105));
    titleLabel3->enableStroke(ccc3(111, 84, 105), .5f);
    kuangSpr->addChild(titleLabel3);
    
    // 显示的结局
    CSJson::Value data = AppUtil::read_json_file("res/story/storyAchievementArr");
    CCDictionary* dic = AppUtil::dictionary_with_json(data);
    CCString* keyStr = CCString::createWithFormat("101_80100_%d", index);
    CCArray* achievemArr = (CCArray* )dic->objectForKey(keyStr->getCString());

    CSJson::Value storyData = AppUtil::read_json_file("story/storyAchievement");
    CCDictionary* storyDic = AppUtil::dictionary_with_json(storyData);
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
                label->setColor(ccc3(127, 104, 104));
                label->enableStroke(ccc3(127, 104, 104), .5f);
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
}











