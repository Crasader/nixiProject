//
//  ChatTableView.hpp
//  tiegao
//
//  Created by stevenLi on 16/8/1.
//
//

#ifndef ChatTableView_hpp
#define ChatTableView_hpp

#include "cocos-ext.h"
#include "cocos2d.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ChatTableView :public CCLayer, CCTableViewDataSource, CCTableViewDelegate
{
public:
    
    CREATE_FUNC(ChatTableView);
    ~ChatTableView();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
//    virtual CCSize tableCellSizeForIndex(CCTableView* table,unsigned int idx);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);
    
private:
    void config_cell(CCTableViewCell* pCell, int index);
    void insertMessage(CCObject* pObj);

public:
    CCTableView* pTableView;
    
//    int cell_num;
    
//    const char* insert_name;
//    const char* insert_chat;
    
};

#endif /* ChatTableView_hpp */
