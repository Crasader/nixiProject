//
//  MysteryLayer.hpp
//  tiegao
//
//  Created by mac on 16/8/23.
//
//

#ifndef MysteryLayer_hpp
#define MysteryLayer_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MysteryLayer
: public CCLayer
, public CCTableViewDataSource
{
public: // Export
    static CCScene* scene();
    
public:
    ~MysteryLayer();
    CREATE_FUNC(MysteryLayer);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
private:
    void config_cell(CCTableViewCell* cell, int idx);
    void on_start_task(CCMenuItem* pObj);
    
    void do_enter();
    void do_exit();
    void remove();
    
private:
    int num_child;
    
    CCSprite*           _panel;
    CCTableView*        _tv;
};

#endif /* MysteryLayer_hpp */
