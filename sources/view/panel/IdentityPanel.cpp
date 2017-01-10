//
//  IdentityPanel.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "IdentityPanel.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "FileManager.h"

#include "PromptLayer.h"
#include "GiftPanel.h"

IdentityPanel::~IdentityPanel() {
}

IdentityPanel* IdentityPanel::create() {
    IdentityPanel* rtn = new IdentityPanel();
    if (rtn && rtn->init()) {
        rtn->autorelease();
        return rtn;
    }
    else {
        delete rtn;
        return NULL;
    }
}

bool IdentityPanel::init() {
    if (CCLayer::init()) {
        num_child = 0;

        return true;
    }
    else {
        return false;
    }
}

void IdentityPanel::create_panel() {
    _panel = CCSprite::create("pic/panel/identity/inde_panel.png");
    this->addChild(_panel);
    
    CCSize panelSize = _panel->boundingBox().size;
    
    // 提示
    CCString* strTip = CCString::createWithFormat("完善资料即可获得%s", DATA->getLogin()->obtain_sid());
    CCLabelTTF* lblTip = CCLabelTTF::create(strTip->getCString(), DISPLAY->fangzhengFont(), 19.f);
    lblTip->setColor(ccc3(255, 69, 143));
    lblTip->setAnchorPoint(ccp(0, 0.5));
    lblTip->setPosition(ccp(panelSize.width * 0.26, panelSize.height * 0.84));
    _panel->addChild(lblTip);
    
    
    //
    CCMenuItemImage* btnFemaleNor = CCMenuItemImage::create("pic/panel/identity/inde_female_nor.png", "pic/panel/identity/inde_female_nor.png");
    CCMenuItemImage* btnFemaleSlt = CCMenuItemImage::create("pic/panel/identity/inde_female_slt.png", "pic/panel/identity/inde_female_slt.png");
    CCMenuItemToggle* btnFemale = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&IdentityPanel::on_music), btnFemaleNor, btnFemaleSlt, NULL);
    
    CCMenuItemImage* btnMaleNor = CCMenuItemImage::create("pic/panel/identity/inde_male_nor.png", "pic/panel/identity/inde_male_nor.png");
    CCMenuItemImage* btnMaleSlt = CCMenuItemImage::create("pic/panel/identity/inde_male_slt.png", "pic/panel/identity/inde_male_slt.png");
    CCMenuItemToggle* btnMale = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&IdentityPanel::on_effect), btnMaleNor, btnMaleSlt, NULL);
    
    CCMenu* menu = CCMenu::create(btnFemale, btnMale, NULL);
    menu->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.5));
    menu->ignoreAnchorPointForPosition(false);
    menu->alignItemsHorizontallyWithPadding(panelSize.width * 0.22);
    _panel->addChild(menu);
    
    // name
    _inputName = CCEditBox::create(CCSizeMake(92, 40), CCScale9Sprite::create("pic/panel/identity/inde_input_name.png"));
    _inputName->setAnchorPoint(ccp(0, 0.5));
    _inputName->setMaxLength(11);
    _inputName->setFontColor(DISPLAY->defalutColor());
    _inputName->setFontName(DISPLAY->fangzhengFont());
    _inputName->setInputMode(kEditBoxInputModePhoneNumber);
    _inputName->setReturnType(kKeyboardReturnTypeDone);
    _inputName->setPosition(ccp(panelSize.width * 0.32, panelSize.height * 0.69));
    _panel->addChild(_inputName);
    
    // age
    _inputAge = CCEditBox::create(CCSizeMake(92, 40), CCScale9Sprite::create("pic/panel/identity/inde_input_name.png"));
    _inputAge->setAnchorPoint(ccp(0, 0.5));
    _inputAge->setMaxLength(11);
    _inputAge->setFontColor(DISPLAY->defalutColor());
    _inputAge->setFontName(DISPLAY->fangzhengFont());
    _inputAge->setInputMode(kEditBoxInputModePhoneNumber);
    _inputAge->setReturnType(kKeyboardReturnTypeDone);
    _inputAge->setPosition(ccp(panelSize.width * 0.68, _inputName->getPositionY()));
    _panel->addChild(_inputAge);
    
    
    // phone
    CCSize size_bar = CCSizeMake(190, 40);
    _inputPhone = CCEditBox::create(CCSizeMake(size_bar.width, size_bar.height), CCScale9Sprite::create("pic/panel/setting/nnr_bar.png"));
    _inputPhone->setAnchorPoint(ccp(0, 0.5));
    _inputPhone->setMaxLength(11);
    _inputPhone->setFontColor(DISPLAY->defalutColor());
    _inputPhone->setFontName(DISPLAY->fangzhengFont());
    _inputPhone->setInputMode(kEditBoxInputModePhoneNumber);
    _inputPhone->setReturnType(kKeyboardReturnTypeDone);
    _inputPhone->setPosition(ccp(_inputName->getPositionX(), panelSize.height * 0.35));
    _panel->addChild(_inputPhone);
    
    CCSprite* spt1 = CCSprite::create("pic/panel/identity/inde_btn_commit.png");
    CCSprite* spt2 = CCSprite::create("pic/panel/identity/inde_btn_commit.png");
    spt2->setScale(DISPLAY->btn_scale());
    CCMenuItem* btnCommit = CCMenuItemSprite::create(spt1, spt2, this, SEL_MenuHandler(&IdentityPanel::on_take_gift));
    CCMenu* menuCommit = CCMenu::createWithItem(btnCommit);
    menuCommit->ignoreAnchorPointForPosition(true);
    menuCommit->setPosition(ccp(panelSize.width * 0.5f, panelSize.height * 0.15f));
    _panel->addChild(menuCommit);
    
    CCLabelTTF* lblNum = CCLabelTTF::create("获得20", DISPLAY->fangzhengFont(), 15);
    lblNum->setAnchorPoint(ccp(0.5, 0.5));
    lblNum->setPosition(ccp(spt1->getContentSize().width * 0.5, spt1->getContentSize().height * 0.25));
    btnCommit->addChild(lblNum);
}

void IdentityPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->create_panel();
    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&IdentityPanel::keyBackStatus), .8f);
}
void IdentityPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void IdentityPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool IdentityPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
//        this->remove();
        this->do_exit();
    }
    
    return true;
}

#pragma mark - export

void IdentityPanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void IdentityPanel::do_enter() {
    _panel->setPosition(_enter_pos);
    _panel->setScale(0.3f);
    //
    float duration = 0.5f;
    CCEaseBounce* moveto = CCEaseBounce::create(CCMoveTo::create(duration, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.65)));
    CCEaseBounce* scaleto = CCEaseBounce::create(CCScaleTo::create(duration, 1.0));
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    _panel->runAction(CCEaseBounce::create(spawn));
}

void IdentityPanel::do_exit() {
    float duration = 0.5f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos);
    CCEaseBounce* seq = CCEaseBounce::create(CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&IdentityPanel::remove)), NULL));
    CCEaseBounce* scaleto = CCEaseBounce::create(CCScaleTo::create(duration, 0.3));
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _panel->runAction(spawn);
}

void IdentityPanel::remove() {
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void IdentityPanel::on_music(cocos2d::CCMenuItem *btn) {
    //    CCDirector::sharedDirector()->replaceScene(scene);
    CCMenuItemToggle* item = (CCMenuItemToggle*)btn;
    int index = item->getSelectedIndex();
    CCLOG("music getSelectedIndex = %d", index);
    AUDIO->set_music_on((bool)index);
}

void IdentityPanel::on_effect(cocos2d::CCMenuItem *btn) {
    CCMenuItemToggle* item = (CCMenuItemToggle*)btn;
    int index = item->getSelectedIndex();
    CCLOG("effect getSelectedIndex = %d", index);
    AUDIO->set_effect_on((bool)index);
}

void IdentityPanel::on_take_gift(CCMenuItem *btn) {
    GiftPanel* giftPanel = GiftPanel::create();
    this->getScene()->addChild(giftPanel);
    this->remove();
}

void IdentityPanel::keyBackClicked(){
    num_child++;
    CCLog("===== IdentityPanel  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}


