//
//  RecommendView.cpp
//  tiegao
//
//  Created by mac on 16/6/30.
//
//

#include "RecommendView.h"
#include "DisplayManager.h"
#include "Loading2.h"
#include "AudioManager.h"

#pragma mark - export

void RecommendView::show(CCNode *parent) {
    RecommendView* view = RecommendView::create();
    parent->addChild(view);
}

#pragma mark -

RecommendView::~RecommendView() {
}

bool RecommendView::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
//        CCSprite* mask = CCSprite::create("res/pic/mask.png");
//        mask->setPosition(DISPLAY->center());
//        this->addChild(mask);
        
        _panel = CCSprite::create("res/pic/panel/energybuy/eb_panel.png");
        _panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.55));
        this->addChild(_panel);
        
        CCSprite* sptBox = CCSprite::create("res/pic/panel/totalRecharge/red_bg.png");
        sptBox->setPosition(ccp(_panel->getContentSize().width * 0.5, _panel->getContentSize().height * 0.5));
        _panel->addChild(sptBox);
        
        CCLabelTTF* lbl = CCLabelTTF::create("填写推荐人提供的推荐码(可选)", DISPLAY->fangzhengFont(), 24.f);
        lbl->setColor(DISPLAY->defalutColor());
        lbl->setPosition(ccp(_panel->getContentSize().width * 0.5, _panel->getContentSize().height * 0.82));
        _panel->addChild(lbl);
        
        _input = CCEditBox::create(CCSizeMake(260, 40), CCScale9Sprite::create("res/pic/loginScene/99.png"));
        _input->setMaxLength(10);
        _input->setFontColor(DISPLAY->defalutColor());
        _input->setPlaceHolder("请输入推荐码");
        _input->setFontName(DISPLAY->fangzhengFont());
        _input->setInputMode(kEditBoxInputModeEmailAddr);
        _input->setReturnType(kKeyboardReturnTypeDone);
        _input->setPosition(ccp(sptBox->getContentSize().width * 0.5, sptBox->getContentSize().height * 0.5));
//        _input->setDelegate(this);
        _input->setFontSize(24.f);
        sptBox->addChild(_input);
        
        CCSprite* cancel1 = CCSprite::create("res/pic/common/btn_cancel.png");
        CCSprite* cancel2 = CCSprite::create("res/pic/common/btn_cancel.png");
        cancel2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btn_canel = CCMenuItemSprite::create(cancel1, cancel2, this, SEL_MenuHandler(&RecommendView::remove));
        btn_canel->setPosition(ccp(_panel->getContentSize().width * 0.25, _panel->getContentSize().height * 0.15));
        
        CCSprite* confirm1 = CCSprite::create("res/pic/common/btn_confirm.png");
        CCSprite* confirm2 = CCSprite::create("res/pic/common/btn_confirm.png");
        confirm2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btn_confirm = CCMenuItemSprite::create(confirm1, confirm2, this, SEL_MenuHandler(&RecommendView::buy));
        btn_confirm->setPosition(ccp(_panel->getContentSize().width * 0.75, _panel->getContentSize().height * 0.15));
        
        CCMenu* menu = CCMenu::create(btn_canel, btn_confirm, NULL);
        menu->ignoreAnchorPointForPosition(false);
        _panel->addChild(menu);

        return true;
    }
    else {
        return false;
    }
}

void RecommendView::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    this->scheduleOnce(SEL_SCHEDULE(&RecommendView::keyBackStatus), .8f);
}

void RecommendView::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void RecommendView::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool RecommendView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    return false;
}

#pragma mark - inner

void RecommendView::remove() {
    AUDIO->goback_effect();
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void RecommendView::buy() {
    LOADING->show_loading();
}

void RecommendView::buy_energy_callback_101(CCObject *pObj) {
    LOADING->remove();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    this->remove();
}

void RecommendView::keyBackClicked(){
    num_child++;
    CCLog("===== RecommendView  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}


