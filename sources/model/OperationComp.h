//
//  OperationComp.hpp
//  tiegao
//
//  Created by mac on 16/8/3.
//
//

#ifndef OperationComp_hpp
#define OperationComp_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json_lib.h"
#include "extend_define.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

// ---------------------------------------------------

class OperationComp : public CCObject
{
// Export
    // 累计充值数
    SYNTHESIZE_READONLY(int, _purchasedTotal, PurchasedTotal);
    // 累计充值模版数组
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _purchaseAchievementTemplate, PurchaseAchievementTemplate);
    // 套装数组
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _paSuits, PASuits);
    // 累计充值玩家数据 record[]
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _purchaseAchievementUser, PurchaseAchievementUser);
    
    // 套装数组
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _suits, Suits);
    // 扭蛋模版数组
    SYNTHESIZE_RETAIN_READONLY(CCDictionary*, _gashaponTemplate, GashaponTemplate);
    // 扭蛋玩家数据
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _gashaponUser, GashaponUser);
    // 玩家拥有碎片
    SYNTHESIZE_READONLY(int, _piece, Piece);
    // 抽10次奖励
    SYNTHESIZE_READONLY(int, _extra, Extra);
    // 玩家免费抽取倒计时
    SYNTHESIZE_READONLY(unsigned long, _freePoint, FreePoint);
    // 单次花费
    SYNTHESIZE_READONLY(int, _signleCost, SignleCost);
    // 十次花费
    SYNTHESIZE_READONLY(int, _tenCost, TenCost);
    
public:
    // 返回用于需要显示的模版下标
    unsigned cur_purchase_achievement_template_index();
    bool has_init_purchase_achievement_template();
    bool has_init_gashapon_template();
    bool has_taken_pAchievement(string& id);
    bool has_owned_gashapon(string& id);
    
public: // Inport
    void init_purchase_achievement_template(Value json);
    void init_costs(Value json);
    void replace_purchase_achievement(Value json);
    
    void init_gashapon_template(Value json);
    void replace_gashapon_user(Value json);
    void init_extra(Value json);
    
public:
    ~OperationComp();
    CREATE_FUNC(OperationComp);
    virtual bool init();
    
private:
    
};

#endif /* OperationComp_hpp */
