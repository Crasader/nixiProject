//
//  DailyWelfareLayer.cpp
//  tiegao
//
//  Created by mac on 16/12/27.
//
//

#include "DailyWelfareLayer.h"

#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "AudioManager.h"

#include "WelfareComp.h"
#include "WelfareCell.h"

#include "Loading2.h"

const float CELL_WIDTH = 470;
const float CELL_HEIGHT = 110;

const int HEADER_START_ZORDER = 10;

#pragma mark - Export

void DailyWelfareLayer::show() {
    DailyWelfareLayer* panel = DailyWelfareLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}

CCRect DailyWelfareLayer::getBoundary() {
    return _panel->boundingBox();
}

#pragma mark - Supper

DailyWelfareLayer::~DailyWelfareLayer() {
}

bool DailyWelfareLayer::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        _dataSource = DATA->getWelfare();
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("res/pic/welfare/white_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);

        //
        this->init_header();
        
        // tableview
        float panelW = CELL_WIDTH + 6;
        float panelH = 524;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->ignoreAnchorPointForPosition(false);
        _tv->setAnchorPoint(ccp(0.5, 1));
        _tv->setPosition(ccp(DISPLAY->halfW(), DISPLAY->halfH() + 200));
        _tv->setDelegate(this);
        this->addChild(_tv);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(false);
        
        return true;
    }
    else {
        return false;
    }
}

void DailyWelfareLayer::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&DailyWelfareLayer::nc_take_welfare_item_reward_631), "HTTP_FINISHED_631", NULL);
    nc->addObserver(this, SEL_CallFuncO(&DailyWelfareLayer::nc_take_welfare_statis_reward_633), "HTTP_FINISHED_633", NULL);
}

void DailyWelfareLayer::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool DailyWelfareLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCLOG("DailyWelfareLayer::ccTouchBegan");
    int count = _boxes->count();
    for (int i = 0; i < count; i++) {
        CCSprite* spt = (CCSprite*)_boxes->objectAtIndex(i);
        CCRect rect = spt->boundingBox();
        CCPoint pos = spt->getParent()->convertTouchToNodeSpace(pTouch);
        if (rect.containsPoint(pos)) {
            CCLOG("Touched box index = %d", i);
            CCDictionary* data = (CCDictionary* )spt->getUserObject();
            if (data) {
                CCString* id = (CCString* )data->objectForKey("id");
                CCBool* enabled = (CCBool* )data->objectForKey("enabled");
                if (enabled) {
                    CCLOG("YES");
                    LOADING->show_loading();
                    NET->take_welfare_total_reward_633(id->getCString());
                }
            }
            
            break;
        }
    }
    
    return true;
}

#pragma mark - inner

void DailyWelfareLayer::remove() {
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_REMOVE");
}

void DailyWelfareLayer::init_header() {
    CCSprite* title = CCSprite::create("res/pic/welfare/daily_title.png");
    title->setPosition(ccp(_panel->getContentSize().width * 0.5, _panel->getContentSize().height * 0.91));
    _panel->addChild(title);
    
    CCSprite* totalBarBottom = CCSprite::create("pic/welfare/welfare_total_bar_1.png");
    totalBarBottom->setPosition(ccp(_panel->getContentSize().width * 0.48, _panel->getContentSize().height * 0.85));
    _panel->addChild(totalBarBottom);
    
    CCSize barSize = totalBarBottom->getContentSize();
    
    _totalProgress = CCProgressTimer::create(CCSprite::create("pic/welfare/welfare_total_bar_2.png"));
    _totalProgress->setPosition(totalBarBottom->getPosition());
    _totalProgress->setType(kCCProgressTimerTypeBar);
    _totalProgress->setMidpoint(ccp(0, 0.5));
    _totalProgress->setBarChangeRate(ccp(1, 0));
    _panel->addChild(_totalProgress);
    
    this->update_statis_rewards();
    
    int totalProgress = _dataSource->obtainTotalProgress();
    WelfareStatisItem* item3 = _dataSource->fetchStatisItem(2);
    float duration = 1.f;
    float per = MIN(totalProgress * 0.1 / item3->goal * 10, 1.0);
    
    _totalProgress->runAction(CCProgressFromTo::create(duration, 0, per * 100));
    
    CCSprite* star = CCSprite::create("pic/welfare/welfare_star2.png");
    _totalProgress->addChild(star, HEADER_START_ZORDER);
    CCSize progressSize = _totalProgress->getContentSize();
    float toX = progressSize.width * per;
    
    CCSequence* starSeq = CCSequence::create(CCPlace::create(ccp(0, progressSize.height * 0.5)), CCMoveTo::create(duration, ccp(toX, progressSize.height * 0.5)), NULL);
    star->runAction(starSeq);
}

void DailyWelfareLayer::update_statis_rewards() {
    _totalProgress->removeAllChildrenWithCleanup(true);
    if (_boxes) {
        CC_SAFE_DELETE(_boxes);
    }
    
    _boxes = CCArray::createWithCapacity(3);
    CC_SAFE_RETAIN(_boxes);
    
    WelfareStatisItem* item3 = _dataSource->fetchStatisItem(2);
    float endGoal = item3->goal * 10 * 0.1;
    CCSize barSize = _totalProgress->getContentSize();
    float baseHeight = barSize.height * 0.5 + 5;
    
    for (int i = 0; i < 3; i++) {
        WelfareStatisItem* item = _dataSource->fetchStatisItem(i);
        float precent = item->goal / endGoal;
        
        CCString* boxName = NULL;
        CCSprite* box = NULL;
        
        if (item->status == 0) {
            CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
            box = CCSprite::create(boxName->getCString());
            box->setPosition(ccp(barSize.width * precent, baseHeight));
            _totalProgress->addChild(box, HEADER_START_ZORDER-1);
            
            CCDictionary* data = CCDictionary::create();
            data->setObject(ccs(item->id), "id");
            data->setObject(CCBool::create(false), "enabled");
            box->setUserObject(data);
        }
        else if (item->status == 1) {
            //            CCString* lightName = CCString::createWithFormat("pic/building/progress/available_light_%d.png", i);
            //            CCSprite* light = CCSprite::create(lightName->getCString());
            //            light->setPosition(ccp(barSize.width * precent, baseHeight));
            //            _totalProgress->addChild(light);
            //
            //            CCSequence* seq = CCSequence::create(CCFadeOut::create(0.6), CCDelayTime::create(0.5), CCFadeIn::create(0.6), CCDelayTime::create(0.3), NULL);
            //            light->runAction(CCRepeatForever::create(seq));
            //
            //
            //            CCString* spotName = CCString::createWithFormat("pic/building/progress/available_spot_%d.png", i);
            //            CCSprite* spot = CCSprite::create(spotName->getCString());
            //            spot->setPosition(ccp(light->getContentSize().width * 0.5, light->getContentSize().height * 0.7));
            //            light->addChild(spot);
            //
            //            CCSequence* seq2 = CCSequence::create(CCFadeOut::create(0.6), CCDelayTime::create(0.5), CCFadeIn::create(0.6), CCDelayTime::create(0.3), NULL);
            //            spot->runAction(CCRepeatForever::create(seq2));
            //
            //
            //            CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
            //            box = CCSprite::create(boxName->getCString());
            //            box->setPosition(light->getPosition());
            //            _totalProgress->addChild(box);
            
            CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
            box = CCSprite::create(boxName->getCString());
            box->setPosition(ccp(barSize.width * precent, baseHeight));
            _totalProgress->addChild(box, HEADER_START_ZORDER-1);
            
            CCRect spt_rect = box->getTextureRect();
            
            CCArray* arrFrame = CCArray::createWithCapacity(2);
            CCSpriteFrame* frame1 = CCSpriteFrame::create(boxName->getCString(), spt_rect);
            arrFrame->addObject(frame1);
            
            CCString* boxName2 = CCString::createWithFormat("pic/building/progress/pack_%d_taken.png", i);
            CCSpriteFrame* frame2 = CCSpriteFrame::create(boxName2->getCString(), spt_rect);
            arrFrame->addObject(frame2);
            
            CCAnimation* animation = CCAnimation::createWithSpriteFrames(arrFrame, 0.4);
            box->runAction(CCRepeatForever::create( CCAnimate::create(animation)) );
            
            
            CCDictionary* data = CCDictionary::create();
            data->setObject(ccs(item->id), "id");
            data->setObject(CCBool::create(false), "enabled");
            box->setUserObject(data);
        }
        else {
            boxName = CCString::createWithFormat("pic/building/progress/pack_%d_taken.png", i);
            box = CCSprite::create(boxName->getCString());
            box->setPosition(ccp(barSize.width * precent, baseHeight));
            box->setUserObject(CCBool::create(false));
            _totalProgress->addChild(box, HEADER_START_ZORDER-1);
            
            CCDictionary* data = CCDictionary::create();
            data->setObject(ccs(item->id), "id");
            data->setObject(CCBool::create(false), "enabled");
            box->setUserObject(data);
        }
        
        if (box) {
            _boxes->addObject(box);
            
            CCNode* reward = this->createStatisRewardContent(item->rewardType, item->rewardNum);
            reward->setPosition(box->getPosition() + ccp(0, -15));
            if (reward) {
                _totalProgress->addChild(reward, HEADER_START_ZORDER+1);
            }
        }
        
        CCSprite* goalSpt = CCSprite::create("res/pic/welfare/welfare_goal_plate.png");
        goalSpt->setPosition(box->getPosition() + ccp(0, -45));
        _totalProgress->addChild(goalSpt);
        
        CCString* strGoal = CCString::createWithFormat("%d", item->goal);
        CCLabelTTF* lblGoal = CCLabelTTF::create(strGoal->getCString(), DISPLAY->fangzhengFont(), 14);
        lblGoal->setPosition(ccp(goalSpt->getContentSize().width * 0.5, goalSpt->getContentSize().height * 0.5));
        goalSpt->addChild(lblGoal);
    }
}

CCNode* DailyWelfareLayer::createStatisRewardContent(string& type, int num) {
    // 奖励内容
    CCSprite* plate = CCSprite::create("pic/building/reward_plate.png");
    CCSize plateSize = plate->getContentSize();
    plate->setPosition(ccp(plateSize.width * 0.5 - 10, 22));
    
    CCString* strReward = CCString::createWithFormat("%d", num);
    CCLabelTTF* lblReward = CCLabelTTF::create(strReward->getCString(), DISPLAY->fangzhengFont(), 18.0f);
    lblReward->setAnchorPoint(ccp(1, 0.5));
    lblReward->setPosition(ccp(plateSize.width * 0.6, plateSize.height * 0.5 - 1));
    lblReward->setColor(ccc3(127, 76, 76));
    plate->addChild(lblReward);
    
    if (type.compare("coin") == 0) {
        CCSprite* icon = CCSprite::create("pic/building/reward_coin.png");
        icon->setPosition(ccp(lblReward->getPositionX() + 10, lblReward->getPositionY()));
        plate->addChild(icon);
    }
    else if (type.compare("diam") == 0) {
        CCSprite* icon = CCSprite::create("pic/building/reward_diam.png");
        icon->setPosition(ccp(lblReward->getPositionX() + 10, lblReward->getPositionY()));
        plate->addChild(icon);
    }
    else if (type.compare("piece") == 0) {
        CCSprite* icon = CCSprite::create("pic/building/reward_piece.png");
        icon->setPosition(ccp(lblReward->getPositionX() + 10, lblReward->getPositionY()));
        plate->addChild(icon);
    }
    
    return plate;
}

void DailyWelfareLayer::nc_take_welfare_item_reward_631(CCObject* pObj) {
    LOADING->remove();
    CCString* id = (CCString* )pObj;
    WelfareItem* item = _dataSource->fetchWelfareItemWithId(id->getCString());
    if (item >= 0) {
        CCTableViewCell* cell = (CCTableViewCell* )_tv->cellAtIndex(item->getIdx());
        WelfareCell* itemCell = (WelfareCell* )cell->getChildByTag(1);
        if (itemCell) {
            itemCell->showRewardAction(item);
        }
        
        this->disappear_cell(itemCell);
    }
}

void DailyWelfareLayer::nc_take_welfare_statis_reward_633(CCObject *pObj) {
    LOADING->remove();
    
    CCString* id = (CCString* )pObj;
    WelfareStatisItem* item = _dataSource->fetchStatisItemWithId(id->getCString());
    this->showRewardTake(item);
    
    this->update_statis_rewards();
}

void DailyWelfareLayer::showRewardTake(WelfareStatisItem *item) {
    int count = _boxes->count();
    for (int i = 0; i < count; i++) {
        CCSprite* spt = (CCSprite*)_boxes->objectAtIndex(i);
        CCDictionary* data = (CCDictionary* )spt->getUserObject();
        if (data) {
            CCString* id = (CCString* )data->objectForKey("id");
            if (id->compare(item->id.c_str()) == 0) {
                CCDictionary* postData = CCDictionary::create();
                postData->setObject(CCInteger::create( item->rewardNum ), "num");
                CCPoint pos = spt->getPosition();
                CCPoint worldPos = spt->getParent()->convertToWorldSpace(pos);
                CCString* from = CCString::createWithFormat("{%f,%f}", worldPos.x, worldPos.y);
                CCLOG("from -- %s", from->getCString());
                postData->setObject(from, "from");
                
                if (item->rewardType.compare("coin") == 0) {
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_COIN_FLY", postData);
                }
                else if (item->rewardType.compare("diam") == 0) {
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_GOLD_FLY", postData);
                }
                else if (item->rewardType.compare("energy") == 0) {
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_ENERGY_FLY", postData);
                }
                else if (item->rewardType.compare("piece") == 0) {
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_PIECE_FLY", postData);
                }
                
                break;
            }
        }
    }
}

void DailyWelfareLayer::disappear_cell(WelfareCell* node) {
    CCCallFunc* finished = CCCallFunc::create(this, SEL_CallFunc(&DailyWelfareLayer::disappear_finished));
    
    CCSequence* seq = CCSequence::create(CCEaseBackIn::create(CCScaleTo::create(0.7, 0.1)), finished, NULL);
    node->runAction(seq);
}

void DailyWelfareLayer::disappear_finished() {
    _tv->reloadData();
}

#pragma mark - CCTableViewDataSource

CCSize DailyWelfareLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return this->cellSizeForTable(table);
}

CCSize DailyWelfareLayer::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCTableViewCell* DailyWelfareLayer::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell* cell = new CCTableViewCell();
    WelfareItem* item = _dataSource->fetchWelfareItem(idx);
    item->setIdx(idx);
    
    WelfareCell* itemCell = WelfareCell::create("pic/welfare/welfare_plane.png");
    itemCell->configWithWelfareItem((int)idx, item, CELL_WIDTH, CELL_HEIGHT);
    itemCell->setAnchorPoint(ccp(0.5f, 0.5f));
    itemCell->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    itemCell->setTag(1);
    cell->addChild(itemCell);
    
    return cell;
}

unsigned int DailyWelfareLayer::numberOfCellsInTableView(CCTableView *table) {
    return _dataSource->itemCount();
}

#pragma mark - CCTableViewDelegate

void DailyWelfareLayer::tableCellTouched(CCTableView *table, CCTableViewCell *cell) {
    //    CCNode* node = cell->getChildByTag(123);
    //    int idx = cell->getIdx();
    //    node->runAction(CCSequence::create(CCScaleTo::create(0.08, 0.9), CCScaleTo::create(0.06, 1.1), CCScaleTo::create(0.08, 0.95), CCScaleTo::create(0.06, 1.0), NULL));
    
    //    WelfareCell* itemCell = (WelfareCell* )cell->getChildByTag(1);
    //    this->disappear_cell(itemCell);
}

void DailyWelfareLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {
    CCPoint contOffsetPos = view->getContentOffset();
    if (this->numberOfCellsInTableView(_tv) > 5) {
        //        _tv->setBounceable(true);
        if (contOffsetPos.y < view->minContainerOffset().y) {
            view->setContentOffset(CCPoint(contOffsetPos.x, view->minContainerOffset().y));
        }else if (contOffsetPos.y > view->maxContainerOffset().y){
            view->setContentOffset(CCPoint(contOffsetPos.x, view->maxContainerOffset().y));
        }
    }
}

void DailyWelfareLayer::scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {
    
}


