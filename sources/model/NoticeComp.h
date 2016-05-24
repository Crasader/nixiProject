//
//  NoticeComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef NoticeComp_hpp
#define NoticeComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class NoticeComp : public CCObject
{
public:
    ~NoticeComp();
    CREATE_FUNC(NoticeComp);
    bool init();
    
    void init_with_json(Value json);
    
public:
    
protected:
    
private:
    
};

#endif /* NoticeComp_hpp */
