//
//  FloorCell.cpp
//  tiegao
//
//  Created by mac on 16/7/6.
//
//

#include "FloorCell.h"
#include "DisplayManager.h"
#include "SpecialManager.h"

const float ROLE_SCALE = 0.25;
const float STAND_HEIGHT = 3;

FloorCell::~FloorCell() {
    CC_SAFE_DELETE(_roles);
}

FloorCell* FloorCell::create(FloorCellType type, int phase, int idx) {
    FloorCell* rtn = new FloorCell();
    if (rtn && rtn->init(type, phase, idx)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool FloorCell::init(FloorCellType type, int phase, int idx) {
    _type = type;
    
    CCArray* arrRoles = CCArray::create();
    
    if (type == FloorCellType_Reception) {
        _sptFloor = CCSprite::create("pic/building/floor_reception.png");
        _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
        this->addChild(_sptFloor);
        
        CCSprite* role = CCSprite::create("pic/building/role_receptionist.png");
        role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
        role->setAnchorPoint(ccp(0.5, 0));
        role->setScale(ROLE_SCALE);
        arrRoles->addObject(role);
        
        CCSprite* take1 = CCSprite::create("res/pic/panel/mail/mail_btn_take.png");
        CCSprite* take2 = CCSprite::create("res/pic/panel/mail/mail_btn_take.png");
        take2->setScale(DISPLAY->btn_scale());
        CCMenuItemSprite* btn_take = CCMenuItemSprite::create(take1, take2, this, SEL_MenuHandler(&FloorCell::on_take_rewards));
        CCMenu* menuTake = CCMenu::createWithItem(btn_take);
        menuTake->setPosition(ccp(FLOOR_CELL_WIDTH * 0.9, 50));
        _sptFloor->addChild(menuTake);
    }
    else if (type == FloorCellType_Office) {
        _sptFloor = CCSprite::create("pic/building/floor_office_1.png");
        _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
        this->addChild(_sptFloor);
        
        if (idx == 1) {
            if (phase < 4) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 1);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
            else if (phase == 4) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 1);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
                
                CCString* strRole2 = CCString::createWithFormat("pic/building/role_staff_%d.png", 2);
                CCSprite* role2 = CCSprite::create(strRole2->getCString());
                role2->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role2->setAnchorPoint(ccp(0.5, 0));
                role2->setScale(ROLE_SCALE);
                arrRoles->addObject(role2);
            }
            else {
                for (int i = 1; i <= 3; i++) {
                    CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                    CCSprite* role = CCSprite::create(strRole->getCString());
                    role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                    role->setAnchorPoint(ccp(0.5, 0));
                    role->setScale(ROLE_SCALE);
                    arrRoles->addObject(role);
                }
            }
        }
        else if (idx == 2) {
            if (phase < 5) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 2);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
            else if (phase == 5) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 2);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
                
                CCString* strRole2 = CCString::createWithFormat("pic/building/role_staff_%d.png", 1);
                CCSprite* role2 = CCSprite::create(strRole2->getCString());
                role2->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role2->setAnchorPoint(ccp(0.5, 0));
                role2->setScale(ROLE_SCALE);
                arrRoles->addObject(role2);
            }
            else {
                for (int i = 1; i <= 3; i++) {
                    CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                    CCSprite* role = CCSprite::create(strRole->getCString());
                    role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                    role->setAnchorPoint(ccp(0.5, 0));
                    role->setScale(ROLE_SCALE);
                    arrRoles->addObject(role);
                }
            }
        }
        else if (idx == 3) {
            if (phase < 6) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 3);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
            else if (phase == 6) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 3);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
                
                CCString* strRole2 = CCString::createWithFormat("pic/building/role_staff_%d.png", 2);
                CCSprite* role2 = CCSprite::create(strRole2->getCString());
                role2->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role2->setAnchorPoint(ccp(0.5, 0));
                role2->setScale(ROLE_SCALE);
                arrRoles->addObject(role2);
            }
            else {
                for (int i = 1; i <= 3; i++) {
                    CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                    CCSprite* role = CCSprite::create(strRole->getCString());
                    role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                    role->setAnchorPoint(ccp(0.5, 0));
                    role->setScale(ROLE_SCALE);
                    arrRoles->addObject(role);
                }
            }
        }
        else if (idx == 4) {
            if (phase < 7) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 1);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
            else if (phase == 7) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 1);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
                
                CCString* strRole2 = CCString::createWithFormat("pic/building/role_staff_%d.png", 2);
                CCSprite* role2 = CCSprite::create(strRole2->getCString());
                role2->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role2->setAnchorPoint(ccp(0.5, 0));
                role2->setScale(ROLE_SCALE);
                arrRoles->addObject(role2);
            }
            else {
                for (int i = 1; i <= 3; i++) {
                    CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                    CCSprite* role = CCSprite::create(strRole->getCString());
                    role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                    role->setAnchorPoint(ccp(0.5, 0));
                    role->setScale(ROLE_SCALE);
                    arrRoles->addObject(role);
                }
            }
        }
        else if (idx == 5) {
            if (phase < 8) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 2);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
            else if (phase == 8) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 2);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
                
                CCString* strRole2 = CCString::createWithFormat("pic/building/role_staff_%d.png", 1);
                CCSprite* role2 = CCSprite::create(strRole2->getCString());
                role2->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role2->setAnchorPoint(ccp(0.5, 0));
                role2->setScale(ROLE_SCALE);
                arrRoles->addObject(role2);
            }
            else {
                for (int i = 1; i <= 3; i++) {
                    CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                    CCSprite* role = CCSprite::create(strRole->getCString());
                    role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                    role->setAnchorPoint(ccp(0.5, 0));
                    role->setScale(ROLE_SCALE);
                    arrRoles->addObject(role);
                }
            }
        }
        else {
            for (int i = 1; i <= 3; i++) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
        }
    }
    else  if (type == FloorCellType_Leisure) {
        if (phase < 5) { // == 4
            CCString* strRole = CCString::createWithFormat("pic/building/role_leisure_%d.png", 1);
            CCSprite* role = CCSprite::create(strRole->getCString());
            role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
            role->setAnchorPoint(ccp(0.5, 0));
            role->setScale(ROLE_SCALE);
            arrRoles->addObject(role);
        }
        else {
            for (int i = 1; i <= 2; i++) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_leisure_%d.png", i);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
        }
        _sptFloor = CCSprite::create("pic/building/floor_bar.png");
        _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
        this->addChild(_sptFloor);
    }
    else  if (type == FloorCellType_Manager_Office) {
        if (phase < 5) {
            _sptFloor = CCSprite::create("pic/building/floor_manager_1.png");
            _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
            this->addChild(_sptFloor);
        }
        else {
            _sptFloor = CCSprite::create("pic/building/floor_manager_2.png");
            _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
            this->addChild(_sptFloor);
        }
        
        CCSprite* role = CCSprite::create("pic/building/role_manager.png");
        role->setPosition(ccp(FLOOR_CELL_WIDTH * 0.3, STAND_HEIGHT));
        role->setAnchorPoint(ccp(0.5, 0));
        role->setScale(ROLE_SCALE);
        arrRoles->addObject(role);
    }
    else  if (type == FloorCellType_Roof) {
        if (phase == 1) {
            _sptFloor = CCSprite::create("pic/building/roof_1.png");
            _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
            this->addChild(_sptFloor);
        }
        else if (phase == 2) {
            _sptFloor = CCSprite::create("pic/building/roof_2.png");
            _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
            this->addChild(_sptFloor);
        }
        else {
            _sptFloor = CCSprite::create("pic/building/roof_3.png");
            _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
            this->addChild(_sptFloor);
        }
        
        CCScale9Sprite* sptPromptBar = CCScale9Sprite::create("pic/clothesScene/gj_dikuang1.png");
        sptPromptBar->setContentSize(CCSizeMake(300, 40));
        sptPromptBar->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, 10));
        _sptFloor->addChild(sptPromptBar);
        
        CCLabelTTF* label = CCLabelTTF::create("公司还差8颗星升级", DISPLAY->fangzhengFont(), 24.f);
        label->setColor(DISPLAY->defalutColor());
        label->setAnchorPoint(ccp(0.5, 0.5));
        label->setPosition(sptPromptBar->getPosition());
        _sptFloor->addChild(label);
    }
    
    _roles = arrRoles;
    _roles->retain();
    
    start();
    
    this->setTouchEnabled(true);
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);

    return true;
}

#pragma mark - Inner

void FloorCell::start() {
    float jumpDelta = 20.f;
    int jumpTimes = 10;
    int count = _roles->count();
    CCArray* temp = CCArray::createWithCapacity(count);
    for (int i = 0; i < count; i++) {
        temp->addObject(_roles->objectAtIndex(i));
    }
    
    while (temp->count() > 0) {
        CCObject* pObj = temp->randomObject();
        //            int idx = temp->indexOfObject(pObj);
        CCSprite* spt = (CCSprite*)pObj;
        _sptFloor->addChild(spt);
        CCJumpBy* move = CCJumpBy::create(randomDuration(), randomEdge(), jumpDelta, jumpTimes);
        
        if (CCRANDOM_0_1() > 0.5) {
            spt->setPosition(randomStartPos(true));
            CCSequence* seq = CCSequence::create(move, CCFlipX::create(true), move->reverse(), CCFlipX::create(false), NULL);
            spt->runAction(CCRepeatForever::create(seq));
        }
        else {
            spt->setPosition(randomStartPos(false));
            CCSequence* seq = CCSequence::create(CCFlipX::create(true), move->reverse(), CCFlipX::create(false), move, NULL);
            spt->runAction(CCRepeatForever::create(seq));
        }
        
        temp->removeObject(pObj);
    }
}

CCPoint FloorCell::randomStartPos(bool left) {
    float widthDelta = 50;
    if (left) {
        float ran = CCRANDOM_MINUS1_1() * widthDelta;
        CCLOG("ran = %f", ran);
        return CCPointMake(FLOOR_CELL_WIDTH * 0.2 + ran, STAND_HEIGHT);
    }
    else {
        float ran = CCRANDOM_MINUS1_1() * widthDelta;
        CCLOG("ran = %f", ran);
        return CCPointMake(FLOOR_CELL_WIDTH * 0.8 + ran, STAND_HEIGHT);
    }
}

float FloorCell::randomDuration() {
    return 10 + CCRANDOM_MINUS1_1() * 1.2;
}

CCPoint FloorCell::randomEdge() {
    float widthDelta = 50;
    return CCPointMake(FLOOR_CELL_WIDTH * 0.5 + CCRANDOM_MINUS1_1() * widthDelta, 0);
}

void FloorCell::on_take_rewards(CCMenuItem *btn) {
    CCLOG("BuildingView::on_take_rewards()");
}

void FloorCell::show_coin() {
    CCNode* role = (CCNode*)_roles->randomObject();
    if (role && role->getParent()) {
        show_coin_at(role->getPosition() + ccp(0, 84));
    }
}

void FloorCell::show_coin_at(CCPoint pos) {
    float statDuration = 0.3f;
    
    CCSprite* star1 = CCSprite::create("pic/loading/loading_star.png");
    star1->setPosition(pos + ccp(0, 20));
    star1->setScale(0.6);
    this->addChild(star1);
    star1->runAction(CCSequence::create(CCMoveBy::create(statDuration, ccp(-50, 20)), CCCallFuncN::create(this, SEL_CallFuncN(&FloorCell::self_remove)), NULL));
    
    CCSprite* star2 = CCSprite::create("pic/loading/loading_star.png");
    star2->setPosition(pos + ccp(0, 20));
    star2->setScale(0.4);
    this->addChild(star2);
    star2->runAction(CCSequence::create(CCMoveBy::create(statDuration, ccp(52, 25)), CCCallFuncN::create(this, SEL_CallFuncN(&FloorCell::self_remove)), NULL));
    
    CCSprite* coinSpr = CCSprite::create("pic/clothesScene/gj_coin.png");
    coinSpr->setPosition(pos);
    coinSpr->setScale(0.88);
    this->addChild(coinSpr);
    if (pos.x < FLOOR_CELL_WIDTH * 0.5) {
        CCSequence* seqCoin = CCSequence::create(CCJumpBy::create(0.5f, CCPoint(50, -80), 80, 1), CCOrbitCamera::create(0.2, 1, 0, 0, -180, 0, 0), NULL);
        coinSpr->runAction(seqCoin);
        
    }
    else {
        CCSequence* seqCoin = CCSequence::create(CCJumpBy::create(0.5f, CCPoint(-50, -80), 80, 1), CCOrbitCamera::create(0.2, 1, 0, 0, 360, 0, 0), NULL);
        coinSpr->runAction(seqCoin);
    }
}

void FloorCell::self_remove(CCNode *node) {
    node->removeFromParentAndCleanup(true);
}


