//
//  SettingPanel.hpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#ifndef SettingPanel_hpp
#define SettingPanel_hpp

#include "cocos2d.h"

USING_NS_CC;

class SettingPanel : public CCLayer
{
public:
    ~SettingPanel();
    CREATE_FUNC(SettingPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_from(CCPoint from);
    
private:
    void do_enter();
    void do_exit();
    void remove();
    
    void on_music(CCMenuItem* btn);
    void on_effect(CCMenuItem* btn);
//    void on_back(CCMenuItem* btn);
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
    CCPoint             _enter_pos;
};

#endif /* SettingPanel_hpp */
