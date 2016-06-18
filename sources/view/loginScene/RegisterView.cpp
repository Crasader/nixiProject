//
//  RegisterView.cpp
//  mm3c
//
//  Created by lakkey on 15-8-11.
//
//

#include "RegisterView.h"
#include "DisplayManager.h"
#include "AccountValidate.h"
#include "PromptLayer.h"

enum {
    tag_tf_account = 101,
    tag_tf_password = 102,
    tag_tf_password2 = 103,
};


RegisterView::~RegisterView()
{
    if (_validate) {
        delete _validate;
    }
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool RegisterView::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    _validate = new AccountValidate();
    
//    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&RegisterView::update_account_info), NOTIFICATION_9669_DONE, NULL);
    
    this->create_view();
    return true;
}

void RegisterView::create_view()
{
    CCSprite* spt_inputbox = CCSprite::create("res/pic/loginScene/regist_box.png");
    spt_inputbox->setPosition(ccp(DISPLAY->W() * 0.5f, DISPLAY->H() * 0.66f));
    this->addChild(spt_inputbox);
    
    CCSize boxsize = spt_inputbox->boundingBox().size;
    ccColor3B yanse = ccc3(199, 199, 213);
    
    //
    CCSize size_bar = CCSizeMake(380, 50);
    _tf_account = CCEditBox::create(CCSizeMake(size_bar.width - 20, size_bar.height), CCScale9Sprite::create("res/pic/loginScene/99.png"));
    _tf_account->setMaxLength(16);
    _tf_account->setFontColor(yanse);
    _tf_account->setPlaceHolder("请输入账号");
    _tf_account->setFontName(DISPLAY->font());
    _tf_account->setInputMode(kEditBoxInputModeEmailAddr);
    _tf_account->setReturnType(kKeyboardReturnTypeDone);
    _tf_account->setTag(tag_tf_account);
    _tf_account->setPosition(ccp(boxsize.width * 0.58, boxsize.height * 0.8));
    _tf_account->setDelegate(this);
    spt_inputbox->addChild(_tf_account);
    
    
    _tf_password = CCEditBox::create(CCSizeMake(size_bar.width - 20, size_bar.height), CCScale9Sprite::create("res/pic/loginScene/99.png"));
    _tf_password->setMaxLength(16);
    _tf_password->setFontColor(yanse);
    _tf_password->setPlaceHolder("请输入密码");
    _tf_password->setFontName(DISPLAY->font());
    _tf_password->setInputMode(kEditBoxInputModeEmailAddr);
    _tf_password->setReturnType(kKeyboardReturnTypeDone);
    _tf_password->setTag(tag_tf_password);
    _tf_password->setPosition(ccp(boxsize.width * 0.58, boxsize.height * 0.63));
    _tf_password->setDelegate(this);
    spt_inputbox->addChild(_tf_password);
    
    
    _tf_password2 = CCEditBox::create(CCSizeMake(size_bar.width - 20, size_bar.height), CCScale9Sprite::create("res/pic/loginScene/99.png"));
    _tf_password2->setMaxLength(16);
    _tf_password2->setFontColor(yanse);
    _tf_password2->setPlaceHolder("重复该密码");
    _tf_password2->setFontName(DISPLAY->fangzhengFont());
    _tf_password2->setInputMode(kEditBoxInputModeEmailAddr);
    _tf_password2->setReturnType(kKeyboardReturnTypeDone);
    _tf_password2->setTag(tag_tf_password2);
    _tf_password2->setPosition(ccp(boxsize.width * 0.58, boxsize.height * 0.45));
    _tf_password2->setDelegate(this);
    spt_inputbox->addChild(_tf_password2);

    //
    CCSprite* regist_normal = CCSprite::create("res/pic/loginScene/login_regist.png");
    CCSprite* regist_effect = CCSprite::create("res/pic/loginScene/login_regist.png");
    regist_effect->setScale(DISPLAY->btn_scale());
    CCMenuItem* btn_regist = CCMenuItemSprite::create(regist_normal, regist_effect, this, SEL_MenuHandler(&RegisterView::send_register_request));
    CCMenu* menu_regist = CCMenu::createWithItem(btn_regist);
    menu_regist->setPosition(ccp(boxsize.width * 0.5, boxsize.height * 0.22));
    spt_inputbox->addChild(menu_regist);
    
    CCSprite* login_normal = CCSprite::create("res/pic/loginScene/login_return.png");
    CCSprite* login_effect = CCSprite::create("res/pic/loginScene/login_return.png");
    login_effect->setScale(DISPLAY->btn_scale());
    CCMenuItem* btn_login = CCMenuItemSprite::create(login_normal, login_effect, this, SEL_MenuHandler(&RegisterView::shift_to_loginview));
    CCMenu* menu_login = CCMenu::createWithItem(btn_login);
    menu_login->setPosition(ccp(boxsize.width * 0.08, boxsize.height * 0.05));
    spt_inputbox->addChild(menu_login);
}

void RegisterView::editBoxEditingDidBegin(CCEditBox* editBox) {
    
}

void RegisterView::editBoxEditingDidEnd(CCEditBox* editBox) {
    
}

void RegisterView::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {

}

void RegisterView::editBoxReturn(CCEditBox* editBox) {
    
}

void RegisterView::update_account_info() {
//    _tf_account->setText(MMDataManager::get_instance()->getStrUsr().c_str());
//    _tf_password->setText(MMDataManager::get_instance()->getStrPwd().c_str());
//    _tf_password2->setText(MMDataManager::get_instance()->getStrPwd().c_str());
}

void RegisterView::shift_to_loginview(CCMenuItem* pSender) {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("shift_to_loginview");
}

bool RegisterView::check_can_send_request()
{
    /* 注册账号需满足：
        账号6-16位英文和数字
        密码为6-16位英文和数字
    */
    string account = _tf_account->getText();
    string password = _tf_password->getText();
    string password2 = _tf_password2->getText();
    
    if (!_validate->check_legal_text(account.c_str())) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this, "账号密码不能使用特殊字符及汉字");
        return false;
    }
    
    if (!_validate->check_legal_text(password.c_str())) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this, "账号密码不能使用特殊字符及汉字");
        return false;
    }
    
    if (!_validate->check_legal_text(password.c_str())) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this, "账号密码不能使用特殊字符及汉字");
        return false;
    }
    
    
    if (account.length() == 0 || password2.length() == 0) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this, "账号密码不能为空");
        return false;
    }
    else if (account.length() < 6 || account.length() > 16) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this, "账号为6－16位英文及数字");
        return false;
    }
    else if (password.length() < 6 || password.length() > 16) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this, "密码为6－16位英文及数字");
        return false;
    }
    else if (password.compare(password2) != 0) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this, "两次输入的密码不一致");
        return false;
    }
    
    return true;
}

void RegisterView::send_register_request(CCMenuItem* pSender)
{
    CCEGLView::sharedOpenGLView()->setIMEKeyboardState(false);
//    if (this->check_can_send_request()) {
//        std::string account = CCUserDefault::sharedUserDefault()->getStringForKey(kUDK_Usr_String);
//        if (account.empty() == false) {
//            AHMessageBox* mb = AHMessageBox::create_with_message("警告！之前登陆的账号会被顶替掉，如果不想丢弃，请先记录旧账号。", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
//            mb->setPosition(ccp(ScreenWidth * 0.5, ScreenHeight * 0.75));
//            this->getScene()->addChild(mb);
//        }
//        else {
//            this->message_box_did_selected_button(NULL, AH_BUTTON_TYPE_YESNO, AH_BUTTON_TAG_YES);
//        }
//    }
}

void RegisterView::start_regist() {
//    AHLoading::showLoading();
//    
//    MMDataManager::get_instance()->setStrSendUsr(_tf_account->getText());
//    MMDataManager::get_instance()->setStrSendPwd(_tf_password->getText());
//    
//    MMNetManager::get_instance()->http_9007_register(_tf_account->getText(), _tf_password->getText(), "");
}

//void RegisterView::message_box_did_selected_button(AHMessageBox *box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag) {
//    if (button_tag == AH_BUTTON_TAG_YES) {
//        PrivacyPolicyView* view = PrivacyPolicyView::create();
//        this->getParent()->addChild(view);
//    }
//    
//    if (box != NULL) {
//        box->animation_out();
//    }
//}

