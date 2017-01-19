//
//  IdentityPanel.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "IdentityPanel.h"
#include "DisplayManager.h"
#include "NetManager.h"
#include "DataManager.h"

#include "Loading2.h"
#include "PromptLayer.h"


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
    
    _sex = 0;
    int num = DATA->getPlayer()->identityDiamReward;
    
    // 提示
    CCString* strTip = CCString::createWithFormat("完善资料即可获得%d", num);
    CCLabelTTF* lblTip = CCLabelTTF::create(strTip->getCString(), DISPLAY->fangzhengFont(), 19.f);
    lblTip->setColor(ccc3(255, 69, 143));
    lblTip->setAnchorPoint(ccp(0, 0.5));
    lblTip->setPosition(ccp(panelSize.width * 0.3, panelSize.height * 0.84));
    _panel->addChild(lblTip);
    //
    CCSprite* diamIcon1 = CCSprite::create("pic/panel/identity/inde_diam_1.png");
    diamIcon1->setAnchorPoint(ccp(0, 0.5));
    diamIcon1->setPosition(lblTip->getPosition() + ccp(lblTip->getContentSize().width + 1.5, 4));
    _panel->addChild(diamIcon1);
    
    //
    CCMenuItemImage* btnFemaleNor = CCMenuItemImage::create("pic/panel/identity/inde_female_nor.png", "pic/panel/identity/inde_female_nor.png");
    CCMenuItemImage* btnFemaleSlt = CCMenuItemImage::create("pic/panel/identity/inde_female_slt.png", "pic/panel/identity/inde_female_slt.png");
    _btnFemale = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&IdentityPanel::onCheckSex), btnFemaleNor, btnFemaleSlt, NULL);
    _btnFemale->setTag(2);
    
    CCMenuItemImage* btnMaleNor = CCMenuItemImage::create("pic/panel/identity/inde_male_nor.png", "pic/panel/identity/inde_male_nor.png");
    CCMenuItemImage* btnMaleSlt = CCMenuItemImage::create("pic/panel/identity/inde_male_slt.png", "pic/panel/identity/inde_male_slt.png");
    _btnMale = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&IdentityPanel::onCheckSex), btnMaleNor, btnMaleSlt, NULL);
    _btnMale->setTag(1);
    
    CCMenu* menu = CCMenu::create(_btnFemale, _btnMale, NULL);
    menu->setPosition(ccp(panelSize.width * 0.6, panelSize.height * 0.53));
    menu->alignItemsHorizontallyWithPadding(panelSize.width * 0.14);
    _panel->addChild(menu);
    
    // name
    _inputName = CCEditBox::create(CCSizeMake(92, 32), CCScale9Sprite::create("pic/panel/identity/inde_input_name.png"));
    _inputName->setAnchorPoint(ccp(0, 0.5));
    _inputName->setMaxLength(12);
    _inputName->setFontColor(DISPLAY->defalutColor());
    _inputName->setFontName(DISPLAY->fangzhengFont());
    _inputName->setInputMode(kEditBoxInputModeAny);
    _inputName->setReturnType(kKeyboardReturnTypeDone);
    _inputName->setPosition(ccp(panelSize.width * 0.32, panelSize.height * 0.69));
    _panel->addChild(_inputName);
    
    // age
    _inputAge = CCEditBox::create(CCSizeMake(92, 32), CCScale9Sprite::create("pic/panel/identity/inde_input_age.png"));
    _inputAge->setAnchorPoint(ccp(0, 0.5));
    _inputAge->setMaxLength(2);
    _inputAge->setFontColor(DISPLAY->defalutColor());
    _inputAge->setFontName(DISPLAY->fangzhengFont());
    _inputAge->setInputMode(kEditBoxInputModePhoneNumber);
    _inputAge->setReturnType(kKeyboardReturnTypeDone);
    _inputAge->setPosition(ccp(panelSize.width * 0.68, _inputName->getPositionY()));
    _panel->addChild(_inputAge);
    
    
    // phone
    CCSize size_bar = CCSizeMake(190, 32);
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
    CCMenuItem* btnCommit = CCMenuItemSprite::create(spt1, spt2, this, SEL_MenuHandler(&IdentityPanel::onCommit));
    CCMenu* menuCommit = CCMenu::createWithItem(btnCommit);
    menuCommit->setPosition(ccp(panelSize.width * 0.5f, panelSize.height * 0.15f));
    _panel->addChild(menuCommit);
    
    CCString* strNum = CCString::createWithFormat("获得%d", num);
    CCLabelTTF* lblNum = CCLabelTTF::create(strNum->getCString(), DISPLAY->fangzhengFont(), 15);
    lblNum->setAnchorPoint(ccp(1, 0.5));
    lblNum->setPosition(ccp(spt1->getContentSize().width * 0.7, spt1->getContentSize().height * 0.25));
    btnCommit->addChild(lblNum);
    //
    CCSprite* diamIcon2 = CCSprite::create("pic/panel/identity/inde_diam_2.png");
    diamIcon2->setAnchorPoint(ccp(0, 0.5));
    diamIcon2->setPosition(lblNum->getPosition() + ccp(1, 3));
    btnCommit->addChild(diamIcon2);
}

void IdentityPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&IdentityPanel::nc_commit_identity_325), "HTTP_FINISHED_325", NULL);
    
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
    CCEaseElasticOut* moveto = CCEaseElasticOut::create(CCMoveTo::create(duration, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.6)));
    CCEaseElasticOut* scaleto = CCEaseElasticOut::create(CCScaleTo::create(duration, 1.0));
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    _panel->runAction(CCEaseBounce::create(spawn));
}

void IdentityPanel::do_exit() {
    float duration = 0.5f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos);
    CCEaseElasticOut* seq = CCEaseElasticOut::create(CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&IdentityPanel::remove)), NULL));
    CCEaseElasticOut* scaleto = CCEaseElasticOut::create(CCScaleTo::create(duration, 0.3));
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _panel->runAction(spawn);
}

void IdentityPanel::remove() {
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

bool IdentityPanel::checkFilled() {
    return (this->checkName() && this->checkAge() && this->checkSex() && this->checkPhone());
}

bool IdentityPanel::checkName() {
    bool rtn = true;
    string text = _inputName->getText();
    size_t len = text.length();
    
    if (text.empty() || len == 0) {
        rtn = false;
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "姓名不能为空~!");
    }
    else if (len > 12 || len % 3 != 0) {
        rtn = false;
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "请输入正确的姓名~!");
    }

    for (size_t i = 0; i<len; i++) {
        if (text[i] >= 0 && text[i] <= 127) {
            rtn = false;
            PromptLayer* prompt = PromptLayer::create();
            prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "姓名错误, 请检查后重新输入~!");
            break;
        }
    }

    return rtn;
}

bool IdentityPanel::checkAge() {
    bool rtn = true;
    string text = _inputAge->getText();
    size_t len = text.length();
    
    if (text.empty() || len == 0 || len > 3) {
        rtn = false;
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "请输入正确的年龄~!");
    }
    
    for (size_t i = 0; i<len; i++) {
        CCLOG("text[i] = %c", text[i]);
        if (text[i] < '0' || text[i] > '9') {
            rtn = false;
            PromptLayer* prompt = PromptLayer::create();
            prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "年龄错误, 请检查后重新输入~!");
            break;
        }
    }
    
    return rtn;
}

bool IdentityPanel::checkSex() {
    if (_sex == 1 || _sex == 2) {
        return true;
    }
    else {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "请选择您的性别~!");
        return false;
    }
}

bool IdentityPanel::checkPhone() {
    bool rtn = true;
    string text = _inputPhone->getText();
    size_t len = text.length();
    
    
    if (text.empty() || len == 0 || len != 11) {
        rtn = false;
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "请输入正确的手机号码~!");
        return rtn;
    }
    
    for (size_t i = 0; i<len; i++) {
        if (text[i] < '0' || text[i] > '9') {
            rtn = false;
            PromptLayer* prompt = PromptLayer::create();
            prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "手机号码错误, 请检查后重新输入~!");
            return rtn;
        }
    }
    
    
    string tempText = text.substr(0, 3);
    CCArray* phoneArr = creat_phoneArr();
    for (int i = 0; i < phoneArr->count(); i++) {
        CCString* str = (CCString* )phoneArr->objectAtIndex(i);
        if (strcmp(str->getCString(), tempText.c_str()) == 0){
        }else{
            rtn = false;
            PromptLayer* prompt = PromptLayer::create();
            prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "请输入正确的手机号码~!");
            return rtn;
        }
    }
    
    return rtn;
}

void IdentityPanel::onCheckSex(cocos2d::CCMenuItem *btn) {
    CCMenuItemToggle* item = (CCMenuItemToggle*)btn;
    if (item->getSelectedIndex() == 1) {
        item->setEnabled(false);
        _sex = item->getTag();
        if (_sex == 1) {
            _btnFemale->setSelectedIndex(0);
            _btnFemale->setEnabled(true);
        }
        else if (_sex == 2) {
            _btnMale->setSelectedIndex(0);
            _btnMale->setEnabled(true);
        }
    }
}

void IdentityPanel::onCommit(CCMenuItem *btn) {
    if (this->checkFilled()) {
        LOADING->show_loading();
        NET->commit_identity_325();
    }
}

void IdentityPanel::nc_commit_identity_325(CCObject *pObj) {
    LOADING->remove();
    
    CCDictionary* reward = dynamic_cast<CCDictionary*>(pObj);
    CCString* type = (CCString* )reward->valueForKey("type");
    CCInteger* num = (CCInteger*)reward->objectForKey("num");
    
    CCDictionary* postData = CCDictionary::create();
    postData->setObject(CCInteger::create(num->getValue()), "num");
    CCString* from = CCString::createWithFormat("{%f,%f}", DISPLAY->halfW(), DISPLAY->halfH());
    CCLOG("from -- %s", from->getCString());
    postData->setObject(from, "from");
    
    if (type->compare("coin") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_COIN_FLY", postData);
    }
    else if (type->compare("diam") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_GOLD_FLY", postData);
    }
    else if (type->compare("energy") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_ENERGY_FLY", postData);
    }
    else if (type->compare("piece") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_PIECE_FLY", postData);
    }
    
    this->do_exit();
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


CCArray* IdentityPanel::creat_phoneArr(){
    CCArray* phoneArr = CCArray::createWithCapacity(50);
    phoneArr->addObject(ccs("139"));
    phoneArr->addObject(ccs("138"));
    phoneArr->addObject(ccs("137"));
    phoneArr->addObject(ccs("136"));
    phoneArr->addObject(ccs("135"));
    phoneArr->addObject(ccs("134"));
    phoneArr->addObject(ccs("147"));
    phoneArr->addObject(ccs("188"));
    phoneArr->addObject(ccs("187"));
    phoneArr->addObject(ccs("184"));
    phoneArr->addObject(ccs("183"));
    phoneArr->addObject(ccs("182"));
    phoneArr->addObject(ccs("170"));
    phoneArr->addObject(ccs("178"));
    phoneArr->addObject(ccs("159"));
    phoneArr->addObject(ccs("158"));
    phoneArr->addObject(ccs("157"));
    phoneArr->addObject(ccs("152"));
    phoneArr->addObject(ccs("151"));
    phoneArr->addObject(ccs("150"));
    phoneArr->addObject(ccs("139"));
    
    phoneArr->addObject(ccs("186"));
    phoneArr->addObject(ccs("185"));
    phoneArr->addObject(ccs("176"));
    phoneArr->addObject(ccs("145"));
    phoneArr->addObject(ccs("156"));
    phoneArr->addObject(ccs("155"));
    phoneArr->addObject(ccs("132"));
    phoneArr->addObject(ccs("131"));
    phoneArr->addObject(ccs("130"));
    phoneArr->addObject(ccs("186"));
    
    phoneArr->addObject(ccs("189"));
    phoneArr->addObject(ccs("181"));
    phoneArr->addObject(ccs("180"));
    phoneArr->addObject(ccs("177"));
    phoneArr->addObject(ccs("153"));
    phoneArr->addObject(ccs("133"));
    phoneArr->addObject(ccs("189"));
    
    return phoneArr;
}
