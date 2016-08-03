//
//  NoticeManager.hpp
//  tiegao
//
//  Created by mac on 16/7/22.
//
//

#ifndef NoticeManager_hpp
#define NoticeManager_hpp

#include "cocos2d.h"

USING_NS_CC;

#define NOTICE      NoticeManager::Inst()

class Notice : public CCObject
{
public:
    virtual ~Notice() {}
    CREATE_FUNC(Notice);
    inline bool init() { return true; }

    CC_SYNTHESIZE_PASS_BY_REF(string, _name, Name);
    CC_SYNTHESIZE_PASS_BY_REF(string, _desc, Desc);
};

// ---------------------------------------------

class NoticeManager : public CCObject
{
public: // Export
    Notice* fetch_notice();
    
public:
    ~NoticeManager();
    static NoticeManager* Inst();
    void init();
    
private:
    Notice* fetch_prior_notice();
    Notice* fetch_second_notice();
    
private:
    CCArray*            _candidates;
};

#endif /* NoticeManager_hpp */
