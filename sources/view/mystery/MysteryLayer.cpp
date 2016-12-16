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

void MysteryLayer::show(CCNode* parent) {
    MysteryLayer* layer = MysteryLayer::create();
    parent->addChild(layer);
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
    nc->addObserver(this, menu_selector(MysteryLayer::after_take_mystery_achv_615), "HTTP_FINISHED_615", NULL);
    
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
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void MysteryLayer::keyBackClicked(){
    num_child++;
    CCLog("===== MysteryLayer  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
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
    CCArray* achvTemplate = comp->fetchAchvTemplate(category->getCString());
    CCDictionary* item_0 = (CCDictionary*)achvTemplate->objectAtIndex(0);
    CCDictionary* item_1 = (CCDictionary*)achvTemplate->objectAtIndex(1);
    CCDictionary* item_2 = (CCDictionary*)achvTemplate->objectAtIndex(2);
    CCDictionary* item_3 = (CCDictionary*)achvTemplate->objectAtIndex(3);

    int goal_0 = ((CCInteger*)item_0->objectForKey("goal"))->getValue();
    int goal_1 = ((CCInteger*)item_1->objectForKey("goal"))->getValue();
    int goal_2 = ((CCInteger*)item_2->objectForKey("goal"))->getValue();
    int goal_3 = ((CCInteger*)item_3->objectForKey("goal"))->getValue();
    // 进度数字
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
    
    // 任务开始按钮
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
    
    // 进度数值表示
    int rating = MIN(goal_3, comp->userRatingOfCategory(category->getCString()));
    CCString* strProgress = CCString::createWithFormat("%d/%d", rating, goal_3);
    CCLabelTTF* lblProgress = CCLabelTTF::create(strProgress->getCString(), DISPLAY->fangzhengFont(), 20);
    lblProgress->setColor(ccc3(229, 123, 100));
    lblProgress->setPosition(ccp(CELL_WIDTH - 65, CELL_HEIGHT * 0.5 + 32));
    plane->addChild(lblProgress);
    
    // 进度条
    CCSprite* progress_0 = CCSprite::create("pic/mystery/my_progress_0.png");
    progress_0->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    plane->addChild(progress_0);
    
    CCSprite* progress_1 = CCSprite::create("pic/mystery/my_progress_1.png");
    CCProgressTimer* progress = CCProgressTimer::create(progress_1);
    progress->setType(kCCProgressTimerTypeBar);
    progress->setMidpoint(ccp(0, 0.5));
    progress->setBarChangeRate(ccp(1, 0));
    progress->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    plane->addChild(progress);
    
    progress->setPercentage(MIN(100, (rating * 0.1 / goal_3) * 1000));
    
    
    // 四个状态
    CCString* achvId_0 = (CCString*)item_0->objectForKey("id");
    CCString* achvId_1 = (CCString*)item_1->objectForKey("id");
    CCString* achvId_2 = (CCString*)item_2->objectForKey("id");
    CCString* achvId_3 = (CCString*)item_3->objectForKey("id");
    int state_0 = comp->userAchvStateOfCategory(category->getCString(), achvId_0->getCString());
    int state_1 = comp->userAchvStateOfCategory(category->getCString(), achvId_1->getCString());
    int state_2 = comp->userAchvStateOfCategory(category->getCString(), achvId_2->getCString());
    int state_3 = comp->userAchvStateOfCategory(category->getCString(), achvId_3->getCString());
    CCLOG("state_0 = %d, state_1 = %d, state_2 = %d, state_3 = %d", state_0 ,state_1, state_2, state_3);
    
    // 四个按钮
    CCString* strBtnImage_0 = CCString::createWithFormat("pic/mystery/my_plate_%d.png", state_0);
    CCMenuItem* btn_0 = CCMenuItemImage::create(strBtnImage_0->getCString(), strBtnImage_0->getCString(), this, SEL_MenuHandler(&MysteryLayer::on_take_achv));
    btn_0->setEnabled(state_0 == 1);
    CCDictionary* itemUserData_0 = CCDictionary::create();
    itemUserData_0->setObject(ccs(category->getCString()), "category");
    itemUserData_0->setObject(ccs(achvId_0->getCString()), "achvId");
    btn_0->setUserObject(itemUserData_0);
    
    CCString* strBtnImage_1 = CCString::createWithFormat("pic/mystery/my_plate_%d.png", state_1);
    CCMenuItem* btn_1 = CCMenuItemImage::create(strBtnImage_1->getCString(), strBtnImage_1->getCString(), this, SEL_MenuHandler(&MysteryLayer::on_take_achv));
    btn_1->setEnabled(state_1 == 1);
    CCDictionary* itemUserData_1 = CCDictionary::create();
    itemUserData_1->setObject(ccs(category->getCString()), "category");
    itemUserData_1->setObject(ccs(achvId_1->getCString()), "achvId");
    btn_1->setUserObject(itemUserData_1);
    
    CCString* strBtnImage_2 = CCString::createWithFormat("pic/mystery/my_plate_%d.png", state_2);
    CCMenuItem* btn_2 = CCMenuItemImage::create(strBtnImage_2->getCString(), strBtnImage_2->getCString(), this, SEL_MenuHandler(&MysteryLayer::on_take_achv));
    btn_2->setEnabled(state_2 == 1);
    CCDictionary* itemUserData_2 = CCDictionary::create();
    itemUserData_2->setObject(ccs(category->getCString()), "category");
    itemUserData_2->setObject(ccs(achvId_2->getCString()), "achvId");
    btn_2->setUserObject(itemUserData_2);
    
    CCString* strBtnImage_3 = CCString::createWithFormat("pic/mystery/my_plate_%d.png", state_3);
    CCMenuItem* btn_3 = CCMenuItemImage::create(strBtnImage_3->getCString(), strBtnImage_3->getCString(), this, SEL_MenuHandler(&MysteryLayer::on_take_achv));
    btn_3->setEnabled(state_3 == 1);
    CCDictionary* itemUserData_3 = CCDictionary::create();
    itemUserData_3->setObject(ccs(category->getCString()), "category");
    itemUserData_3->setObject(ccs(achvId_3->getCString()), "achvId");
    btn_3->setUserObject(itemUserData_3);
    
    CCMenu* btnMenu = CCMenu::create(btn_0, btn_1, btn_2, btn_3, NULL);
    btnMenu->alignItemsHorizontallyWithPadding(CELL_WIDTH * 0.25 - 94);
    btnMenu->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.2));
    plane->addChild(btnMenu);
    
    
    // 按钮上的奖励内容
    CCString* type_0 = (CCString*)item_0->objectForKey("type");
    CCString* type_1 = (CCString*)item_1->objectForKey("type");
    CCString* type_2 = (CCString*)item_2->objectForKey("type");
    CCString* type_3 = (CCString*)item_3->objectForKey("type");
    // 奖励数目
    int num_0 = ((CCInteger*)item_0->objectForKey("num"))->getValue();
    int num_1 = ((CCInteger*)item_1->objectForKey("num"))->getValue();
    int num_2 = ((CCInteger*)item_2->objectForKey("num"))->getValue();
    int num_3 = ((CCInteger*)item_3->objectForKey("num"))->getValue();
    
    CCSprite* rewardIcon_0 = this->createRewardIcon(type_0, num_0);
    if (rewardIcon_0 != NULL) {
        btn_0->addChild(rewardIcon_0);
    }
    
    CCSprite* rewardIcon_1 = this->createRewardIcon(type_1, num_1);
    if (rewardIcon_1 != NULL) {
        btn_1->addChild(rewardIcon_1);
    }
    
    CCSprite* rewardIcon_2 = this->createRewardIcon(type_2, num_2);
    if (rewardIcon_2 != NULL) {
        btn_2->addChild(rewardIcon_2);
    }
    
    CCSprite* rewardIcon_3 = this->createRewardIcon(type_3, num_3);
    if (rewardIcon_3 != NULL) {
        btn_3->addChild(rewardIcon_3);
    }
}

CCSprite* MysteryLayer::createRewardIcon(CCString* type, int num) {
    ccColor3B numColor = ccc3(178, 84, 122);
    float iconX = 47;
    float iconY = 60;
    CCSprite* rtn = NULL;
    if (type->compare("coin") == 0) {
        rtn = CCSprite::create("pic/clothesScene/gj_coin.png");
        rtn->setPosition(ccp(iconX, iconY));
        
        CCString* srtNum = CCString::createWithFormat("%d", num);
        CCLabelTTF* lblNum = CCLabelTTF::create(srtNum->getCString(), DISPLAY->fangzhengFont(), 16);
        lblNum->setColor(numColor);
        lblNum->setPosition(ccp(rtn->getContentSize().width * 0.5, -12));
        rtn->addChild(lblNum);
    }
    else if (type->compare("diam") == 0) {
        rtn = CCSprite::create("pic/clothesScene/gj_gold.png");
        rtn->setPosition(ccp(iconX, iconY));
        
        CCString* srtNum = CCString::createWithFormat("%d", num);
        CCLabelTTF* lblNum = CCLabelTTF::create(srtNum->getCString(), DISPLAY->fangzhengFont(), 16);
        lblNum->setColor(numColor);
        lblNum->setPosition(ccp(rtn->getContentSize().width * 0.5, -12));
        rtn->addChild(lblNum);
    }
    else if (type->compare("energy") == 0) {
        rtn = CCSprite::create("pic/clothesScene/gj_xin.png");
        rtn->setPosition(ccp(iconX, iconY));
        
        CCString* srtNum = CCString::createWithFormat("%d", num);
        CCLabelTTF* lblNum = CCLabelTTF::create(srtNum->getCString(), DISPLAY->fangzhengFont(), 16);
        lblNum->setColor(numColor);
        lblNum->setPosition(ccp(rtn->getContentSize().width * 0.5, -12));
        rtn->addChild(lblNum);
    }
    else if (type->compare("clothes") == 0) {
        CCString* icon = CCString::createWithFormat("pic/mystery/my_%d.png", num);
        CCLOG("icon = %s", icon->getCString());
        rtn = CCSprite::create(icon->getCString());
        if (rtn) {
            rtn->setPosition(ccp(iconX, iconY - 14));
        }
    }
    
    return rtn;
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

void MysteryLayer::on_take_achv(CCMenuItem *pObj) {
    CCDictionary* data = dynamic_cast<CCDictionary*>(pObj->getUserObject());
    CCString* category = dynamic_cast<CCString*>(data->objectForKey("category"));
    CCString* achvId = dynamic_cast<CCString*>(data->objectForKey("achvId"));
    CCLOG("MysteryLayer::on_take_achv() - category = %s, achvId = %s", category->getCString(), achvId->getCString());
    LOADING->show_loading();
    NET->take_mystery_achv_615(category->getCString(), achvId->getCString());
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

void MysteryLayer::after_take_mystery_achv_615(CCObject *pObj) {
    LOADING->remove();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    CCString* category = dynamic_cast<CCString*>(pObj);
    int idx = category->intValue() - 1;
    if (idx >= 0) {
        _tv->updateCellAtIndex(idx);
    }
    
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(this->getScene(), "成功领取奖励~!");
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

