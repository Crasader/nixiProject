//
//  NewsComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
// {"news":{"S":0,"P":0,"O":0,"M":0,"I":0,"E":0,"C":1}}
// C: 金币奖励可用数量
// E: 新体力奖励(未启用)
// I: 新邮件
// M: 新玩家消息
// N: 全服通告(未启用)
// O: 新一天(未启用)
// P: 新纸条
// S: 新签到

#ifndef NewsComp_hpp
#define NewsComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class NewsComp : public CCObject
{
public:
    bool otherday;
    int coin;
    int energy1;
    int energy2;
    int mail;
    int message;
    int paper;
    int purchaseAchievement;
    int signin7;
    int signin30;
    int dailyShareCount; // 每日分享次数
    int trystOn;         // 约会时间
    int tempSignin;      // 节日临时签到
    
    void print_news();

public:
    ~NewsComp();
    CREATE_FUNC(NewsComp);
    bool init();
    void init_with_json(Value json);
    
protected:
    
private:
};

#endif /* NewsComp_hpp */
