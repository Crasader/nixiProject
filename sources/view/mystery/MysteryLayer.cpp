//
//  MysteryLayer.cpp
//  tiegao
//
//  Created by mac on 16/8/23.
//
//

#include "MysteryLayer.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "NetManager.h"
#include "AudioManager.h"

#include "MysteryDialogScene.h"
#include "PromptLayer.h"
#include "Loading2.h"


#define CELL_WIDTH          576
#define CELL_HEIGHT         310

#pragma mark - Export

CCScene* MysteryLayer::scene() {
    MysteryLayer* layer = MysteryLayer::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

#pragma mark - Inherit

MysteryLayer::~MysteryLayer() {
}

bool MysteryLayer::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        CCSprite* mask = CCSprite::create("pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _panel = CCSprite::create("pic/mystery/my_bg.png");
        _panel->setPosition(DISPLAY->center());
        this->addChild(_panel);
        
        float panelW = CELL_WIDTH;
        float panelH = 980;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->ignoreAnchorPointForPosition(false);
        _tv->setPosition(ccp(_panel->getContentSize().width * 0.5, _panel->getContentSize().height * 0.5 - 44));
        _panel->addChild(_tv);
        // 暂时不让滚动
        _tv->setBounceable(false);
        
        CCSprite* txt_close = CCSprite::create("pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.05));
        this->addChild(txt_close);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        return true;
    }
    else {
        return false;
    }
}

void MysteryLayer::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(MysteryLayer::after_start_mystery_611), "HTTP_FINISHED_611", NULL);
    
    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&MysteryLayer::keyBackStatus), .8f);
}

void MysteryLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void MysteryLayer::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

bool MysteryLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = this->convertToNodeSpace(pTouch->getLocation());
    if (! _panel->boundingBox().containsPoint(location)) {
        this->do_exit();
    }
    
    return false;
}

#pragma mark - Inner

void MysteryLayer::do_enter() {
    _panel->setScale(0.3);
    CCSequence* seq = CCSequence::create(CCScaleTo::create(0.4, 1.03), CCScaleTo::create(0.3, 1), NULL);
    _panel->runAction(CCEaseElasticOut::create(seq));
}

void MysteryLayer::do_exit() {
    _panel->setScale(0.3);
    CCCallFunc* remove = CCCallFunc::create(this, SEL_CallFunc(&MysteryLayer::remove));
    CCSequence* seq = CCSequence::create(CCScaleTo::create(0.4, 1.03), CCScaleTo::create(0.3, 0.3), remove, NULL);
    _panel->runAction(CCEaseElasticOut::create(seq));
}

void MysteryLayer::remove() {
    this->removeFromParentAndCleanup(true);
}

void MysteryLayer::keyBackClicked(){
    num_child++;
    CCLog("===== MysteryLayer  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->remove();
}

void MysteryLayer::config_cell(CCTableViewCell *cell, int idx) {
    CCLOG("MysteryLayer::config_cell() - idx = %d", idx);
    CCSprite* plane = CCSprite::create("pic/mystery/my_plane.png");
    plane->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    cell->addChild(plane);
    
    MysteryComp* comp = DATA->getMystery();
    
    CCString* category = CCString::createWithFormat("%d", idx + 1);
    CCArray* goalArray = comp->fetchTemplate(category->getCString());
    CCDictionary* item_0 = (CCDictionary*)goalArray->objectAtIndex(0);
    CCDictionary* item_1 = (CCDictionary*)goalArray->objectAtIndex(1);
    CCDictionary* item_2 = (CCDictionary*)goalArray->objectAtIndex(2);
    CCDictionary* item_3 = (CCDictionary*)goalArray->objectAtIndex(3);
    int goal_0 = ((CCInteger*)item_0->objectForKey("goal"))->getValue();
    int goal_1 = ((CCInteger*)item_1->objectForKey("goal"))->getValue();
    int goal_2 = ((CCInteger*)item_2->objectForKey("goal"))->getValue();
    int goal_3 = ((CCInteger*)item_3->objectForKey("goal"))->getValue();
    CCString* type_0 = (CCString*)item_0->objectForKey("type");
    CCString* type_1 = (CCString*)item_1->objectForKey("type");
    CCString* type_2 = (CCString*)item_2->objectForKey("type");
    CCString* type_3 = (CCString*)item_3->objectForKey("type");
    int num_0 = ((CCInteger*)item_0->objectForKey("num"))->getValue();
    int num_1 = ((CCInteger*)item_1->objectForKey("num"))->getValue();
    int num_2 = ((CCInteger*)item_2->objectForKey("num"))->getValue();
    int num_3 = ((CCInteger*)item_3->objectForKey("num"))->getValue();
    
    {
        ccColor3B goalColor = ccc3(99, 67, 96);
        float goalStandY = CELL_HEIGHT * 0.44;
        
        CCLabelTTF* lblGoal_0 = CCLabelTTF::create(CCString::createWithFormat("%d", goal_0)->getCString(), DISPLAY->fangzhengFont(), 18);
        lblGoal_0->setColor(goalColor);
        lblGoal_0->setPosition(ccp(CELL_WIDTH * 0.125, goalStandY));
        plane->addChild(lblGoal_0);
        
        CCSprite* adhereStar_0 = CCSprite::create("pic/mystery/my_star.png");
        adhereStar_0->setPosition(ccp(lblGoal_0->getPositionX() + lblGoal_0->getContentSize().width * 0.5 + 12, lblGoal_0->getPositionY()));
        plane->addChild(adhereStar_0);
        
        
        CCLabelTTF* lblGoal_1 = CCLabelTTF::create(CCString::createWithFormat("%d", goal_1)->getCString(), DISPLAY->fangzhengFont(), 18);
        lblGoal_1->setColor(goalColor);
        lblGoal_1->setPosition(ccp(CELL_WIDTH * 0.375, goalStandY));
        plane->addChild(lblGoal_1);
        
        CCSprite* adhereStar_1 = CCSprite::create("pic/mystery/my_star.png");
        adhereStar_1->setPosition(ccp(lblGoal_1->getPositionX() + lblGoal_1->getContentSize().width * 0.5 + 12, lblGoal_1->getPositionY()));
        plane->addChild(adhereStar_1);
        
        
        CCLabelTTF* lblGoal_2 = CCLabelTTF::create(CCString::createWithFormat("%d", goal_2)->getCString(), DISPLAY->fangzhengFont(), 18);
        lblGoal_2->setColor(goalColor);
        lblGoal_2->setPosition(ccp(CELL_WIDTH * 0.625, goalStandY));
        plane->addChild(lblGoal_2);
        
        CCSprite* adhereStar_2 = CCSprite::create("pic/mystery/my_star.png");
        adhereStar_2->setPosition(ccp(lblGoal_2->getPositionX() + lblGoal_2->getContentSize().width * 0.5 + 12, lblGoal_2->getPositionY()));
        plane->addChild(adhereStar_2);
        
        
        CCLabelTTF* lblGoal_3 = CCLabelTTF::create(CCString::createWithFormat("%d", goal_3)->getCString(), DISPLAY->fangzhengFont(), 18);
        lblGoal_3->setColor(goalColor);
        lblGoal_3->setPosition(ccp(CELL_WIDTH * 0.875, goalStandY));
        plane->addChild(lblGoal_3);
        
        CCSprite* adhereStar_3 = CCSprite::create("pic/mystery/my_star.png");
        adhereStar_3->setPosition(ccp(lblGoal_3->getPositionX() + lblGoal_3->getContentSize().width * 0.5 + 12, lblGoal_3->getPositionY()));
        plane->addChild(adhereStar_3);
    }
    
    
    CCSprite* progress_0 = CCSprite::create("pic/mystery/my_progress_0.png");
    progress_0->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    plane->addChild(progress_0);
    
    CCSprite* progress_1 = CCSprite::create("pic/mystery/my_progress_1.png");
    CCProgressTimer* progress = CCProgressTimer::create(progress_1);
    progress->setReverseDirection(true);
    progress->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    plane->addChild(progress);
    
    
    CCMenuItem* btn = NULL;
    float btnScale = 1.002;
    if (category->compare("1") == 0) {
        CCSprite* btn1 = CCSprite::create("pic/mystery/my_btn_1.png");
        CCSprite* btn2 = CCSprite::create("pic/mystery/my_btn_1.png");
        btn2->setScale(btnScale);
        btn = CCMenuItemSprite::create(btn1, btn2, this, SEL_MenuHandler(&MysteryLayer::on_start_task));
    }
    else if (category->compare("2") == 0) {
        CCSprite* btn1 = CCSprite::create("pic/mystery/my_btn_2.png");
        CCSprite* btn2 = CCSprite::create("pic/mystery/my_btn_2.png");
        btn2->setScale(btnScale);
        btn = CCMenuItemSprite::create(btn1, btn2, this, SEL_MenuHandler(&MysteryLayer::on_start_task));
    }
    else if (category->compare("3") == 0) {
        CCSprite* btn1 = CCSprite::create("pic/mystery/my_btn_3.png");
        CCSprite* btn2 = CCSprite::create("pic/mystery/my_btn_3.png");
        btn2->setScale(btnScale);
        btn = CCMenuItemSprite::create(btn1, btn2, this, SEL_MenuHandler(&MysteryLayer::on_start_task));
    }
    
    if (btn != NULL) {
        btn->setUserObject(category);
        CCMenu* menu = CCMenu::createWithItem(btn);
        menu->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT - 63));
        plane->addChild(menu);
    }
    
}

void MysteryLayer::on_start_task(CCMenuItem *pObj) {
    AUDIO->common_effect();
    //
    if (DATA->getPlayer()->energy < 12) {
//        AHMessageBox* mb = AHMessageBox::create_with_message("体力不够，是否购买体力?", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
//        mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
//        CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(this->getScene(), "体力不够啦~~!");
    }
    else {
        LOADING->show_loading();
        CCString* category = dynamic_cast<CCString*>(pObj->getUserObject());
        this->setCategoryTempSaved(category);
        
        NET->start_mystery_611(category->getCString());
    }
}

//void MysteryLayer::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
//    box->animation_out();
//    
//    if (button_type == AH_BUTTON_TYPE_YESNO) {
//        if (button_tag == AH_BUTTON_TAG_YES) {
//            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_BUY_ENERGY");
//        }else if (button_tag == AH_BUTTON_TAG_NO){
//            PromptLayer* layer = PromptLayer::create();
//            layer->show_prompt(this->getScene(), "据说体力藏在活动里~!去看看活动吧.");
//        }
//    }
//}

void MysteryLayer::after_start_mystery_611(CCObject *pObj) {
    LOADING->remove();
    CCString* choosedTaskId = dynamic_cast<CCString*>(pObj);
    CCLOG("MysteryLayer::after_start_mystery_611() - %s", choosedTaskId->getCString());
    CCArray* dialogs = CONFIG->mysteryDialog(choosedTaskId->getCString());
    CCScene* scene = MysteryDialogScene::scene(this->getCategoryTempSaved()->getCString(), dialogs);
    CCDirector::sharedDirector()->replaceScene(scene);
}

#pragma mark - CCTableViewDataSource

CCSize MysteryLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCSize MysteryLayer::cellSizeForTable(CCTableView *table) {
    return CCSizeZero;
}

CCTableViewCell* MysteryLayer::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell* cell = new CCTableViewCell();
    this->config_cell(cell, idx);
    return cell;
}

unsigned int MysteryLayer::numberOfCellsInTableView(CCTableView *table) {
    return 3;
}

