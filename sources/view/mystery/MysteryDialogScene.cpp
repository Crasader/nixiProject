//
//  MysteryDialogScene.cpp
//  tiegao
//
//  Created by mac on 16-7-8.
//
//

#include "MysteryDialogScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "SpecialManager.h"
#include "NetManager.h"

#include "ClothesScene.h"
#include "MainScene.h"
#include "Loading2.h"
#include "TaskLabelColorLayer.h"


CCScene* MysteryDialogScene::scene(const char* category, CCArray* dialogs){
    CCScene* scene = CCScene::create();
    MysteryDialogScene* layer = MysteryDialogScene::create(category, dialogs);
    scene->addChild(layer);
    
    return scene;
}

MysteryDialogScene::~MysteryDialogScene() {
    CC_SAFE_RELEASE_NULL(dialogs);
}

MysteryDialogScene* MysteryDialogScene::create(const char* category, CCArray* dialogs) {
    MysteryDialogScene* rtn = new MysteryDialogScene();
    if (rtn && rtn->init(category, dialogs)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_DELETE(rtn);
        rtn = NULL;
    }
    
    return rtn;
}

bool MysteryDialogScene::init(const char* category, CCArray* dialogs){
    if (!BaseScene::init()) {
        return false;
    }
    
    this->category = category;
    this->dialogs = dialogs;
    dialogs->retain();
    
    CCString* strBg = CCString::createWithFormat("pic/mystery/my_bg_%s.png", category);
    CCSprite* bg = CCSprite::create(strBg->getCString());
    if (bg) {
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
    }
    else {
        bg = CCSprite::create("pic/taskScene/task_bg.png");
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
    }
    // 任务数据
    curDialogDic = (CCDictionary* )dialogs->objectAtIndex(0);
    subscriptIndex = curDialogDic->valueForKey("id")->intValue();
    DATA->setStoryLabelArr(CCArray::create());
    this->init(curDialogDic);
    num_child = 0;
    OpenToWhichOne = 0;
    taskPhase = 0;
    taskIndex = 0;
    
    index = 0;
    tagIndex = 0;
    nowIndex = 0;
    wordCount = 0;
    contentLength = 0;
    
    openStory = false;
    quanBool = false;
    m_bIsKJSelect = false;
    buttonBool = false;
    startBool = false;
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    allClothesDic = CONFIG->clothes(); // 所有衣服
    
    _ManSpr1 = CCSprite::create();
    this->addChild(_ManSpr1, def_man1_oder);
    _ManSpr2 = CCSprite::create();
    this->addChild(_ManSpr2, def_man2_oder);
    _dkSpr = CCSprite::create();
    this->addChild(_dkSpr, 50);
    
    CCArray* animations = CCArray::createWithCapacity(5);
    char strPei[100] = {};
    for (int i = 2; i >= 1; i--) {
        
        sprintf(strPei, "res/pic/qingjingScene/storyscene/qj_act%d.png", i);
        CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 107, 107));
        animations->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "quanStr");
    
    
    kuangSpr = CCSprite::create("res/pic/taskScene/task_di.png");
    kuangSpr->setAnchorPoint(ccp(.5f, 0));
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5, 80 - 500));
    kuangSpr->setTag(Tag_Task_kuang);
    _dkSpr->addChild(kuangSpr, 5);
    
    // 退出
    CCSprite* fhSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* fhSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    fhSpr2->setColor(ccGRAY);
    backItem = CCMenuItemSprite::create(fhSpr1, fhSpr2, this, menu_selector(MysteryDialogScene::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 100);
    
    // 小箭头
    quanSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_act1.png");
    quanSpr->setPosition(ccp(kuangSpr->getContentSize().width* .96f, -12));
    quanSpr->setTag(0x88999);
    kuangSpr->addChild(quanSpr, 100);
    CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("quanStr");
    CCAnimate* animate = CCAnimate::create(manAnimation);
    CCRepeatForever* rep = CCRepeatForever::create(animate);
    CCDirector::sharedDirector()->getActionManager()->addAction(rep, quanSpr, false);
    quanSpr->setVisible(false);
    
    // 开始
    CCSprite* startSpr1 = CCSprite::create("res/pic/common/btn_start.png");
    CCSprite* startSpr2 = CCSprite::create("res/pic/common/btn_start.png");
    startSpr2->setScale(1.02f);
    startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(MysteryDialogScene::startCallBack));
//    startItem->setPosition(ccp(kuangSpr->getContentSize().width* .85f, kuangSpr->getContentSize().height* .18f));
    startItem->setPosition(ccp(DISPLAY->ScreenWidth()* .83f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* startMenu = CCMenu::create(startItem, NULL);
    startMenu->setPosition(CCPointZero);
    this->addChild(startMenu, 100);
    startItem->setVisible(false);
    
    float widSize = kuangSpr->getContentSize().width;
    float heiSize = kuangSpr->getContentSize().height;
    
    nameKuang = CCSprite::create("res/pic/qingjingScene/storyscene/qj_namekuang.png");
    nameKuang->setPosition(ccp(kuangSpr->getContentSize().width* .2f, kuangSpr->getContentSize().height - 10));
    kuangSpr->addChild(nameKuang);
    nameKuang->setVisible(false);
    
    saidLabel = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 25, CCSizeMake(widSize* .8f, heiSize* .6f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    saidLabel->setAnchorPoint(CCPointZero);
    saidLabel->setPosition(ccp(widSize* .11, 25));
    saidLabel->setTag(0x222);
    saidLabel->setColor(ccc3(80, 63, 68));
    kuangSpr->addChild(saidLabel, 8);
    
    nameLab = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 27);
    nameLab->setPosition(ccp(nameKuang->getContentSize().width* .5f, nameKuang->getContentSize().height* .5f));
    nameLab->setTag(0x111);
    nameLab->setColor(ccWHITE);
    nameLab->enableStroke(ccWHITE, .4f);
    nameKuang->addChild(nameLab, 8);
    this->addButton();
    this->creat_Man(0, 0, 1.f);
    this->initClothes();
    this->init_Man();
    return true;
}

void MysteryDialogScene::onEnter(){
    BaseScene::onEnter();
    BaseScene::hideBaseScene();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    nc->addObserver(this, SEL_CallFuncO(&MysteryDialogScene::LabelColorFhCallBack), "TaskLabelColorFhCallBack", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MysteryDialogScene::nc_clothes_info_400), "HTTP_FINISHED_400", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&MysteryDialogScene::keyBackStatus), .8f);
    //
//    DISPLAY->setZRSpr(_zrSpr1);
//    DISPLAY->blink();
}

void MysteryDialogScene::onExitTransitionDidStart() {
//    DISPLAY->stopBlink();
}

void MysteryDialogScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void MysteryDialogScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    BaseScene::onExit();
}

void MysteryDialogScene::keyBackClicked(){
    num_child++;
    CCLog("===== MysteryDialogScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
}


bool MysteryDialogScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    SPECIAL->showSpotAt(this->getParent(), pTouch->getLocation(), 1);
    if (! openStory) {
        logIndex++;
        
        if (logIndex == 1) {
            this->unschedule(SEL_SCHEDULE(&MysteryDialogScene::getIndex));
            
            if (wordCount < contentLength) {
                wordCount = getContentLength();
                this->setTouchEnabled(false);
                
                this->scheduleOnce(SEL_SCHEDULE(&MysteryDialogScene::openTouch), .5f);
                
            }else{
                
                this->setTouchEnabled(false);
                
                this->scheduleOnce(SEL_SCHEDULE(&MysteryDialogScene::getIndex), .3f);
                
            }
        }
    }else{
        if (startBool) {
            if (this->getChildByTag(0x22222) != NULL) {
                this->removeChildByTag(0x22222);
            }
            
            int tili = DATA->getPlayer()->energy;
            int tili_AllIndex = 12;
            if (tili >= tili_AllIndex) {
                this->setTouchEnabled(false);
                //                LOADING->show_loading();
                //                NET->commit_mission_603(curDialogDic->valueForKey("taskID")->intValue());
            }else{
                LOADING->remove();
                AHMessageBox* mb = AHMessageBox::create_with_message("体力不够,是否购买体力.", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
                mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
            }
        }
    }
    
    return true;
}

void MysteryDialogScene::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "日常对话界面", "点击退出");
    
    if (curDialogDic->valueForKey("states")->intValue() == 1){// ==1 一个人
        if (_touSpr->getChildByTag(0x44444) != NULL) {
            _touSpr->removeChildByTag(0x44444);
        }
        
        CCMoveTo* moveTo = CCMoveTo::create(.4f, ccp(0, 0));
        CCScaleTo* scaleTo = CCScaleTo::create(.4f, 1.f);
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
        CCCallFunc* goTaskScene = CCCallFunc::create(this, SEL_CallFunc(&MysteryDialogScene::gotoMainScene));
        _ManSpr1->runAction(CCSequence::create(CCDelayTime::create(.4f), spawn, NULL));
        
        CCFiniteTimeAction* _actMove1 = CCMoveBy::create(.1f, ccp(0, 35));
        CCFiniteTimeAction* _actMove2 = CCMoveBy::create(.3f, ccp(0, -535));
        _dkSpr->runAction(CCSequence::create(_actMove1, _actMove2, CCDelayTime::create(.2f), goTaskScene, NULL));
    }else if (curDialogDic->valueForKey("states")->intValue() == 2){
        if (_touSpr->getChildByTag(0x44444) != NULL) {
            _touSpr->removeChildByTag(0x44444);
        }
        
        CCFiniteTimeAction* _actMove1 = CCMoveBy::create(.1f, ccp(0, 35));
        CCFiniteTimeAction* _actMove2 = CCMoveBy::create(.3f, ccp(0, -535));
        _dkSpr->runAction(CCSequence::create(_actMove1, _actMove2, NULL));
        
        std::string zishiStr = curDialogDic->valueForKey("zishi_1")->getCString();
        if (zishiStr.size() > 1) {
            CCSprite* spr = (CCSprite* )_ManSpr2->getChildByTag(0x77777);
            if (spr != NULL) {
                if (spr->getChildByTag(0x55555) != NULL) {
                    spr->removeChildByTag(0x55555);
                }
            }
            
            CCMoveTo* moveTo2 = CCMoveTo::create(.4f, ccp(DISPLAY->ScreenWidth()* .5f - 1150, DISPLAY->ScreenHeight()* .45f));
            spr->runAction(CCSequence::create(CCDelayTime::create(.4f), CCSpawn::create(moveTo2, NULL), NULL));
            
            CCMoveTo* moveTo = CCMoveTo::create(.4f, ccp(0, 0));
            CCScaleTo* scaleTo = CCScaleTo::create(.4f, 1.f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
            CCCallFunc* goTaskScene = CCCallFunc::create(this, SEL_CallFunc(&MysteryDialogScene::gotoMainScene));
            _ManSpr1->runAction(CCSequence::create(CCDelayTime::create(.8f), spawn, CCDelayTime::create(.2f), goTaskScene, NULL));
        }
    }
}

void MysteryDialogScene::creat_Man(float _widthFolt, float _heightFloat, float _scaleFloat){
    float widthFolt = DISPLAY->ScreenWidth()* .5f + 95;
    float heightFloat = DISPLAY->ScreenHeight()* .3f;
    float scaleFloat = 1.5f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(widthFolt, heightFloat));
    _ManSpr1->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(widthFolt, heightFloat));
    _ManSpr1->addChild(_touSpr, 210);
    
    _ManSpr1->setPosition(ccp(_widthFolt, _heightFloat));
    _ManSpr1->setScale(_scaleFloat);
    
    int expression_1 = curDialogDic->valueForKey("Expression_1")->intValue();
    if (expression_1 != 0) {
        CCString* expressionStr = CCString::createWithFormat("res/pic/taskScene/biaoqing/%d.png", expression_1);
        CCSprite* expressionSpr = CCSprite::create(expressionStr->getCString());
        expressionSpr->setPosition(ccp(_touSpr->getContentSize().width* .26f, _touSpr->getContentSize().height* .82f));
        expressionSpr->setScale(.6f);
        expressionSpr->setFlipX(true);
        expressionSpr->setTag(0x44444);
        _touSpr->addChild(expressionSpr);
        
        _ManSpr1->setZOrder(def_biaoqing_oder);
    }
    
}
void MysteryDialogScene::creat_Man2(float _widthFolt, float _heightFloat, float _scaleFloat){
    std::string zishiStr = curDialogDic->valueForKey("zishi_1")->getCString();
    int zishi = curDialogDic->valueForKey("zishi_1")->intValue();
    if (zishiStr.size() > 1) {
        CCString* str = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", zishiStr.c_str());
//        CCString* str = CCString::createWithFormat("res/pic/qingjingScene/zishi/%d_1.png", zishi);
        CCSprite* spr = CCSprite::create(str->getCString());
        spr->setPosition(ccp(_widthFolt, _heightFloat));
        spr->setScale(_scaleFloat);
        spr->setTag(0x77777);
        _ManSpr2->addChild(spr);
        
        int expression_2 = curDialogDic->valueForKey("Expression_2")->intValue();
//        expression_2 = 21;
        if (expression_2 != 0) {
            CCString* expressionStr = CCString::createWithFormat("res/pic/taskScene/biaoqing/%d.png", expression_2);
            CCSprite* expressionSpr = CCSprite::create(expressionStr->getCString());
            
            if (zishi == 1002) {
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .7f, spr->getContentSize().height* .85f));
            }else if (zishi == 1003){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .7f, spr->getContentSize().height* .7f));
            }else if (zishi == 1004){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .7f, spr->getContentSize().height* .75f));
            }else if (zishi == 1005){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .7f, spr->getContentSize().height* .8f));
            }else if (zishi == 1006){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .68f, spr->getContentSize().height* .75f));
            }else if (zishi == 1007){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .7f, spr->getContentSize().height* .87f));
            }else if (zishi == 1008){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .7f, spr->getContentSize().height* .68f));
            }else if (zishi == 1009){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .7f, spr->getContentSize().height* .8f));
            }else if (zishi == 1010){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .67f, spr->getContentSize().height* .68f));
            }else if (zishi == 1015){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .64f, spr->getContentSize().height* .8f));
            }else if (zishi == 1102){
                expressionSpr->setPosition(ccp(spr->getContentSize().width* .7f, spr->getContentSize().height* .87f));
            }
            expressionSpr->setTag(0x55555);
            spr->addChild(expressionSpr);
            
            _ManSpr2->setZOrder(def_biaoqing_oder);
        }
    }
}
void MysteryDialogScene::initClothes(){//穿衣服
    float widthFolt = DISPLAY->ScreenWidth()* .5f + 95;
    float heightFloat = DISPLAY->ScreenHeight()* .3f;
    float scaleFloat = 1.5f;
    
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_TS_TouFa; i <= Tag_TS_ZhuangRong; i++) {
        if (i == Tag_TS_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(widthFolt, heightFloat));
                _tfSpr1->setTag(Tag_TS_TouFa1);
                _ManSpr1->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(widthFolt, heightFloat));
                _tfSpr2->setTag(Tag_TS_TouFa2);
                _ManSpr1->addChild(_tfSpr2, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr1->setTag(Tag_TS_TouFa1);
                            _ManSpr1->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr2->setTag(Tag_TS_TouFa2);
                            _ManSpr1->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr3->setTag(Tag_TS_TouFa3);
                            _ManSpr1->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_TS_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(widthFolt, heightFloat));
                _wtSpr1->setTag(Tag_TS_WaiTao1);
                _ManSpr1->addChild(_wtSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr1->setTag(Tag_TS_WaiTao1);
                            _ManSpr1->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr2->setTag(Tag_TS_WaiTao2);
                            _ManSpr1->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr3->setTag(Tag_TS_WaiTao3);
                            _ManSpr1->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_TS_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(widthFolt, heightFloat));
                _sySpr1->setTag(Tag_TS_ShangYi1);
                _ManSpr1->addChild(_sySpr1, 350);
                
//                CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
//                xingSpr1->setFlipX(true);
//                xingSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight()* .488f));
//                _ManSpr1->addChild(xingSpr1, 420);
//                
//                CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
//                xingSpr2->setFlipX(true);
//                xingSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .385f, DISPLAY->ScreenHeight()* .473f));
//                _ManSpr1->addChild(xingSpr2, 420);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        sub_part = clothDic->valueForKey("sub_part")->intValue();
                        
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr1->setTag(Tag_TS_ShangYi1);
                            _ManSpr1->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr2->setTag(Tag_TS_ShangYi2);
                            _ManSpr1->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr3->setTag(Tag_TS_ShangYi3);
                            _ManSpr1->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_TS_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                    _kzSpr1->setTag(Tag_TS_KuZi1);
                    _ManSpr1->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                    _kzSpr1->setTag(Tag_TS_KuZi1);
                    _ManSpr1->addChild(_kzSpr1, 290);
                }
                
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr1->setTag(Tag_TS_KuZi1);
                            _ManSpr1->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr2->setTag(Tag_TS_KuZi2);
                            _ManSpr1->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr3->setTag(Tag_TS_KuZi3);
                            _ManSpr1->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_TS_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(widthFolt, heightFloat));
                _wzSpr1->setTag(Tag_TS_WaZi1);
                _ManSpr1->addChild(_wzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr1->setTag(Tag_TS_WaZi1);
                            _ManSpr1->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr2->setTag(Tag_TS_WaZi2);
                            _ManSpr1->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr3->setTag(Tag_TS_WaZi3);
                            _ManSpr1->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_TS_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(widthFolt, heightFloat));
                _xzSpr1->setTag(Tag_TS_XieZi1);
                _ManSpr1->addChild(_xzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr1->setTag(Tag_TS_XieZi1);
                            _ManSpr1->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr2->setTag(Tag_TS_XieZi2);
                            _ManSpr1->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr3->setTag(Tag_TS_XieZi3);
                            _ManSpr1->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_TS_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr1->addChild(_spSpr1, 50);
                }else{
                    CCDictionary* dic = CONFIG->clothes();// 所有衣服
                    CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                    for (int k = 0; k < clothesArr->count(); k++) {
                        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(k);
                        int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                        if (now_clothes_Id == cloth_id->getValue()) {
                            const CCString* layer1 =  clothDic->valueForKey("layer1");
                            const CCString* layer2 =  clothDic->valueForKey("layer2");
                            const CCString* layer3 =  clothDic->valueForKey("layer3");
                            if (layer1->compare("") != 0) {
                                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr1->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr1->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr1->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_TS_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(widthFolt, heightFloat));
                _bSpr1->setTag(Tag_TS_Bao1);
                _ManSpr1->addChild(_bSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr1->setTag(Tag_TS_Bao1);
                            _ManSpr1->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr2->setTag(Tag_TS_Bao2);
                            _ManSpr1->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr3->setTag(Tag_TS_Bao3);
                            _ManSpr1->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_TS_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(widthFolt, heightFloat));
                _zrSpr1->setTag(Tag_TS_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr1->addChild(_zrSpr1, 220);
                //
//                DISPLAY->setCurZRId(90000);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _zrSpr1->setTag(Tag_TS_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr1->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                            //
//                            DISPLAY->setCurZRId(layer1->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}

void MysteryDialogScene::init(CCDictionary* dic){
    content = dic->valueForKey("said")->getCString();
    CCLOG("content == %s", content.c_str());
    contentLength = 0;
    
    int length = (int)content.length();
    int i = 0;
    while (i < length) {
        char ch = getContent()[i];
        
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        contentLength++;
    }
}

std::string MysteryDialogScene::getContentByLength(int length){
    if (length >= contentLength) {
        return getContent();
    }
    
    int i = 0;
    int _index = 0;
    while (_index < length) {
        char ch = getContent()[i];
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        _index++;
    }
    std::string str = content.substr(0, i);
    return str;
}
int MysteryDialogScene::getContentLength(){
    return contentLength;
}

void MysteryDialogScene::init_Man(){
    if (_touSpr->getChildByTag(0x44444) != NULL) {
        _touSpr->removeChildByTag(0x44444);
    }
    CCSprite* spr = (CCSprite* )_ManSpr2->getChildByTag(0x77777);
    if (spr != NULL) {
        if (spr->getChildByTag(0x55555) != NULL) {
            spr->removeChildByTag(0x55555);
        }
    }
    
    if (curDialogDic->valueForKey("states")->intValue() == 1){// ==1 一个人
        CCLOG("============     1");
        CCMoveTo* moveTo = CCMoveTo::create(.4f, ccp(0, -DISPLAY->ScreenHeight()* .2f));
        CCScaleTo* scaleTo = CCScaleTo::create(.4f, 1.2f);
        _ManSpr1->runAction(CCSpawn::create(moveTo, scaleTo, NULL));
        
        _dkSpr->setVisible(true);
        CCFiniteTimeAction* _actMove1 = CCMoveBy::create(.3f, ccp(0, 535));
        CCFiniteTimeAction* _actMove2 = CCMoveBy::create(.1f, ccp(0, -35));
        _dkSpr->runAction(CCSequence::create(_actMove1, _actMove2, NULL));
        
        this->scheduleOnce(SEL_SCHEDULE(&MysteryDialogScene::dialogueControl), .8f);
    }else if (curDialogDic->valueForKey("states")->intValue() == 2){// ==2 两个人
        CCLOG("============     2");
//        DISPLAY->ScreenWidth()* .5f + 95
//        CCMoveTo* moveTo = CCMoveTo::create(.4f, ccp(DISPLAY->ScreenWidth()* .4f, -DISPLAY->ScreenHeight()* .1f));
        CCFiniteTimeAction* moveTo = CCMoveBy::create(.4f, ccp(+250, -100));
        CCScaleTo* scaleTo = CCScaleTo::create(.4f, 1.f);
        _ManSpr1->runAction(CCSpawn::create(moveTo, scaleTo, NULL));
        
        std::string zishiStr = curDialogDic->valueForKey("zishi_1")->getCString();
        if (zishiStr.size() > 1) {
            CCString* str = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", zishiStr.c_str());
            CCSprite* spr = CCSprite::create(str->getCString());
            spr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f - 1150, DISPLAY->ScreenHeight()* .5f));
            spr->setTag(0x77777);
            _ManSpr2->addChild(spr);
            
            CCMoveTo* moveTo2 = CCMoveTo::create(.4f, ccp(DISPLAY->ScreenWidth()* .5f - 160, DISPLAY->ScreenHeight()* .45f));
            CCScaleTo* scaleTo2 = CCScaleTo::create(.4f, .9f);
            spr->runAction(CCSequence::create(CCDelayTime::create(.5f), CCSpawn::create(moveTo2, scaleTo2, NULL), NULL));
        }
        
        _dkSpr->setVisible(true);
        CCFiniteTimeAction* _actMove1 = CCMoveBy::create(.3f, ccp(0, 535));
        CCFiniteTimeAction* _actMove2 = CCMoveBy::create(.1f, ccp(0, -35));
        _dkSpr->runAction(CCSequence::create(CCDelayTime::create(.8f), _actMove1, _actMove2, NULL));
        
        this->scheduleOnce(SEL_SCHEDULE(&MysteryDialogScene::dialogueControl), 1.3f);
    }
}

void MysteryDialogScene::dialogueControl(float dt){
//    CCLOG("id ==== %s", curDialogDic->valueForKey("id")->getCString());
//    CCLOG("taskID ==== %d", curDialogDic->valueForKey("taskID")->intValue());
//    CCLOG("name ==== %s", curDialogDic->valueForKey("name")->getCString());
//    CCLOG("nameID ==== %s", curDialogDic->valueForKey("nameID")->getCString());
//    CCLOG("states ==== %d", curDialogDic->valueForKey("states")->intValue());
//    CCLOG("zishi_1 ==== %s", curDialogDic->valueForKey("zishi_1")->getCString());
//    CCLOG("Expression_1 ==== %d", curDialogDic->valueForKey("Expression_1")->intValue());
//    CCLOG("zishi_2 ==== %s", curDialogDic->valueForKey("zishi_2")->getCString());
//    CCLOG("Expression_2 ==== %d", curDialogDic->valueForKey("Expression_2")->intValue());
//    CCLOG("said ==== %s", curDialogDic->valueForKey("said")->getCString());
//    CCLOG("next ==== %d", curDialogDic->valueForKey("next")->intValue());
//    
//    CCLOG("tag1 ==== %d", curDialogDic->valueForKey("tag1")->intValue());
//    CCLOG("tag2 ==== %d", curDialogDic->valueForKey("tag2")->intValue());
//    CCLOG("tag3 ==== %d", curDialogDic->valueForKey("tag3")->intValue());
    
    if (_touSpr->getChildByTag(0x44444) != NULL) {
        _touSpr->removeChildByTag(0x44444);
    }
    CCSprite* spr = (CCSprite* )_ManSpr2->getChildByTag(0x77777);
    if (spr != NULL) {
        if (spr->getChildByTag(0x55555) != NULL) {
            spr->removeChildByTag(0x55555);
        }
    }
    _ManSpr1->setZOrder(def_man1_oder);
    _ManSpr2->setZOrder(def_man2_oder);
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    this->setTouchEnabled(false);
    logIndex = 0;
    
    if (curDialogDic->valueForKey("states")->intValue() == 1){// ==1 一个人
        
        this->recordLabel(curDialogDic->valueForKey("name")->getCString(), curDialogDic->valueForKey("said")->getCString());
        
        this->unschedule(schedule_selector(MysteryDialogScene::logic));
        openStory = true;
        
        this->removeMan();
        this->emptyLabel();
        this->creat_Man(0, -DISPLAY->ScreenHeight()* .2f, 1.2f);
        this->initClothes();
        this->schedule(schedule_selector(MysteryDialogScene::logic), .1f);
        
    }else if (curDialogDic->valueForKey("states")->intValue() == 2){// ==2 两个人
        
        this->removeMan();
        this->recordLabel(curDialogDic->valueForKey("name")->getCString(), curDialogDic->valueForKey("said")->getCString());
        
        this->unschedule(schedule_selector(MysteryDialogScene::logic));
        
        openStory = true;
        
        this->emptyLabel();
        this->creat_Man(+250, -100, 1.f);
        this->initClothes();
        this->creat_Man2(DISPLAY->ScreenWidth()* .5f - 160, DISPLAY->ScreenHeight()* .45f, .9f);
        this->schedule(schedule_selector(MysteryDialogScene::logic), .1f);
    }
}
void MysteryDialogScene::emptyLabel(){
    nameLab->setString("");
    saidLabel->setString(getContentByLength(wordCount).c_str());
}
void MysteryDialogScene::removeMan(){
    _ManSpr1->removeAllChildrenWithCleanup(true);
    _ManSpr2->removeAllChildrenWithCleanup(true);
}

void MysteryDialogScene::addButton(){
    
    // 回顾
    CCSprite* hgSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_huikan.png");
    CCSprite* hgSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_huikan.png");
    hgSpr2->setColor(ccGRAY);
    CCMenuItem* hkButton = CCMenuItemSprite::create(hgSpr1, hgSpr2, this, menu_selector(MysteryDialogScene::huikanCallBack));
    hkButton->setPosition(ccp(kuangSpr->getContentSize().width* .73, kuangSpr->getContentSize().height + 10));
    
    // 快进
    CCSprite* kjSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin1.png");
    CCSprite* kjSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin1.png");
    kjSpr2->setColor(ccGRAY);
    CCSprite* ztSpr3 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin2.png");
    CCSprite* ztSpr4 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin2.png");
    ztSpr4->setColor(ccGRAY);
    CCMenuItem* kuaijinItemOn = CCMenuItemSprite::create(kjSpr1, kjSpr2);
    CCMenuItem* kuaijinItemOff = CCMenuItemSprite::create(ztSpr3, ztSpr4);
    kuaijinToggleItem = CCMenuItemToggle::createWithTarget(this, menu_selector(MysteryDialogScene::kuaijinCallBack), kuaijinItemOn,kuaijinItemOff,NULL);
    kuaijinToggleItem->setPosition(ccp(kuangSpr->getContentSize().width* .88, kuangSpr->getContentSize().height + 10));
    if (m_bIsKJSelect) {
        kuaijinToggleItem->setSelectedIndex(1);
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
    
    CCMenu* menu = CCMenu::create(hkButton, kuaijinToggleItem, NULL);
    menu->setPosition(CCPointZero);
    menu->setTag(0x88888);
    kuangSpr->addChild(menu, 30);
}

void MysteryDialogScene::huikanCallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "神秘事件 对话界面", "点击回顾");
    
    TaskLabelColorLayer* layer = TaskLabelColorLayer::create_with_index(labStr.c_str());
    layer->setTag(0x999999);
    this->addChild(layer, 100);
}

void MysteryDialogScene::kuaijinCallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "日常对话界面", "点击快进");
    
    if (!buttonBool) {
        buttonBool = true;
        
        this->setTouchEnabled(false);
        
        kuaijinToggleItem->setSelectedIndex(1);
        
        index = curDialogDic->valueForKey("next")->intValue();
        if (index != -1 || index != -2 || index != -3) {
            wordCount = getContentLength();
            this->unschedule(schedule_selector(MysteryDialogScene::logic));
            this->schedule(schedule_selector(MysteryDialogScene::logic), .1f);
        }
    }else{
        buttonBool = false;
        
        this->setTouchEnabled(true);
        
        kuaijinToggleItem->setSelectedIndex(0);
    }
}

void MysteryDialogScene::recordLabel(std::string nameStr, std::string saidStr){
    if (nameStr.length() != 0) {
        labStr.append(nameStr.c_str());
        labStr.append("\n\t");
    }
    if (saidStr.length() != 0) {
        labStr.append(saidStr.c_str());
        labStr.append("\n\n");
    }
    
    nowIndex++;
    if (nowIndex == 5) {
        nowIndex = 0;
//        char* labChar = strdup(labStr.c_str());
        
        CCString* str = CCString::createWithFormat("%s", labStr.c_str());
        DATA->getStoryLabelArr()->addObject(str);
        labStr = "";
    }
}

void MysteryDialogScene::logic(float dt){
    openStory = false;
    
    this->unschedule(schedule_selector(MysteryDialogScene::logic));
    if (!buttonBool) {
        this->openTouch(0);
    }
    
    std::string _name = curDialogDic->valueForKey("name")->getCString();
    if (_name.length() <= 0) {
        nameKuang->setVisible(false);
    }else{
        nameKuang->setVisible(true);
    }
    nameLab->setString(_name.c_str());
    
    if (wordCount > contentLength) {
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        if (curDialogDic->valueForKey("next")->intValue() == -1 ||
            curDialogDic->valueForKey("next")->intValue() == -2 ||
            curDialogDic->valueForKey("next")->intValue() == -3 ||
            curDialogDic->valueForKey("next")->intValue() == -4 ||
            curDialogDic->valueForKey("next")->intValue() == -5 ||
            curDialogDic->valueForKey("next")->intValue() == -6) {
            startItem->setVisible(true);
            
            if (DATA->current_guide_step() == 4) {
                CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideQuan");
            }
        }else{
            if (!quanBool) {
                quanBool = true;
                
                quanSpr->setVisible(true);
            }
        }
        
        return;
    }else{
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        if (buttonBool) {
            wordCount = getContentLength();
            saidLabel->setString(getContentByLength(wordCount).c_str());
            this->scheduleOnce(SEL_SCHEDULE(&MysteryDialogScene::getIndex), .1f);
        }
        
        this->schedule(schedule_selector(MysteryDialogScene::logic), .1f);
    }
}

void MysteryDialogScene::startCallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    CCMenuItem* item = (CCMenuItem* )pSender;
    item->setEnabled(false);
    
    if (DATA->getClothes()->has_init_clothes == true) {
        this->nc_clothes_info_400(NULL);
    }
    else {
        LOADING->show_loading();
        NET->owned_clothes_400();
    }
}

void MysteryDialogScene::nc_clothes_info_400(CCObject* pSender){
    LOADING->remove();
    
//    int categ = ccs(this->category)->intValue();
    CCDictionary* firstDialog = dynamic_cast<CCDictionary*>(this->dialogs->objectAtIndex(0));
    const CCString* tishi = firstDialog->valueForKey("tishi");
    CCLayer* layer = ClothesScene::create_with_mystery(3, this->category, tishi->getCString());
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MysteryDialogScene::openTouch(float dt){
    logIndex = 0;
    this->setTouchEnabled(true);
}

void MysteryDialogScene::getIndex(float dt){
    if (quanSpr != NULL) {
        quanBool = false;
        quanSpr->setVisible(false);
    }
    
    index = curDialogDic->valueForKey("next")->intValue();
    CCLOG("MysteryDialogScene::getIndex() - idx = %d", index);
    if (index == -1) { // 结束
        if (buttonBool) {
            buttonBool = false;
            logIndex = 0;
            return;
        }
        
        this->setTouchEnabled(false);
    }
    else {
        // 更新对话数据
        curDialogDic = (CCDictionary* )dialogs->objectAtIndex(index);
        this->init(curDialogDic);
        wordCount = 0;
        this->dialogueControl(dt);
    }
}

void MysteryDialogScene::LabelColorFhCallBack(CCObject* pSender){
    if (this->getChildByTag(0x999999) != NULL) {
        this->removeChildByTag(0x999999);
    }
    
    this->setTouchEnabled(true);
}

void MysteryDialogScene::gotoMainScene(){
    CCLayer* layer = MainScene::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(scene);
}

//void MysteryDialogScene::clothesCallBack(CCObject* pSender){
//    CCLayer* layer = ClothesScene::create_with_type(2, 0, 0);
//    CCScene* scene = CCScene::create();
//    scene->addChild(layer);
//    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
//    CCDirector::sharedDirector()->replaceScene(trans);
//}


