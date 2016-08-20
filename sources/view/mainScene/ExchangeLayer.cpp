//
//  ExchangeLayer.cpp
//  tiegao
//
//  Created by mac on 16-8-15.
//
//

#include "ExchangeLayer.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "NetManager.h"
#include "Loading2.h"
#include "DisplayManager.h"
#include "QingjingScene.h"


ExchangeLayer::ExchangeLayer(){
    
}
ExchangeLayer::~ExchangeLayer(){
    
}

void ExchangeLayer::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&ExchangeLayer::_311CallBack), "HTTP_FINISHED_311", NULL);
}
void ExchangeLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

bool ExchangeLayer::init(){
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(250, 250, 250, 0), DISPLAY->ScreenWidth(), DISPLAY->ScreenHeight()));
        
        this->setTouchSwallowEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchEnabled(true);
        
        touchBool = false;
        nowIndex = 0;
        
        allClothesDic = CONFIG->clothes();// 所有衣服
        suitsArr = (CCArray* )DATA->getOperation()->getSuits();
        userArr = DATA->getOperation()->getGashaponUser();
        myClothesTemp = (CCDictionary*)suitsArr->objectAtIndex(nowIndex);
        
        bgSpr = CCSprite::create("res/pic/gashapon/gashapon_bg2.png");
        bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        this->addChild(bgSpr);
        
        dikuangSpr = CCSprite::create("res/pic/gashapon/exchange/exchange_bg.png");
        dikuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        this->addChild(dikuangSpr, 10);
        
        _ManSpr = CCSprite::create();
        dikuangSpr->addChild(_ManSpr, 3);
        
        this->formattingData();
        this->creat_View();
        this->creat_Man();
        this->initClothes();
        
        this->scheduleOnce(SEL_SCHEDULE(&ExchangeLayer::openTouch), .5f);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

bool ExchangeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (touchBool) {
        CCPoint location = pTouch->getLocation();
        if (! dikuangSpr->boundingBox().containsPoint(location)) {
            this->removeFromParentAndCleanup(true);
        }
    }
    
    return true;
}

void ExchangeLayer::openTouch(){
    touchBool = true;
}

void ExchangeLayer::formattingData(){
    allClthoes = CCArray::create();
    for (int i = 1; i <= suitsArr->count(); i++) {
        CCDictionary* allDic = DATA->getOperation()->getGashaponTemplate();
        CCDictionary* dic = CCDictionary::create();
        CCDictElement* pElem = NULL;
        int index = 0;
        CCDICT_FOREACH(allDic, pElem) {
            CCDictionary* pElemDic = (CCDictionary* )pElem->getObject();
            CCInteger* value = (CCInteger* )pElemDic->objectForKey("group");
            if (value->getValue() == i) {
                CCString* indexStr = CCString::createWithFormat("%d", index);
                index++;
                dic->setObject(pElemDic, indexStr->getCString());
            }
            
        }
        allClthoes->addObject(dic);
    }
    
    for (int i = 0; i < allClthoes->count(); i++) {
        CCDictionary* dic = (CCDictionary* )allClthoes->objectAtIndex(i);
        for (int k = 0; k < dic->count(); k++) {
            CCString* str = CCString::createWithFormat("%d", k);
            CCDictionary* kDic = (CCDictionary* )dic->objectForKey(str->getCString());
            CCString* id = (CCString*)kDic->objectForKey("id");
            CCInteger* group = (CCInteger*)kDic->objectForKey("group");
            CCLog("<><><> group == %d, id == %s", group->getValue(), id->getCString());
        }
    }
}
// OperationComp   exchange_clothes_311
void ExchangeLayer::creat_View(){
    
    CCLog("allClthoes === %d", allClthoes->count());
    CCDictionary* dic = (CCDictionary* )allClthoes->objectAtIndex(nowIndex);
    for (int i = 0; i < dic->count(); i++) {
        CCString* str = CCString::createWithFormat("%d", i);
        CCDictionary* kDic = (CCDictionary* )dic->objectForKey(str->getCString());
        CCString* id = (CCString*)kDic->objectForKey("id");
        CCInteger* price = (CCInteger* )kDic->objectForKey("price");
        int idIndex = atoi(id->getCString());
        CCSprite* diSpr = CCSprite::create("res/pic/gashapon/exchange/exchange_di.png");
        float heightFloat = .2f;
        if (i == 1 || i == 3 || i == 5) {
            if (i == 1) {
                diSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .85f, dikuangSpr->getContentSize().height* (.86f - heightFloat* 0)));
            }else if (i == 3){
                diSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .85f, dikuangSpr->getContentSize().height* (.86f - heightFloat* 1)));
            }else if (i == 5){
                diSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .85f, dikuangSpr->getContentSize().height* (.86f - heightFloat* 2)));
            }
        }else{
            if (i == 0) {
                diSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .15f, dikuangSpr->getContentSize().height* (.86f - heightFloat* 0)));
            }else if (i == 2){
                diSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .15f, dikuangSpr->getContentSize().height* (.86f - heightFloat* 1)));
            }else if (i == 4){
                diSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .15f, dikuangSpr->getContentSize().height* (.86f - heightFloat* 2)));
            }else if (i == 6){
                diSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .15f, dikuangSpr->getContentSize().height* (.86f - heightFloat* 3)));
            }
        }
        this->creat_Clothes(diSpr, idIndex);
        diSpr->setTag(i);
        dikuangSpr->addChild(diSpr, 10);
        
        CCSprite* buttonSpr1 = CCSprite::create("res/pic/gashapon/exchange/exchange_button1.png");
        this->creat_Label(buttonSpr1, price->getValue(), 1);
        CCSprite* buttonSpr2 = CCSprite::create("res/pic/gashapon/exchange/exchange_button1.png");
        this->creat_Label(buttonSpr2, price->getValue(), 1);
        buttonSpr2->setScale(1.02f);
        CCSprite* buttonSpr3 = CCSprite::create("res/pic/gashapon/exchange/exchange_button2.png");
        this->creat_Label(buttonSpr3, price->getValue(), 2);
        CCMenuItem* exchangeItem = CCMenuItemSprite::create(buttonSpr1, buttonSpr2, buttonSpr3, this, menu_selector(ExchangeLayer::buttonCallBack));
        exchangeItem->setAnchorPoint(ccp(.5f, 1.f));
        exchangeItem->setPosition(ccp(diSpr->getContentSize().width* .47f, diSpr->getContentSize().height* .2f));
        exchangeItem->setTag(idIndex);
        CCMenu* menu = CCMenu::create(exchangeItem, NULL);
        menu->setPosition(CCPointZero);
        menu->setTag(i);
        diSpr->addChild(menu, 5);
        
        
        for (int k = 0; k < userArr->count(); k++) {
//            CCString* userIdStr = (CCString* )userArr->objectAtIndex(k);
            int userId = ((CCInteger*)userArr->objectAtIndex(k))->getValue();
            if (userId == idIndex) {
                exchangeItem->setEnabled(false);
                
                CCSprite* yiSpr = CCSprite::create("res/pic/gashapon/exchange/exchange_yi.png");
                yiSpr->setPosition(ccp(diSpr->getContentSize().width* .7f, diSpr->getContentSize().height* .35f));
                diSpr->addChild(yiSpr, 10);
            }
        }
    }
    
    // 1/2
    CCSprite* kuangSpr2 = CCSprite::create("res/pic/gashapon/exchange/exchange_kuang.png");
    kuangSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* .5f, dikuangSpr->getContentSize().height* .14f));
    dikuangSpr->addChild(kuangSpr2, 10);
    
    CCString* labelStr2 = CCString::createWithFormat("%d/%d", nowIndex + 1, suitsArr->count());
    CCLabelTTF* label2 = CCLabelTTF::create(labelStr2->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(kuangSpr2->getContentSize().width* .95f, 18), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    label2->setPosition(ccp(kuangSpr2->getContentSize().width* .5f, kuangSpr2->getContentSize().height* .5f));
    label2->setColor(ccc3(249, 128, 128));
    kuangSpr2->addChild(label2);
    
    // 碎片数量
    CCSprite* tiaoSpr = CCSprite::create("res/pic/gashapon/exchange/exchange_tiao.png");
    tiaoSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .67f, dikuangSpr->getContentSize().height* .072f));
    dikuangSpr->addChild(tiaoSpr);
    CCString* exchangeStr = CCString::createWithFormat("已有碎片数量: %d", DATA->getOperation()->getPiece());
    exchangeLabel = CCLabelTTF::create(exchangeStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(tiaoSpr->getContentSize().width* .9f, 25), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    exchangeLabel->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .5f));
    exchangeLabel->setColor(ccWHITE);
    tiaoSpr->addChild(exchangeLabel);
    
    
    CCSprite* jiantouSpr1_1 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr1_1->setFlipX(true);
    CCSprite* jiantouSpr1_2 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr1_2->setFlipX(true);
    jiantouSpr1_2->setScale(1.02f);
    jiantouItem1 = CCMenuItemSprite::create(jiantouSpr1_1, jiantouSpr1_2, this, menu_selector(ExchangeLayer::jiantou1CallBack));
    jiantouItem1->setPosition(ccp(dikuangSpr->getContentSize().width* .3f, dikuangSpr->getContentSize().height* .61f));
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.5f, ccp(dikuangSpr->getContentSize().width* .3f - 15, dikuangSpr->getContentSize().height* .61f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.8f, ccp(dikuangSpr->getContentSize().width* .3f, dikuangSpr->getContentSize().height* .61f));
    CCSequence* seq1 = CCSequence::create(moveTo1_1, moveTo1_2, NULL);
    jiantouItem1->runAction(CCRepeatForever::create(seq1));
    
    CCSprite* jiantouSpr2_1 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    CCSprite* jiantouSpr2_2 = CCSprite::create("res/pic/qingjingScene/gj_jiantou.png");
    jiantouSpr2_2->setScale(1.02f);
    jiantouItem2 = CCMenuItemSprite::create(jiantouSpr2_1, jiantouSpr2_2, this, menu_selector(ExchangeLayer::jiantou2CallBack));
    jiantouItem2->setPosition(ccp(dikuangSpr->getContentSize().width* .7f, dikuangSpr->getContentSize().height* .61f));
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.5f, ccp(dikuangSpr->getContentSize().width* .7f + 15, dikuangSpr->getContentSize().height* .61f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.8f, ccp(dikuangSpr->getContentSize().width* .7f, dikuangSpr->getContentSize().height* .61f));
    CCSequence* seq2 = CCSequence::create(moveTo2_1, moveTo2_2, NULL);
    jiantouItem2->runAction(CCRepeatForever::create(seq2));
    
    CCMenu* jiantouMenu = CCMenu::create(jiantouItem1, jiantouItem2, NULL);
    jiantouMenu->setPosition(CCPointZero);
    dikuangSpr->addChild(jiantouMenu, 20);
    
    if (nowIndex == suitsArr->count() - 1) {
        jiantouItem2->setColor(ccGRAY);
        jiantouItem2->setEnabled(true);
        jiantouItem1->setColor(ccWHITE);
        jiantouItem1->setEnabled(true);
    }else if (nowIndex == 0){
        jiantouItem1->setColor(ccGRAY);
        jiantouItem1->setEnabled(false);
        jiantouItem2->setColor(ccWHITE);
        jiantouItem2->setEnabled(true);
    }else{
        jiantouItem1->setColor(ccWHITE);
        jiantouItem1->setEnabled(true);
        jiantouItem2->setColor(ccWHITE);
        jiantouItem2->setEnabled(true);
    }
}
void ExchangeLayer::jiantou1CallBack(CCObject* pSender){
    jiantouItem1->setEnabled(false);
    jiantouItem2->setEnabled(false);
    nowIndex--;
    if (nowIndex >= 0) {
        _ManSpr->removeAllChildrenWithCleanup(true);
        dikuangSpr->removeAllChildrenWithCleanup(true);
        
        _ManSpr = CCSprite::create();
        dikuangSpr->addChild(_ManSpr, 3);
        myClothesTemp = (CCDictionary*)suitsArr->objectAtIndex(nowIndex);
        
        this->formattingData();
        this->creat_View();
        this->creat_Man();
        this->initClothes();
    }else{
        nowIndex = 0;
    }
    this->scheduleOnce(SEL_SCHEDULE(&ExchangeLayer::updataButton), .3f);
}
void ExchangeLayer::jiantou2CallBack(CCObject* pSender){
    jiantouItem1->setEnabled(false);
    jiantouItem2->setEnabled(false);
    nowIndex++;
    if (nowIndex < suitsArr->count()) {
        _ManSpr->removeAllChildrenWithCleanup(true);
        dikuangSpr->removeAllChildrenWithCleanup(true);
        
        _ManSpr = CCSprite::create();
        dikuangSpr->addChild(_ManSpr, 3);
        myClothesTemp = (CCDictionary*)suitsArr->objectAtIndex(nowIndex);
        
        this->formattingData();
        this->creat_View();
        this->creat_Man();
        this->initClothes();
    }else{
        nowIndex = suitsArr->count() - 1;
    }
    this->scheduleOnce(SEL_SCHEDULE(&ExchangeLayer::updataButton), .3f);
}
void ExchangeLayer::updataButton(){
    if (nowIndex == suitsArr->count() - 1) {
        jiantouItem2->setColor(ccGRAY);
        jiantouItem2->setEnabled(true);
        jiantouItem1->setColor(ccWHITE);
        jiantouItem1->setEnabled(true);
    }else if (nowIndex == 0){
        jiantouItem1->setColor(ccGRAY);
        jiantouItem1->setEnabled(false);
        jiantouItem2->setColor(ccWHITE);
        jiantouItem2->setEnabled(true);
    }else{
        jiantouItem1->setColor(ccWHITE);
        jiantouItem1->setEnabled(true);
        jiantouItem2->setColor(ccWHITE);
        jiantouItem2->setEnabled(true);
    }
}

void ExchangeLayer::creat_Label(CCSprite* spr, int price, int index){
    CCString* priceStr = CCString::createWithFormat("%d碎片", price);
//    CCString* priceStr = CCString::createWithFormat("%d碎片", 999);
    CCLabelTTF* label = CCLabelTTF::create(priceStr->getCString(), DISPLAY->fangzhengFont(), 15, CCSizeMake(spr->getContentSize().width, 15), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    if (index == 1) {
        label->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .7f));
    }else if (index == 2){
        label->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .5f));
    }
    label->setColor(ccWHITE);
    spr->addChild(label);
}
void ExchangeLayer::creat_Clothes(CCSprite* spr, int index){
    CCString* iconStr;
    if (index >= 10000 && index < 20000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/1toufa/icon%d.png", index);
    }else if (index >= 20000 && index < 30000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/2waitao/icon%d.png", index);
    }else if (index >= 30000 && index < 40000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/3shangyi/icon%d.png", index);
    }else if (index >= 40000 && index < 50000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/4kuzi/icon%d.png", index);
    }else if (index >= 50000 && index < 60000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/5wazi/icon%d.png", index);
    }else if (index >= 60000 && index < 70000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/6xiezi/icon%d.png", index);
    }else if (index >= 70000 && index < 80000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/7shipin/icon%d.png", index);;
    }else if (index >= 80000 && index < 90000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/8bao/icon%d.png", index);
    }else if (index >= 90000 && index < 100000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/9zhuangrong/icon%d.png", index);
    }
    CCSprite* iconSpr = CCSprite::create(iconStr->getCString());
    iconSpr->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .5f));
    iconSpr->setScale(.7f);
    spr->addChild(iconSpr);
}

void ExchangeLayer::creat_Man(){
    float widthFolt = .63f;
    float heightFloat = .61f;
    float scaleFloat = .63f;
    bool flipxBool = false;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setFlipX(flipxBool);
    _touSpr->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void ExchangeLayer::initClothes(){//穿衣服
    float widthFolt = .63f;
    float heightFloat = .61f;
    float scaleFloat = .63f;
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_QJ_TouFa; i <= Tag_QJ_ZhuangRong; i++) {
        if (i == Tag_QJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                _tfSpr1->setTag(Tag_QJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                _tfSpr2->setTag(Tag_QJ_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
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
                            _tfSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _tfSpr1->setTag(Tag_QJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _tfSpr2->setTag(Tag_QJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _tfSpr3->setTag(Tag_QJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                _wtSpr1->setTag(Tag_QJ_WaiTao1);
                _ManSpr->addChild(_wtSpr1, 50);
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
                            _wtSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _wtSpr1->setTag(Tag_QJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _wtSpr2->setTag(Tag_QJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _wtSpr3->setTag(Tag_QJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                _sySpr1->setTag(Tag_QJ_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
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
                            _sySpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _sySpr1->setTag(Tag_QJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _sySpr2->setTag(Tag_QJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _sySpr3->setTag(Tag_QJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                    _kzSpr1->setTag(Tag_QJ_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                    
                }else {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                    _kzSpr1->setTag(Tag_QJ_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
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
                            _kzSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _kzSpr1->setTag(Tag_QJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _kzSpr2->setTag(Tag_QJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _kzSpr3->setTag(Tag_QJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                _wzSpr1->setTag(Tag_QJ_WaZi1);
                _ManSpr->addChild(_wzSpr1, 50);
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
                            _wzSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _wzSpr1->setTag(Tag_QJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _wzSpr2->setTag(Tag_QJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _wzSpr3->setTag(Tag_QJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                _xzSpr1->setTag(Tag_QJ_XieZi1);
                _ManSpr->addChild(_xzSpr1, 50);
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
                            _xzSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _xzSpr1->setTag(Tag_QJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _xzSpr2->setTag(Tag_QJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _xzSpr3->setTag(Tag_QJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr->addChild(_spSpr1, 50);
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
                                _spSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_QJ_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                _bSpr1->setTag(Tag_QJ_Bao1);
                _ManSpr->addChild(_bSpr1, 50);
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
                            _bSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _bSpr1->setTag(Tag_QJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _bSpr2->setTag(Tag_QJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _bSpr3->setTag(Tag_QJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_QJ_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                _zrSpr1->setTag(Tag_QJ_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
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
                            _zrSpr1->setPosition(ccp(dikuangSpr->getContentSize().width* widthFolt, dikuangSpr->getContentSize().height* heightFloat));
                            _zrSpr1->setTag(Tag_QJ_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}

void ExchangeLayer::buttonCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    CCString* str = CCString::createWithFormat("%d", item->getTag());
    
    LOADING->show_loading();
    NET->exchange_clothes_311(str->getCString());
}

void ExchangeLayer::_311CallBack(CCObject* pSender){
    LOADING->remove();
    
    dikuangSpr->removeAllChildrenWithCleanup(true);
    userArr = DATA->getOperation()->getGashaponUser();
    
    _ManSpr = CCSprite::create();
    dikuangSpr->addChild(_ManSpr, 3);
    
    this->formattingData();
    this->creat_View();
    
    this->creat_Man();
    this->initClothes();
}

