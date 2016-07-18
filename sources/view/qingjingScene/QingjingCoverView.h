//
//  QingjingCoverView.h
//  tiegao
//
//  Created by mac on 16-7-13.
//
//

#ifndef __tiegao__QingjingCoverView__
#define __tiegao__QingjingCoverView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class QingjingCoverView
: public CCNode
, public CCScrollViewDelegate// , public CCTouchDelegate
{
public:
    QingjingCoverView();
    ~QingjingCoverView();
    /**
     *
     * @param scrollView的rect
     * @param scrollView滚动size
     * @param card之间的距离
     * @param card之间的缩放
     */
    static QingjingCoverView* create(CCRect swBox, CCSize slSize ,float disDistance , float disScale );
    virtual bool init(CCRect swBox , CCSize slSize , float disDistance , float disScale);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
public:
    
    void initData();
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    void adjustCardScale(CCPoint adjustPoint);
    void adjustScrollView(CCPoint adjustPoint);
    void adjusetEndScrollView();
    void adjusetEndScrollView2(float offsetPoint);
    void cardViewEnd_callBack(CCNode* pSender);
    int getCurCardIndex();//当前中间card索引 从0开始
    void addCard(CCNode * card);
    void addCard(CCNode * card, int zOrder);
    void addCard(CCNode* card, int zOrder, int tag);
    
    void rightMethods();
    void leftMethods();
    void scrollStatus(int index);
public:
    
    CC_SYNTHESIZE(CCPoint , swPosition , SwPosition);//scrollView 位置
    CC_SYNTHESIZE(CCSize , swSize , SwSize);//scrollView大小
    CC_SYNTHESIZE(CCSize , slSize , SlSize);//scrollLayer 大小
    CC_SYNTHESIZE(float , disDistance , DisDistance);//card距离间隔
    CC_SYNTHESIZE(float , disScale , DisScale);//crad缩放间隔
    CC_SYNTHESIZE(CCRect , swBox , SwBox);//scrollview 边框
    CC_SYNTHESIZE(CCLayer* , scrollLayer , scrollLayer);//scrollView的containLayer
    CC_SYNTHESIZE(int , cardNum , CardNum);//card索引
    CC_PROPERTY(CCPoint , offsetPosition , OffsetPosition);//card起始位置
    
private:
    CCSize wSize;
    CCArray* cardArray;
    CCScrollView* scrollView;
    
    bool moveBool;
};
#endif /* defined(__tiegao__QingjingCoverView__) */
