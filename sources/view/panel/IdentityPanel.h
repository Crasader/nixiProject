//
//  IdentityPanel.hpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#ifndef IdentityPanel_hpp
#define IdentityPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class IdentityPanel : public CCLayer
{
public:
    ~IdentityPanel();
    static IdentityPanel* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_from(CCPoint from);
    
private:
    void create_panel();
    void do_enter();
    void do_exit();
    void remove();
    
    void on_music(CCMenuItem* btn);
    void on_effect(CCMenuItem* btn);
    void on_take_gift(CCMenuItem* btn);
    
private:
    int num_child;
    
    CCLayer*            _content;
    CCSprite*           _panel;
    CCPoint             _enter_pos;
    
    CCEditBox*          _inputName;
    CCEditBox*          _inputAge;
    CCEditBox*          _inputPhone;
};

#endif /* IdentityPanel_hpp */
