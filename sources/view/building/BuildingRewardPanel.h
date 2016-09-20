//
//  BuildingRewardPanel.hpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#ifndef BuildingRewardPanel_hpp
#define BuildingRewardPanel_hpp

#include "cocos2d.h"

USING_NS_CC;

class BuildingRewardPanel : public CCLayer
{
public: // Export
    static void show(CCNode* parent, string type, int value);
    
public:
    ~BuildingRewardPanel();
    static BuildingRewardPanel* createWithReward(string type, int value);
    bool initWithReward(string type, int value);
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    void do_enter();
    void do_exit();
    
private:
    int num_child;
    
    CCSprite*           _panel;
    string              _type;
    int                 _value;
};

#endif /* BuildingRewardPanel_hpp */
