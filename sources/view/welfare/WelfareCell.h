//
//  WelfareCell.hpp
//  tiegao
//
//  Created by mac on 16/12/24.
//
//

#ifndef WelfareCell_hpp
#define WelfareCell_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class WelfareItem;

class WelfareCell : public CCSprite
{
public:
    void showRewardAction(WelfareItem* item);

public:
    ~WelfareCell();
    static WelfareCell* create(const char *pszFileName);
    bool init(const char *pszFileName);
//    void onEnter();
    void configWithWelfareItem(int idx, WelfareItem* item, float cellWidth, float cellHeight);
    
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

#endif /* WelfareCell_hpp */
