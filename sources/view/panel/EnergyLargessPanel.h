//
//  EnergyLargessPanel.hpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#ifndef EnergyLargessPanel_hpp
#define EnergyLargessPanel_hpp

#include "cocos2d.h"

USING_NS_CC;

class EnergyLargessPanel : public CCLayer
{
public: // Export
    static void show(CCNode* parent);
    
public:
    ~EnergyLargessPanel();
    CREATE_FUNC(EnergyLargessPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    void on_take(CCMenuItem*);
    void nc_take_energy_301(CCObject *pObj);
    
    void update_state();
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
    CCMenu*             _menu;
};

#endif /* EnergyLargessPanel_hpp */
