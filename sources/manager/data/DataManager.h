//
//  DataManager.hpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#ifndef DataManager_hpp
#define DataManager_hpp

#include "cocos2d.h"
//#include "cocos-ext.h"

USING_NS_CC;
//USING_NS_CC_EXT;

class DataManager
{
public:
    ~DataManager();
    static DataManager* Inst();
    
public:
    void config_UUID();
    const char* obtain_UUID();
    time_t cur_timestamp();
    
    const char* obtain_sid();
    
private:
    string          _uuid;
    string          _sid;
};

#endif /* DataManager_hpp */
