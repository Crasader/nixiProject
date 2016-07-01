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
    void showStarAt(CCNode* parent, CCPoint pos, int zOrder);
    void showPetalAt(CCNode* parent, CCPoint pos, int zOrder);
    
private:
    CCAnimation* animationByName(const char* name);
    void purgeActionNode(CCNode* node);
    
private:
    CCDictionary*           _specials;
    CCSprite*               _petal;
    CCSprite*               _star;
};

#endif /* SpecialManager_hpp */
