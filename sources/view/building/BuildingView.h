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
{
public:
    ~BuildingView();
    static BuildingView* create(int phase);
    bool init(int phase);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    //    virtual void scrollViewDidScroll(CCScrollView* view);
    //    virtual void scrollViewDidZoom(CCScrollView* view);
    
private:
    void start();
    void go_back();
    
private:
    UnreusedTableView* _tbView;
    int             _phase;
    CCArray*        _floors;
};

#endif /* BuildingViewView_hpp */
