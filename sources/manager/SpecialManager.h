//
//  SpecialManager.hpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#ifndef SpecialManager_hpp
#define SpecialManager_hpp

#include "cocos2d.h"

USING_NS_CC;

#define SPECIAL    SpecialManager::Inst()

class SpecialManager : public CCObject
{
public:
    ~SpecialManager();
    static SpecialManager* Inst();
    void init();
    
public:
    void stopExistAnimation();
//    void showStarAt(CCNode* parent, CCPoint pos, int zOrder);
    void showStar2At(CCNode* parent, CCPoint pos, int zOrder);
//    void showPetalAt(CCNode* parent, CCPoint pos, int zOrder);
    void showPetal2At(CCNode* parent, CCPoint pos, int zOrder);
    void showSpotAt(CCNode* parent, CCPoint pos, int zOrder);
    void showFlowerAt(CCNode* parent, CCPoint pos, int zOrder);
    
    void show_coin_reward(CCNode *parent, int num, CCPoint start, CCPoint end, float duration=1.2f);
    void show_gold_reward(CCNode *parent, int num, CCPoint start, CCPoint end, float duration=1.2f);
    void show_energy_reward(CCNode *parent, int num, CCPoint start, CCPoint end, float duration=1.2f);
    
private:
    CCAnimation* animationByName(const char* name);
    void purgeActionNode(CCNode* node);
    
    void showStarParticleEffect(int color,CCPoint position,CCNode* node);
    ccColor4F getColor4F(int color);
    
    void reward_display(CCNode* icon, CCNode* num, CCPoint end, CCCallFuncN* complete, float duration);
    CCActionInterval* fly_action(CCPoint start, CCPoint end, float duration);
    void show_flower(CCNode* node);
    void coin_animation_completed(CCNode* node);
    void gold_animation_completed(CCNode* node);
    void energy_animation_completed(CCNode* node);
    
private:
    CCDictionary*           _specials;
};

#endif /* SpecialManager_hpp */
