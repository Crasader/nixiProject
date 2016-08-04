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
/*
class PurchaseAchievementItem : public CCObject
{
public: // Export
    string          id;
    int             goal;
    string          uri;
    
    inline void init_with_json(Value json) {
        if (json.type() == nullValue) {
            CCLOG("PurchaseAchievementItem::init_with_json() json object error.");
            return;
        }
        
        id = json["id"].asString();
        goal = json["goal"].asInt();
        uri = json["uri"].asString();
    }
    
    inline void print_self() {
        CCLOG("    PurchaseAchievementItem:");
        CCLOG("        id = %s", id.c_str());
        CCLOG("        goal = %d", goal);
        CCLOG("        uri = %s", uri.c_str());
    }
    
public:
    ~PurchaseAchievementItem() {}
    CREATE_FUNC(PurchaseAchievementItem);
    virtual bool init() { return true; }
};
*/

// ---------------------------------------------------

class OperationComp : public CCObject
{
// Export
    // 累计充值数
    SYNTHESIZE_READONLY(int, _purchasedTotal, PurchasedTotal);
    // 累计充值模版数组
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _purchaseAchievementTemplate, PurchaseAchievementTemplate);
    // 累计充值玩家数据
    SYNTHESIZE_RETAIN_READONLY(CCDictionary*, _purchaseAchievementUser, PurchaseAchievementUser);
    
    // 玩家拥有碎片
    SYNTHESIZE_READONLY(int, _piece, Piece);
    // 玩家上次免费抽取的时间戳
    SYNTHESIZE_READONLY(unsigned long, _freePoint, FreePoint);
    // 扭蛋模版数组
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _gashaponTemplate, GashaponTemplate);
    // 扭蛋玩家数据
    SYNTHESIZE_RETAIN_READONLY(CCDictionary*, _gashaponUser, GashaponUser);
    
public:
    bool has_init_purchase_achievement_template();
    bool has_init_gashapon_template();
    
public: // Inport
    void init_purchase_achievement_template(Value json);
    void replace_purchase_achievement(Value json);
    
    void init_gashapon_template(Value json);
    void replace_gashapon_user(Value json);
    
public:
    ~OperationComp();
    CREATE_FUNC(OperationComp);
    virtual bool init();

protected:
    
    
private:
    
};

#endif /* OperationComp_hpp */
