//
//  MMCursorTextField.cpp
//  manman_client
//
//  Created by stevenLi on 14-1-7.
//
//

#include "MMCursorTextField.h"
#include "DisplayManager.h"

//const static float DELTA = 1000;
bool MMCursorTextField::imeBool = false;

MMCursorTextField::MMCursorTextField(){
    CCTextFieldTTF();
    
    m_pCursorAction = NULL;
    m_pCursorSprite = NULL;
    
    m_pInputText = NULL;
    m_pStr = NULL;
    
    openBool = false;
}

MMCursorTextField::~MMCursorTextField(){
    delete m_pInputText;
    delete m_pStr;
}

void MMCursorTextField::onEnter(){
    CCTextFieldTTF::onEnter();
    this->setDelegate(this);
}

MMCursorTextField * MMCursorTextField::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize)
{
    MMCursorTextField *pRet = new MMCursorTextField();
    
    if(pRet && pRet->initWithString("", fontName, fontSize))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        pRet->initCursorSprite(fontSize);
        
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

void MMCursorTextField::initCursorSprite(int nHeight)
{
    // 初始化光标
    int column = 4;
    int pixels[nHeight][column];
    for (int i=0; i < nHeight; ++i) {
        for (int j=0; j < column; ++j) {
            pixels[i][j] = 0xffffffff;
        }
    }
    
    CCTexture2D *texture = new CCTexture2D();
    texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, CCSizeMake(column, nHeight));
    
    m_pCursorSprite = CCSprite::createWithTexture(texture);
    CCSize winSize = getContentSize();
    m_cursorPos = ccp(0, winSize.height / 2 + 2);
    m_pCursorSprite->setPosition(m_cursorPos);
    this->addChild(m_pCursorSprite);
    
    m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.25f), CCFadeIn::create(0.25f), NULL));
    
    m_pCursorSprite->runAction(m_pCursorAction);
    m_pCursorSprite->setVisible(false);
    
    m_pInputText = new std::string();
}

bool MMCursorTextField::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_beginPos = pTouch->getLocationInView();
    m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);
    
    
    return true;
}

CCRect MMCursorTextField::getRect(){
//    CCSize size = getContentSize();
//    MZLog("size === %d,%d",size.width, size.height );
//    return CCRectMake(-size.width / 2, -size.height / 2, size.width, size.height);
    return CCRectMake(DISPLAY->ScreenWidth()* 0.5 - 183, DISPLAY->ScreenHeight()* 0.37, 366, 80);
}

bool MMCursorTextField::isInTextField(cocos2d::CCTouch *pTouch)
{
    //    return CCRect::CCRectContainsPoint(getRect(), convertTouchToNodeSpaceAR(pTouch));
//    return getRect().containsPoint(convertTouchToNodeSpaceAR(pTouch));
    return getRect().containsPoint(convertTouchToNodeSpace(pTouch));
}

void MMCursorTextField::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
//    CCPoint endPos = pTouch->getLocationInView();
//    endPos = CCDirector::sharedDirector()->convertToGL(endPos);
//    
//    // 判断是否为点击事件
//    if (::abs(endPos.x - m_beginPos.x) > DELTA ||
//        ::abs(endPos.y - m_beginPos.y))
//    {
//        // 不是点击事件
//        m_beginPos.x = m_beginPos.y = -1;
//
//        return;
//    }
//    0.5 - 0.035 = 0.465
    
    if (!openBool) {
        CCRect rect = CCRectMake(DISPLAY->ScreenWidth()* 0.5 - 270, DISPLAY->ScreenHeight()* 0.454, 305, 100);
        CCPoint point = pTouch->getLocationInView();
        point = CCDirector::sharedDirector()->convertToGL(point);
//        if (rect.containsPoint(point)) {
//            MZLog("111112222233333");
//        }else{
//            MZLog("78912356456");
//        }
        if (!rect.containsPoint(point)) {
            m_beginPos.x = m_beginPos.y = -1;
            return;
        }
        
        // 判断是打开输入法还是关闭输入法
        isInTextField(pTouch) ? closeIME() : openIME();
    }else{
        CCRect rect1 = CCRectMake(0, DISPLAY->ScreenWidth()* 0.3, DISPLAY->ScreenHeight(), 800);
        CCPoint point1 = pTouch->getLocationInView();
        point1 = CCDirector::sharedDirector()->convertToGL(point1);
        if (rect1.containsPoint(point1)) {
            closeIME();
        }
    }
    
}

bool MMCursorTextField::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    if (m_pInputText->empty()) {
        return false;
    }
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    return false;
}


bool MMCursorTextField::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, int nLen)
{
    if ('\n' == *text)
    {
        return false;
    }
    
    string textStr = text;
    
    char nameChar[128];
    strcpy(nameChar, textStr.c_str());
    
    int zhongwen = 0;
    int yingwen = 0;
    bool zhongBool = false;
    bool yingBool = false;
    
    int i = 0, nlen = strlen(nameChar);
    for (; i<nlen; i++) {
        if (nameChar[i] >= 0 && nameChar[i] <= 127) {
            yingwen++;
            yingBool = true;
        }else{
            zhongwen++;
            zhongBool = true;
        }
    }
    
    for (int i = 0; i < textStr.size(); i++) {
        if (zhongBool && yingBool == false) {
            if (m_pInputText->length() < 15) {
                string str;
                str += textStr[i];
                m_pInputText->append(str);
            }else{
                continue;
            }
        }else if (yingBool && zhongBool == false){
            if (yingwen > 0 && yingwen <= 12) {
                if (m_pInputText->length() < 12) {
                    string str;
                    str += textStr[i];
                    m_pInputText->append(str);
                }else{
                    continue;
                }
            }
        }else if (zhongBool && yingBool){
            if (zhongwen+yingwen < 15) {
                if (m_pInputText->length() < 15) {
                    string str;
                    str += textStr[i];
                    m_pInputText->append(str);
                }else{
                    continue;
                }
            }
        }
    }
    
//    MZLog("m_pInputText->length() === %ld", m_pInputText->length());
//    MZLog("m_pInputText == %s", m_pInputText->c_str());
    
    if (m_pInputText->length() < 16) {
//        m_pInputText->append(text);
        
        setString(m_pInputText->c_str());
        
        m_pCursorSprite->setPositionX(getContentSize().width);
    }

    return true;
}

bool MMCursorTextField::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, int nLen)
{
    m_pInputText->resize(m_pInputText->size() - nLen);
    setString(m_pInputText->c_str());
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    if (m_pInputText->empty()) {
        m_pCursorSprite->setPositionX(0);
    }
    
    return false;
}

bool MMCursorTextField::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    if (m_pInputText->empty()) {
        openBool = false;
        
        CCUserDefault::sharedUserDefault()->setStringForKey("NAME", "");
    }else{
        openBool = false;
        
        CCUserDefault::sharedUserDefault()->setStringForKey("NAME", m_pInputText->c_str());
    }
    CCUserDefault::sharedUserDefault()->flush();
    
    return false;
}

void MMCursorTextField::openIME()
{
    openBool = true;
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("ScreenAction", (CCObject* ) 0);
    m_pCursorSprite->setVisible(true);
    this->attachWithIME();
}

void MMCursorTextField::closeIME()
{
    openBool = false;
    
    imeBool = true;
    m_pCursorSprite->setVisible(false);
    this->detachWithIME();
}

void MMCursorTextField::onExit()
{
    CCTextFieldTTF::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void MMCursorTextField::keyboardWillHide(CCIMEKeyboardNotificationInfo& info){
    
    openBool = false;
    
    imeBool = true;
    m_pCursorSprite->setVisible(false);
    this->detachWithIME();
}

