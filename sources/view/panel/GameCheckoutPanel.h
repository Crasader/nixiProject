//
//  GameCheckoutPanel.hpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#ifndef GameCheckoutPanel_hpp
#define GameCheckoutPanel_hpp

#include "cocos2d.h"

USING_NS_CC;

class GameCheckoutPanel : public CCLayer
{
public: // Export
    static void show(CCNode* parent, string gameId, int score, int history, CCDictionary* firstInfo);
    
public:
    ~GameCheckoutPanel();
    static GameCheckoutPanel* createWithScore(string gameId, int score, int history, CCDictionary* firstInfo);
    bool initWithScore(string gameId, int score, int history, CCDictionary* firstInfo);
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void _605CallBack(CCObject* pObj);
    
private:
    void remove();
    void do_enter();
    void do_exit();
    
    void on_anim_finish();
    
private:
    int gameScore;
    CCSprite*           _panel;
    bool                _couldRemove;
};

#endif /* GameCheckoutPanel_hpp */
