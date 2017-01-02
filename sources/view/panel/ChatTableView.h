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
#include "AHMessageBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ChatTableView
    : public CCLayer
    , public AHMessageBoxDelegate
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
public:
    
    CREATE_FUNC(ChatTableView);
    ~ChatTableView();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);

    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    virtual CCSize tableCellSizeForIndex(CCTableView* table,unsigned int idx);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);
    
private:
    void config_cell(CCTableViewCell* pCell, int index);
    void insertMessage(CCObject* pObj);
    void addfriend_callback_803(CCObject* pObj);
    
public:
    CCTableView* pTableView;
    
    void updateCellPosition();
    
    const char* insert_name;
    const char* insert_chat;
    
    float cell_height;
    
};

#endif /* ChatTableView_hpp */
