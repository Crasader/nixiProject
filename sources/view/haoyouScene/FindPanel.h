//
//  FindPanel.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/21.
//
//

#ifndef FindPanel_hpp
#define FindPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class FindPanel
: public CCLayer
, public CCEditBoxDelegate
{
public:
    ~FindPanel();
    
    CREATE_FUNC(FindPanel);
    bool init();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
        return true;
    }
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
public:
    
    CCSprite* find_panel;
    
    CCMenu* menu;
    CCEditBox* _input_name;
    
public:
    
    void initView();
    void btn_find_callback();
    void btn_back_callback();
    void find_callback_801(CCObject* obj);
    bool check_nickname(std::string str);
};
#endif /* FindPanel_hpp */
