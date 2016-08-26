//
//  GameOneScene.cpp
//  tiegao
//
//  Created by stevenLi on 16/8/17.
//
//

#include "GameOneScene.h"
#include "DisplayManager.h"
#include "HomeLayer.h"


GameOneScene::~GameOneScene(){
    
}

bool GameOneScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
//    this->initView();
    
    return true;
}

CCScene* GameOneScene::scene(){
    CCScene* scene = CCScene::create();
    GameOneScene* layer = GameOneScene::create();
    scene->addChild(layer);
    return scene;
}

void GameOneScene::onEnter(){
    CCLayer::onEnter();
}

void GameOneScene::onExit(){
    CCLayer::onExit();
}

//void GameOneScene::initView(){
//    CCSprite* bg = CCSprite::create("res/pic/game/game_1/background.png");
//    bg->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
//    this->addChild(bg);
//    
//    CCSprite* back_spr = CCSprite::create("res/pic/common/btn_goback2.png");
//    CCSprite* back_spr2 = CCSprite::create("res/pic/common/btn_goback2.png");
//    back_spr2->setScale(1.02f);
//    CCMenuItemSprite* item_back = CCMenuItemSprite::create(back_spr, back_spr2, this, menu_selector(GameOneScene::btn_back_callback));
//    item_back->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
//    CCMenu* menu_back = CCMenu::create(item_back, NULL);
//    menu_back->setPosition(CCPointZero);
//    this->addChild(menu_back);
//    
//    _time = CCLabelTTF::create("00:20", DISPLAY->fangzhengFont(), 18);
//    
//    this->scheduleOnce(SEL_SCHEDULE(&GameOneScene::startCreateGarbage), .8f);
//}

void GameOneScene::startCreateGarbage(){
    this->schedule(SEL_SCHEDULE(&GameOneScene::createGarbage), 0.2f);
    this->schedule(SEL_SCHEDULE(&GameOneScene::updateTime), 1.0f);
    this->scheduleUpdate();
}

void GameOneScene::createGarbage(float dt){
    
}

void GameOneScene::updateTime(float dt){
    
}

void GameOneScene::update(float dt){
    
}

bool GameOneScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    
    return true;
}

void GameOneScene::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void GameOneScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void GameOneScene::btn_back_callback(cocos2d::CCObject *obj){
    CCScene* scene = HomeLayer::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6f, scene, ccWHITE);
    CCDirector::sharedDirector()->replaceScene(trans);
}