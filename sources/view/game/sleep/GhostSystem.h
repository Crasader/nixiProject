//
//  GhostSystem.h
//  mm3c
//
//  Created by lakkey on 15-12-19.
//
//

#ifndef __mm3c__GhostSystem__
#define __mm3c__GhostSystem__

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
    e_ghosttype_agile = 1,
    e_ghosttype_normal,
    e_ghosttype_flickering,
    e_ghosttype_meatshield,
} GHOSTTYPE;

class Ghost : public CCObject
{
public:
    ~Ghost() {}
    virtual bool init() {return true;}
    CREATE_FUNC(Ghost);
    
    GHOSTTYPE       type;
    float           speed;
    float           scale;
    int             health;
    int             orderZ;
    int             score;
};



class GhostSystem : public CCLayer
{
public:
    ~GhostSystem();
    virtual bool init();
    CREATE_FUNC(GhostSystem);
    
    void create_ghost();
    
    void on_hit(CCPoint pos);
    
//    int             agile_total;
//    int             agile_hit;
//    int             normal_total;
//    int             normal_hit;
//    int             flickering_total;
//    int             flickering_hit;
//    int             meatshield_total;
//    int             meatshield_hit;
    CC_SYNTHESIZE(int, _total_score, Total_score);
    CC_SYNTHESIZE(int, _lostCount, LostCount);
    
protected:
    GHOSTTYPE rand_ghosttype();
    Ghost*  _create_ghost(GHOSTTYPE type);
    CCSprite* create_ghost_sprite(Ghost* ghost);
    CCSequence* random_ghost_route();
    
    bool hit_ghost(int idx, Ghost* ghost, CCSprite* spt);
    void purge_after_dyinganimation(CCNode* node);
    void on_ghost_escape(CCNode* node);

    CCArray*        arr_ghost;
    CCArray*        arr_sprite;
};

#endif /* defined(__mm3c__GhostSystem__) */
