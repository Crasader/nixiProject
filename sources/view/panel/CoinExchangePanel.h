//
//  CoinExchangePanel.hpp
//  tiegao
//
//  Created by mac on 16/6/30.
//
//

#ifndef CoinExchangePanel_hpp
#define CoinExchangePanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CoinExchangePanel : public CCLayer
{
public:
    ~CoinExchangePanel();
    CREATE_FUNC(CoinExchangePanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show();
    void show_from(CCPoint from);
    
private:
    void do_enter();
    void do_exit();
    void remove();
    void buy();
    
    void nc_exchange_coin_103(CCObject* pObj);
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
    CCPoint             _enter_pos;
};

#endif /* CoinExchangePanel_hpp */
