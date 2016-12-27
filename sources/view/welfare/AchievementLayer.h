//
//  AchievementLayer.hpp
//  tiegao
//
//  Created by mac on 16/12/27.
//
//

#ifndef AchievementLayer_hpp
#define AchievementLayer_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class WelfareComp;
class WelfareCell;
class WelfareStatisItem;

class AchievementLayer
: public CCLayer
, CCTableViewDataSource
, CCTableViewDelegate
{
    
public: // Export
    static void show();
    CCRect getBoundary();
    
public:
    ~AchievementLayer();
    CREATE_FUNC(AchievementLayer);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);
    
private:
    void do_enter();
    void do_exit();
    void remove();
    
    void nc_take_welfare_item_reward_631(CCObject* pObj);
    void nc_take_welfare_statis_reward_633(CCObject* pObj);
    
    void disappear_cell(WelfareCell* cell);
    void disappear_finished();
    
    void showRewardTake(WelfareStatisItem* item);
    
private:
    WelfareComp*        _dataSource;
    
    int                 num_child;
    
    CCPoint             _touchLocation;
    CCLayer*            _content;
    CCSprite*           _panel;
    CCTableView*        _tv;
    
    CCArray*            _boxes;
    CCProgressTimer*    _totalProgress;
};

#endif /* AchievementLayer_hpp */
