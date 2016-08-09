//
//  RecommendView.hpp
//  tiegao
//
//  Created by mac on 16/8/9.
//
//

#ifndef RecommendView_hpp
#define RecommendView_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class RecommendView :
public CCLayer,
public CCEditBoxDelegate
{
public:
    ~RecommendView();
    CREATE_FUNC(RecommendView);
    bool init();

    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
        return false;
    }
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
    void show();
    
private:
    void on_btn_confirem(CCObject* pSender);
    
private:
    CCEditBox*                  _input;
};

#endif /* RecommendView_hpp */
