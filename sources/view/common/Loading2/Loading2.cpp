//
//  Loading2.cpp
//  tiegao
//
//  Created by mac on 16/6/20.
//
//

#include "Loading2.h"
#include "DisplayManager.h"

static Loading2* _instance = nullptr;
const float DUR = 2.0f;

Loading2::~Loading2() {
}

Loading2* Loading2::Inst() {
    if (_instance == nullptr) {
        _instance = new Loading2();
        _instance->addChild(CCLayerColor::create(ccc4(255, 255, 255, 0)));
        
        CCArray* arr = CCArray::createWithCapacity(5);
        arr->addObject(ccs("res/pic/loading/loading_icon1.png"));
        arr->addObject(ccs("res/pic/loading/loading_icon2.png"));
        arr->addObject(ccs("res/pic/loading/loading_icon3.png"));
        arr->addObject(ccs("res/pic/loading/loading_icon4.png"));
        arr->addObject(ccs("res/pic/loading/loading_icon5.png"));
        arr->retain();
        _instance->_icons = arr;
        _instance->_choosed_icon = arr->objectAtIndex(0);
        
        _instance->init_content();
    }
    
    return _instance;
}

void Loading2::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    _content->setVisible(true);
    this->schedule(SEL_SCHEDULE(&Loading2::loading), 0.1f);
}

void Loading2::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool Loading2::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    return true;
}

#pragma mark - export

void Loading2::show_loading() {
    _content->setVisible(true);
    this->show_loading(CCDirector::sharedDirector()->getRunningScene());
}

void Loading2::show_loading(CCNode* node) {
    if (this->getParent()) {
        return;
    }
    node->addChild(this, 3003);
}

void Loading2::stop_loading() {
    this->unschedule(SEL_SCHEDULE(&Loading2::loading));
    _content->setVisible(false);
}

void Loading2::remove() {
    this->stop_loading();
    this->removeFromParentAndCleanup(true);
}

#pragma mark - inner

void Loading2::init_content() {
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    _content = CCLayer::create();
    this->addChild(_content);
    
    CCSprite* bg = CCSprite::create("res/pic/loading/loading_bg.png");
    bg->setPosition(DISPLAY->center());
    _content->addChild(bg);
    
    CCString* icon = this->choose_icon();
    _icon = CCSprite::create(icon->getCString());
    _icon->setPosition(DISPLAY->center() + ccp(0, 20));
    _content->addChild(_icon);
    
    _star1 = CCSprite::create("res/pic/loading/loading_star.png");
    _star1->setPosition(DISPLAY->center() + ccp(-80, 36));
    _star1->setScale(1.16);
    _content->addChild(_star1);
    
    _star2 = CCSprite::create("res/pic/loading/loading_star.png");
    _star2->setPosition(DISPLAY->center() + ccp(-76, -28));
    _star2->setScale(0.94);
    _content->addChild(_star2);
    
    _star3 = CCSprite::create("res/pic/loading/loading_star.png");
    _star3->setPosition(DISPLAY->center() + ccp(55, -32));
    _content->addChild(_star3);
    
    _star4 = CCSprite::create("res/pic/loading/loading_star.png");
    _star4->setPosition(DISPLAY->center() + ccp(60, 16));
    _star4->setScale(0.8);
    _content->addChild(_star4);
    
    CCSprite* text = CCSprite::create("res/pic/loading/loading_text.png");
    text->setPosition(DISPLAY->center() - ccp(20, 62));
    _content->addChild(text);
    
    _dot1 = CCSprite::create("res/pic/loading/loading_dot.png");
    _dot1->setPosition(text->getPosition() + ccp(58, 0));
    _dot1->setVisible(false);
    _content->addChild(_dot1);
    
    _dot2 = CCSprite::create("res/pic/loading/loading_dot.png");
    _dot2->setPosition(_dot1->getPosition() + ccp(12, 0));
    _dot2->setVisible(false);
    _content->addChild(_dot2);
    
    _dot3 = CCSprite::create("res/pic/loading/loading_dot.png");
    _dot3->setPosition(_dot2->getPosition() + ccp(12, 0));
    _dot3->setVisible(false);
    _content->addChild(_dot3);
}

CCString* Loading2::choose_icon() {
    CCString* pObj = NULL;
    do {
        pObj = (CCString* )_icons->randomObject();
//        CCLOG("choosed_icon = %s", pObj->getCString());
    } while (pObj->isEqual(_choosed_icon));
    
    _choosed_icon = pObj;
    return pObj;
}

void Loading2::loading(float dt) {
    static float dur = 0.0;
    static int dot = 0;
    dur += dt;
    if (dur > DUR) {
        dur -= DUR;
        CCString* str = this->choose_icon();
        CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(str->getCString());
        _icon->setTexture(texture);
        if (dot == 0) {
            _dot1->setVisible(false);
            _dot2->setVisible(false);
            _dot3->setVisible(false);
        }
        if (dot == 1) {
            _dot1->setVisible(true);
        }
        else if (dot == 2) {
            _dot2->setVisible(true);
        }
        else if (dot == 3) {
            _dot3->setVisible(true);
        }
        
        dot += 1;
        if (dot > 3) {
            dot = 0;
        }
    }
    
    _star1->setRotation(_star1->getRotation() + 30);
    _star2->setRotation(_star2->getRotation() + 20);
    _star3->setRotation(_star3->getRotation() + 36);
    _star4->setRotation(_star4->getRotation() + 25);
}


