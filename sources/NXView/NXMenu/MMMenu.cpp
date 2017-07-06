//
//  MMMenu.cpp
//  mm3c
//
//  Created by mac on 15-5-26.
//
//

#include "MMMenu.h"
#include "CCDirector.h"
#include "CCApplication.h"
#include "support/CCPointExtension.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouch.h"
#include "CCStdC.h"
#include "cocoa/CCInteger.h"
#include "platform/CCEGLViewProtocol.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "platform/ios/CCEGLView.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/CCEGLView.h"
#endif

#include <vector>
#include <stdarg.h>

using namespace std;

NS_CC_BEGIN

bool MMMenu::m_bTouchesBool = true;
bool MMMenu::m_bCheckScissor = false;

static std::vector<unsigned int> ccarray_to_std_vector(CCArray* pArray)
{
    std::vector<unsigned int> ret;
    CCObject* pObj;
    CCARRAY_FOREACH(pArray, pObj)
    {
        CCInteger* pInteger = (CCInteger*)pObj;
        ret.push_back((unsigned int)pInteger->getValue());
    }
    return ret;
}

enum
{
    kDefaultPadding =  5,
};

//
//MMMenu
//

MMMenu* MMMenu::create()
{
    return MMMenu::create(NULL, NULL);
}

MMMenu * MMMenu::create(CCMenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    MMMenu *pRet = MMMenu::createWithItems(item, args);
    
    va_end(args);
    
    return pRet;
}

MMMenu* MMMenu::createWithArray(CCArray* pArrayOfItems)
{
    MMMenu *pRet = new MMMenu();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

MMMenu* MMMenu::createWithItems(CCMenuItem* item, va_list args)
{
    CCArray* pArray = NULL;
    if( item )
    {
        pArray = CCArray::create(item, NULL);
        CCMenuItem *i = va_arg(args, CCMenuItem*);
        while(i)
        {
            pArray->addObject(i);
            i = va_arg(args, CCMenuItem*);
        }
    }
    
    return MMMenu::createWithArray(pArray);
}

MMMenu* MMMenu::createWithItem(CCMenuItem* item)
{
    return MMMenu::create(item, NULL);
}

bool MMMenu::init()
{
    return initWithArray(NULL);
}

bool MMMenu::initWithArray(CCArray* pArrayOfItems)
{
    if (CCLayer::init())
    {
//        setTouchPriority(kMMMenuHandlerPriority);
        this->setTouchSwallowEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchEnabled(true);
        
        m_bEnabled = true;
        // menu in the center of the screen
        CCSize s = CCDirector::sharedDirector()->getWinSize();
        
        this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(ccp(0.5f, 0.5f));
        this->setContentSize(s);
        
        setPosition(ccp(s.width/2, s.height/2));
        
        if (pArrayOfItems != NULL)
        {
            int z=0;
            CCObject* pObj = NULL;
            CCARRAY_FOREACH(pArrayOfItems, pObj)
            {
                CCMenuItem* item = (CCMenuItem*)pObj;
                this->addChild(item, z);
                z++;
            }
        }
        
        //    [self alignItemsVertically];
        m_pSelectedItem = NULL;
        m_eState = kMMMenuStateWaiting;
        
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        
        return true;
    }
    return false;
}

/*
 * override add:
 */
void MMMenu::addChild(CCNode * child)
{
    CCLayer::addChild(child);
}

void MMMenu::addChild(CCNode * child, int zOrder)
{
    CCLayer::addChild(child, zOrder);
}

void MMMenu::addChild(CCNode * child, int zOrder, int tag)
{
    CCAssert( dynamic_cast<CCMenuItem*>(child) != NULL, "Menu only supports MenuItem objects as children");
    CCLayer::addChild(child, zOrder, tag);
}

void MMMenu::onExit()
{
    if (m_eState == kMMMenuStateTrackingTouch)
    {
        if (m_pSelectedItem)
        {
            m_pSelectedItem->unselected();
            m_pSelectedItem = NULL;
        }
        
        m_eState = kMMMenuStateWaiting;
    }
    
    CCLayer::onExit();
}

void MMMenu::removeChild(CCNode* child, bool cleanup)
{
    CCMenuItem *pMenuItem = dynamic_cast<CCMenuItem*>(child);
    CCAssert(pMenuItem != NULL, "Menu only supports MenuItem objects as children");
    
    if (m_pSelectedItem == pMenuItem)
    {
        m_pSelectedItem = NULL;
    }
    
    CCNode::removeChild(child, cleanup);
}

//Menu - Events

void MMMenu::setHandlerPriority(int newPriority)
{
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    pDispatcher->setPriority(newPriority, this);
}

void MMMenu::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    //    pDirector->getTouchDispatcher()->addTargetedDelegate(this, this->getTouchPriority(), true);
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
}

void MMMenu::setTouchesBool(bool isTouchesBool){
    m_bTouchesBool = isTouchesBool;
}
void MMMenu::setCheckScissor(bool isCheckScissor){
    m_bCheckScissor = isCheckScissor;
}

bool MMMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    if (m_eState != kMMMenuStateWaiting || ! m_bVisible || !m_bEnabled)
    {
        return false;
    }
    
    for (CCNode* c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    if (m_bCheckScissor) {
        CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();
        const CCRect & scissorBox = eglView->getScissorRect();
        if (!scissorBox.containsPoint(touch->getLocation())) {
            return false;
        }
    }
    
    m_pSelectedItem = this->itemForTouch(touch);
    if (m_pSelectedItem)
    {
        m_eState = kMMMenuStateTrackingTouch;
        m_pSelectedItem->selected();
        
        if (!m_bTouchesBool) {
            m_touchBeginWorldPos = convertToWorldSpace(getPosition());
        }
        return true;
    }
    return false;
}

void MMMenu::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kMMMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    if (m_pSelectedItem)
    {
        m_pSelectedItem->unselected();
        do {
            if (!m_bTouchesBool) {
                CCPoint newWorldPos = convertToWorldSpace(getPosition());
                const static float kMenuMinMove = 2;
                if (fabs(newWorldPos.x - m_touchBeginWorldPos.x) > kMenuMinMove || fabs(newWorldPos.y - m_touchBeginWorldPos.y) > kMenuMinMove) {
                    break;
                }
            }
            
            m_pSelectedItem->activate();
            
        } while (false);
    }
    m_eState = kMMMenuStateWaiting;
}

void MMMenu::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kMMMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
    if (m_pSelectedItem)
    {
        m_pSelectedItem->unselected();
    }
    m_eState = kMMMenuStateWaiting;
}

void MMMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kMMMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    CCMenuItem *currentItem = this->itemForTouch(touch);
    if (currentItem != m_pSelectedItem)
    {
        if (m_pSelectedItem)
        {
            m_pSelectedItem->unselected();
        }
        m_pSelectedItem = currentItem;
        if (m_pSelectedItem)
        {
            m_pSelectedItem->selected();
        }
    }
}

//Menu - Alignment
void MMMenu::alignItemsVertically()
{
    this->alignItemsVerticallyWithPadding(kDefaultPadding);
}

void MMMenu::alignItemsVerticallyWithPadding(float padding)
{
    float height = -padding;
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                height += pChild->getContentSize().height * pChild->getScaleY() + padding;
            }
        }
    }
    
    float y = height / 2.0f;
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                pChild->setPosition(ccp(0, y - pChild->getContentSize().height * pChild->getScaleY() / 2.0f));
                y -= pChild->getContentSize().height * pChild->getScaleY() + padding;
            }
        }
    }
}

void MMMenu::alignItemsHorizontally(void)
{
    this->alignItemsHorizontallyWithPadding(kDefaultPadding);
}

void MMMenu::alignItemsHorizontallyWithPadding(float padding)
{
    
    float width = -padding;
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                width += pChild->getContentSize().width * pChild->getScaleX() + padding;
            }
        }
    }
    
    float x = -width / 2.0f;
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                pChild->setPosition(ccp(x + pChild->getContentSize().width * pChild->getScaleX() / 2.0f, 0));
                x += pChild->getContentSize().width * pChild->getScaleX() + padding;
            }
        }
    }
}

void MMMenu::alignItemsInColumns(unsigned int columns, ...)
{
    va_list args;
    va_start(args, columns);
    
    this->alignItemsInColumns(columns, args);
    
    va_end(args);
}

void MMMenu::alignItemsInColumns(unsigned int columns, va_list args)
{
    CCArray* rows = CCArray::create();
    while (columns)
    {
        rows->addObject(CCInteger::create(columns));
        columns = va_arg(args, unsigned int);
    }
    alignItemsInColumnsWithArray(rows);
}

void MMMenu::alignItemsInColumnsWithArray(CCArray* rowsArray)
{
    vector<unsigned int> rows = ccarray_to_std_vector(rowsArray);
    
    int height = -5;
    unsigned int row = 0;
    unsigned int rowHeight = 0;
    unsigned int columnsOccupied = 0;
    unsigned int rowColumns;
    
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                CCAssert(row < rows.size(), "");
                
                rowColumns = rows[row];
                // can not have zero columns on a row
                CCAssert(rowColumns, "");
                
                float tmp = pChild->getContentSize().height;
                rowHeight = (unsigned int)((rowHeight >= tmp || isnan(tmp)) ? rowHeight : tmp);
                
                ++columnsOccupied;
                if (columnsOccupied >= rowColumns)
                {
                    height += rowHeight + 5;
                    
                    columnsOccupied = 0;
                    rowHeight = 0;
                    ++row;
                }
            }
        }
    }
    
    // check if too many rows/columns for available menu items
    CCAssert(! columnsOccupied, "");
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    row = 0;
    rowHeight = 0;
    rowColumns = 0;
    float w = 0.0;
    float x = 0.0;
    float y = (float)(height / 2);
    
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                if (rowColumns == 0)
                {
                    rowColumns = rows[row];
                    w = winSize.width / (1 + rowColumns);
                    x = w;
                }
                
                float tmp = pChild->getContentSize().height;
                rowHeight = (unsigned int)((rowHeight >= tmp || isnan(tmp)) ? rowHeight : tmp);
                
                pChild->setPosition(ccp(x - winSize.width / 2,
                                        y - pChild->getContentSize().height / 2));
                
                x += w;
                ++columnsOccupied;
                
                if (columnsOccupied >= rowColumns)
                {
                    y -= rowHeight + 5;
                    
                    columnsOccupied = 0;
                    rowColumns = 0;
                    rowHeight = 0;
                    ++row;
                }
            }
        }
    }
}

void MMMenu::alignItemsInRows(unsigned int rows, ...)
{
    va_list args;
    va_start(args, rows);
    
    this->alignItemsInRows(rows, args);
    
    va_end(args);
}

void MMMenu::alignItemsInRows(unsigned int rows, va_list args)
{
    CCArray* pArray = CCArray::create();
    while (rows)
    {
        pArray->addObject(CCInteger::create(rows));
        rows = va_arg(args, unsigned int);
    }
    alignItemsInRowsWithArray(pArray);
}

void MMMenu::alignItemsInRowsWithArray(CCArray* columnArray)
{
    vector<unsigned int> columns = ccarray_to_std_vector(columnArray);
    
    vector<unsigned int> columnWidths;
    vector<unsigned int> columnHeights;
    
    int width = -10;
    int columnHeight = -5;
    unsigned int column = 0;
    unsigned int columnWidth = 0;
    unsigned int rowsOccupied = 0;
    unsigned int columnRows;
    
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                // check if too many menu items for the amount of rows/columns
                CCAssert(column < columns.size(), "");
                
                columnRows = columns[column];
                // can't have zero rows on a column
                CCAssert(columnRows, "");
                
                // columnWidth = fmaxf(columnWidth, [item contentSize].width);
                float tmp = pChild->getContentSize().width;
                columnWidth = (unsigned int)((columnWidth >= tmp || isnan(tmp)) ? columnWidth : tmp);
                
                columnHeight += (int)(pChild->getContentSize().height + 5);
                ++rowsOccupied;
                
                if (rowsOccupied >= columnRows)
                {
                    columnWidths.push_back(columnWidth);
                    columnHeights.push_back(columnHeight);
                    width += columnWidth + 10;
                    
                    rowsOccupied = 0;
                    columnWidth = 0;
                    columnHeight = -5;
                    ++column;
                }
            }
        }
    }
    
    // check if too many rows/columns for available menu items.
    CCAssert(! rowsOccupied, "");
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    column = 0;
    columnWidth = 0;
    columnRows = 0;
    float x = (float)(-width / 2);
    float y = 0.0;
    
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                if (columnRows == 0)
                {
                    columnRows = columns[column];
                    y = (float) columnHeights[column];
                }
                
                // columnWidth = fmaxf(columnWidth, [item contentSize].width);
                float tmp = pChild->getContentSize().width;
                columnWidth = (unsigned int)((columnWidth >= tmp || isnan(tmp)) ? columnWidth : tmp);
                
                pChild->setPosition(ccp(x + columnWidths[column] / 2,
                                        y - winSize.height / 2));
                
                y -= pChild->getContentSize().height + 10;
                ++rowsOccupied;
                
                if (rowsOccupied >= columnRows)
                {
                    x += columnWidth + 5;
                    rowsOccupied = 0;
                    columnRows = 0;
                    columnWidth = 0;
                    ++column;
                }
            }
        }
    }
}

CCMenuItem* MMMenu::itemForTouch(CCTouch *touch)
{
    CCPoint touchLocation = touch->getLocation();
    
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCMenuItem* pChild = dynamic_cast<CCMenuItem*>(pObject);
            if (pChild && pChild->isVisible() && pChild->isEnabled())
            {
                CCPoint local = pChild->convertToNodeSpace(touchLocation);
                CCRect r = pChild->rect();
                r.origin = CCPointZero;
                
                if (r.containsPoint(local))
                {
                    return pChild;
                }
            }
        }
    }
    
    return NULL;
}

NS_CC_END