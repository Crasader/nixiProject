//
//  MysteryLayer.hpp
//  tiegao
//
//  Created by mac on 16/8/23.
//
//

#ifndef MysteryLayer_hpp
#define MysteryLayer_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MysteryLayer : public CCLayer
{
public: // Export
    static CCScene* scene();
    
public:
    ~MysteryLayer();
    CREATE_FUNC(MysteryLayer);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
private:
    void do_enter();
    void do_exit();
    void remove();
    
private:
    int num_child;
    
    CCSprite*           _panel;
};

#endif /* MysteryLayer_hpp */
