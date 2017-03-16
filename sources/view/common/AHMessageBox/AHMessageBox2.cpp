//
//  AHMessageBox2.cpp
//  AnHeiShiJie_Self
//
//  Created by iLakkey on 13-9-7.
//
//

#include "AHMessageBox2.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "AppUtil.h"

AHMessageBox2::~AHMessageBox2()
{
    
}


bool AHMessageBox2::init()
{
    if (CCLayerColor::initWithColor(ccc4(0, 0, 0, 0))) {
        return true;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    m_main_menu = NULL;
    m_close_menu = NULL;
    
    return false;
}


AHMessageBox2* AHMessageBox2::create_with_message2(const char *message, cocos2d::CCObject *deleaget, AH_AVATAR_TYPE2 avatar_type, AH_BUTTON_TYPE2 button_type, bool has_close_btn)
{
    AHMessageBox2* rtn = AHMessageBox2::create();
    rtn->ignoreAnchorPointForPosition(false);
    rtn->setAnchorPoint(ccp(0.5f, 0.5f));
    
    rtn->m_has_close_btn = has_close_btn;
    rtn->m_item_yes = NULL;
    rtn->m_item_middle = NULL;
    rtn->m_item_no = NULL;
    
    if (deleaget) {
        rtn->_delegate = dynamic_cast<AHMessageBox2Delegate*>(deleaget);
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/common/ah_btn_done.plist");
    
    // 为动画准备
    rtn->setScale(0.2f);
    rtn->setTouchEnabled(true);
    rtn->setMessage(ccs(message));
    rtn->setAvatar_type(avatar_type);
    rtn->setButton_type(button_type);
    rtn->load_content();
    
    return rtn;
}


void AHMessageBox2::load_content()
{
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bgSpr = CCSprite::create("res/pic/salesPromotion/sp_bg.png");
    bgSpr->setPosition(ccp(win_size.width* .5f, win_size.height* .5f));
    this->addChild(bgSpr);
    
    CCSprite* box = CCSprite::create("res/pic/common/ah_box_message.png");
    //box->setPosition(ccp(win_size.width * 0.5f, win_size.height * 0.5f));
    box->setPosition(ccp(win_size.width * .5f, win_size.height* .5f));
    
    box->setAnchorPoint(ccp(0.5f, 0.5f));
    this->addChild(box, 2);
    
    CCSize box_size = box->boundingBox().size;
    
    // 文字
    m_lbl_msg = CCLabelTTF::create(_message->getCString(), DISPLAY->fangzhengFont(), 34.f, CCSizeMake(box_size.width - 60, box_size.height * 0.44f), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    m_lbl_msg->setPosition(ccp(box_size.width * 0.5f, box_size.height * 0.6f));
    //    m_lbl_msg->setColor(ccc3(125, 125, 125));
    m_lbl_msg->setColor(ccc3(102, 102, 154));
    box->addChild(m_lbl_msg);
    
    // close button
    if (m_has_close_btn) {
        CCMenuItemImage* item_close = CCMenuItemImage::create("res/pic/common/ah_guanbi.png", "res/pic/common/ah_guanbi.png", "res/pic/common/ah_guanbi.png", this, SEL_MenuHandler(&AHMessageBox2::on_menuitem_click));
        item_close->setTag(AH_BUTTON_TAG2_CLOSE);
        m_close_menu = CCMenu::create(item_close, NULL);
        box->addChild(m_close_menu);
        m_close_menu->setPosition(ccp(box_size.width - 40.f, box_size.height - 50.f));
    }
    
    //    CCMenu* menu = CCMenu::create();
    m_main_menu = CCMenu::create();
    
    switch (_button_type) {
        case AH_BUTTON_TYPE2_YES:
        case AH_BUTTON_TYPE2_YES2:
        case AH_BUTTON_TYPE2_YES3:
        case AH_BUTTON_TYPE2_YES4:{
            //            m_item_yes = CCMenuItemImage::create("res/pic/common/btn_confirm.png", "res/pic/common/btn_confirm.png", this, SEL_MenuHandler(&AHMessageBox2::on_menuitem_click));
            CCSprite* yes1 = CCSprite::create("res/pic/common/btn_confirm.png");
            CCSprite* yes2 = CCSprite::create("res/pic/common/btn_confirm.png");
            yes2->setScale(1.01);
            m_item_yes = CCMenuItemSprite::create(yes1, yes2, this, SEL_MenuHandler(&AHMessageBox2::on_menuitem_click));
            m_item_yes->setTag(AH_BUTTON_TAG2_YES);
            m_item_yes->setUserObject(ccs("res/pic/common/btn_confirm.png"));
            m_main_menu->addChild(m_item_yes);
            
            //确定特效
            CCSprite* btnSpr = CCSprite::createWithSpriteFrameName("ah_btn_done1.png");
            btnSpr->setAnchorPoint(CCPointZero);
            btnSpr->setPosition(CCPointZero);
            CCAnimation* anim_btn = AppUtil::animationWithFrame("ah_btn_done%d.png", 8, 1, .1f);
            CCSequence* seq_btn = CCSequence::create(CCAnimate::create(anim_btn), CCDelayTime::create(1.5f), NULL);
            btnSpr->runAction(CCRepeatForever::create(seq_btn));
            m_item_yes->addChild(btnSpr);
        } break;
        
        case AH_BUTTON_TYPE2_YESNO:
        case AH_BUTTON_TYPE2_YESNO2:
        case AH_BUTTON_TYPE2_YESNO3:
        case AH_BUTTON_TYPE2_YESNO4:
        case AH_BUTTON_TYPE2_YESNO5:{
            //            m_item_no = CCMenuItemImage::create("res/pic/common/ah_btn_cancel_normal.png", "res/pic/common/ah_btn_cancel_selected.png", this, SEL_MenuHandler(&AHMessageBox2::on_menuitem_click));
            CCSprite* no1 = CCSprite::create("res/pic/salesPromotion/sp_no.png");
            CCSprite* no2 = CCSprite::create("res/pic/salesPromotion/sp_no.png");
            no2->setScale(1.01);
            m_item_no = CCMenuItemSprite::create(no1, no2, this, SEL_MenuHandler(&AHMessageBox2::on_menuitem_click));
            m_item_no->setTag(AH_BUTTON_TAG2_NO);
            m_item_no->setUserObject(ccs("res/pic/salesPromotion/sp_no.png"));
            m_main_menu->addChild(m_item_no);
            
            //            m_item_yes = CCMenuItemImage::create("res/pic/common/btn_confirm.png", "res/pic/common/btn_confirm.png", this, SEL_MenuHandler(&AHMessageBox2::on_menuitem_click));
            CCSprite* yes1 = CCSprite::create("res/pic/salesPromotion/sp_yes.png");
            CCSprite* yes2 = CCSprite::create("res/pic/salesPromotion/sp_yes.png");
            yes2->setScale(1.01);
            m_item_yes = CCMenuItemSprite::create(yes1, yes2, this, SEL_MenuHandler(&AHMessageBox2::on_menuitem_click));
            m_item_yes->setTag(AH_BUTTON_TAG2_YES);
            m_item_yes->setUserObject(ccs("res/pic/salesPromotion/sp_yes.png"));
            m_main_menu->addChild(m_item_yes);
            
        } break;
        
        default:
        break;
    }
    
    m_main_menu->alignItemsHorizontallyWithPadding(80.f);
    m_main_menu->setPosition(ccp(box_size.width * 0.5f, box_size.height * 0.2f));
    box->addChild(m_main_menu);
}


//void AHMessageBox2::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, AHMessageBox2_TouchPriority, true);
//}


void AHMessageBox2::change_menu_priority()
{
    if (m_main_menu) {
        //        m_main_menu->setHandlerPriority(AHMessageBox2_TouchPriority - 1);
    }
    
    if (m_close_menu) {
        //        m_close_menu->setTouchPriority(AHMessageBox2_TouchPriority - 1);
    }
}


void AHMessageBox2::onEnter()
{
    
    CCLayerColor::onEnter();
    
    // 进入动画
    CCScaleTo* animation_in = CCScaleTo::create(.3f, 1.2f);
    CCScaleTo* animation_out = CCScaleTo::create(.1f, 1.f);
    CCSequence* seq = CCSequence::create(animation_in, animation_out, NULL);
    this->runAction(CCEaseExponentialOut::create(seq));
    this->scheduleOnce(SEL_SCHEDULE(&AHMessageBox2::change_menu_priority), 0.1f);
}


void AHMessageBox2::onExit()
{
    setTouchEnabled(false);
    
    CCLayerColor::onExit();
}


void AHMessageBox2::on_menuitem_click(CCObject* pSender)
{
    CCMenuItem* item = dynamic_cast<CCMenuItem*>(pSender);
    switch (this->getButton_type()) {
        case AH_BUTTON_TYPE2_YES:
        case AH_BUTTON_TYPE2_YES2:
        case AH_BUTTON_TYPE2_YES3:
        case AH_BUTTON_TYPE2_YES4:
        case AH_BUTTON_TYPE2_YESNO:
        case AH_BUTTON_TYPE2_YESNO2:
        case AH_BUTTON_TYPE2_YESNO3:
        case AH_BUTTON_TYPE2_YESNO4:
        case AH_BUTTON_TYPE2_YESNO5: {
            if (item->getTag() == AH_BUTTON_TAG2_YES) {
                // 确认音效
                AUDIO->comfirm_effect();
            }
            else if (item->getTag() == AH_BUTTON_TAG2_NO) {
                // 取消音效
                AUDIO->common_effect();
            }
        } break;
        
        default:
        break;
    }
    
    if (_delegate) {
        _delegate->message_box_did_selected_button2(this, _button_type, (AH_BUTTON_TAGS2)item->getTag());
    }
    else {
        // 若无代理，直接移除box
        AHMessageBox2::animation_out();
    }
}


bool AHMessageBox2::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //    MZLog("Touch in AHMessageBox2.");
    return true;
}


void AHMessageBox2::clean_box()
{
    CCLog("AHMessageBox2::clean_box()");
    this->removeFromParentAndCleanup(true);
}


void AHMessageBox2::animation_out()
{
    // 退出动画
    CCCallFunc* stopClick = CCCallFunc::create(this, SEL_CallFunc(&AHMessageBox2::stopClick));
    CCScaleTo* animation_in = CCScaleTo::create(.06f, 1.2f);
    CCScaleTo* animation_out = CCScaleTo::create(.2f, .2f);
    CCCallFunc* do_quit = CCCallFunc::create(this, SEL_CallFunc(&AHMessageBox2::on_out));
    CCCallFunc* openClick = CCCallFunc::create(this, SEL_CallFunc(&AHMessageBox2::stopClick));
    CCSequence* seq1 = CCSequence::create(animation_in, animation_out, NULL);
    CCSequence* seq2 = CCSequence::create(stopClick,CCEaseExponentialIn::create(seq1), do_quit,openClick, NULL);
    this->runAction(seq2);
}


void AHMessageBox2::on_out()
{
    AHMessageBox2::clean_box();
}
void AHMessageBox2::stopClick()
{
    
    if (m_item_yes != NULL) {
        m_item_yes->setEnabled(false);
    }
    if (m_item_no != NULL) {
        m_item_no->setEnabled(false);
    }
}
void AHMessageBox2::openClick()
{
    if (m_item_yes != NULL) {
        m_item_yes->setEnabled(true);
    }
    if (m_item_no != NULL) {
        m_item_no->setEnabled(true);
    }
}



