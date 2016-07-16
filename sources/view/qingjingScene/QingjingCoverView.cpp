//
//  QingjingCoverView.cpp
//  tiegao
//
//  Created by mac on 16-7-13.
//
//

#include "QingjingCoverView.h"
#include "DataManager.h"
#include "AppUtil.h"


QingjingCoverView::QingjingCoverView()
{
    
}

QingjingCoverView::~QingjingCoverView()
{
    CC_SAFE_RELEASE_NULL(cardArray);
}

QingjingCoverView* QingjingCoverView::create(CCRect swBox, CCSize slSize , float disDistance , float disScale)
{
    QingjingCoverView* cover = new QingjingCoverView();
    if(cover && cover->init(swBox,slSize,disDistance,disScale))
    {
        cover->autorelease();
        return cover;
    }
    CC_SAFE_DELETE(cover);
    return NULL;
}

bool QingjingCoverView::init(CCRect swBox , CCSize slSize , float disDistance , float disScale)
{
    if(!CCNode::init()) return false;
    this->swBox = swBox;
    this->swPosition = swBox.origin;
    this->swSize = swBox.size;
    this->slSize = slSize;
    this->disDistance = disDistance;
    this->disScale = disScale;
    
    initData();
    return true;
}


void QingjingCoverView::initData()
{
    wSize = CCDirector::sharedDirector()->getWinSize();
    cardArray = CCArray::create();
    cardArray->retain();
    cardNum = 0;
    
    offsetPosition = ccp(swSize.width/2,swSize.height/2);
    
    scrollLayer = CCLayer::create();
    scrollLayer->setAnchorPoint(CCPointZero);
    scrollLayer->setPosition(CCPointZero);
    scrollLayer->setContentSize(slSize);
    scrollView = CCScrollView::create(swSize,scrollLayer);
    scrollView->setAnchorPoint(CCPointZero);
    scrollView->setContentOffset(ccp(0,0));
    scrollView->setTouchEnabled(false);
    scrollView->setDelegate(this);
    scrollView->setDirection(kCCScrollViewDirectionHorizontal);
    addChild(scrollView,1);
}

void QingjingCoverView::onEnter()
{
    CCNode::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0 , false);
}

void QingjingCoverView::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    removeAllChildren();
    CCNode::onExit();
}

bool QingjingCoverView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void QingjingCoverView::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint scroll_prepoint = pTouch->getPreviousLocation();
    CCPoint scroll_movepoint = pTouch->getLocation();
    if(swBox.containsPoint(scroll_movepoint))
    {
        moveBool = true;
        CCPoint adjustPoint = scroll_movepoint-scroll_prepoint;
        adjustScrollView(adjustPoint);
        adjustCardScale(adjustPoint);
    }
}

void QingjingCoverView::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    adjusetEndScrollView();
}

void QingjingCoverView::adjustCardScale(CCPoint adjustPoint)
{
    //    float disX = adjustPoint.x;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(cardArray,obj)
    {
        CCNode* card = (CCNode*) obj;
        float offset = scrollView->getContentOffset().x;
        float posX = card->getPositionX() + offset;
        float disMid = fabs(swSize.width/2-posX);
        int zOr = (int) (1000-disMid*0.1);
        card->setZOrder(zOr);
    }
}

void QingjingCoverView::adjustScrollView(CCPoint adjustPoint)
{
    CCPoint endPoint = ccpAdd(scrollView->getContentOffset(),ccp(adjustPoint.x,0));
    scrollView->unscheduleAllSelectors();
    scrollView->setContentOffset(endPoint,false);
}

void QingjingCoverView::adjusetEndScrollView()
{
    CCObject* obj = NULL;
    float minX = wSize.height;
    float midX = swSize.width/2;
    //获取距离中间最小值的card
    CCARRAY_FOREACH(cardArray,obj)
    {
        CCNode* card = (CCNode*) obj;
        float offset = scrollView->getContentOffset().x;
        //转化父类坐标
        float posX = card->getPositionX() + offset;
        float disMid = midX-posX;
        if(fabs(disMid) < fabs(minX)){
            minX = disMid;
        }
    }
    
    CCARRAY_FOREACH(cardArray,obj)
    {
        CCNode* item = (CCNode*) obj;
        //转化父类坐标
        float offset = scrollView->getContentOffset().x;
        float posX = item->getPositionX() + offset ;
        //距离中间长度
        float disMid = fabs(midX - posX - minX);
        int zOr = (int) (1000-disMid*0.1);
        item->setZOrder(zOr);
    }
    CCLayer* scrollLayer = (CCLayer*)scrollView->getContainer();
    CCMoveBy* moveBy = CCMoveBy::create(0.2f,ccp(minX,0));
    
    CCCallFuncN* callFuncN = CCCallFuncN::create(this,callfuncN_selector(QingjingCoverView::cardViewEnd_callBack));
    CCSequence* seq = CCSequence::create(moveBy,callFuncN,NULL);
    scrollLayer->runAction(seq);
}
void QingjingCoverView::adjusetEndScrollView2(float offsetPoint){
    CCObject* obj = NULL;
    float minX = wSize.height;
    float midX = swSize.width/2;
    //获取距离中间最小值的card
    CCARRAY_FOREACH(cardArray,obj)
    {
        CCNode* card = (CCNode*) obj;
        float offset = scrollView->getContentOffset().x + offsetPoint;
        //转化父类坐标
        float posX = card->getPositionX() + offset;
        float disMid = midX-posX;
        if(fabs(disMid) < fabs(minX)){
            minX = disMid;
        }
    }
    
    CCARRAY_FOREACH(cardArray,obj)
    {
        CCNode* item = (CCNode*) obj;
        //转化父类坐标
        float offset = scrollView->getContentOffset().x + offsetPoint;
        float posX = item->getPositionX() + offset ;
        //距离中间长度
        float disMid = fabs(midX - posX - minX);
        int zOr = (int) (1000-disMid*0.1);
        item->setZOrder(zOr);
    }
    CCLayer* scrollLayer = (CCLayer*)scrollView->getContainer();
    CCMoveBy* moveBy = CCMoveBy::create(0.2f,ccp(minX,0));
    
    CCCallFuncN* callFuncN = CCCallFuncN::create(this,callfuncN_selector(QingjingCoverView::cardViewEnd_callBack));
    CCSequence* seq = CCSequence::create(moveBy,callFuncN,NULL);
    scrollLayer->runAction(seq);
}
void QingjingCoverView::linshi(){
    CCPoint endPoint = ccpAdd(scrollView->getContentOffset(),ccp(-517,0));
    scrollView->unscheduleAllSelectors();
    scrollView->setContentOffset(endPoint, false);
}
void QingjingCoverView::linshi2(){
    CCPoint endPoint = ccpAdd(scrollView->getContentOffset(),ccp(517,0));
    scrollView->unscheduleAllSelectors();
    scrollView->setContentOffset(endPoint, false);
}

void QingjingCoverView::cardViewEnd_callBack(CCNode* pSender)
{
    getCurCardIndex();
}

void QingjingCoverView::scrollViewDidScroll(CCScrollView* view)
{
    //    CCLog("%f", view->getContentOffset().x);
}

void QingjingCoverView::scrollViewDidZoom(CCScrollView* view)
{
    
}



void QingjingCoverView::addCard(CCNode * card)
{
    int zOrder = 1000 - cardNum;
    this->addCard(card, zOrder, 0);
}

void QingjingCoverView::addCard(CCNode * card, int zOrder)
{
    this->addCard(card, zOrder,0);
}

void QingjingCoverView::addCard(CCNode* card, int zOrder, int tag)
{
    float positionX = offsetPosition.x + disDistance*cardNum;
    card->setPosition(ccp(positionX,offsetPosition.y));
    cardArray->addObject(card);
    scrollLayer->addChild(card , zOrder,tag);
    cardNum++;
//    CCLog("crad%d:[%f , %f]",cardNum,card->getPositionX(),card->getPositionY());
}

int QingjingCoverView::getCurCardIndex()
{
    float distance1 = - scrollLayer->getPositionX();
    float distance2 = swSize.width/2 - offsetPosition.x;
    //+5 浮点数误差
    int index = (distance1 + distance2 + 5) / (disDistance);
    CCLog("card index:%d  distance1:%f",index,distance1);
    
    if (DATA->getChapterNumberBool()) {
        DATA->setChapterNumberBool(false);
    }else{
        CSJson::Value taskConditionsData = AppUtil::read_json_file("res/story/taskConditions");
        CCDictionary* taskConditionsDic = AppUtil::dictionary_with_json(taskConditionsData);
        int allNumber = taskConditionsDic->count();
        
        if (index >= allNumber-1) {
            index = allNumber-1;
        }
        DATA->setChapterNumber(index);
    }
    
    if (moveBool) {
        moveBool = false;
        CCNotificationCenter::sharedNotificationCenter()->postNotification("Qingjing_UpdataButton");
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification("Qingjing_UpdataMan");
    
    return index;
}

void QingjingCoverView::setOffsetPosition(CCPoint var)
{
    offsetPosition = var;
    CCObject* obj = NULL;
    cardNum = 0;
    CCARRAY_FOREACH(cardArray,obj)
    {
        CCNode* card = (CCNode*) obj;
        float positionX = offsetPosition.x + disDistance*cardNum;
        card->setPosition(ccp(positionX,offsetPosition.y));
        cardNum++;
    }
    adjustCardScale(CCPointZero);
}

CCPoint QingjingCoverView::getOffsetPosition()
{
    return offsetPosition;
}










