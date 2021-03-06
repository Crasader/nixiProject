//
//  PurchasePanel.hpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#ifndef PurchasePanel_hpp
#define PurchasePanel_hpp

#include "cocos2d.h"

USING_NS_CC;

class PurchasePanel : public CCLayer
{
public:
    ~PurchasePanel();
    CREATE_FUNC(PurchasePanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_from(CCPoint from);
    
    void updatePay(float dt);
    void sendPay(float dt);
    void send105();
    
private:
    void init_content();
    void update_content();
    void do_enter();
    void do_exit();
    void remove();
    void on_bar_clicked(CCMenuItem* item);
    
    void nc_verify_android_105(CCObject* pObj);
    void nc_verify_iOS_107(CCObject* pObj);
    
private:
    int num_child;
    
    CCLayer*            _content;
    CCSprite*           _panel;
    CCPoint             _enter_pos;
    CCMenu*             _menu;
};

#endif /* PurchasePanel_hpp */
