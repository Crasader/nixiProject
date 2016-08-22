//
//  GameJingli.h
//  mm3c
//
//  Created by lakkey on 15-12-19.
//
//

#ifndef __mm3c__GameJingli__
#define __mm3c__GameJingli__

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
    e_gamestate_idle = 1,
    e_gamestate_onstart,
    e_gamestate_starting,
    e_gamestate_ongoing,
    e_gamestate_onend,
    e_gamestate_ending,
} GAMESTATE;


class GhostSystem;

class GameJingli : public CCLayer
{
public:
    ~GameJingli();
    CREATE_FUNC(GameJingli);
    virtual bool init();

    virtual void onEnter();
    virtual void onExit();
    virtual void update(float dt);
    
    void init_UI();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
protected:    
    void show_game_help();
    
    void starAnimation();
    void funCallback(CCNode* pSender);
    
    void check_end();
    void game_over();
    
    void create_ghost(float dt);
    void update_UI();
    
    GhostSystem*    _ghostlayer;
    GAMESTATE       _gamestate;
    CCLabelTTF*     _lbl_time;
    CCLabelTTF*     _lbl_score;
    CCLabelTTF*     _Worng_word;
    float           _create_ghost_interval;
    
private:
    void nc_commit_game_707(CCObject* pObj);    
};

#endif /* defined(__mm3c__GameJingli__) */
