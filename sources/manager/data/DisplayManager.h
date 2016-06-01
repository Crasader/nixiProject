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

class DisplayManager
{
public:
    ~DisplayManager();
    static DisplayManager* Inst();
    void init();
    
public:
    CCPoint             center();
    float               ScreenWidth();
    float               ScreenHeight();
    float               OriginWidth();
    float               W(); // screen width
    float               H(); // screen height
    float               halfW(); // screen half width
    float               halfH(); // screen half height
    
    CCString* GetOffTheNumber(int index);
    
private:
    float               _width;
    float               _height;
};

#endif /* DisplayManager_hpp */
