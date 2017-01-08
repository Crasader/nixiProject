//
//  RankListView.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#ifndef RankListView_hpp
#define RankListView_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class RankListCell;

class RankListView
: public CCLayer
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    void reload();
    CC_SYNTHESIZE(CCArray*, _datasource, Datasource);
    
public:
    ~RankListView();
    CREATE_FUNC(RankListView);
    virtual bool init();
    
    void onEnter();
    void onExit();
    
    //CCTableViewDelegate继承自CCScrollViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);
    
    //cell的数量
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    //每个cell的size
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    //生成cell
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
    //点击哪个cell
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    //按下去的时候，就是高亮显示，这里可以设置高亮状态
    virtual void tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    //松开的时候，取消高亮状态
    virtual void tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
private:
    void showEmptyTip();
    void hideEmptyTip();
    
    void showUpdateDays();
    
    CCSprite* createSelfPlate(unsigned int idx, int type);
    RankListCell* createItemCell(unsigned int idx);
    
    void onTitleToggle(CCMenuItemToggle* btn);
    void onSelfBarToggle();
    
    void tobeFriend_callback_803();
    
private:
    bool            _isShowCompetition;
    CCTableView*    _tv;
    int             _selectedIndex;
    CCMenuItemToggle* _toggleSelf;
    CCNode*         _emptyTip;
    CCSprite*       _competitionUpdateTip;
};


#endif /* RankListView_hpp */
