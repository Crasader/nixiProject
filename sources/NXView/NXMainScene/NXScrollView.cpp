//
//  NXScrollView.cpp
//  tiegao
//
//  Created by mac on 17-6-30.
//
//

#include "NXScrollView.h"
#include "DisplayManager.h"







bool NXScrollView::init(){
	bool bRet = false;
	do
	{
        CC_BREAK_IF( !CCLayer::init() );
        
        m_nCurPage = 1;
        
        tempSpr = CCSprite::create("res/nxpic/NXMainscene/huodong/NXMainscene_dikuang.png");
        tempSpr->setAnchorPoint(ccp(1, 0));
        tempSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 2.f, 4.f));
        tempSpr->setVisible(false);
        this->addChild(tempSpr, 10);
        
        CCSprite *pLayer = CCSprite::create();
        char helpstr[60] = {0};
        for (int i = 1; i <= 5; ++ i)
        {
            memset(helpstr, 0, sizeof(helpstr));
//            sprintf(helpstr,"bg_%02d.png",i);
            sprintf(helpstr,"res/nxpic/NXMainscene/huodong/NXMainscene_kuang.png");
            CCSprite *pSprite = CCSprite::create(helpstr);
            pSprite->setAnchorPoint(CCPointZero);
            pSprite->setPosition(ccp(0 + pSprite->getContentSize().width* (i - 1), 0));
            pLayer->addChild(pSprite);
            
            CCSprite* hdSpr = CCSprite::create("res/nxpic/NXMainscene/huodong/NXMainscene_title.png");
            hdSpr->setAnchorPoint(CCPointZero);
            hdSpr->setPosition(CCPointZero);
            pSprite->addChild(hdSpr);
            
        }
        
        m_pScrollView = CCScrollView::create(CCSizeMake(328, 166), pLayer);
        m_pScrollView->setContentOffset(CCPointZero);
        m_pScrollView->setPosition(ccp(0, 35));
        m_pScrollView->setTouchEnabled(false);
        m_pScrollView->setDelegate(this);
        m_pScrollView->setBounceable(true);
        m_pScrollView->setDirection(kCCScrollViewDirectionHorizontal);
        pLayer->setContentSize(CCSizeMake(328*5, 166));
        
        this->addChild(m_pScrollView);
        
        CCSpriteFrameCache *pCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
        
        pCache->addSpriteFrame(CCSpriteFrame::create("res/nxpic/NXMainscene/huodong/NXbutton_normal.png",CCRectMake(0, 0, 18, 18)),"res/nxpic/NXMainscene/huodong/NXbutton_normal.png");
        pCache->addSpriteFrame(CCSpriteFrame::create("res/nxpic/NXMainscene/huodong/NXbutton_selected.png",CCRectMake(0, 0, 18, 18)),"res/nxpic/NXMainscene/huodong/NXbutton_selected.png");
        
        CCSprite* tempSpr = CCSprite::create("res/nxpic/NXMainscene/huodong/NXMainscene_dikuang.png");
        int count = 5;
        for (int i = 1; i <= count; ++ i)
        {
            CCSprite *pPoint = CCSprite::createWithSpriteFrameName("res/nxpic/NXMainscene/huodong/NXbutton_normal.png");
            pPoint->setTag(i);
            pPoint->setPosition(ccp(((tempSpr->getContentSize().width* .5f) - (pPoint->getContentSize().width* (count/2)) - (10* (count/2))) + (pPoint->getContentSize().width * (i-1) + (10* i)), 25));
            this->addChild(pPoint);
        }
		CCSprite *pPoint = (CCSprite *)this->getChildByTag(1);
		pPoint->setDisplayFrame(pCache->spriteFrameByName("res/nxpic/NXMainscene/huodong/NXbutton_selected.png"));
        
		bRet = true;
	}while(0);
    
	return bRet;
    
}

void NXScrollView::menuCloseCallback(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem*)pSender;
    long index = (long)item->getUserData();
    CCLog("<><><><><><><>%ld", index);
    
    
}


void NXScrollView::scrollViewDidScroll(CCScrollView* view){
    CCPoint offset = view->getContentOffset();
    CCLog("offset == %f", offset.x);
    if (offset.x < view->minContainerOffset().x) {
        view->setContentOffset(CCPoint(view->minContainerOffset().x, offset.y));
    }else if (offset.x > view->maxContainerOffset().x){
        view->setContentOffset(CCPoint(view->maxContainerOffset().x, offset.y));
    }
}
void NXScrollView::scrollViewDidZoom(CCScrollView* view){
    CCLOG("zoom");
}

void NXScrollView::onEnter(){
    CCLayer::onEnter();
	
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
}
void NXScrollView::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

bool NXScrollView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    if (tempSpr->boundingBox().containsPoint(location)) {
        m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
        return true;
    }
    
	return false;
}
void NXScrollView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void NXScrollView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 50)
	{
		adjustScrollView(distance);
	}else{
        CCLog("<><><><><> %d", m_nCurPage);
    }
}
void NXScrollView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 50)
	{
		adjustScrollView(distance);
	}
}


void NXScrollView::adjustScrollView(float offset)
{
	CCSpriteFrameCache *pCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
	CCSprite *pPoint = (CCSprite *)this->getChildByTag(m_nCurPage);
	pPoint->setDisplayFrame(pCache->spriteFrameByName("res/nxpic/NXMainscene/huodong/NXbutton_normal.png"));
	if (offset < 0)
	{
		m_nCurPage ++;
	}else
	{
		m_nCurPage --;
	}
    
	if (m_nCurPage < 1)
	{
		m_nCurPage = 1;
	}
    
	if(m_nCurPage > 5)
	{
		m_nCurPage = 5;
	}
    
    pPoint = (CCSprite *)this->getChildByTag(m_nCurPage);
    pPoint->setDisplayFrame(pCache->spriteFrameByName("res/nxpic/NXMainscene/huodong/NXbutton_selected.png"));
    
    CCSprite* tempSpr = CCSprite::create("res/nxpic/NXMainscene/huodong/NXMainscene_kuang.png");
    CCPoint  adjustPos = ccp(0 - tempSpr->getContentSize().width* (m_nCurPage - 1), 0);
    m_pScrollView->setContentOffset(adjustPos, true);
}







