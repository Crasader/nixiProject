//
//  OperationPanel.hpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#ifndef OperationPanel_hpp
#define OperationPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class OperationPanel
    : public CCLayer
    , CCTableViewDataSource
    , CCTableViewDelegate
{
    
public: // Export
    static void show();
    
public:
    ~OperationPanel();
    CREATE_FUNC(OperationPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
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
    
    void on_purchase();
    void on_purchase_achievement();
    void on_signin7();
    
    void on_take_energy(CCMenuItem* btn);
    
    void nc_take_energy_301(CCObject* pObj);
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
    CCTableView*        _tv;
};

#endif /* OperationPanel_hpp */
