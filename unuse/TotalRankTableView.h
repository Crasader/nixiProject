//
//  TotalRankTableView.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#ifndef TotalRankTableView_hpp
#define TotalRankTableView_hpp

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TotalRankTableView
: public CCLayer
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    ~TotalRankTableView();
    
    virtual bool init();
    
    
    CREATE_FUNC(TotalRankTableView);
    
public:
    
    //CCTableViewDelegate继承自CCScrollViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){}
    
    //点击哪个cell
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    //每个cell的size
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    
    //生成cell
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
    //cell的数量
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    //按下去的时候，就是高亮显示，这里可以设置高亮状态
    virtual void tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
    
    //松开的时候，取消高亮状态
    virtual void tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
    
    void onEnter();
    
    void onExit();
    
public:
    
    CCTableView* pTableView;
    
    CCNode* sprNode;
    
    int OpenToWhichOne;
    int allNumber;
    int selectedIndex;
    
    CCArray* _rankers;
    
public:
    
    void bigSprite(int index, CCSprite* spr);
    void smallSprite(int index, CCSprite* spr);
    
    CCSprite* getNumSprite(int num);
    
    void updateTabelView();
    
    void toBeFriend(CCMenuItem* btn);
    void tobeFriend_callback_803();
    
};


#endif /* TotalRankTableView_hpp */
