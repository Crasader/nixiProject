//
//  TrystProgress.hpp
//  tiegao
//
//  Created by mac on 16/11/2.
//
//

#ifndef TrystProgress_hpp
#define TrystProgress_hpp

#include "cocos2d.h"

USING_NS_CC;

class TrystProgress : public CCNode
{
public:
    ~TrystProgress();
    static TrystProgress* create(int max, int left);
    CCSize sizeOfContent() const;
    
    void onEnter();
    void onExit();
        
private:
    virtual bool init(int max, int left);
    void updateProgress(float dt);
    
    void showBoxAnimation();
    void stopBoxAnimation();
    void onOpenRewardBox(CCMenuItem* pSender);
    
    void after_receive_tryst_reward_625(CCObject* pObj);
    
private:
    int             max;
    int             left;

    time_t          savedTs;
    CCSprite*       plane;
    CCSize          planeSize;
    float           barWidth;
    CCMenuItem*     itemBox;
    CCProgressTimer* prog;
    CCSprite*       heart;
    CCLabelTTF*     countDown;
};

#endif /* TrystProgress_hpp */
