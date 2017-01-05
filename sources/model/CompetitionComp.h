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
    CC_SYNTHESIZE(int, _day, Day);
    CC_SYNTHESIZE(int, _hour, Hour);
    CC_SYNTHESIZE(int, _minu, Minu);
    CC_SYNTHESIZE(int, _secd, Secd);
    
    CC_SYNTHESIZE_RETAIN(CCDictionary*, _buffs, Buffs); // 祝福Buffs，待定
    
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
    CC_SYNTHESIZE_RETAIN(CCDictionary*, _ornaments, Ornaments); // 同show的首饰
    
    CC_SYNTHESIZE(int, _dailyTimes, DailyTimes);    // 今日主动挑战次数
    CC_SYNTHESIZE_RETAIN(CCArray*, _buffs, Buffs);  // 使用中的祝福能力 Ids
    CC_SYNTHESIZE_RETAIN(CCArray*, _dailyOpponents, DailyOpponents); // 今日挑战过的对手
    
    CC_SYNTHESIZE(int, _match, Match);              // 搭配分
    CC_SYNTHESIZE(int, _puplar, Puplar);            // 人气分
    CC_SYNTHESIZE(int, _charm, Charm);              // 魅力分
    CC_SYNTHESIZE(int, _luck, Luck);                // 运气分
    
public:
    ~CompetitionItem() {}
    CREATE_FUNC(CompetitionItem);
    virtual bool init() { return true; }
    void config(Value json);
};


class CompetitionComp : public CCObject
{
public:
    CC_SYNTHESIZE_RETAIN(CCArray*, _ranklist, Ranklist);            // 排行
    CC_SYNTHESIZE_RETAIN(CompetitionThemeInfo*, _theme, Theme);     // 主题信息
    CC_SYNTHESIZE_RETAIN(CompetitionItem*, _self, Self);            // 自己
    CC_SYNTHESIZE_RETAIN(CompetitionItem*, _opponent, Opponent);   // 对手信息
    
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
