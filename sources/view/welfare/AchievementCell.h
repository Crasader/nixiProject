//
//  AchievementCell.hpp
//  tiegao
//
//  Created by mac on 16/12/24.
//
//

#ifndef AchievementCell_hpp
#define AchievementCell_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class AchievementItem;

class AchievementCell : public CCSprite
{
public:
    void showRewardAction(AchievementItem* item);

public:
    ~AchievementCell();
    static AchievementCell* create(const char *pszFileName);
    bool init(const char *pszFileName);
    void configWithAchievementItem(int idx, AchievementItem* item, float cellWidth, float cellHeight, int status);
    
    void goStar(int idx, int progress, int goal, int status);
    
private:
    void moveFinish(CCObject* pObj);
    
    void on_btn_take(CCMenuItem* btn);
    
    CCProgressTimer*        _progress;
    CCMenu*                 _menuBtn;
    CCSprite*               _sptButton1;
    CCSprite*               _sptButton2;
    CCSprite*               _star;
    
    CCSprite*               _rewardIcon;
};

#endif /* AchievementCell_hpp */
