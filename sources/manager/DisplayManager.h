//
//  DisplayManager.hpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#ifndef DisplayManager_hpp
#define DisplayManager_hpp

#include "cocos2d.h"

USING_NS_CC;

#define DISPLAY         DisplayManager::Inst()

class DisplayManager: public CCObject
{
public:
    ~DisplayManager();
    static DisplayManager* Inst();
    void init();
    
public:

    const char*         fangzhengFont();
    
    ccColor3B           defalutColor();
    ccColor3B           dullBlueColor();
    
    CCPoint             center();
    float               ScreenWidth();
    float               ScreenHeight();
    float               OriginWidth();
    float               W(); // screen width
    float               H(); // screen height
    float               halfW(); // screen half width
    float               halfH(); // screen half height
    
    CCString* GetOffTheNumber(int index);
    CCString* GetOffTheName(int index);
    
    CCString* GetOffTheNumber2(int index);
    CCString* GetOffTheName2(int index);

    float               btn_scale();
    
private:
    float               _width;
    float               _height;
   
// >> blink
public:
    void blink();
    void stopBlink();
    
    CC_SYNTHESIZE(int, _curZRId, CurZRId);
    CC_SYNTHESIZE(int, _savedZRId, SavedZRId);
    CC_SYNTHESIZE(CCSprite*, _zrSpr1, ZRSpr);
    CC_SYNTHESIZE_RETAIN(CCAnimate*, _blinkAnim, BlinkAnim);

private:
    float               _nextInterval;
// <<
};

#endif /* DisplayManager_hpp */
