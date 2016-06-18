//
//  CreateName.h
//  mm3c
//
//  Created by lakkey on 14-12-31.
//
//

#ifndef __mm3c__CreateName__
#define __mm3c__CreateName__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CreateName :
    public CCScene,
    public CCEditBoxDelegate
{
public:
    ~CreateName();
    CREATE_FUNC(CreateName);
    bool init();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
        return true;
    }
    virtual void onEnter();
    virtual void onExit();
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
    void show();
    
protected:
    CCEditBox*                      _tf_nickname;
    
    CCMenuItemToggle*               _item_male;
    CCMenuItemToggle*               _item_fmale;
    
    std::string                     _oldStr;
    
    void create_baes_view();
    void create_content_view();

    void on_btn_confirem(CCObject* pSender);
    bool check_nickname(std::string str);
    
    void randName(CCNode* pSender);
};

#endif /* defined(__mm3c__CreateName__) */
