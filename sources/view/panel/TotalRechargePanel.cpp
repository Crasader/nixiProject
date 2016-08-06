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
    _template = DATA->getOperation()->getPurchaseAchievementTemplate();
    
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    _content = CCLayer::create();
    this->addChild(_content);
    
    _panel = CCSprite::create("pic/panel/totalRecharge/recharge_panel.png");
    _panel->setPosition(DISPLAY->center());
    _content->addChild(_panel);
    
    CCSize panelSize = _panel->boundingBox().size;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            CCSprite* icon_bg = CCSprite::create("pic/panel/signin7/icon_bg.png");
            icon_bg->setPosition(ccp(_panel->getContentSize().width* .52f + _panel->getContentSize().width* .26f *j, _panel->getContentSize().height* .72f - _panel->getContentSize().height* .14f * i));
            _panel->addChild(icon_bg);
            
            CCDictionary* dic = (CCDictionary*)_template->objectAtIndex(j + i*2);
            CCString* uri = (CCString*)dic->valueForKey("uri");
            CCSprite* icon = CCSprite::create(DATA->clothes_icon_path_with_id(uri->intValue())->getCString());
            icon->setPosition(ccp(icon_bg->getContentSize().width* .5f, icon_bg->getContentSize().height* .45f));
            icon->setScale(0.6f);
            icon_bg->addChild(icon);
        }
        
    }
    
    CCSprite* gril = CCSprite::create("pic/panel/totalRecharge/gril.png");
    gril->setPosition(ccp(_panel->getContentSize().width* .25f, _panel->getContentSize().height* .52f));
    _panel->addChild(gril);
    
    CCSprite* red_bg = CCSprite::create("pic/panel/totalRecharge/red_bg.png");
    red_bg->setPosition(ccp(_panel->getContentSize().width* .3f, _panel->getContentSize().height* .08f));
    _panel->addChild(red_bg);
    
    CCLabelTTF* lab1 = CCLabelTTF::create("累计充值200元 赠送限量版时装", DISPLAY->fangzhengFont(), 18);
    lab1->setPosition(ccp(red_bg->getContentSize().width* .5f, red_bg->getContentSize().height* .65f));
    red_bg->addChild(lab1);
    
    CCString* str = CCString::createWithFormat("当前已充值: %d元", DATA->getOperation()->getPurchasedTotal());
    CCLabelTTF* lab2 = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 18);
    lab2->setPosition(ccp(red_bg->getContentSize().width* .5f, red_bg->getContentSize().height* .35f));
    red_bg->addChild(lab2);
    
    if (DATA->getOperation()->getPurchasedTotal() >= 200) {
        CCSprite* btn_nor = CCSprite::create("pic/panel/totalRecharge/lingqu.png");
        CCSprite* btn_sel = CCSprite::create("pic/panel/totalRecharge/lingqu.png");
        btn_sel->setScale(1.02f);
        CCSprite* btn_dis = CCSprite::create("pic/panel/totalRecharge/lingqu.png");
        btn_dis->setColor(ccGRAY);
        CCMenuItemSprite* item = CCMenuItemSprite::create(btn_nor, btn_sel, btn_dis, this, menu_selector(TotalRechargePanel::btn_lingqu_callback));
        item->setPosition(ccp(_panel->getContentSize().width* .80f, _panel->getContentSize().height* .08f));
        CCMenu* menu = CCMenu::create(item, NULL);
        menu->setAnchorPoint(CCPointZero);
        menu->setPosition(ccp(0, 0));
        _panel->addChild(menu);
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
    
    
    return true;
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
//    int idx = ((CCInteger*)item->getUserObject())->getValue();
//    CCDictionary* dic = (CCDictionary*)_template->objectAtIndex(idx);
//    string id = dic->valueForKey("id")->getCString();
//    NET->take_purchase_achievement_305(id);
}

void TotalRechargePanel::btn_chongzhi_callback(){
    this->removeFromParentAndCleanup(true);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_PURCHASEPANEL");
}

void TotalRechargePanel::reward_callback_305(cocos2d::CCObject *obj){
    LOADING->remove();
    
}