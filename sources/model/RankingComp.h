//
//  RankingComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef RankingComp_hpp
#define RankingComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class RankingComp : public CCObject
{
public:
    ~RankingComp();
    CREATE_FUNC(RankingComp);
    bool init();
    void init_with_json(Value json);
    
    CC_SYNTHESIZE(int, _selfRank, SelfRank);    // 保存自己的排名，第一为0，未上榜-1
    
public:

    CCArray* ranking();
    
protected:
    
private:

    CCArray*                 _ranking;
};

#endif /* RankingComp_hpp */
