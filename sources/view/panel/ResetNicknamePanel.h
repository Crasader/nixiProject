//
//  ResetNicknamePanel.hpp
//  tiegao
//
//  Created by mac on 16/9/5.
//
//

#ifndef ResetNicknamePanel_hpp
#define ResetNicknamePanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ResetNicknamePanel : public CCLayer
{
public: // Export
    static void show(CCNode* parent);
    
public:
    ~ResetNicknamePanel();
    static ResetNicknamePanel* create(const char* cost);
    virtual bool init(const char* cost);
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
//    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
//    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
//    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
//    virtual void editBoxReturn(CCEditBox* editBox);
    
private:
    void do_enter();
    void do_exit();
    void remove();
    void onBtnCommit(CCMenuItem* btn);
    
private:
    int num_child;
    
    CCLayer*            _content;
    CCSprite*           _panel;
    CCEditBox*          _eb;
};

#endif /* ResetNicknamePanel_hpp */
