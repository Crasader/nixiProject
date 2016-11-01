//
//  TrystView.cpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#include "TrystView.h"

#include "DisplayManager.h"

const float TABLEVIEW_WIDTH = 580.0;
const float TABLEVIEW_HEIGH = 130.0;
const float LETTER_SIZE = 20.0;
const int ROW_LETTER_NUM = 16;

CCLayerColor* TrystDialogDisplayFactory::createDialogDisplay(bool isLeft, const char *dialog) {
    int zhongwen = 0;
    int yingwen = 0;
    size_t i = 0, nlen = strlen(dialog);
    for (; i < nlen; i++) {
        if (dialog[i] >= 0 && dialog[i] <= 127) {
            yingwen++;
        }else{
            zhongwen++;
        }
    }
    CCLOG("zhongwen = %d, yingwen = %d", zhongwen, yingwen);
    
    int charLength = zhongwen /3 + yingwen;
    int rowCount = ceil((float(charLength) / ROW_LETTER_NUM));
    CCLOG("charLength = %d, rowCount = %d", charLength, rowCount);
    
    float hPadding = 20.0, vPadding = 10.0;
    
    CCSize textSize;
    if (rowCount == 1) {
        textSize = CCSizeMake(charLength * LETTER_SIZE, 60);
    }
    else if (rowCount == 2) {
        textSize = CCSizeMake(ROW_LETTER_NUM * LETTER_SIZE, 60);
    }
    else {
        textSize = CCSizeMake(ROW_LETTER_NUM * LETTER_SIZE, rowCount * (LETTER_SIZE + 6));
    }
    
    CCLabelTTF* lblText = CCLabelTTF::create(dialog, DISPLAY->fangzhengFont(), LETTER_SIZE, textSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    lblText->setAnchorPoint(CCPointZero);
    
    CCLayerColor* rtn = NULL;
    if (isLeft == true) {
        rtn = CCLayerColor::create(ccc4(0, 0, 0, 0), TABLEVIEW_WIDTH, TABLEVIEW_HEIGH);
        
        CCSprite* avatar = CCSprite::create("pic/tryst/tryst_avatar_left.png");
        avatar->setPosition(ccp(avatar->getContentSize().width * 0.5, TABLEVIEW_HEIGH * 0.5));
        rtn->addChild(avatar);
        
        CCScale9Sprite* wrapper = CCScale9Sprite::create("pic/tryst/tryst_bubble_left.png", CCRectMake(0, 0, 67, 60));
        wrapper->setContentSize(CCSizeMake(textSize.width + hPadding, textSize.height + vPadding));
        wrapper->setAnchorPoint(ccp(0, 0.5));
        wrapper->setPosition(avatar->getContentSize().width, TABLEVIEW_HEIGH * 0.5);
        rtn->addChild(wrapper);
        
        lblText->setPosition(ccp((hPadding - 6), 0));
        lblText->setColor(ccBLACK);
        wrapper->addChild(lblText);
    }
    else {
        rtn = CCLayerColor::create(ccc4(0, 0, 0, 0), TABLEVIEW_WIDTH, TABLEVIEW_HEIGH);
        
        CCSprite* avatar = CCSprite::create("pic/tryst/tryst_avatar_right.png");
        avatar->setPosition(ccp(TABLEVIEW_WIDTH - avatar->getContentSize().width * 0.5, TABLEVIEW_HEIGH * 0.5));
        rtn->addChild(avatar);
        
        CCScale9Sprite* wrapper = CCScale9Sprite::create("pic/tryst/tryst_bubble_right.png", CCRectMake(0, 0, 67, 60));
        wrapper->setContentSize(CCSizeMake(textSize.width + hPadding, textSize.height + vPadding));
        wrapper->setAnchorPoint(ccp(1, 0.5));
        wrapper->setPosition(TABLEVIEW_WIDTH - avatar->getContentSize().width - 3, TABLEVIEW_HEIGH * 0.5);
        rtn->addChild(wrapper);
        
        lblText->setPosition(ccp(6, 0));
        lblText->setColor(ccBLACK);
        wrapper->addChild(lblText);
    }

    return rtn;
}

// ------------------------------------------------------

#pragma mark - Export
TrystView::~TrystView() {
    CC_SAFE_DELETE(_dataSource);
}

TrystView* TrystView::create(const char *id) {
    TrystView* rtn = new TrystView();
    if (rtn && rtn->init(id)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

void TrystView::insertDialog(bool isLeft, const char *dialog) {
    CCLayerColor* dialogDisplay = TrystDialogDisplayFactory::createDialogDisplay(isLeft, dialog);
    _dataSource->addObject(dialogDisplay);
    _dialogListView->reloadData();
    this->appearDialog(dialogDisplay);
}


#pragma mark - Inner

bool TrystView::init(const char *id) {
    if (CCLayer::init()) {
        _dataSource = CCArray::create();
        _dataSource->retain();
        
        CCSprite* bg = CCSprite::create("pic/tryst/tryst_bg.png");
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
        
        _dialogListView = CCTableView::create(this, CCSizeMake(TABLEVIEW_WIDTH, 920));
        _dialogListView->setVerticalFillOrder(kCCTableViewFillTopDown);
        _dialogListView->setDirection(kCCScrollViewDirectionVertical);
        _dialogListView->ignoreAnchorPointForPosition(false);
        _dialogListView->setPosition(DISPLAY->center());
        _dialogListView->setTouchEnabled(false);
        this->addChild(_dialogListView);

        return  true;
    }
    else {
        return false;
    }
}

void TrystView::onEnter() {
    CCLayer::onEnter();

}

void TrystView::onExit() {
    
    CCLayer::onExit();
}

void TrystView::appearDialog(CCLayerColor *dialogDisplay) {
    CCPoint offset = _dialogListView->getContentOffset();
    CCSize size = _dialogListView->getContentSize();
    CCLOG("offset.y = %f", offset.y);
    CCLOG("size.height = %f\n", size.height);
    
    if (offset.y < 0) {
        _dialogListView->setContentOffset(ccp(offset.x, 0));
    }
    
    if (dialogDisplay != NULL) {
        
        CCCallFunc* done = CCCallFunc::create(this, SEL_CallFunc(&TrystView::whenDialogAppeared));
        CCSequence* seq = CCSequence::create(CCDelayTime::create(1), done, NULL);
        dialogDisplay->runAction(seq);
    }
}

void TrystView::whenDialogAppeared() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("TRYST_DIALOG_APPEARED");
}



#pragma mark - CCTableViewDataSource

CCSize TrystView::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
//    CCSize size;
//    int count = _dataSource->count();
//    if (idx < count) {
//        CCLayerColor* display = dynamic_cast<CCLayerColor*>(_dataSource->objectAtIndex(idx));
//        size = display->getContentSize();
//    }
//    else {
//        size = CCSizeMake(TABLEVIEW_WIDTH, 50);
//    }
//    
////    CCLOG("size.width = %f, size.height = %f.", size.width, size.height);
//    return size;
    return CCSizeMake(TABLEVIEW_WIDTH, TABLEVIEW_HEIGH);
}

CCTableViewCell* TrystView::tableCellAtIndex(CCTableView *table, unsigned int idx) {
//    CCLOG("TrystView::tableCellAtIndex() - idx = %d", idx);
    CCTableViewCell* cell = new CCTableViewCell();
    int count = _dataSource->count();
    if (idx < count) {
        CCLayerColor* display = dynamic_cast<CCLayerColor*>(_dataSource->objectAtIndex(idx));
        display->removeFromParent();
        cell->addChild(display);
    }
    
    return cell;
}

unsigned int TrystView::numberOfCellsInTableView(CCTableView *table) {
    return _dataSource->count();
}

