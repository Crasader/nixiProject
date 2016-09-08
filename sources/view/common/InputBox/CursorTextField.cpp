//
//  CursorTextField.cpp
//  mm3c
//
//  Created by lakkey on 15-8-29.
//
//

#include "CursorTextField.h"
#include "DisplayManager.h"


CursorTextField::~CursorTextField() {
    
}

CursorTextField* CursorTextField::cursorTextFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize)
{
    CursorTextField *pRet = new CursorTextField();
    if(pRet && pRet->initWithPlaceHolder(placeholder, dimensions, alignment, fontName, fontSize))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CursorTextField::initWithPlaceHolder(const char* placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char* fontName, float fontSize) {
    if (!CCLayer::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->ignoreAnchorPointForPosition(true);
    this->setAnchorPoint(ccp(.5f, .5f));
    
    _tf = CCTextFieldTTF::textFieldWithPlaceHolder(placeholder, dimensions, alignment, fontName, fontSize);
    _tf->setAnchorPoint(ccp(.5f, .5f));
    _tf->setPosition(CCPointZero);
    _tf->setDelegate(this);
    this->addChild(_tf);
    
    _delegate = NULL;
    _isDebug = false;
    _oldText = "";
    _attached = false;
    openBool = false;
    
    return true;
}

CursorTextField* CursorTextField::cursorTextFieldWithPlaceHolder(const char* placeholder, const char* fontName, float fontSize) {
    CursorTextField *pRet = new CursorTextField();
    
    if(pRet && pRet->initWithPlaceHolder(placeholder, fontName, fontSize))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CursorTextField::initWithPlaceHolder(const char* placeholder, const char* fontName, float fontSize) {
    if (!CCLayer::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->ignoreAnchorPointForPosition(true);
    this->setAnchorPoint(ccp(.5f, .5f));
    
    _tf = CCTextFieldTTF::textFieldWithPlaceHolder(placeholder, fontName, fontSize);
    _tf->setAnchorPoint(ccp(.5f, .5f));
    _tf->setPosition(CCPointZero);
    _tf->setDelegate(this);
    this->addChild(_tf);
    
    _delegate = NULL;
    _isDebug = false;
    _cursorShow = false;
    _oldText = "";
    _attached = false;
    _nCurTextBytes = 0;
    _nMaxTextBytes = 30;
    
    return true;
}

void CursorTextField::onEnter() {
    CCLayer::onEnter();
    
    this->setDimensions(_tf->getDimensions());
}

void CursorTextField::onExit() {
    this->unscheduleAllSelectors();
    this->stopAllActions();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCLayer::onExit();
}

bool CursorTextField::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    m_beginPos = pTouch->getLocationInView();
    m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);
    
    return true;
}
CCRect CursorTextField::getRect(){
    return CCRectMake(DISPLAY->ScreenWidth()* .5f - 190, DISPLAY->ScreenHeight()* .5f - 175, 375, 360);
}

bool CursorTextField::isInTextField(cocos2d::CCTouch *pTouch)
{
    return getRect().containsPoint(convertTouchToNodeSpace(pTouch));
}
void CursorTextField::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (!openBool) {
        CCRect rect = CCRectMake(DISPLAY->ScreenWidth()* .5f - 190, DISPLAY->ScreenHeight()* .5f - 175, 375, 360);
        CCPoint point = pTouch->getLocationInView();
        point = CCDirector::sharedDirector()->convertToGL(point);
//        if (rect.containsPoint(point)) {
//            CCLog("111112222233333");
//        }else{
//            CCLog("78912356456");
//        }
        if (!rect.containsPoint(point)) {
            m_beginPos.x = m_beginPos.y = -1;
            return;
        }
        
//        CCLog("width: %f, height: %f.", getContentSize().width, getContentSize().height);
        
        // 判断是打开输入法还是关闭输入法
        isInTextField(pTouch) ? closeIME() : openIME();
    }else{
        CCRect rect = CCRectMake(0, DISPLAY->ScreenHeight()* .3f, DISPLAY->ScreenWidth(), 800);
        CCPoint point = pTouch->getLocationInView();
        point = CCDirector::sharedDirector()->convertToGL(point);
        if (rect.containsPoint(point)) {
            closeIME();
        }
    }
}

void CursorTextField::setDimensions(const cocos2d::CCSize &dim) {
    _tf->setDimensions(dim);
    CCSize size = CCSizeMake(dim.width + 40, dim.height + 20);
    _touchRect = CCRectMake(-size.width * .5f, -size.height * .5f, size.width, size.height);
}

void CursorTextField::showCursor() {
    CCCallFunc* show = CCCallFunc::create(this, SEL_CallFunc(&CursorTextField::insertCursor));
    CCCallFunc* hide = CCCallFunc::create(this, SEL_CallFunc(&CursorTextField::deleteCursor));
    CCSequence* action = CCSequence::create(show, CCDelayTime::create(.3f), hide, CCDelayTime::create(.3f), NULL);
    action->setTag(1111);
    this->runAction(CCRepeatForever::create(action));
}

void CursorTextField::hideCursor() {
    this->stopAllActions();
    _tf->setString(_oldText.c_str());
}

void CursorTextField::insertCursor() {
    CCString* str = CCString::createWithFormat("%s|", _oldText.c_str());
    _tf->setString(str->getCString());
    CCLog("insertCursor:");
    CCLog("%s", _tf->getString());
}

void CursorTextField::deleteCursor() {
    if (_oldText.empty()) {
        _tf->setString(" ");
    }
    else {
        CCString* str = CCString::createWithFormat("%s", _oldText.c_str());
//        CCLOG("delete:");
//        CCLOG("%s", str->getCString());
        _tf->setString(str->getCString());
    }
//    CCLOG("insertCursor len = %ld", strlen(_tf->getString()));
}

#pragma mark - ------------------ CCTextFieldDelegate ------------------

bool CursorTextField::onTextFieldAttachWithIME(CCTextFieldTTF * sender) {
    CCLOG("CursorTextField::onTextFieldAttachWithIME()");
    _attached = true;
    
    this->showCursor();
    
    if (_delegate != NULL) {
        _delegate->onCursorTextFieldAttachWithIME(this);
    }
    
    return false;
}

bool CursorTextField::onTextFieldDetachWithIME(CCTextFieldTTF * sender) {
    CCLOG("CursorTextField::onTextFieldDetachWithIME()");
    _attached = false;
    this->hideCursor();
    
    if (_delegate != NULL) {
        _delegate->onCursorTextFieldDetachWithIME(this);
    }
    
    return false;
}

bool CursorTextField::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
    CCLOG("CursorTextField::onTextFieldInsertText(const char * text = %s, int nLen = %d)", text, nLen);
    
    if (_nCurTextBytes >= _nMaxTextBytes) {
        return false;
    }
    
//    CCSize tfSize = _tf->boundingBox().size;
//    CCSize tfDimession = _tf->getDimensions();
////    CCLOG("tfSize.width = %f, tfDimession.width = %f", tfSize.width, tfDimession.width);
////    CCLOG("tfSize.height = %f, tfDimession.height = %f", tfSize.height, tfDimession.height);
////    CCLOG("%d, %d", int(tfSize.width - tfDimession.width), int(tfSize.height - tfDimession.height));
//    if (int(tfSize.width - tfDimession.width) != 2 ||
//        int(tfSize.height - tfDimession.height) != 2) {
//        return true;
//    }
//    
    if (_delegate != NULL) {
        if (_delegate->onCursorTextFieldInsertText(this, text, nLen)) {
            return true;
        }
    }
    
    _oldText.append(text);
    _nCurTextBytes += nLen;
    return false;
}

bool CursorTextField::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen) {
    CCLOG("CursorTextField::onTextFieldDeleteBackward() delText::%s", delText);
    if (_oldText.empty()) {
        return true;
    }
    
    if (strcmp("|", delText) == 0) {
        return true;
    }
    
    _oldText = _oldText.erase(_oldText.length() - nLen);
    _nCurTextBytes -= nLen;
    
    if (_delegate != NULL) {
        _delegate->onCursorTextFieldDeleteBackward(this, delText, nLen);
    }
    
    return false;
}

bool CursorTextField::onDraw(CCTextFieldTTF * sender) {
    return false;
}

void CursorTextField::openIME()
{
    openBool = true;
    
    _tf->attachWithIME();
}

void CursorTextField::closeIME()
{
    openBool = false;
    
    _tf->detachWithIME();
}

void CursorTextField::keyboardWillHide(CCIMEKeyboardNotificationInfo& info){
    openBool = false;
    
    _tf->detachWithIME();
}

#pragma mark - ------------------ Draw ------------------

void CursorTextField::beforeDraw()
{
    
}

void CursorTextField::afterDraw()
{
    CHECK_GL_ERROR_DEBUG();
    ccDrawColor4B(200, 10, 200, 200);
    glLineWidth(4.f);
    
    ccDrawRect(_touchRect.origin, _touchRect.origin + ccp(_touchRect.size.width, _touchRect.size.height));
}

void CursorTextField::visit()
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
    
    if (_isDebug) {
        this->beforeDraw();
    }
    
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
    
    if (_isDebug) {
        this->afterDraw();
    }
    
    if ( m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }
    
    kmGLPopMatrix();
}