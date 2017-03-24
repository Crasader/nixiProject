//
//  ChatBar.hpp
//  tiegao
//
//  Created by mac on 16/9/28.
//
//

#ifndef ChatBar_hpp
#define ChatBar_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

#include "FlashNumberLabel.h"

USING_NS_CC;
USING_NS_CC_EXT;


class ChatBar : public CCLayer, FlashNumberLabelDelegate
{
public: // Export
    void update_num(int num);
    void update_display();
    void isTouchOpen(bool isOpen);
    
public:
    ~ChatBar();
    CREATE_FUNC(ChatBar);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    virtual void will_number_scroll();
    virtual void did_number_stoped();
    
private:
    void do_enter();
    void do_exit();
    void remove();
    
    void show_nmu_plate();
    void display_chat();
    void display_chat_content(const char* name, const char* content);
    void display_next_chat();
    
    void onClicked(CCMenuItem* btn);
    
private:
    CCMenu*         _container;
    CCMenuItem*     _bar;
    CCNode*         _content;
    CCSprite*       _plate;
    FlashNumberLabel*  _num;
    
//    int             _curDisplayIndex;
    bool            _isIdle;
};

#endif /* ChatBar_hpp */
