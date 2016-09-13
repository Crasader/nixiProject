//
//  EmoticonPanel.hpp
//  tiegao
//
//  Created by mac on 16/8/23.
//
//

#ifndef EmoticonPanel_hpp
#define EmoticonPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class EmoticonPanel
: public CCLayer
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public: // Export
    static void show(CCNode* parent);
    
public:
    ~EmoticonPanel();
    CREATE_FUNC(EmoticonPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void do_enter();
    void do_exit();
    void remove();
    
    void config_cell(CCTableViewCell* cell, int idx);
    
private:
    int num_child;
    
    CCSprite*           _panel;
    CCTableView*        _tv;
};

#endif /* EmoticonPanel_hpp */
