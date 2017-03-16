//
//  CreateName.cpp
//  mm3c
//
//  Created by lakkey on 14-12-31.
//
//

#include "CreateName.h"
#include "DisplayManager.h"
#include "NetManager.h"
#include "DataManager.h"
//#include "AHMessageBox.h"
#include "FileManager.h"

#include "Loading2.h"
#include "PromptLayer.h"

CreateName::~CreateName() {
}

bool CreateName::init()
{
    if (!CCScene::init()) {
        return false;
    }
    
    // talkingData
    DATA->onEvent("启动事件", "昵称界面", "进入昵称界面");
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    return true;
}

void CreateName::onEnter()
{
    CCScene::onEnter();
    
    this->create_baes_view();
    this->create_content_view();
    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&CreateName::show_chooselayer), NOTIFICATION_LOGIN_COMPLETED, NULL);
}

void CreateName::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCEGLView::sharedOpenGLView()->setIMEKeyboardState(false);
    CCScene::onExit();
}

void CreateName::show()
{
    //    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
    CCDirector::sharedDirector()->replaceScene(this);
}

void CreateName::create_baes_view()
{
    CCSprite* spt_inputbox = CCSprite::create("res/pic/loginScene/nickname_box.png");
    spt_inputbox->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.62f));
//    spt_inputbox->setPosition(ccp(DISPLAY->W() * 0.5f, DISPLAY->H() * 1.4f));
    this->addChild(spt_inputbox);
    
    CCSize boxsize = spt_inputbox->boundingBox().size;
    ccColor3B yanse = ccWHITE;
    
    //
    CCLabelTTF* lbl_content = CCLabelTTF::create("魅力与智慧并存的总裁大人,我该如何称呼您?", DISPLAY->fangzhengFont(), 26, CCSizeMake(340, 80), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    lbl_content->setColor(ccc3(128, 128, 155));
    lbl_content->setPosition(ccp(boxsize.width * 0.5, boxsize.height * 0.68));
    spt_inputbox->addChild(lbl_content);
    
    //
    CCSize size_bar = CCSizeMake(380, 44);
    _tf_nickname = CCEditBox::create(CCSizeMake(size_bar.width, size_bar.height), CCScale9Sprite::create("res/pic/loginScene/99.png"));
    _tf_nickname->setMaxLength(12);
    _tf_nickname->setFontColor(yanse);
    _tf_nickname->setPlaceHolder("请输入昵称");
    _tf_nickname->setFontName(DISPLAY->fangzhengFont());
    _tf_nickname->setInputMode(kEditBoxInputModeEmailAddr);
    _tf_nickname->setReturnType(kKeyboardReturnTypeDone);
//    _tf_nickname->setPosition(ccp(boxsize.width * 0.5, boxsize.height * 0.45));
    _tf_nickname->setPosition(ccp(boxsize.width * 0.55, boxsize.height * 0.45));
    _tf_nickname->setDelegate(this);
    _tf_nickname->setFontSize(36.f);
    CCString* rand_name = CCString::createWithFormat("游客%s", DATA->getLogin()->obtain_sid());
    _tf_nickname->setText(rand_name->getCString());
    spt_inputbox->addChild(_tf_nickname);
    
    CCLabelTTF* warning = CCLabelTTF::create("游戏中有一次免费改名的机会!", DISPLAY->fangzhengFont(), 24);
    warning->setAnchorPoint(ccp(0.5, 0.5));
    warning->setPosition(ccp(boxsize.width * 0.5, boxsize.height * 0.32));
    warning->setColor(DISPLAY->defalutColor());
    spt_inputbox->addChild(warning);
    
    // rand
    /*
    CCSprite* randNormal = CCSprite::create("res/pic/loginScene/btn_random.png");
    CCSprite* randSelected = CCSprite::create("res/pic/loginScene/btn_random.png");
    randSelected->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* rand = CCMenuItemSprite::create(randNormal, randSelected, this, SEL_MenuHandler(&CreateName::randName));
    CCMenu* randMenu = CCMenu::createWithItem(rand);
    randMenu->setPosition(ccp(boxsize.width * 0.86, _tf_nickname->getPositionY() - 6));
    spt_inputbox->addChild(randMenu);
    */
    //
    CCSprite* spt_confirm_normal = CCSprite::create("res/pic/common/btn_confirm.png");
    CCSprite* spt_confirm_selected = CCSprite::create("res/pic/common/btn_confirm.png");
    spt_confirm_normal->setScale(1.1);
    spt_confirm_selected->setScale(spt_confirm_normal->getScale() * DISPLAY->btn_scale());
    CCMenuItem* btn_confirm = CCMenuItemSprite::create(spt_confirm_normal, spt_confirm_selected, this, SEL_MenuHandler(&CreateName::on_btn_confirem));
    CCMenu* menu_confirm = CCMenu::createWithItem(btn_confirm);
    menu_confirm->setPosition(ccp(boxsize.width * 0.5f, boxsize.height * 0.16f));
    spt_inputbox->addChild(menu_confirm);
    
    //
//    spt_inputbox->runAction(CCEaseExponentialOut::create(CCMoveTo::create(0.6f, ccp(DISPLAY->W() * 0.5f, DISPLAY->H() * 0.66f))));
    
    
    struct timeval star;
    struct tm* time;
    gettimeofday(&star,  NULL);
    time = localtime(&star.tv_sec);
    CCUserDefault::sharedUserDefault()->setStringForKey("Tm_MDay", CCString::createWithFormat("%d", time->tm_mday)->getCString());
    CCUserDefault::sharedUserDefault()->setStringForKey("Tm_One", CCString::createWithFormat("%d", 0)->getCString());
    CCUserDefault::sharedUserDefault()->setStringForKey("Tm_Ten", CCString::createWithFormat("%d", 0)->getCString());
}

void CreateName::create_content_view()
{
    
}

bool CreateName::check_nickname(std::string str)
{
    int lenLimit = 10; // 昵称总长限制
    
    if (str.empty()) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称不能为空");
        return false;
    }else {
        char nameChar[128];
        strcpy(nameChar, str.c_str());
        CCLOG("nameChar == %s", nameChar);
        
        int zhongwen = 0;
        int yingwen = 0;
        bool zhongBool = false;
        bool yingBool = false;
        
        size_t i = 0, nlen = strlen(nameChar);
        for (; i<nlen; i++) {
            if (nameChar[i] >= 0 && nameChar[i] <= 127) {
                yingwen++;
                yingBool = true;
            }else{
                zhongwen++;
                zhongBool = true;
            }
        }
        
        int zhongWenCount = zhongwen / 3;
        CCLOG("中文== %d个 英文== %d个", zhongWenCount, yingwen);
        
        if (zhongWenCount + yingwen == 0) { // 输入为空
            PromptLayer* prompt = PromptLayer::create();
            prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称错误,请重新输入");
            return false;
        }
        
        if (zhongwen % 3 != 0) {
            PromptLayer* prompt = PromptLayer::create();
            prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称中含有非法字符,请重新输入");
            return false;
        }
        
        if (zhongBool && (yingBool == false)) { // 只有中文
            if (zhongWenCount + yingwen <= lenLimit) {
                if (FILEM->is_illegal(nameChar) == false) {
                    return true;
                }else{
                    PromptLayer* prompt = PromptLayer::create();
                    prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称中含有非法字符,请重新输入");
                    return false;
                }
            }else{
                PromptLayer* prompt = PromptLayer::create();
                prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称过长, 中文不能超过六个汉字~!");
                return false;
            }
        }
        else if (yingBool && (zhongBool == false)){ // 只有英文和数字
            if (zhongWenCount + yingwen <= lenLimit) {
                if (FILEM->is_illegal(nameChar) == false) {
                    return true;
                }else{
                    PromptLayer* prompt = PromptLayer::create();
                    prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称中含有非法字符,请重新输入");
                    return false;
                }
            }else{
                PromptLayer* prompt = PromptLayer::create();
                prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称过长, 中文不能超过六个汉字~!");
                return false;
            }
        }else if (zhongBool && yingBool){ // 混合英文、数字和中文
            if (zhongWenCount + yingwen <= lenLimit) {
                if (FILEM->is_illegal(nameChar) == false) {
                    return true;
                }else{
                    PromptLayer* prompt = PromptLayer::create();
                    prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称中含有非法字符,请重新输入");
                    return false;
                }
            }else{
                PromptLayer* prompt = PromptLayer::create();
                prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称过长, 中文不能超过六个汉字~!");
                return false;
            }
        }
    }
    
    return false;
}

#pragma mark - CCEditBoxDelegate

void CreateName::editBoxEditingDidBegin(CCEditBox* editBox) {
//    _oldStr = string(editBox->getText());
}

void CreateName::editBoxEditingDidEnd(CCEditBox* editBox) {
    
}

void CreateName::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {
}

void CreateName::editBoxReturn(CCEditBox* editBox) {
    
}


#pragma mark -

void CreateName::on_btn_confirem(cocos2d::CCObject *pSender)
{
    DATA->setInit_TalkingBool(true);
    
    _tf_nickname->detachWithIME();
    if (this->check_nickname(_tf_nickname->getText())) {
        LOADING->show_loading();
        NET->save_nickname_904(_tf_nickname->getText());
    }
}

void CreateName::randName(CCNode* pSender) {
//            int sex = _item_male->getSelectedIndex() == 1 ? 1 : 2;
//            string randName = _creator->createName(sex);
//            _tf_nickname->setText(randName.c_str());
//            _oldStr = string(_tf_nickname->getText());
}

