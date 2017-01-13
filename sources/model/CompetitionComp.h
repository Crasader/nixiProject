//
//  CompetitionComp.hpp
//  tiegao
//
//  Created by mac on 17/1/5.
//
//

#ifndef CompetitionComp_hpp
#define CompetitionComp_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

class CompetitionThemeInfo : public CCObject
{
public:
    CC_SYNTHESIZE_PASS_BY_REF(string, _themeName, ThemeName);
    CC_SYNTHESIZE_RETAIN(CCArray*, _tags, Tags);        // 搭配标签要求
    CC_SYNTHESIZE_RETAIN(CCArray*, _rule, Rule);        // 得分规则：3个CCString
    
    CC_SYNTHESIZE(int, _searchFreeCount, SearchFreeCount);  // 免费搜索对手次数的最大值
    CC_SYNTHESIZE(int, _startFreeCount, StartFreeCount);    // 免费开始次数的最大值
    
    CC_SYNTHESIZE(int, _day, Day);
    CC_SYNTHESIZE(int, _hour, Hour);
    CC_SYNTHESIZE(int, _minu, Minu);
    CC_SYNTHESIZE(int, _secd, Secd);
    
public:
    ~CompetitionThemeInfo() {}
    CREATE_FUNC(CompetitionThemeInfo);
    bool init() { return true; }
    void config(Value json);
};

class CompetitionItem : public CCObject
{
public:
    CC_SYNTHESIZE_PASS_BY_REF(string, _id, Id);     // 玩家Sid
    CC_SYNTHESIZE_PASS_BY_REF(string, _nickname, Nickname); // 昵称
    CC_SYNTHESIZE(int, _collected, Collected);      // 衣服收集
    CC_SYNTHESIZE(int, _lastRank, LastRank);        // 上期名次
    CC_SYNTHESIZE_RETAIN(CCDictionary*, _ondress, Ondress);     // 同show的衣着
    
    CC_SYNTHESIZE(int, _searchTimes, SearchTimes);  // 今日主动搜索次数(没有上线)(从0开始计算，>=最大值花钻石)
    CC_SYNTHESIZE(int, _startTimes, StartTimes);    // 今日主动挑战次数(从0开始计算，>=最大值花钻石)
    CC_SYNTHESIZE_PASS_BY_REF(string, _curOpponent, CurOpponent);    // 当前对手sid
    CC_SYNTHESIZE_RETAIN(CCArray*, _dailyOpponents, DailyOpponents); // 今日挑战过的对手
    
    CC_SYNTHESIZE(int, _searchCost, SearchCost);    // 查找对手钻石花费 5
    CC_SYNTHESIZE(int, _startCost, StartCost);      // 开始钻石花费 20
    CC_SYNTHESIZE(int, _score, Score);              // 当前累计得分
    
    CC_SYNTHESIZE(int, _match, Match);              // 搭配分
    CC_SYNTHESIZE(int, _puplar, Puplar);            // 人气分
    CC_SYNTHESIZE(int, _charm, Charm);              // 魅力分
    CC_SYNTHESIZE(int, _luck, Luck);                // 运气分
    
    CC_SYNTHESIZE(int, _buffId, BuffId);           // 祝福Buff，玩家自己的, 0表示无
    CC_SYNTHESIZE_PASS_BY_REF(string, _buffDesc, BuffDesc); // 玩家自己的祝福描述, _buffId=0时为空串
    
    CC_SYNTHESIZE(int, _buffedId, BuffedId);       // 本对象表示对手信息时，为对手的祝福效果ID，0表示无
    CC_SYNTHESIZE_PASS_BY_REF(string, _buffedDesc, BuffedDesc); // 对手的祝福描述, _buffedId=0时为空串
    
    CC_SYNTHESIZE(bool, _added, Added);             // 客户端使用临时判断是否添加了好友
    
public:
    ~CompetitionItem() {}
    CREATE_FUNC(CompetitionItem);
    virtual bool init() { return true; }
    void config(Value json);
};


class CompetitionComp : public CCObject
{
public:
    CC_SYNTHESIZE(int, _selfRank, SelfRank);                // 保存自己的排名，第一为0，未上榜-1
    CC_SYNTHESIZE_RETAIN(CCArray*, _ranklist, Ranklist);            // 排行
    CC_SYNTHESIZE_RETAIN(CompetitionThemeInfo*, _theme, Theme);     // 主题信息
    CC_SYNTHESIZE_RETAIN(CompetitionItem*, _self, Self);            // 自己
    CC_SYNTHESIZE_RETAIN(CompetitionItem*, _opponent, Opponent);   // 对手信息
    
    bool hasInitRankInfo();

public:
    ~CompetitionComp();
    CREATE_FUNC(CompetitionComp);
    bool init();
    
    void createRanklist(Value json);
    void createThemeInfo(Value json);
    void createSelfInfo(Value json);
    void createOpponentInfo(Value json);
};

#endif /* CompetitionComp_hpp */
