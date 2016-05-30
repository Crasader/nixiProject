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

USING_NS_CC;
USING_NS_CC_EXT;

class MailItem;

class MailCell : public CCSprite
{
public:
    ~MailCell();
    CREATE_FUNC(MailCell);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

public:
    void config_with_module(MailItem* item);
    
private:
};


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
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
public:
    void show();
    
private:
    void do_enter();
    void do_exit();
    void config_cell(CCTableViewCell* cell, int idx);
    
    void on_mail_delete(CCMenuItem* btn);
    void on_mail_take(CCMenuItem* btn);
    
private:
    CCTableView*        _tv;
};

#endif /* MailPanel_hpp */
