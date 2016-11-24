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
const char* FONT_NAME = "Arial";

TrystInputBar::~TrystInputBar() {}

TrystInputBar* TrystInputBar::create(CCObject *target, SEL_CallFunc callback) {
    TrystInputBar* rtn = new TrystInputBar();
    if (rtn && rtn->init(target, callback)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

void TrystInputBar::startInput(const char *text) {
    _inputCount = 0;
    _origText = text;
    _lbl->setString("");
    
    this->schedule(SEL_SCHEDULE(&TrystInputBar::gogogo), 0.3);
}


bool TrystInputBar::init(CCObject *target, SEL_CallFunc callback) {
    if (CCNode::create()) {
        _target = target;
        _callback = callback;
        
        CCSprite* inputBar = CCSprite::create("pic/tryst/tryst_input.png");
        this->addChild(inputBar);
        
        CCSprite* mask = CCSprite::create("pic/tryst/tryst_input_mask.png");
        mask->setAnchorPoint(CCPointZero);
        CCClippingNode* node = CCClippingNode::create(mask);
        node->setPosition(74.5, 10);
        inputBar->addChild(node);
        
        _savedInputboxWidth = mask->getContentSize().width;
        
//        CCLayerColor* test = CCLayerColor::create(ccc4(200, 200, 200, 200), 800, 200);
//        test->setAnchorPoint(ccp(0.5, 0.5));
//        node->addChild(test);
        
        _lbl = CCLabelTTF::create("", FONT_NAME, 28);
        _lbl->setAnchorPoint(ccp(0, 0.5));
        _lbl->setPosition(ccp(0, mask->getContentSize().height * 0.5));
        _lbl->setColor(ccBLACK);
        node->addChild(_lbl);
        
        return true;
    }
    else {
        return false;
    }
}


void TrystInputBar::gogogo() {
    _inputCount += 3;
    if (_inputCount > _origText.length()) {
        _lbl->setString("");
        _lbl->setAnchorPoint(ccp(0, 0.5));
        _lbl->setPosition(ccp(0, _lbl->getPositionY()));
        
        this->unscheduleAllSelectors();
        if (_target && _callback) {
            (_target->*_callback)();
        }
    }
    else {
        std::string strTemp = _origText.substr(0, _inputCount);
//        CCLOG("INPUT: %s", strTemp.c_str());
        _lbl->setString(strTemp.c_str());
        
        if (_lbl->getContentSize().width >= _savedInputboxWidth) {
            _lbl->setAnchorPoint(ccp(1, 0.5));
            _lbl->setPosition(ccp(_savedInputboxWidth, _lbl->getPositionY()));
        }
    }
}

// ------------------------------------------------------

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
    CCLayerColor* dialogDisplay = this->createDialogDisplay(isLeft, dialog);
    _dataSource->addObject(dialogDisplay);
    // 左对话，调用左对话显示方法
    if (isLeft == true) {
        this->prepareLeftDialog(dialogDisplay);
    }
    else { // 右对话，调用右对话显示方法
        this->prepareRightDialog(dialog);
    }
}

#pragma mark - Inner

bool TrystView::init(const char *id) {
    if (CCLayer::init()) {
        _dataSource = CCArray::create();
        _dataSource->retain();
        
        CCSprite* mask = CCSprite::create("pic/panel/dailysignin/ds_mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
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
        
        _otherInputPrompt = CCLabelTTF::create("对方正在输入", FONT_NAME, 30.f);
        _otherInputPrompt->setAnchorPoint(ccp(0, 0.5));
        _otherInputPrompt->setPosition(ccp(DISPLAY->halfW() - 100, DISPLAY->H() *0.95));
        this->addChild(_otherInputPrompt);
        _otherInputPrompt->setVisible(false);
        
        _inputBar = TrystInputBar::create(this, SEL_CallFunc(&TrystView::whenRightInputCompleted));
        _inputBar->setPosition(DISPLAY->halfW(), 37.5);
        this->addChild(_inputBar);
        
        return  true;
    }
    else {
        return false;
    }
}

void TrystView::onEnter() {
    CCLayer::onEnter();
    
    
//    this->schedule(SEL_SCHEDULE(&TrystView::updateOtherInputPrompt), 1.2f);
}

void TrystView::onExit() {
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

CCLayerColor* TrystView::createDialogDisplay(bool isLeft, const char *dialog) {
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
    
    float hPadding = 20.0;
    
    CCSize textSize;
    if (rowCount == 1) {
        textSize = CCSizeMake(charLength * LETTER_SIZE, 44);
    }
    else if (rowCount == 2) {
        textSize = CCSizeMake(ROW_LETTER_NUM * LETTER_SIZE, 60);
    }
    else {
        textSize = CCSizeMake(ROW_LETTER_NUM * LETTER_SIZE, rowCount * (LETTER_SIZE + 6));
    }
    
    CCLabelTTF* lblText = CCLabelTTF::create(dialog, FONT_NAME, LETTER_SIZE, textSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    lblText->setAnchorPoint(ccp(0, 0.5));
    
    CCLayerColor* rtn = NULL;
    if (isLeft == true) {
        rtn = CCLayerColor::create(ccc4(0, 0, 0, 0), TABLEVIEW_WIDTH, TABLEVIEW_HEIGH);
        
        CCSprite* avatar = CCSprite::create("pic/tryst/tryst_avatar_left.png");
        avatar->setPosition(ccp(avatar->getContentSize().width * 0.5, TABLEVIEW_HEIGH * 0.5));
        rtn->addChild(avatar);
        
        CCScale9Sprite* wrapper = NULL;
        if (rowCount == 1) {
            wrapper = CCScale9Sprite::create("pic/tryst/tryst_bubble_left_1.png", CCRectMake(0, 0, 51, 44));
        }
        else {
            wrapper = CCScale9Sprite::create("pic/tryst/tryst_bubble_left_2.png", CCRectMake(0, 0, 67, 60));
        }
        wrapper->setContentSize(CCSizeMake(textSize.width + hPadding, textSize.height));
        wrapper->setAnchorPoint(ccp(0, 0.5));
        wrapper->setPosition(avatar->getContentSize().width, TABLEVIEW_HEIGH * 0.5);
        rtn->addChild(wrapper);
        
        lblText->setPosition(ccp((hPadding - 6), wrapper->getContentSize().height * 0.5 - 1.5));
        lblText->setColor(ccBLACK);
        wrapper->addChild(lblText);
    }
    else {
        rtn = CCLayerColor::create(ccc4(0, 0, 0, 0), TABLEVIEW_WIDTH, TABLEVIEW_HEIGH);
        
        CCSprite* avatar = CCSprite::create("pic/tryst/tryst_avatar_right.png");
        avatar->setPosition(ccp(TABLEVIEW_WIDTH - avatar->getContentSize().width * 0.5, TABLEVIEW_HEIGH * 0.5));
        rtn->addChild(avatar);
        
        CCScale9Sprite* wrapper = NULL;
        if (rowCount == 1) {
            wrapper = CCScale9Sprite::create("pic/tryst/tryst_bubble_right_1.png", CCRectMake(0, 0, 51, 44));
        }
        else {
            wrapper = CCScale9Sprite::create("pic/tryst/tryst_bubble_right_2.png", CCRectMake(0, 0, 67, 60));
        }
        wrapper->setContentSize(CCSizeMake(textSize.width + hPadding, textSize.height));
        wrapper->setAnchorPoint(ccp(1, 0.5));
        wrapper->setPosition(TABLEVIEW_WIDTH - avatar->getContentSize().width - 3, TABLEVIEW_HEIGH * 0.5);
        rtn->addChild(wrapper);
        
        lblText->setPosition(ccp(6, wrapper->getContentSize().height * 0.5 - 1.5));
        lblText->setColor(ccBLACK);
        wrapper->addChild(lblText);
    }
    
    return rtn;
}

void TrystView::prepareLeftDialog(CCLayerColor *dialogDisplay) {
    // 第一句话直接显示
    if (_dataSource->count() == 1) {
        this->appearDialog(dialogDisplay);
    }
    else {
        CCCallFuncO* inputPrompt1 = CCCallFuncO::create(this, SEL_CallFuncO(&TrystView::updateOtherInputPrompt), ccs("对方正在输入。"));
        CCCallFuncO* inputPrompt2 = CCCallFuncO::create(this, SEL_CallFuncO(&TrystView::updateOtherInputPrompt), ccs("对方正在输入。。"));
        CCCallFuncO* inputPrompt3 = CCCallFuncO::create(this, SEL_CallFuncO(&TrystView::updateOtherInputPrompt), ccs("对方正在输入。。。"));
        CCCallFuncO* done = CCCallFuncO::create(this, SEL_CallFuncO(&TrystView::appearDialog), dialogDisplay);
        CCSequence* seq = CCSequence::create(CCShow::create(), inputPrompt1, CCDelayTime::create(0.8), inputPrompt2, CCDelayTime::create(0.8), inputPrompt3, CCDelayTime::create(0.8), CCHide::create(), done, NULL);
        _otherInputPrompt->runAction(seq);
    }
}

void TrystView::prepareRightDialog(const char* inputText) {
    // 输入框跑字。。。
    _inputBar->startInput(inputText);
}

void TrystView::appearDialog(CCLayerColor *dialogDisplay) {
    _dialogListView->reloadData();
    //
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

void TrystView::updateOtherInputPrompt(CCString* newText) {
    _otherInputPrompt->setString(newText->getCString());
}

void TrystView::whenRightInputCompleted() {
    CCLayerColor* dialogDisplay = dynamic_cast<CCLayerColor*>(_dataSource->lastObject());
    this->appearDialog(dialogDisplay);
}


#pragma mark - CCTableViewDataSource

CCSize TrystView::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
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

