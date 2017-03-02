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
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SettingPanel : public CCLayer
{
public:
    ~SettingPanel();
    static SettingPanel* create(const char *cost);
    virtual bool init(const char *cost);
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_from(CCPoint from);
    
private:
    void create_nickname_reset_bar(CCSize panelSize, const char *cost);
    void do_enter();
    void do_exit();
    void remove();
    
    void on_music(CCMenuItem* btn);
    void on_effect(CCMenuItem* btn);
    void on_back(CCMenuItem* btn);
    void on_take_gift(CCMenuItem* btn);
    void on_free_diam(CCMenuItem* btn);
    void onBtnCommitRest(CCMenuItem *btn);
    
    void unreloginCallback(CCObject* pSender);
    
private:
    int num_child;
    
    CCLayer*            _content;
    CCSprite*           _panel;
    CCPoint             _enter_pos;
    CCEditBox*          _eb;
};

#endif /* SettingPanel_hpp */
