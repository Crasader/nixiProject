//
//  ChatComp.hpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#ifndef ChatComp_hpp
#define ChatComp_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

class ChatItem : public CCObject
{
public:
    bool init_with_json(Value json);
    void print_self();
    
public:
    ~ChatItem() {}
    CREATE_FUNC(ChatItem);
    virtual bool init() { return true; }
    string name;
    string chat;
    string id;
};

class ChatComp : public CCObject
{
public:
    void addItem(ChatItem* item);
    CCColor3bObject* randColor();
    CC_SYNTHESIZE(int, _interval, Interval);
    CC_SYNTHESIZE(int, _newChatCount, NewChatCount);    // 刷新后新消息的条数
    
public:
    ~ChatComp();
    CREATE_FUNC(ChatComp);
    bool init();
    void initColors();
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _colors, Colors);
    CC_SYNTHESIZE_RETAIN(CCArray*, _items, Items);
    
private:
    
};

#endif /* ChatComp_hpp */
