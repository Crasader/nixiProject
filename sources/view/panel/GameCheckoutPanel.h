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
    static void show(CCNode* parent, string gameId, int score, CCDictionary* firstInfo);
    
public:
    ~GameCheckoutPanel();
    static GameCheckoutPanel* createWithScore(string gameId, int score, CCDictionary* firstInfo);
    bool initWithScore(string gameId, int score, CCDictionary* firstInfo);
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    void do_enter();
    void do_exit();
    
private:
    CCSprite*           _panel;
};

#endif /* GameCheckoutPanel_hpp */
