//
//  AccountValidate.h
//  mm3c
//
//  Created by lakkey on 14-12-25.
//
//

#ifndef __mm3c__AccountValidate__
#define __mm3c__AccountValidate__

#include "cocos2d.h"

USING_NS_CC;


class AccountValidate
{
public:
    ~AccountValidate();
    AccountValidate();
    
    bool check_legal_number(const char* text);
    bool check_legal_text(const char* text);
    bool check_legal_char(const char* text);
    
    int checkLiteralLegal(std::string str, int minLimmit, int maxLimmit);
//    void showErrorMessage(int state, int type);
    
    bool check_can_send_request();
};

#endif /* defined(__mm3c__AccountValidate__) */
