//
//  LoginView.h
//  mm3c
//
//  Created by lakkey on 15-8-11.
//
//

#ifndef __mm3c__LoginView__
#define __mm3c__LoginView__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class AccountValidate;

class LoginView : public CCLayer, public CCEditBoxDelegate
{
public:
    ~LoginView();
    CREATE_FUNC(LoginView);
    virtual bool init();
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
    void create_view();

protected:
    AccountValidate*                _validate;
    
    CCEditBox*                      _tf_account;
    CCEditBox*                      _tf_password;
    
    void shift_to_registerview(CCMenuItem* pSender);
    void send_login_request(CCMenuItem* pSender);
    bool check_can_send_request();
    
private:
};


#endif /* defined(__mm3c__LoginView__) */
