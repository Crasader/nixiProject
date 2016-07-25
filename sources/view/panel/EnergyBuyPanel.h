//
//  EnergyBuyPanel.hpp
//  tiegao
//
//  Created by mac on 16/6/30.
//
//

#ifndef EnergyBuyPanel_hpp
#define EnergyBuyPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class EnergyBuyPanel : public CCLayer
{
public:
    ~EnergyBuyPanel();
    CREATE_FUNC(EnergyBuyPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show();
    void show_from(CCPoint from);
    
private:
    void do_enter();
    void do_exit();
    void remove();
    void buy();
    
    void buy_energy_callback_101(CCObject* pObj);
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
    CCPoint             _enter_pos;
};

#endif /* EnergyBuyPanel_hpp */
