//
//  OperationPanel.hpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#ifndef OperationPanel_hpp
#define OperationPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class OperationPanel : public CCLayer
{
public:
    ~OperationPanel();
    CREATE_FUNC(OperationPanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_from(CCPoint from);
    
private:
    void do_enter();
    void do_exit();
    void remove();
    
    void on_purchase(CCMenuItem* btn);
    void on_monthlycard(CCMenuItem* btn);
    
private:
    CCLayer*            _content;
    CCSprite*           _bg;
    CCPoint             _enter_pos;
};

#endif /* OperationPanel_hpp */
