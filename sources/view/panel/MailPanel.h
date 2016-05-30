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
    void config_with_module();
    
private:
};


class MailPanel : public CCLayer
{
public:
    ~MailPanel();
    CREATE_FUNC(MailPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
private:
    void do_enter();
    void do_exit();
};

#endif /* MailPanel_hpp */
