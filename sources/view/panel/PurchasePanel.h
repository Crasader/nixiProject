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
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_from(CCPoint from);
    
private:
    void init_content();
    void do_enter();
    void do_exit();
    void remove();
    void on_bar_clicked(CCMenuItem* item);
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
    CCPoint             _enter_pos;
};

#endif /* PurchasePanel_hpp */
