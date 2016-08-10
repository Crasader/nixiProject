//
//  RecommendView.cpp
//  tiegao
//
//  Created by mac on 16/8/9.
//
//

#include "RecommendView.h"
//
//  RecommendView.cpp
//  mm3c
//
//  Created by lakkey on 14-12-31.
//
//

#include "RecommendView.h"
#include "DisplayManager.h"
#include "PromptLayer.h"

RecommendView::~RecommendView() {
}

bool RecommendView::init()
{
    if (! CCLayer::init()) {
        return false;
    }
    
    CCSprite* spt_inputbox = CCSprite::create("res/pic/loginScene/nickname_box.png");
    spt_inputbox->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.62f));
    //    spt_inputbox->setPosition(ccp(DISPLAY->W() * 0.5f, DISPLAY->H() * 1.4f));
    this->addChild(spt_inputbox);
    
    CCSize boxsize = spt_inputbox->boundingBox().size;
    ccColor3B yanse = ccWHITE;
    
    CCSize size_bar = CCSizeMake(380, 40);
    _input = CCEditBox::create(CCSizeMake(size_bar.width, size_bar.height), CCScale9Sprite::create("res/pic/loginScene/99.png"));
    _input->setMaxLength(30);
    _input->setFontColor(yanse);
    _input->setPlaceHolder("请输入昵称");
    _input->setFontName(DISPLAY->fangzhengFont());
    _input->setInputMode(kEditBoxInputModeEmailAddr);
    _input->setReturnType(kKeyboardReturnTypeDone);
    //    _input->setPosition(ccp(boxsize.width * 0.5, boxsize.height * 0.45));
    _input->setPosition(ccp(boxsize.width * 0.55, boxsize.height * 0.45));
    _input->setDelegate(this);
    _input->setFontSize(36.f);
    spt_inputbox->addChild(_input);
    
    CCSprite* spt_confirm_normal = CCSprite::create("res/pic/common/btn_confirm.png");
    CCSprite* spt_confirm_selected = CCSprite::create("res/pic/common/btn_confirm.png");
    spt_confirm_normal->setScale(1.1);
    spt_confirm_selected->setScale(spt_confirm_normal->getScale() * DISPLAY->btn_scale());
    CCMenuItem* btn_confirm = CCMenuItemSprite::create(spt_confirm_normal, spt_confirm_selected, this, SEL_MenuHandler(&RecommendView::on_btn_confirem));
    CCMenu* menu_confirm = CCMenu::createWithItem(btn_confirm);
    menu_confirm->setPosition(ccp(boxsize.width * 0.5f, boxsize.height * 0.16f));
    spt_inputbox->addChild(menu_confirm);
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    return true;
}

void RecommendView::onEnter()
{
    CCLayer::onEnter();
}

void RecommendView::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCEGLView::sharedOpenGLView()->setIMEKeyboardState(false);
    CCLayer::onExit();
}

void RecommendView::show()
{
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - CCEditBoxDelegate

void RecommendView::editBoxEditingDidBegin(CCEditBox* editBox) {
    //    _oldStr = string(editBox->getText());
}

void RecommendView::editBoxEditingDidEnd(CCEditBox* editBox) {
    
}

void RecommendView::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {
}

void RecommendView::editBoxReturn(CCEditBox* editBox) {
    
}


#pragma mark -

void RecommendView::on_btn_confirem(cocos2d::CCObject *pSender)
{
    _input->detachWithIME();

}

