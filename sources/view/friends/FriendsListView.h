//
//  FriendsListView.hpp
//  tiegao
//
//  Created by mac on 16/7/17.
//
//

#ifndef FriendsListView_hpp
#define FriendsListView_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class FriendsListView
    : public CCLayer
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
public:
    CREATE_FUNC(FriendsListView);
    
    
public:
    ~FriendsListView();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);
    
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void config_cell(CCTableViewCell* cell, int idx);
    void seleted_cell(int idx);
    void unseleted_cells();
    
    void add_ranking_num(CCSprite* plate, int ranking, bool selected);
    void add_name(CCSprite* plate, const char* nickname, bool selected);
    CCSprite* num_sprite(int num);
    
private:
    CCTableView*        _tv;
    CCArray*            _data;
    int                 _seletedIndex;
};


#endif /* FriendsListView_hpp */
