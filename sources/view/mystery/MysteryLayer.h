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
#include "AHMessageBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MysteryLayer
: public CCLayer
//, public AHMessageBoxDelegate
, public CCTableViewDataSource
{
public: // Export
    static CCScene* scene();
    static void show(CCNode* parent);
    
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
    
//    virtual void message_box_did_selected_button(AHMessageBox* box,
//                AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    CC_SYNTHESIZE_RETAIN(CCString*, _categoryTempSaved, CategoryTempSaved);
    
private:
    void config_cell(CCTableViewCell* cell, int idx);
    void on_start_task(CCMenuItem* pObj);
    void on_take_achv(CCMenuItem* pObj);
    
    CCSprite* createRewardIcon(CCString* type, int num);
    
    void do_enter();
    void do_exit();
    void remove();
    
    void after_start_mystery_611(CCObject* pObj);
    void after_take_mystery_achv_615(CCObject* pObj);
    
private:
    int num_child;
    
    CCSprite*           _panel;
    CCTableView*        _tv;
};

#endif /* MysteryLayer_hpp */
