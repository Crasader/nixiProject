//
//  TrystRewardPanel.hpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#ifndef TrystRewardPanel_hpp
#define TrystRewardPanel_hpp

#include "cocos2d.h"

USING_NS_CC;

class TrystRewardPanel : public CCLayer
{
public: // Export
    static void show(CCNode* parent, string type, int num);
    
public:
    ~TrystRewardPanel();
    static TrystRewardPanel* createWithReward(string type, int num);
    bool initWithReward(string type, int num);
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    void do_enter();
    void do_exit();
    
//    CCNode* createAttachedNode_deprecated();
//    void showReward_deprecated();
    void showReward();
    void changeRewardDisplay();
    void chooseRewardDisplay();
    void showLightAndRewardNum();
    
private:
    int                 num_child;
    string              type;
    int                 num;
    int                 lastType;
    bool                couldClose;
    
private:
    CCSprite*           panel;
    CCSize              panelSize;
    CCSprite*           rewardDisplay;

//    CCNode*             moveNode;
//    CCNode*             attachedNode1;
//    CCNode*             attachedNode2;
//    int                 crossCount;
};

#endif /* TrystRewardPanel_hpp */
