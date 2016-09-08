//
//  CursorTextField.h
//  mm3c
//
//  Created by lakkey on 15-8-29.
//
//

#ifndef __mm3c__CursorTextField__
#define __mm3c__CursorTextField__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class CursorTextField;

class CursorTextFieldDelegate
{
public:
    /**
     @brief    If the sender doesn't want to insert the text, return true;
     */
    virtual bool onCursorTextFieldInsertText(CursorTextField* sender, const char* newText, int newTextByte)=0;
    virtual bool onCursorTextFieldDeleteBackward(CursorTextField * sender, const char * delText, int nLen)=0;
    virtual void onCursorTextFieldAttachWithIME(CursorTextField * sender) = 0;
    virtual void onCursorTextFieldDetachWithIME(CursorTextField * sender) = 0;
};


class CursorTextField : public CCLayer, public CCTextFieldDelegate
{
public:
    ~CursorTextField();
    
    static CursorTextField * cursorTextFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    
    bool initWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    
    static CursorTextField * cursorTextFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
    bool initWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
    virtual void onEnter();
    virtual void onExit();
    virtual void visit();
    
    /**
     @brief    If the sender doesn't want to attach to the IME, return true;
     */
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
    
    /**
     @brief    If the sender doesn't want to detach from the IME, return true;
     */
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
    
    /**
     @brief    If the sender doesn't want to insert the text, return true;
     */
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    
    /**
     @brief    If the sender doesn't want to delete the delText, return true;
     */
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
    /**
     @brief    If the sender doesn't want to draw, return true.
     */
    virtual bool onDraw(CCTextFieldTTF * sender);
    
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {};
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {};
    
    CC_SYNTHESIZE(CursorTextFieldDelegate*, _delegate, Delegate);
    
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    
public:
    bool openBool;
    // 点击开始位置
    CCPoint m_beginPos;
    void openIME();
    void closeIME();
    // 判断是否点击在TextField处
    bool isInTextField(CCTouch *pTouch);
    // 得到TextField矩形
    CCRect getRect();
    
    void setDebugMode(bool debug) { _isDebug = debug; }
    void setDimensions(const cocos2d::CCSize &dim);
    void setText(const char* text) { _oldText = std::string(text); } // 小心使用
    std::string& getText(void) { return _oldText; }
    void setMaxTextBytes(int bytes) { _nMaxTextBytes = bytes; }
    
    // >>>
    void setTextColor(const ccColor3B& color) { _tf->setColor(color); }
    
    const ccColor3B& getColorSpaceHolder() { return _tf->getColorSpaceHolder(); }
    void setColorSpaceHolder(const ccColor3B& color) { _tf->setColorSpaceHolder(color); }
    
    void setPlaceHolder(const char * text) { _tf->setPlaceHolder(text); }
    const char* getPlaceHolder(void) { return _tf->getPlaceHolder(); }

    void setSecureTextEntry(bool value) { _tf->setSecureTextEntry(value); }
    bool isSecureTextEntry() { return _tf->isSecureTextEntry(); }
    // <<<
    
protected:
    void beforeDraw();
    void afterDraw();
    
    void showCursor();
    void hideCursor();
    void insertCursor();
    void deleteCursor();
    
    bool                        _isDebug;
    bool                        _attached;
    bool                        _cursorShow;
    CCRect                      _touchRect;
    std::string                 _oldText;
    int                         _nMaxTextBytes;
    int                         _nCurTextBytes;
    
private:
    CCTextFieldTTF*             _tf;
};

#endif /* defined(__mm3c__CursorTextField__) */
