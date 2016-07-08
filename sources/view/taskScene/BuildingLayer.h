//
//  BuildingLayer.hpp
//  tiegao
//
//  Created by mac on 16/7/5.
//
//

#ifndef BuildingLayer_hpp
#define BuildingLayer_hpp

#include "cocos2d.h"

USING_NS_CC;

class BuildingLayer : public CCLayer
{
public:
    ~BuildingLayer();
    static BuildingLayer* create(int phase);
    bool init(int phase);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    
private:
    void building_touch_callback();
    void show_building();
    
private:
    int                 _phase;
    CCSprite*           _building;
};

#endif /* BuildingLayer_hpp */
