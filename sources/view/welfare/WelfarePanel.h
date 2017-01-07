//
//  WelfarePanel.hpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#ifndef WelfarePanel_hpp
#define WelfarePanel_hpp

#include "cocos2d.h"

USING_NS_CC;

class DailyWelfareLayer;
class AchievementLayer;

class WelfarePanel : public CCLayer
{
    
public: // Export
    static void show();
    
public:
    ~WelfarePanel();
    CREATE_FUNC(WelfarePanel);
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
    
    void onDailyToggle(CCMenuItemToggle* btn);
    void onAchieveToggle(CCMenuItemToggle* btn);
    
    void updateHongDian();

private:
    CCSprite*                   _panel;
    int                         num_child;
    DailyWelfareLayer*          _dailywelfare;
    AchievementLayer*           _achievement;
    
    CCMenuItemToggle*           _btnDaily;
    CCMenuItemToggle*           _btnAchieve;
};

#endif /* WelfarePanel_hpp */
