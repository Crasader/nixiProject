//
//  MailPanel.hpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#ifndef MailPanel_hpp
#define MailPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

class MailPanel
: public CCLayer
, public CCTableViewDataSource
{
public:
    ~MailPanel();
    CREATE_FUNC(MailPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_from(CCPoint from);
    
private:
    void do_enter();
    void do_exit();
    void remove();
    
    void config_cell(CCTableViewCell* cell, int idx);
    float calcCellHeight(int idx);
    
    void on_mail_delete(CCMenuItem* btn);
    void on_mail_take(CCMenuItem* btn);
    
    void hanle_mail_oper(CCObject* pObj);
    void display_reward_take(CSJson::UInt64 mail_id);
    void take_reward_done();
    
    void whether_mailbox_empty();
    
private:
    int num_child;
    
    CCLayer*            _content;
    CCSprite*           _bg;
    CCTableView*        _tv;
    CCPoint             _enter_pos;
};

#endif /* MailPanel_hpp */
