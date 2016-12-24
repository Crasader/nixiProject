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

class WelfareStatisItem : public CCObject
{
public:
    ~WelfareStatisItem() {}
    CREATE_FUNC(WelfareStatisItem);
    bool init() { return true; }
    void config(Value json);
    string id;
    int goal;
    int status;
};

class WelfareItem : public CCObject
{
public:
    ~WelfareItem() {}
    CREATE_FUNC(WelfareItem);
    virtual bool init() { return true; }
    void config(Value json);
    string id;
    string name;
    int status;
    int progress;
    int goal;
    string rewardType;
    int rewardNum;
};


class WelfareComp : public CCObject
{
public:
    ~WelfareComp();
    CREATE_FUNC(WelfareComp);
    bool init();
    
    void update_statis(Value json);
    void update_items(Value json);
    
    int obtainTotalProgress();
    WelfareStatisItem* fetchStatisItem(int idx);
    
    int itemCount();
    WelfareItem* fetchWelfareItem(int idx);

private:
    int                     _totalProgress;
    CCDictionary*           _statis;
    CCArray*                _items;
};

#endif /* WelfareComp_hpp */
