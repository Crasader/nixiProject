//
//  TaskLabelColorLayer.cpp
//  tiegao
//
//  Created by mac on 16-7-15.
//
//

#include "TaskLabelColorLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "AudioManager.h"


TaskLabelColorLayer::TaskLabelColorLayer(){
    
}
TaskLabelColorLayer::~TaskLabelColorLayer(){
    
}

void TaskLabelColorLayer::onEnter(){
    CCLayer::onEnter();
    
    
    this->scheduleOnce(SEL_SCHEDULE(&TaskLabelColorLayer::keyBackStatus), .8f);
}
void TaskLabelColorLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void TaskLabelColorLayer::onExit(){
    
    setTouchEnabled(false);
    CCLayer::onExit();
}

bool TaskLabelColorLayer::init(){
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(250, 250, 250, 0), DISPLAY->ScreenWidth(), DISPLAY->ScreenHeight()));
        
        this->setTouchSwallowEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchEnabled(true);
        
        num_child = 0;
        
        the_current_number_of_pages = 0;
        
        _labelArr = CCArray::create();
        _labelArr->retain();
        
        
        dkSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_dikuang.png");
        dkSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .6f));
        this->addChild(dkSpr);
        
        
//        CCSprite* gbSpr1 = CCSprite::create("");
//        gbSpr1->setScale(.8f);
//        CCSprite* gbSpr2 = CCSprite::create("");
//        gbSpr2->setScale(.81f);
//        CCMenuItem* gbItem = CCMenuItemSprite::create(gbSpr1, gbSpr2, this, menu_selector(TaskLabelColorLayer::fhCallBack));
//        gbItem->setPosition(ccp(dkSpr->boundingBox().size.width* .93f, dkSpr->boundingBox().size.height* .95f));
//        CCMenu* menu = CCMenu::create(gbItem, NULL);
//        menu->setPosition(CCPointZero);
//        dkSpr->addChild(menu);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void TaskLabelColorLayer::fhCallBack(CCObject* pSender){
    this->setTouchEnabled(false);
    AUDIO->goback_effect();
    
//    if (MMAudioManager::get_instance()->is_effect_on()) {
//        MMAudioManager::get_instance()->play_effect(kAudio_Button_Back, false);
//    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("TaskLabelColorFhCallBack");
}

TaskLabelColorLayer* TaskLabelColorLayer::create_with_index(const char* text){
    TaskLabelColorLayer* rtn = TaskLabelColorLayer::create();
    rtn->init_with_index(text);
    
    return rtn;
}
void TaskLabelColorLayer::init_with_index(const char* text){
    
    for (int i = 0; i < DATA->getStoryLabelArr()->count(); i++) {
        _labelArr->addObject(DATA->getStoryLabelArr()->objectAtIndex(i));
    }
    
    CCString* str = CCString::createWithFormat("%s", (char* )text);
    if (str->length() != 0) {
        _labelArr->addObject(str);
    }
    this->creatLabel();
}

void TaskLabelColorLayer::creatLabel(){
    
    if (dkSpr->getChildByTag(0x77777) != NULL) {
        dkSpr->removeChildByTag(0x77777);
    }
    if (dkSpr->getChildByTag(0x3333) != NULL) {
        dkSpr->removeChildByTag(0x3333);
    }
    
    if (the_current_number_of_pages < _labelArr->count()) {
        for (int i = 0; i < _labelArr->count(); i++) {
            if (i == the_current_number_of_pages) {
//                CCString* str = CCString::createWithFormat("%s", (char* )_labelArr->objectAtIndex(i));
                CCString* str = (CCString* )_labelArr->objectAtIndex(i);
//                CCLog("str ==== %s", str->getCString());
                m_pLabel = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24, CCSizeMake(dkSpr->boundingBox().size.width* .85, dkSpr->boundingBox().size.height* .8), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
                m_pLabel->setPosition(ccp(dkSpr->boundingBox().size.width/2, dkSpr->boundingBox().size.height/2));
                m_pLabel->setColor(ccc3(80, 63, 68));
//                m_pLabel->enableStroke(ccc3(80, 63, 68), .4f);
                m_pLabel->setTag(0x77777);
                dkSpr->addChild(m_pLabel, 10);
            }
        }
    }
    
    // 翻页按钮
    // -- lfet
    CCSprite* leftSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_lift.png");
    CCSprite* leftSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_lift.png");
    leftSpr2->setScale(1.01f);
    CCSprite* leftSpr3 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_lift.png");
    CCMenuItem* m_button_left = CCMenuItemSprite::create(leftSpr1, leftSpr2, leftSpr3, this, SEL_MenuHandler(&TaskLabelColorLayer::on_left_button_clicked));
    m_button_left->setTag(0x1111);
    m_button_left->setPosition(ccp(dkSpr->boundingBox().size.width* .2, dkSpr->boundingBox().size.height* .1));
    
    // -- right
    CCSprite* rightSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_right.png");
    CCSprite* rightSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_right.png");
    CCSprite* rightSpr3 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_right.png");
    rightSpr2->setScale(1.01f);
    CCMenuItem* m_button_right = CCMenuItemSprite::create(rightSpr1, rightSpr2, rightSpr3, this, SEL_MenuHandler(&TaskLabelColorLayer::on_right_button_clicked));
    m_button_right->setTag(0x2222);
    m_button_right->setPosition(ccp(dkSpr->boundingBox().size.width* .8, dkSpr->boundingBox().size.height* .1));
    
    CCMenu* menu_flip = CCMenu::create(m_button_left, m_button_right, NULL);
    menu_flip->setPosition(CCPointZero);
    menu_flip->setTag(0x3333);
    dkSpr->addChild(menu_flip);
    
    
    if (the_current_number_of_pages > 0) {
        m_button_left->setEnabled(true);
    }else{
        m_button_left->setEnabled(false);
    }
    
    if (the_current_number_of_pages == _labelArr->count() - 1)
        m_button_right->setEnabled(false);
    else
        m_button_right->setEnabled(true);
}

void TaskLabelColorLayer::on_left_button_clicked(CCObject* pSender){
    the_current_number_of_pages--;
    
    //    if (MMAudioManager::get_instance()->is_effect_on()) {
    //        MMAudioManager::get_instance()->play_effect(kAudio_Button_Common, false);
    //    }
    
    if (the_current_number_of_pages < 0) {
        the_current_number_of_pages = 0;
    }
    
    this->creatLabel();
}
void TaskLabelColorLayer::on_right_button_clicked(CCObject* pSender){
    the_current_number_of_pages++;
    
//    if (MMAudioManager::get_instance()->is_effect_on()) {
//        MMAudioManager::get_instance()->play_effect(kAudio_Button_Common, false);
//    }
    
    if (_labelArr->count() > 0) {
        if (the_current_number_of_pages == _labelArr->count()) {
            the_current_number_of_pages = _labelArr->count() - 1;
        }
    }else{
        the_current_number_of_pages = 0;
    }
    this->creatLabel();
}

bool TaskLabelColorLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    CCPoint pos = pTouch->getLocation();
//    CCLog("<><><><><>posx:%f, posy:%f", pos.x, pos.y);
    if (!isDkContainTouchPoint1(pTouch)) {
        this->fhCallBack(this);
    }
    
    return true;
}

CCRect TaskLabelColorLayer::isDkRect(){
    return CCRectMake(-223, -203, 448, 632);
}
bool TaskLabelColorLayer::isDkContainTouchPoint1(CCTouch* touch){
    return isDkRect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

void TaskLabelColorLayer::keyBackClicked(){
    num_child++;
    CCLog("===== TaskLabelColorLayer  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->fhCallBack(NULL);
}

