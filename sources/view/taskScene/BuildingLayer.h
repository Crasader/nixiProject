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
    
    void on_construction_finish(CCNode* node);
    void on_phaseup_finish(CCNode* node);
    
    void nc_building_disappear(CCObject* pObj);
    void nc_coffers_info_200(CCObject* pObj);
    void nc_take_income_203(CCObject* pObj);
    
    void addAction();
    void update(float dt);
    
private:
    int                 _phase;
    bool                _isPhaseUp;
    CCSprite*           _building;
    
    CCSprite*           _cloud_1;
    CCSprite*           _cloud_2;
    
    CCSprite*           _bird_1;
    CCSprite*           _bird_2;
    CCSprite*           _bird_3;
    CCSprite*           _bird_4;
    bool                _isAction;
};

#endif /* BuildingLayer_hpp */
