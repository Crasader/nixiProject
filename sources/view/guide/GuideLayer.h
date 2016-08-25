//
//  GuideLayer.h
//  tiegao
//
//  Created by mac on 16-8-24.
//
//

#ifndef __tiegao__GuideLayer__
#define __tiegao__GuideLayer__

#include <iostream>
#include "cocos2d.h"
#include "PromptLayer.h"

USING_NS_CC;

class GuideLayer : public CCLayer
{
public:
    GuideLayer();
    ~GuideLayer();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(GuideLayer);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void visit();
    
    static GuideLayer* create_with_guide(int _index);
    void init_with_guide(int _index);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
public:
    
    void setHighlightRect(const CCRect& rect);
    
    void flyLabel();
    
    void creatShou();
    void creatView();
    void creatAnimation();
    
    void openAnimation();
    void closeAnimation();
    
    void openTouch(float dt);
    
    void creatLabel(float dt);
    void creatDiKuang(int index);
    
    void logic(float dt);
    void getIndex(float dt);
    void creat_ContentLength(std::string str);
    const char* getContent(){return content.c_str();}
    std::string getContentByLength(int length);
    int getContentLength();
    
    
    CCRect isRect1_1();
    bool isContainTouchPoint1_1(CCTouch* touch);
    
public:
    
    PromptLayer* promptLayer;
    CCRenderTexture* m_pTarget;
    
    bool touchBool;
    
    int contentLength;
    int wordCount;
    int guideIndex;
    
    CCLabelTTF* saidLabel;
    
    std::string content;
    
    CCSprite* guideSpr;
    CCSprite* dkSpr;
    CCSprite* manSpr;
    
protected:
    CCRect      _testRect;
    void creat_testRect();
    void beforeDraw();
    void afterDraw();
    
};
#endif /* defined(__tiegao__GuideLayer__) */
