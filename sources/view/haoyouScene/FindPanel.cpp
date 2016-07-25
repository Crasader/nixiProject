//
//  FindPanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/21.
//
//

#include "FindPanel.h"
#include "DisplayManager.h"
#include "FileManager.h"
#include "NetManager.h"
#include "Loading2.h"

#include "PromptLayer.h"
#include "ShowComp.h"

FindPanel::~FindPanel(){
}

bool FindPanel::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->initView();
    return true;
}

void FindPanel::initView(){
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    find_panel = CCSprite::create("res/pic/haoyoupaihang/find_kuang.png");
    find_panel->setPosition(DISPLAY->center());
    this->addChild(find_panel);

    _input_name = CCEditBox::create(CCSizeMake(300, 40), CCScale9Sprite::create("res/pic/loginScene/99.png"));
    _input_name->setMaxLength(30);
    _input_name->setFontColor(ccWHITE);
    _input_name->setPlaceHolder("请输入玩家昵称");
    _input_name->setFontName(DISPLAY->fangzhengFont());
    _input_name->setInputMode(kEditBoxInputModeEmailAddr);
    _input_name->setReturnType(kKeyboardReturnTypeDone);
    _input_name->setPosition(ccp(find_panel->getContentSize().width/2, find_panel->getContentSize().height * .54));
    _input_name->setDelegate(this);
//    _input_name->setFontSize(36.f);
    find_panel->addChild(_input_name);
    
    
    
    CCSprite* find_spr = CCSprite::create("res/pic/common/btn_find.png");
    CCSprite* find_spr2 = CCSprite::create("res/pic/common/btn_find.png");
    find_spr2->setScale(1.02f);
    CCMenuItemSprite* item_find = CCMenuItemSprite::create(find_spr, find_spr2, this, menu_selector(FindPanel::btn_find_callback));
    item_find->setPosition(ccp(find_panel->getContentSize().width/2 - 90, find_panel->getContentSize().height*.2f));
    CCMenu* menu_find = CCMenu::create(item_find, NULL);
    menu_find->setPosition(CCPointZero);
    find_panel->addChild(menu_find);
    
    CCSprite* back_spr = CCSprite::create("res/pic/common/btn_goback1.png");
    CCSprite* back_spr2 = CCSprite::create("res/pic/common/btn_goback1.png");
    back_spr2->setScale(1.02f);
    CCMenuItemSprite* item_back = CCMenuItemSprite::create(back_spr, back_spr2, this, menu_selector(FindPanel::btn_back_callback));
    item_back->setPosition(ccp(find_panel->getContentSize().width/2 + 90, find_panel->getContentSize().height*.2f));
    CCMenu* menu_back = CCMenu::create(item_back, NULL);
    menu_back->setPosition(CCPointZero);
    find_panel->addChild(menu_back);
}

void FindPanel::btn_find_callback(){    
    _input_name->detachWithIME();
    if (this->check_nickname(_input_name->getText())) {
        LOADING->show_loading();
        NET->search_other_801(_input_name->getText());
    }
}

void FindPanel::find_callback_801(CCObject* obj){
    CCString* id = (CCString*)obj;
    NET->send_message_803(id->getCString(), 1);
    LOADING->remove();
//    this->removeFromParentAndCleanup(true);
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "好友请求发送成功");
}

void FindPanel::btn_back_callback(){
    this->removeFromParentAndCleanup(true);
}

void FindPanel::onEnter(){
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&FindPanel::find_callback_801), "HTTP_FINISHED_801", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&FindPanel::keyBackStatus), .8f);
}
void FindPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void FindPanel::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCLayer::onExit();
}

bool FindPanel::check_nickname(std::string str)
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
                prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称过长, 不能超过10位英文或汉字");
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
                prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称过长, 不能超过10位英文或汉字");
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
                prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "昵称过长, 不能超过10位英文或汉字");
                return false;
            }
        }
    }
    
    return false;
}

#pragma mark - CCEditBoxDelegate

void FindPanel::editBoxEditingDidBegin(CCEditBox* editBox) {
    //    _oldStr = string(editBox->getText());
}

void FindPanel::editBoxEditingDidEnd(CCEditBox* editBox) {
    
}

void FindPanel::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {
}

void FindPanel::editBoxReturn(CCEditBox* editBox) {
    
}

void FindPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->btn_back_callback();
}

