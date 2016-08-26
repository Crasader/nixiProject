//
//  TotalRechargePanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/8/5.
//
//

#include "TotalRechargePanel.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "OperationComp.h"
#include "Loading2.h"
#include "NetManager.h"
#include "PromptLayer.h"
#include "Shower.h"


#pragma mark - Export

void TotalRechargePanel::show() {
    TotalRechargePanel* panel = TotalRechargePanel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}


#pragma mark - Super

TotalRechargePanel::~TotalRechargePanel() {
}

bool TotalRechargePanel::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    _content = CCLayer::create();
    this->addChild(_content);
    
    _panel = CCSprite::create("pic/panel/totalRecharge/recharge_panel.png");
    _panel->setPosition(DISPLAY->center());
    _content->addChild(_panel);
    
    this->updatePanel();
    
    return true;
}

void TotalRechargePanel::updatePanel(){
    _panel->removeAllChildrenWithCleanup(true);
    
    CCArray* _template = DATA->getOperation()->getPurchaseAchievementTemplate();
    CCDictionary* temp = (CCDictionary*)_template->objectAtIndex(DATA->getOperation()->cur_purchase_achievement_template_index());
    CCArray* clothes = (CCArray*)temp->objectForKey("clothes");
    
    CCLOG("index = %d", DATA->getOperation()->cur_purchase_achievement_template_index());
    
    CCSize panelSize = _panel->boundingBox().size;
    int count = clothes->count();
    int row = ceil(count/2);
    int col = 2;
    for (int i = 0; i < row; i++) {
        if (count % 2) {
            if (i == row - 1) {
                col = 1;
            }
        }else{
            
        }
        for (int j = 0; j < col; j++) {
            CCSprite* icon_bg = CCSprite::create("pic/panel/signin7/icon_bg.png");
            icon_bg->setPosition(ccp(_panel->getContentSize().width* .52f + _panel->getContentSize().width* .26f *j, _panel->getContentSize().height* .73f - _panel->getContentSize().height* .14f * i));
            _panel->addChild(icon_bg);
            
            CCInteger* id = (CCInteger*)clothes->objectAtIndex(j + i*2);
            CCSprite* icon = CCSprite::create(DATA->clothes_icon_path_with_id(id->getValue())->getCString());
            icon->setPosition(ccp(icon_bg->getContentSize().width* .5f, icon_bg->getContentSize().height* .45f));
            icon->setScale(0.6f);
            icon_bg->addChild(icon);
        }
        
    }
    
//    CCSprite* girl = CCSprite::create("pic/panel/totalRecharge/gril.png");
//    girl->setPosition(ccp(_panel->getContentSize().width* .25f, _panel->getContentSize().height* .52f));
//    _panel->addChild(girl);
    
    CCArray* suits = DATA->getOperation()->getPASuits();
    Shower* shower = Shower::create();
    
    int index = DATA->getOperation()->getPurchaseAchievementUser()->count() == 2 ? 1 : DATA->getOperation()->getPurchaseAchievementUser()->count();
    shower->ondress((CCDictionary*)suits->objectAtIndex(index));
    shower->setScale(0.6f);
    shower->setPosition(ccp(_panel->getContentSize().width* .0f, _panel->getContentSize().height* .1f));
    _panel->addChild(shower);
    
    CCSprite* red_bg = CCSprite::create("pic/panel/totalRecharge/red_bg.png");
    red_bg->setPosition(ccp(_panel->getContentSize().width* .3f, _panel->getContentSize().height* .08f));
    _panel->addChild(red_bg);
    
    int goal = ((CCInteger*)temp->objectForKey("goal"))->getValue();
    CCString* str1 = CCString::createWithFormat("累计充值%d元 赠送限量版时装", goal);
    CCLabelTTF* lab1 = CCLabelTTF::create(str1->getCString(), DISPLAY->fangzhengFont(), 18);
    lab1->setPosition(ccp(red_bg->getContentSize().width* .5f, red_bg->getContentSize().height* .65f));
    red_bg->addChild(lab1);
    
    CCString* str2 = CCString::createWithFormat("当前已充值: %d元", DATA->getOperation()->getPurchasedTotal());
    CCLabelTTF* lab2 = CCLabelTTF::create(str2->getCString(), DISPLAY->fangzhengFont(), 18);
    lab2->setPosition(ccp(red_bg->getContentSize().width* .5f, red_bg->getContentSize().height* .35f));
    red_bg->addChild(lab2);
    
    if (DATA->getOperation()->getPurchasedTotal() >= goal) {
        CCSprite* btn_nor = CCSprite::create("pic/panel/totalRecharge/lingqu.png");
        CCSprite* btn_sel = CCSprite::create("pic/panel/totalRecharge/lingqu.png");
        btn_sel->setScale(1.02f);
        CCSprite* btn_dis = CCSprite::create("pic/panel/totalRecharge/lingqu.png");
        btn_dis->setColor(ccGRAY);
        _item = CCMenuItemSprite::create(btn_nor, btn_sel, btn_dis, this, menu_selector(TotalRechargePanel::btn_lingqu_callback));
        _item->setPosition(ccp(_panel->getContentSize().width* .80f, _panel->getContentSize().height* .08f));
        CCMenu* menu = CCMenu::create(_item, NULL);
        menu->setAnchorPoint(CCPointZero);
        menu->setPosition(ccp(0, 0));
        _panel->addChild(menu);
        menu->setTag(300);
        
        if (DATA->getOperation()->getPurchaseAchievementUser()->count() == 3) {
            _item->setEnabled(false);
        }
        
    }else{
        CCSprite* btn_nor = CCSprite::create("pic/panel/totalRecharge/chongzhi.png");
        CCSprite* btn_sel = CCSprite::create("pic/panel/totalRecharge/chongzhi.png");
        btn_sel->setScale(1.02f);
        CCMenuItemSprite* item = CCMenuItemSprite::create(btn_nor, btn_sel, this, menu_selector(TotalRechargePanel::btn_chongzhi_callback));
        item->setPosition(ccp(_panel->getContentSize().width* .80f, _panel->getContentSize().height* .08f));
        CCMenu* menu = CCMenu::create(item, NULL);
        menu->setAnchorPoint(CCPointZero);
        menu->setPosition(ccp(0, 0));
        _panel->addChild(menu);
    }
}

void TotalRechargePanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&TotalRechargePanel::reward_callback_305), "HTTP_FINISHED_305", NULL);
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->scheduleOnce(SEL_SCHEDULE(&TotalRechargePanel::keyBackStatus), .8f);
}
void TotalRechargePanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void TotalRechargePanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool TotalRechargePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

#pragma mark - Inner

void TotalRechargePanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void TotalRechargePanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}

void TotalRechargePanel::btn_lingqu_callback(){
    LOADING->show_loading();
    CCArray* _template = DATA->getOperation()->getPurchaseAchievementTemplate();
    CCDictionary* temp = (CCDictionary*)_template->objectAtIndex(DATA->getOperation()->cur_purchase_achievement_template_index());
    string id = temp->valueForKey("id")->getCString();
    NET->take_purchase_achievement_305(id);
}

void TotalRechargePanel::btn_chongzhi_callback(){
    this->removeFromParentAndCleanup(true);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_PURCHASEPANEL");
}

void TotalRechargePanel::reward_callback_305(cocos2d::CCObject *obj){
    LOADING->remove();
    
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "领取成功");
    
//    if (DATA->getOperation()->getPurchaseAchievementUser()->count() == 3) {
//        _item->setEnabled(false);
//    }
    
    this->updatePanel();
}