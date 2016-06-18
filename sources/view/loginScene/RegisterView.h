//
//  RegisterView.h
//  mm3c
//
//  Created by lakkey on 15-8-11.
//
//

#ifndef __mm3c__RegisterView__
#define __mm3c__RegisterView__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class AccountValidate;

class RegisterView : public CCLayer, public CCEditBoxDelegate
{
public:
    ~RegisterView();
    CREATE_FUNC(RegisterView);
    virtual bool init();
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
    void create_view();
    void start_regist();

protected:
    AccountValidate*                _validate;
    
    CCEditBox*                      _tf_account;
    CCEditBox*                      _tf_password;
    CCEditBox*                      _tf_password2;
    
    void update_account_info();
    void shift_to_loginview(CCMenuItem* pSender);
    void send_register_request(CCMenuItem* pSender);
    bool check_can_send_request();
    
private:
};


#endif /* defined(__mm3c__RegisterView__) */
