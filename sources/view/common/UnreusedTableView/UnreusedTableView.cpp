//
//  UnreusedTableView.cpp
//  tiegao
//
//  Created by mac on 16/7/6.
//
//

#include "UnreusedTableView.h"

UnreusedTableView::~UnreusedTableView() {
}

UnreusedTableView* UnreusedTableView::create(CCTableViewDataSource* dataSource, CCSize size)
{
    return UnreusedTableView::create(dataSource, size, NULL);
}

UnreusedTableView* UnreusedTableView::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    UnreusedTableView *table = new UnreusedTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

bool UnreusedTableView::initWithViewSize(CCSize size, CCNode* container/* = NULL*/)
{
    if (CCTableView::initWithViewSize(size,container))
    {
        _eageBounched = true;
        
        m_pCellsUsed      = new CCArrayForObjectSorting();
        m_pCellsFreed     = new CCArrayForObjectSorting();
        m_pIndices        = new std::set<unsigned int>();
        m_eVordering      = kCCTableViewFillBottomUp;
        this->setDirection(kCCScrollViewDirectionVertical);
        
        CCScrollView::setDelegate(this);
        return true;
    }
    return false;
}

void UnreusedTableView::scrollViewDidScroll(CCScrollView* view)
{
    if (! _eageBounched) {
        CCPoint offset = view->getContentOffset();
        if (m_eDirection == kCCScrollViewDirectionVertical) {
            if (offset.y < view->minContainerOffset().y) {
                view->setContentOffset(CCPoint(offset.x, view->minContainerOffset().y));
            }else if (offset.y > view->maxContainerOffset().y){
                view->setContentOffset(CCPoint(offset.x, view->maxContainerOffset().y));
            }
        }
        else {
            if (offset.x < view->minContainerOffset().x) {
                view->setContentOffset(CCPoint(view->minContainerOffset().x, offset.y));
            }else if (offset.y > view->maxContainerOffset().y){
                view->setContentOffset(CCPoint(view->maxContainerOffset().x, offset.y));
            }
        }
    }
    
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems)
    {
        return;
    }
    
    if(m_pTableViewDelegate != NULL) {
        m_pTableViewDelegate->scrollViewDidScroll(this);
    }
    
    unsigned int startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    CCPoint offset = ccpMult(this->getContentOffset(), -1);
    maxIdx = MAX(uCountOfItems-1, 0);
    
    if (m_eVordering == kCCTableViewFillTopDown)
    {
        offset.y = offset.y + m_tViewSize.height/this->getContainer()->getScaleY();
    }
    startIdx = this->_indexFromOffset(offset);
    if (startIdx == CC_INVALID_INDEX)
    {
        startIdx = uCountOfItems - 1;
    }
    
    if (m_eVordering == kCCTableViewFillTopDown)
    {
        offset.y -= m_tViewSize.height/this->getContainer()->getScaleY();
    }
    else
    {
        offset.y += m_tViewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += m_tViewSize.width/this->getContainer()->getScaleX();
    
//    endIdx   = this->_indexFromOffset(offset);
//    if (endIdx == CC_INVALID_INDEX)
//    {
//        endIdx = uCountOfItems - 1;
//    }
    
//    if (m_pCellsUsed->count() > 0)
//    {
//        CCTableViewCell* cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
//        
//        idx = cell->getIdx();
//        while(idx <startIdx)
//        {
//            this->_moveCellOutOfSight(cell);
//            if (m_pCellsUsed->count() > 0)
//            {
//                cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
//                idx = cell->getIdx();
//            }
//            else
//            {
//                break;
//            }
//        }
//    }
//    if (m_pCellsUsed->count() > 0)
//    {
//        CCTableViewCell *cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
//        idx = cell->getIdx();
//        
//        while(idx <= maxIdx && idx > endIdx)
//        {
//            this->_moveCellOutOfSight(cell);
//            if (m_pCellsUsed->count() > 0)
//            {
//                cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
//                idx = cell->getIdx();
//                
//            }
//            else
//            {
//                break;
//            }
//        }
//    }
    
//    for (unsigned int i=startIdx; i <= endIdx; i++)
    for (unsigned int i=0; i < uCountOfItems; i++)
    {
        //if ([m_pIndices containsIndex:i])
        if (m_pIndices->find(i) != m_pIndices->end())
        {
            continue;
        }
        this->updateCellAtIndex(i);
    }
}

void UnreusedTableView::_moveCellOutOfSight(CCTableViewCell *cell) {
    CCLOG("UnreusedTableView::_moveCellOutOfSight");
    if(m_pTableViewDelegate != NULL) {
        m_pTableViewDelegate->tableCellWillRecycle(this, cell);
    }
    
//    m_pCellsFreed->addObject(cell);
//    m_pCellsUsed->removeSortedObject(cell);
//    m_pIndices->erase(cell->getIdx());
//    // [m_pIndices removeIndex:cell.idx];
//    cell->reset();
//    if (cell->getParent() == this->getContainer()) {
//        this->getContainer()->removeChild(cell, true);;
//    }
}

