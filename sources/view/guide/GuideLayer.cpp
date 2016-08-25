//
//  GuideLayer.cpp
//  tiegao
//
//  Created by mac on 16-8-24.
//
//

#include "GuideLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"



GuideLayer::GuideLayer(){
    
}
GuideLayer::~GuideLayer(){
    
}

bool GuideLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
//    m_pTarget = CCRenderTexture::create(DISPLAY->ScreenWidth(), DISPLAY->ScreenHeight(), kCCTexture2DPixelFormat_RGBA8888);
//    m_pTarget->setPosition(ccp(DISPLAY->ScreenWidth() * .5f, DISPLAY->ScreenHeight() * .5f));
//    m_pTarget->setOpacity(155);
//    this->addChild(m_pTarget);
//    this->setHighlightRect(CCRectMake(100, 100, 0, 0));// x,y,w,h
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    wordCount = 0;
    guideIndex = 0;
    touchBool = false;
    
    guideSpr = CCSprite::create();
    this->addChild(guideSpr, 10);
    
    this->creatAnimation();
    
    
    return true;
}
CCScene* GuideLayer::scene(){
    CCScene* scene = CCScene::create();
    GuideLayer* layer = GuideLayer::create();
    scene->addChild(layer);
    
    return scene;
}

void GuideLayer::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
}
void GuideLayer::onExit(){
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeAllObservers(this);
    
    CCLayer::onExit();
}

void GuideLayer::setHighlightRect(const CCRect& rect)
{
    CCSprite* pMask = CCSprite::create("res/pic/guide/guide_kuang1.png");
    pMask->setPosition(ccp(rect.origin.x, rect.origin.y));
    pMask->setAnchorPoint(ccp(0,0));
    pMask->setScaleX(rect.size.width / pMask->getContentSize().width);
    pMask->setScaleY(rect.size.height / pMask->getContentSize().height);
    
    ccBlendFunc blend;
    blend.src = GL_ZERO;
    blend.dst = GL_ONE_MINUS_SRC_ALPHA;
    pMask->setBlendFunc(blend);
    
    m_pTarget->clear(0, 0, 0, .5f);
    m_pTarget->begin();
    pMask->visit();
    m_pTarget->end();
}


GuideLayer* GuideLayer::create_with_guide(int _index){
    GuideLayer* rtn = GuideLayer::create();
    rtn->init_with_guide(_index);
    
    return rtn;
}
void GuideLayer::init_with_guide(int _index){
    guideIndex = _index;
    
    if (guideIndex == 1) {
        if (!DATA->_guideBool1[0]) {
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]){
            DATA->_guideBool1[2] = true;
            this->creatShou();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
    }else if (guideIndex == 2){
        if (!DATA->_guideBool2[0]) {
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }
    }
    
    
}

bool GuideLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    if (touchBool) {
        touchBool = false;
        
        if (guideIndex == 1) {
            if (!DATA->_guideBool1[0]) {
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool1[0] && !DATA->_guideBool1[1]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool1[2] && !DATA->_guideBool1[3]){
                if (isContainTouchPoint1_1(pTouch)) {
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("RichangMethods");
                    this->setTouchSwallowEnabled(true);
                }else {
                    this->openTouch(0);
                    this->setTouchSwallowEnabled(true);
                }
            }
        }else if (guideIndex == 2){
            if (!DATA->_guideBool2[0]) {
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool2[0] && !DATA->_guideBool2[1]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool2[1] && !DATA->_guideBool2[2]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool2[2] && !DATA->_guideBool2[3]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }
            
            
        }
        
        
    }else{
        this->setTouchSwallowEnabled(true);
    }
    
    return true;
}

void GuideLayer::openTouch(float dt){
    touchBool = true;
}

void GuideLayer::creatView(){
    saidLabel = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 22,CCSizeMake(dkSpr->getContentSize().width* .85f, dkSpr->getContentSize().height* .75f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    saidLabel->setPosition(ccp(dkSpr->getContentSize().width* .5f, dkSpr->getContentSize().height* .41f));
    saidLabel->setColor(ccWHITE);
    saidLabel->setTag(0x88888);
    dkSpr->addChild(saidLabel, 2);
    
    this->schedule(schedule_selector(GuideLayer::logic), .5f);
}
void GuideLayer::creatAnimation(){
    CCArray* arrowAnimations = CCArray::createWithCapacity(8);
    char arrowStr[100] = {};
    for (int i = 1; i <= 8; i++) {
        
        sprintf(arrowStr, "res/pic/guide/quan/guide_quan%d.png", i);
        CCSpriteFrame* arrowFrame = CCSpriteFrame::create(arrowStr, CCRectMake(0, 0, 129, 129));
        arrowAnimations->addObject(arrowFrame);
    }
    CCAnimation* arrowAnimation = CCAnimation::createWithSpriteFrames(arrowAnimations, .2f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation, "quanStr");
}

void GuideLayer::openAnimation(){
    
}
void GuideLayer::closeAnimation(){
    
}

void GuideLayer::getIndex(float dt){
    if (guideIndex == 1) {
        if (!DATA->_guideBool1[0]) {
            DATA->_guideBool1[0] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool1[0] && !DATA->_guideBool1[1]){
            DATA->_guideBool1[1] = true;
            if (guideSpr) {
                guideSpr->removeAllChildren();
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]){
            DATA->_guideBool1[2] = true;
            if (guideSpr) {
                guideSpr->removeAllChildren();
            }
            
            this->creatShou();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
    }else if (guideIndex == 2){
        if (!DATA->_guideBool2[0]) {
            DATA->_guideBool2[0] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool2[0] && !DATA->_guideBool2[1]){
            DATA->_guideBool2[1] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool2[1] && !DATA->_guideBool2[2]){
            DATA->_guideBool2[2] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool2[2] && !DATA->_guideBool2[3]){
            DATA->_guideBool2[3] = true;
            this->creatShou();
            this->creat_testRect();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
    }
    
    
    
    
}
void GuideLayer::creatShou(){
    CCSprite* quanSpr = CCSprite::create("res/pic/guide/quan/guide_quan1.png");
    if (guideIndex == 1) {
        if (DATA->_guideBool1[2] && !DATA->_guideBool1[3]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .15f, DISPLAY->ScreenHeight()* .65f));
        }
    }else if (guideIndex == 2){
        if (DATA->_guideBool2[3] && !DATA->_guideBool2[4]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth() - quanSpr->getContentSize().width* .7f, DISPLAY->ScreenHeight()* .79f));
        }
    }
    
    quanSpr->setTag(0x77777);
    this->addChild(quanSpr);
    CCAnimation* quanAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("quanStr");
    CCAnimate* quanAnimate = CCAnimate::create(quanAnimation);
    quanSpr->runAction(CCRepeatForever::create(quanAnimate));
    
    CCSprite* arrowSpr = CCSprite::create("res/pic/guide/guide_jiantou1.png");
    arrowSpr->setAnchorPoint(ccp(0, 1));
    arrowSpr->setPosition(ccp(quanSpr->getContentSize().width* .3f, quanSpr->getContentSize().height* .62f));
    quanSpr->addChild(arrowSpr);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(.3f, ccp(quanSpr->getContentSize().width* .45f, quanSpr->getContentSize().height* .48f));
    CCMoveTo* moveTo2 = CCMoveTo::create(.3f, ccp(quanSpr->getContentSize().width* .3f, quanSpr->getContentSize().height* .62f));
    arrowSpr->runAction(CCRepeatForever::create(CCSequence::create(moveTo1, moveTo2, NULL)));
}

void GuideLayer::creatLabel(float dt){
    CCString* str;
    wordCount = 0;
    if (guideIndex == 1) {
        if (!DATA->_guideBool1[0]) {
            this->creatDiKuang(1);
            str = CCString::createWithFormat("欢迎(%s)来到女总的世界.\n我是剧里的后期编辑小妮.", DATA->getShow()->nickname());
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool1[0] && !DATA->_guideBool1[1]){
            str = CCString::createWithFormat("剧里各种飘字就是我干的.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]){
            this->creatDiKuang(4);
            str = CCString::createWithFormat("开始游戏的第一步,让我们建立一家公司吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }
        
        
    }else if (guideIndex == 2){
        if (!DATA->_guideBool2[0]) {
            this->creatDiKuang(6);
            str = CCString::createWithFormat("右侧的列表,就是云总的日常事务了.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool2[0] && !DATA->_guideBool2[1]){
            str = CCString::createWithFormat("每个任务最高可获得5颗星星.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool2[1] && !DATA->_guideBool2[2]){
            str = CCString::createWithFormat("第一个公司阶段累计达到88颗星星,诗彤的公司就可以升级.\n可以去更广阔的空间发展.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool2[2] && !DATA->_guideBool2[3]){
            str = CCString::createWithFormat("那么先让我们去注册第一间公司吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }
        
        
    }
}
void GuideLayer::logic(float dt){
    this->unschedule(schedule_selector(GuideLayer::logic));
    
    if (wordCount > contentLength) {
        wordCount += 2;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        if (guideIndex == 1) {
            if (!DATA->_guideBool1[0]) {
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool1[0] && !DATA->_guideBool1[1]) {
                this->flyLabel();
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]) {
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }
        }else if (guideIndex == 2){
            if (!DATA->_guideBool2[0]) {
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool2[0] && !DATA->_guideBool2[1]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool2[1] && !DATA->_guideBool2[2]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool2[2] && !DATA->_guideBool2[3]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }
            
            
        }
        
    }else{
        wordCount += 2;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        this->schedule(schedule_selector(GuideLayer::logic), .001f);
    }
}
void GuideLayer::creatDiKuang(int index){
    
    CCString* manStr = CCString::createWithFormat("res/pic/guide/mishu/guide_mishu%d.png", index);
    manSpr = CCSprite::create(manStr->getCString());
    manSpr->setScale(.5f);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .15f, DISPLAY->ScreenHeight()* .15f));
    guideSpr->addChild(manSpr, 10);
    
    CCSprite* xingSpr1 = CCSprite::create("res/pic/guide/guide_xing2.png");
    xingSpr1->setAnchorPoint(CCPointZero);
    xingSpr1->setRotation(25);
    xingSpr1->setPosition(ccp(manSpr->getPosition().x + 121, manSpr->getPosition().y + 130));
    xingSpr1->setVisible(false);
    guideSpr->addChild(xingSpr1, 8);
    CCSprite* xingSpr2 = CCSprite::create("res/pic/guide/guide_xing1.png");
    xingSpr2->setAnchorPoint(CCPointZero);
    xingSpr2->setRotation(-13);
    xingSpr2->setPosition(ccp(manSpr->getPosition().x + 138, manSpr->getPosition().y + 138));
    xingSpr2->setVisible(false);
    guideSpr->addChild(xingSpr2, 6);
    
    dkSpr = CCSprite::create("res/pic/guide/guide_kuang2.png");
    dkSpr->setAnchorPoint(CCPointZero);
    dkSpr->setPosition(ccp(manSpr->getPosition().x + 162, manSpr->getPosition().y + 162));
    dkSpr->setVisible(false);
    guideSpr->addChild(dkSpr, 4);
    
    CCSprite* dkXingSpr1 = CCSprite::create("res/pic/guide/guide_xing3.png");
    dkXingSpr1->setPosition(ccp(dkSpr->getContentSize().width* .25f, dkSpr->getContentSize().height* .89f));
    dkXingSpr1->setVisible(false);
    dkSpr->addChild(dkXingSpr1);
    
    CCSprite* dkXingSpr2 = CCSprite::create("res/pic/guide/guide_xing3.png");
    dkXingSpr2->setFlipX(true);
    dkXingSpr2->setPosition(ccp(dkSpr->getContentSize().width* .75f, dkSpr->getContentSize().height* .89f));
    dkXingSpr2->setVisible(false);
    dkSpr->addChild(dkXingSpr2);
    
    CCLabelTTF* dkLabel = CCLabelTTF::create("新手引导", DISPLAY->fangzhengFont(), 25);
    dkLabel->setPosition(ccp(dkSpr->getContentSize().width* .5f, dkSpr->getContentSize().height* .89f));
    dkLabel->setColor(ccc3(255, 222, 222));
    dkLabel->setVisible(false);
    dkSpr->addChild(dkLabel);
    
    
    xingSpr1->runAction(CCShow::create());
    xingSpr2->runAction(CCSequence::create(CCDelayTime::create(.2f), CCShow::create(), NULL));
    dkSpr->runAction(CCSequence::create(CCDelayTime::create(.4f), CCShow::create(), NULL));
    dkXingSpr1->runAction(CCSequence::create(CCDelayTime::create(.6f), CCShow::create(), NULL));
    dkXingSpr2->runAction(CCSequence::create(CCDelayTime::create(.6f), CCShow::create(), NULL));
    dkLabel->runAction(CCSequence::create(CCDelayTime::create(.6f), CCShow::create(), NULL));
}
void GuideLayer::creat_ContentLength(std::string str){
    content = str;
    contentLength = 0;
    
    long length = content.length();
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
    
//    CCLog("content=%s, length=%d", content.c_str(), contentLength);
}
std::string GuideLayer::getContentByLength(int length){
    if (length >= contentLength) {
        return getContent();
    }
    
    int i = 0;
    int index = 0;
    while (index < length) {
        char ch = getContent()[i];
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        index++;
    }
    std::string str = content.substr(0, i);
//    CCLog("index ======= %d", index);
    return str;
}
int GuideLayer::getContentLength(){
    return contentLength;
}



void GuideLayer::flyLabel(){
    CCSprite* spr1 = CCSprite::create("res/pic/guide/guide_label1.png");
    spr1->setPosition(ccp(-500, DISPLAY->ScreenHeight()* .92f));
    this->addChild(spr1);
    CCSprite* spr2 = CCSprite::create("res/pic/guide/guide_label2.png");
    spr2->setPosition(ccp(-500, DISPLAY->ScreenHeight()* .82f));
    this->addChild(spr2);
    CCSprite* spr3 = CCSprite::create("res/pic/guide/guide_label3.png");
    spr3->setPosition(ccp(-500, DISPLAY->ScreenHeight()* .72f));
    this->addChild(spr3);
    CCSprite* spr4 = CCSprite::create("res/pic/guide/guide_label4.png");
    spr4->setPosition(ccp(-500, DISPLAY->ScreenHeight()* .62f));
    this->addChild(spr4);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(12.f, ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .92f));
    CCMoveTo* moveTo2 = CCMoveTo::create(8.f, ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .82f));
    CCMoveTo* moveTo3 = CCMoveTo::create(6.f, ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .72f));
    CCMoveTo* moveTo4 = CCMoveTo::create(9.f, ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .62f));
    spr1->runAction(moveTo1);
    spr2->runAction(moveTo2);
    spr3->runAction(moveTo3);
    spr4->runAction(moveTo4);
}





CCRect GuideLayer::isRect1_1(){
    return CCRectMake(0, DISPLAY->ScreenHeight()* .48f, 250, 400);
}
bool GuideLayer::isContainTouchPoint1_1(CCTouch* touch){
    return isRect1_1().containsPoint(convertTouchToNodeSpace(touch));
}










#pragma mark - ------------------ Draw ------------------

void GuideLayer::beforeDraw()
{
    
}

void GuideLayer::creat_testRect(){
    _testRect = CCRectMake(DISPLAY->ScreenWidth() - 180.f, DISPLAY->ScreenHeight()* .77f, 165, 85);
}

void GuideLayer::afterDraw()
{
    CHECK_GL_ERROR_DEBUG();
    ccDrawColor4B(200, 10, 200, 200);
    glLineWidth(4.f);
    
    ccDrawRect(_testRect.origin, _testRect.origin + ccp(_testRect.size.width, _testRect.size.height));
}

void GuideLayer::visit()
{
    m_drawOrder = ++g_drawOrder;
    
    // quick return if not visible
    if (!isVisible())
    {
        return;
    }
    
    kmGLPushMatrix();
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }
    
    this->transform();
    
//#if (DEBUG_GUIDE)
    this->beforeDraw();
//#endif
    
    if(m_pChildren)
    {
        ccArray *arrayData = m_pChildren->data;
        unsigned int i=0;
        
        // draw children zOrder < 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode *child =  (CCNode*)arrayData->arr[i];
            if ( child->getZOrder() < 0 )
            {
                child->visit();
            }
            else
            {
                break;
            }
        }
        
        // this draw
        this->draw();
        
        // draw children zOrder >= 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode* child = (CCNode*)arrayData->arr[i];
            child->visit();
        }
        
    }
    else
    {
        this->draw();
    }
    
//#if (DEBUG_GUIDE)
    this->afterDraw();
//#endif
    
    if ( m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }
    
    kmGLPopMatrix();
}




