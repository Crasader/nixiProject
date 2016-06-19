//
//  LoginView.cpp
//  mm3c
//
//  Created by lakkey on 15-8-11.
//
//

#include "LoginView.h"
#include "DisplayManager.h"
#include "AccountValidate.h"
#include "PromptLayer.h"

enum {
    tag_tf_account = 101,
    tag_tf_password = 102,
};


LoginView::~LoginView()
{
    if (_validate) {
        delete _validate;
    }
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool LoginView::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->create_view();
    return true;
}

void LoginView::create_view()
{
    CCSprite* spt_inputbox = CCSprite::create("res/pic/loginScene/login_box.png");
//    spt_inputbox->setPosition(ccp(DISPLAY->W() * 0.5f, DISPLAY->H() * 1.4f));
    spt_inputbox->setPosition(ccp(DISPLAY->W() * 0.5f, DISPLAY->H() * 0.66f));
    this->addChild(spt_inputbox);
    
    CCSize boxsize = spt_inputbox->boundingBox().size;
    ccColor3B yanse = ccBLACK;
    ccColor3B yanse2 = ccc3(199, 199, 213);
    
    //
    CCSize size_bar = CCSizeMake(380, 44);
    _tf_account = CCEditBox::create(CCSizeMake(size_bar.width - 20, size_bar.height), CCScale9Sprite::create("res/pic/loginScene/99.png"));
    _tf_account->setMaxLength(16);
    _tf_account->setFontColor(yanse);
    _tf_account->setPlaceholderFontColor(yanse2);
    _tf_account->setPlaceHolder("点击输入账号");
    _tf_account->setFontName(DISPLAY->fangzhengFont());
    _tf_account->setInputMode(kEditBoxInputModeEmailAddr);
    _tf_account->setInputFlag(kEditBoxInputFlagSensitive);
    _tf_account->setReturnType(kKeyboardReturnTypeDone);
    _tf_account->setTag(tag_tf_account);
    _tf_account->setPosition(ccp(boxsize.width * 0.58, boxsize.height * 0.7));
    _tf_account->setDelegate(this);
    spt_inputbox->addChild(_tf_account);
    
    
    _tf_password = CCEditBox::create(CCSizeMake(size_bar.width - 20, size_bar.height), CCScale9Sprite::create("res/pic/loginScene/99.png"));
    _tf_password->setMaxLength(16);
    _tf_password->setFontColor(yanse);
    _tf_password->setPlaceholderFontColor(yanse2);
    _tf_password->setPlaceHolder("点击输入密码");
    _tf_password->setFontName(DISPLAY->fangzhengFont());
    _tf_password->setInputMode(kEditBoxInputModeEmailAddr);
    _tf_password->setReturnType(kKeyboardReturnTypeDone);
    _tf_password->setTag(tag_tf_password);
    _tf_password->setPosition(ccp(boxsize.width * 0.58, boxsize.height * 0.46));
    _tf_password->setDelegate(this);
    spt_inputbox->addChild(_tf_password);
    
    //
    CCSprite* login_normal = CCSprite::create("res/pic/loginScene/login_btn_login.png");
    CCSprite* login_effect = CCSprite::create("res/pic/loginScene/login_btn_login.png");
    login_effect->setScale(DISPLAY->btn_scale());
    CCMenuItem* btn_login = CCMenuItemSprite::create(login_normal, login_effect, this, SEL_MenuHandler(&LoginView::send_login_request));
    CCMenu* menu_login = CCMenu::createWithItem(btn_login);
    menu_login->setPosition(ccp(boxsize.width * 0.5, boxsize.height * 0.22));
    spt_inputbox->addChild(menu_login);
    
    CCSprite* regist_normal = CCSprite::create("res/pic/loginScene/login_btn_regist.png");
    CCSprite* regist_effect = CCSprite::create("res/pic/loginScene/login_btn_regist.png");
    regist_effect->setScale(DISPLAY->btn_scale());
    CCMenuItem* btn_regist = CCMenuItemSprite::create(regist_normal, regist_effect, this, SEL_MenuHandler(&LoginView::shift_to_registerview));
    CCMenu* menu_regist = CCMenu::createWithItem(btn_regist);
    menu_regist->setPosition(ccp(boxsize.width * 0.12, boxsize.height * 0.05));
    spt_inputbox->addChild(menu_regist);
    
    //
//    spt_inputbox->runAction(CCEaseExponentialOut::create(CCMoveTo::create(0.6f, ccp(DISPLAY->W() * 0.5f, DISPLAY->H() * 0.66f))));
}

void LoginView::editBoxEditingDidBegin(CCEditBox* editBox) {

}

void LoginView::editBoxEditingDidEnd(CCEditBox* editBox) {

}

void LoginView::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {

}

void LoginView::editBoxReturn(CCEditBox* editBox) {

}

void LoginView::shift_to_registerview(CCMenuItem* pSender) {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("shift_to_registerview");
}

bool LoginView::check_can_send_request()
{
    string account = _tf_account->getText();
    string password = _tf_password->getText();
    if (account.length() == 0 || password.length() == 0) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "账号密码不能为空");
        return false;
    }
//    else if (account.length() < 6) {
//        MMPromptLayer* prompt = MMPromptLayer::create();
//        this->addChild(prompt);
//        prompt->promptBox(ccs(def_waring_msg_103));
//        return false;
//    }
//    else if (password.length() < 6) {
//        MMPromptLayer* prompt = MMPromptLayer::create();
//        this->addChild(prompt);
//        prompt->promptBox(ccs(def_waring_msg_104));
//        return false;
//    }
    
    return true;
}

void LoginView::send_login_request(CCMenuItem* pSender)
{
    CCEGLView::sharedOpenGLView()->setIMEKeyboardState(false);
    
    if (this->check_can_send_request()) {
        CCDictionary* dic = CCDictionary::create();
        dic->setObject(ccs(_tf_account->getText()), "account");
        dic->setObject(ccs(_tf_password->getText()), "password");
        CCNotificationCenter::sharedNotificationCenter()->postNotification("start_login", dic);
    }
}



