//
//  CoffersComp.hpp
//  tiegao
//
//  Created by mac on 16/7/11.
//
//

#ifndef CoffersComp_hpp
#define CoffersComp_hpp

#include "cocos2d.h"
#include "json_lib.h"
#include "extend_define.h"

USING_NS_CC;
using namespace CSJson;

class CoffersComp : public CCObject
{
// Export
    SYNTHESIZE_READONLY(bool, _hasInitTemplate, HasInitTemplate);
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _companyTemplate, CompanyTemplate);
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _companyRewardRecord, CompanyRewardRecord);
public:
//    bool could_prduce();
    void produce();
    void reset_collected();
    
public: // Inport
    void init_company_template(Value json);
    void replace_user_data(Value json);
    
public:
    ~CoffersComp();
    CREATE_FUNC(CoffersComp);
    bool init();
    
public:
    int         profit;
    int         top;
    int         collected;
    
private:
    
};

#endif /* CoffersComp_hpp */
