//
//  DailySigninRewardPanel.hpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#ifndef DailySigninRewardPanel_hpp
#define DailySigninRewardPanel_hpp

#include "cocos2d.h"

USING_NS_CC;

class DailySigninRewardPanel : public CCLayer
{
public: // Export
    static void show(CCNode* parent);
    
public:
    ~DailySigninRewardPanel();
    CREATE_FUNC(DailySigninRewardPanel);
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void create_best_show();
    void remove();
    void do_enter();
    void do_exit();
    
    void startBoxAnimation();
    void showBoxAnimation();
    void showReward();
    void after_commit_daily_signin_323(CCObject* pObj);
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
    CCNode*             panel;
    CCMenuItem*         boxItem;
    
    CCSprite*           box;
    CCSize              boxlSize;
    CCSprite*           rewardDisplay;
    
};

#endif /* DailySigninRewardPanel_hpp */
