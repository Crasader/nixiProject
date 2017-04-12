//
//  SettingPanel.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "SettingPanel.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "FileManager.h"
#include "ConfigManager.h"
#include "JNIController.h"

#include "PromptLayer.h"
#include "GiftPanel.h"
#include "IdentityPanel.h"

SettingPanel::~SettingPanel() {
}

SettingPanel* SettingPanel::create(const char *cost) {
    SettingPanel* rtn = new SettingPanel();
    if (rtn && rtn->init(cost)) {
        rtn->autorelease();
        return rtn;
    }
    else {
        delete rtn;
        return NULL;
    }
}

bool SettingPanel::init(const char *cost) {
    if (CCLayer::init()) {
        num_child = 0;
        
        _panel = CCSprite::create("pic/panel/setting/set_panel.png");
        _panel->setPosition(DISPLAY->center());
        this->addChild(_panel);
        
        CCSize panelSize = _panel->boundingBox().size;
        
        this->create_nickname_reset_bar(panelSize, cost);
        
        // 玩家ID
        CCString* id = CCString::createWithFormat("%s", DATA->getLogin()->obtain_sid());
        CCLabelTTF* lblId = CCLabelTTF::create(id->getCString(), DISPLAY->fangzhengFont(), 24.f);
        lblId->setColor(ccc3(255, 85, 132));
        lblId->setAnchorPoint(ccp(0, 0.5));
        lblId->setPosition(ccp(panelSize.width * 0.22, panelSize.height * 0.85));
        _panel->addChild(lblId);

        
        //
        CCMenuItemImage* btn_music_off = CCMenuItemImage::create("pic/panel/setting/set_music_off.png", "pic/panel/setting/set_music_off.png");
        CCMenuItemImage* btn_music_on = CCMenuItemImage::create("pic/panel/setting/set_music_on.png", "pic/panel/setting/set_music_on.png");
        CCMenuItemToggle* toggle_music = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&SettingPanel::on_music), btn_music_off, btn_music_on, NULL);
        toggle_music->setSelectedIndex((int)AUDIO->is_music_on());
        
        CCMenuItemImage* btn_effect_off = CCMenuItemImage::create("pic/panel/setting/set_effect_off.png", "pic/panel/setting/set_effect_off.png");
        CCMenuItemImage* btn_effect_on = CCMenuItemImage::create("pic/panel/setting/set_effect_on.png", "pic/panel/setting/set_effect_on.png");
        CCMenuItemToggle* toggle_effect = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&SettingPanel::on_effect), btn_effect_off, btn_effect_on, NULL);
        toggle_effect->setSelectedIndex((int)AUDIO->is_effect_on());
        
        CCMenu* menu = CCMenu::create(toggle_music, toggle_effect, NULL);
        menu->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.315));
        menu->alignItemsHorizontallyWithPadding(panelSize.width * 0.18);
        _panel->addChild(menu);
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        bool needOpenGift = false;
        
        CCDictionary* conf = DATA->getLogin()->config();
        if (conf) {
            CCInteger* formal = (CCInteger*)conf->objectForKey("formal");
            if (formal && formal->getValue() == 1) {
                needOpenGift = true;
            }
        }

        if (needOpenGift) {
            if (DATA->getPlayer()->hasCommitIdentity()) {
                CCSprite* gift1 = CCSprite::create("pic/panel/setting/set_gift.png");
                CCSprite* gift2 = CCSprite::create("pic/panel/setting/set_gift.png");
                gift2->setScale(1.02f);
                CCMenuItemSprite* btnGift = CCMenuItemSprite::create(gift1, gift2, this, menu_selector(SettingPanel::on_take_gift));
                
                CCSprite* relogin1 = CCSprite::create("pic/panel/setting/relogin.png");
                CCSprite* relogin2 = CCSprite::create("pic/panel/setting/relogin.png");
                relogin2->setScale(1.02f);
                CCMenuItemSprite* btnRelogin = CCMenuItemSprite::create(relogin1, relogin2, this, menu_selector(SettingPanel::on_back));
                
                CCMenu* menuBottom = CCMenu::create(btnGift, btnRelogin, NULL);
                menuBottom->alignItemsHorizontallyWithPadding(panelSize.width * 0.32);
                menuBottom->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.14));
                _panel->addChild(menuBottom);
            }
            else {
                CCSprite* freeDiam1 = CCSprite::create("pic/panel/setting/set_free.png");
                CCSprite* freeDiam2 = CCSprite::create("pic/panel/setting/set_free.png");
                freeDiam2->setScale(1.02f);
                CCMenuItemSprite* btnFreeDiam = CCMenuItemSprite::create(freeDiam1, freeDiam2, this, menu_selector(SettingPanel::on_free_diam));
                
                CCLabelTTF* lbl1 = CCLabelTTF::create("免费获取", DISPLAY->fangzhengFont(), 20.f);
                lbl1->setAnchorPoint(ccp(1, 0.5));
                lbl1->setPosition(ccp(freeDiam1->getContentSize().width * 0.8, freeDiam1->getContentSize().height * 0.5));
                btnFreeDiam->addChild(lbl1);
                
                CCSprite* diamIcon = CCSprite::create("pic/panel/setting/inde_diam_3.png");
                diamIcon->setAnchorPoint(ccp(0, 0.5));
                diamIcon->setPosition(lbl1->getPosition() + ccp(-2, 4));
                btnFreeDiam->addChild(diamIcon);

                
                CCSprite* relogin1 = CCSprite::create("pic/panel/setting/relogin.png");
                CCSprite* relogin2 = CCSprite::create("pic/panel/setting/relogin.png");
                relogin2->setScale(1.02f);
                CCMenuItemSprite* btnRelogin = CCMenuItemSprite::create(relogin1, relogin2, this, menu_selector(SettingPanel::on_back));
                
                CCMenu* menuBottom = CCMenu::create(btnFreeDiam, btnRelogin, NULL);
                menuBottom->alignItemsHorizontallyWithPadding(panelSize.width * 0.32);
                menuBottom->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.14));
                _panel->addChild(menuBottom);
            }
        }
        else {
            CCSprite* relogin1 = CCSprite::create("pic/panel/setting/relogin.png");
            CCSprite* relogin2 = CCSprite::create("pic/panel/setting/relogin.png");
            relogin2->setScale(1.02f);
            CCMenuItemSprite* btnRelogin = CCMenuItemSprite::create(relogin1, relogin2, this, menu_selector(SettingPanel::on_back));
            
            CCMenu* menuBottom = CCMenu::create(btnRelogin, NULL);
            menuBottom->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.14));
            _panel->addChild(menuBottom);
        }

        
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if (DATA->getPlayer()->hasCommitIdentity()) {
            CCSprite* gift1 = CCSprite::create("pic/panel/setting/set_gift.png");
            CCSprite* gift2 = CCSprite::create("pic/panel/setting/set_gift.png");
            gift2->setScale(1.02f);
            CCMenuItemSprite* btnGift = CCMenuItemSprite::create(gift1, gift2, this, menu_selector(SettingPanel::on_take_gift));
            
            CCMenu* menu = CCMenu::create(btnGift, NULL);
            menu->alignItemsHorizontallyWithPadding(panelSize.width * 0.32);
            menu->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.14));
            _panel->addChild(menu);
        }
        else {
            if (CONFIG->channelId != 8) {
                CCSprite* gift1 = CCSprite::create("pic/panel/setting/set_gift.png");
                CCSprite* gift2 = CCSprite::create("pic/panel/setting/set_gift.png");
                gift2->setScale(1.02f);
                CCMenuItemSprite* btnGift = CCMenuItemSprite::create(gift1, gift2, this, menu_selector(SettingPanel::on_take_gift));
                
                CCSprite* freeDiam1 = CCSprite::create("pic/panel/setting/set_free.png");
                CCSprite* freeDiam2 = CCSprite::create("pic/panel/setting/set_free.png");
                freeDiam2->setScale(1.02f);
                CCMenuItemSprite* btnFreeDiam = CCMenuItemSprite::create(freeDiam1, freeDiam2, this, menu_selector(SettingPanel::on_free_diam));
                
                CCLabelTTF* lbl1 = CCLabelTTF::create("免费获取", DISPLAY->fangzhengFont(), 20.f);
                lbl1->setAnchorPoint(ccp(1, 0.5));
                lbl1->setPosition(ccp(freeDiam1->getContentSize().width * 0.8, freeDiam1->getContentSize().height * 0.5));
                btnFreeDiam->addChild(lbl1);
                
                CCSprite* diamIcon = CCSprite::create("pic/panel/setting/inde_diam_3.png");
                diamIcon->setAnchorPoint(ccp(0, 0.5));
                diamIcon->setPosition(lbl1->getPosition() + ccp(-2, 4));
                btnFreeDiam->addChild(diamIcon);
                
                
                CCMenu* menuBottom = CCMenu::create(btnGift, btnFreeDiam, NULL);
                menuBottom->alignItemsHorizontallyWithPadding(panelSize.width * 0.32);
                menuBottom->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.14));
                _panel->addChild(menuBottom);
            }else{
                CCSprite* gift1 = CCSprite::create("pic/panel/setting/set_gift.png");
                CCSprite* gift2 = CCSprite::create("pic/panel/setting/set_gift.png");
                gift2->setScale(1.02f);
                CCMenuItemSprite* btnGift = CCMenuItemSprite::create(gift1, gift2, this, menu_selector(SettingPanel::on_take_gift));
                
                CCMenu* menu = CCMenu::create(btnGift, NULL);
                menu->alignItemsHorizontallyWithPadding(panelSize.width * 0.32);
                menu->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.14));
                _panel->addChild(menu);
            }
            
        }
#endif
        
        
        if (CONFIG->channelId == 900) {
            
            CCSprite* unreloginSpr1 = CCSprite::create("pic/baseScene/base_unrelogin.png");
            CCSprite* unreloginSpr2 = CCSprite::create("pic/baseScene/base_unrelogin.png");
            unreloginSpr2->setScale(1.02f);
            CCMenuItem* unreloginItem = CCMenuItemSprite::create(unreloginSpr1, unreloginSpr2, this, menu_selector(SettingPanel::unreloginCallback));
            unreloginItem->setAnchorPoint(ccp(.5f, 1.f));
            unreloginItem->setPosition(ccp(panelSize.width* .5f, - 10));
            CCMenu* unreloginMenu = CCMenu::create(unreloginItem, NULL);
            unreloginMenu->setPosition(CCPointZero);
            _panel->addChild(unreloginMenu);
        }
        
        
        
        return true;
    }
    else {
        return false;
    }
}
void SettingPanel::unreloginCallback(CCObject* pSender){
    
    JNIController::isUnLanding();
    DATA->relogin();
}

void SettingPanel::create_nickname_reset_bar(CCSize panelSize, const char *cost) {
    CCSize size_bar = CCSizeMake(222, 40);
    _eb = CCEditBox::create(CCSizeMake(size_bar.width, size_bar.height), CCScale9Sprite::create("pic/panel/setting/nnr_bar.png"));
    _eb->setMaxLength(12);
    _eb->setFontColor(DISPLAY->defalutColor());
    _eb->setPlaceHolder("点此输入新昵称");
    _eb->setFontName(DISPLAY->fangzhengFont());
    _eb->setInputMode(kEditBoxInputModeAny);
    _eb->setReturnType(kKeyboardReturnTypeDone);
    _eb->setPosition(ccp(panelSize.width * 0.4, panelSize.height * 0.68));
    _panel->addChild(_eb);
    
    CCSprite* spt1 = CCSprite::create("pic/panel/setting/nnr_commit.png");
    CCSprite* spt2 = CCSprite::create("pic/panel/setting/nnr_commit.png");
    spt2->setScale(DISPLAY->btn_scale());
    CCMenuItem* btnCommit = CCMenuItemSprite::create(spt1, spt2, this, SEL_MenuHandler(&SettingPanel::onBtnCommitRest));
    CCMenu* menu = CCMenu::createWithItem(btnCommit);
    menu->ignoreAnchorPointForPosition(true);
    menu->setPosition(ccp(panelSize.width * 0.82, _eb->getPositionY()));
    _panel->addChild(menu);
    
    CCLabelTTF* lblCost = CCLabelTTF::create(cost, DISPLAY->fangzhengFont(), 20);
    lblCost->setAnchorPoint(ccp(0.5, 0.5));
    lblCost->setPosition(ccp(spt1->getContentSize().width * 0.5, spt1->getContentSize().height * 0.5));
    btnCommit->addChild(lblCost);
}

void SettingPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
//    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&SettingPanel::keyBackStatus), .8f);
}
void SettingPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void SettingPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool SettingPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->remove();
    }
    
    return true;
}

#pragma mark - export

void SettingPanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    _panel->setPosition(_enter_pos + ccp(20, 25));
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void SettingPanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.1f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    _content->runAction(spawn);
}

void SettingPanel::do_exit() {
    float duration = 0.1f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&SettingPanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(spawn);
}

void SettingPanel::remove() {
    AUDIO->goback_effect();
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void SettingPanel::on_music(cocos2d::CCMenuItem *btn) {
    //    CCDirector::sharedDirector()->replaceScene(scene);
    CCMenuItemToggle* item = (CCMenuItemToggle*)btn;
    int index = item->getSelectedIndex();
    CCLOG("music getSelectedIndex = %d", index);
    AUDIO->set_music_on((bool)index);
}

void SettingPanel::on_effect(cocos2d::CCMenuItem *btn) {
    CCMenuItemToggle* item = (CCMenuItemToggle*)btn;
    int index = item->getSelectedIndex();
    CCLOG("effect getSelectedIndex = %d", index);
    AUDIO->set_effect_on((bool)index);
}

void SettingPanel::on_back(CCMenuItem *btn){
    DATA->setAutoLogin(false);
    DATA->relogin();
}

void SettingPanel::on_take_gift(CCMenuItem *btn) {
    GiftPanel* giftPanel = GiftPanel::create();
    this->getScene()->addChild(giftPanel);
    this->remove();
}

void SettingPanel::on_free_diam(CCMenuItem *btn) {
    this->removeFromParent();
    IdentityPanel* panel = IdentityPanel::create();
    panel->show_from(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75f));
}

void SettingPanel::keyBackClicked(){
    num_child++;
    CCLog("===== SettingPanel  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}

void SettingPanel::onBtnCommitRest(CCMenuItem *btn) {
    string text = _eb->getText();
    if (text.length() == 0) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this->getScene(), "请输入要修改名称~!");
    }
    else if (FILEM->is_illegal(text.c_str()) == true) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this->getScene(), "名称中包含非法字符~!");
    }else{
        CCString* str = CCString::create(text);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ON_RESET_NICKNAME", str);
        this->remove();
    }
}
