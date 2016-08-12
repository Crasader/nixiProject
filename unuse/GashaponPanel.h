//
//  GashaponPanel.hpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#ifndef GashaponPanel_hpp
#define GashaponPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GashaponPanel : public CCLayer
{
    
public: // Export
    static void show();
    
public:
    ~GashaponPanel();
    CREATE_FUNC(GashaponPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void update_content();
    void remove();
    
    void on_purchase();
    void on_purchase_achievement();
    void on_signin7();
    
    void on_take_energy(CCMenuItem* btn);
    
    void nc_take_energy_301(CCObject* pObj);
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
};

#endif /* GashaponPanel_hpp */
