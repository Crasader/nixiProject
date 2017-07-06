//
//  NXScrollView.h
//  tiegao
//
//  Created by mac on 17-6-30.
//
//

#ifndef __tiegao__NXScrollView__
#define __tiegao__NXScrollView__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class NXScrollView
: public cocos2d::CCLayer
,public CCScrollViewDelegate
{
public:
    virtual bool init();
    
    void menuCloseCallback(CCObject* pSender);
    
    CREATE_FUNC(NXScrollView);
    
public:
	void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);
    
	virtual void onEnter();
	virtual void onExit();
    
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void adjustScrollView(float offset);
    CCScrollView *m_pScrollView;
    CCPoint m_touchPoint;
    int m_nCurPage;
    CCSprite* tempSpr;
    
};

#endif /* defined(__tiegao__NXScrollView__) */
