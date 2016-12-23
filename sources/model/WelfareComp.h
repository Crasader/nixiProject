//
//  WelfareComp.hpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#ifndef WelfareComp_hpp
#define WelfareComp_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

class WelfareItem : public CCObject
{
public:
    bool init_with_json(Value json);
    void print_self();
    
public:
    ~WelfareItem() {}
    CREATE_FUNC(WelfareItem);
    virtual bool init() { return true; }
    string name;
    string chat;
};

class WelfareComp : public CCObject
{
public:
    void addItem(WelfareItem* item);
    CCColor3bObject* randColor();
    CC_SYNTHESIZE(int, _interval, Interval);
    CC_SYNTHESIZE(int, _newChatCount, NewChatCount);    // 刷新后新消息的条数
    
public:
    ~WelfareComp();
    CREATE_FUNC(WelfareComp);
    bool init();
    void initColors();
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _colors, Colors);
    CC_SYNTHESIZE_RETAIN(CCArray*, _items, Items);
    
private:
    
};

#endif /* WelfareComp_hpp */
