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
    static BuildingLayer* create(int phase, bool isPhaseUp);
    bool init(int phase, bool isPhaseUp);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    
private:
    void show_arrow();
    void building_shaking();
    void building_touch_callback();
    
    void show_building();
    void show_phase_up();
    
    void nc_building_disappear(CCObject* pObj);
    void nc_coffers_info_200(CCObject* pObj);
    
private:
    int                 _phase;
    bool                _isPhaseUp;
    CCSprite*           _building;
};

#endif /* BuildingLayer_hpp */
