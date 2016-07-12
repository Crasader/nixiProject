//
//  BuildingViewView.hpp
//  tiegao
//
//  Created by mac on 16/7/6.
//
//

#ifndef BuildingViewView_hpp
#define BuildingViewView_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class UnreusedTableView;

class BuildingView
    : public CCLayer
    , CCTableViewDataSource
    , CCTableViewDelegate
{
public:
    ~BuildingView();
    static BuildingView* create(int phase);
    bool init(int phase);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void update_produce(float dt);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell) {};
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell) {};
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell) {};
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
private:
    void start();
    void go_back();
    
    void nc_collect_coin(CCObject* pObj);
    void nc_take_income(CCObject* pObj);
    void nc_collect_coin_201(CCObject* pObj);
    void nc_take_income_203(CCObject* pObj);
    
private:
    UnreusedTableView* _tbView;
    int             _phase;
    CCArray*        _floors;
};

#endif /* BuildingViewView_hpp */
