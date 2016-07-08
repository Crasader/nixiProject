//
//  Floor.cpp
//  paperai
//
//  Created by mac on 16/7/3.
//
//

#include "Floor.h"

const float ROLE_SCALE = 0.25;
const float STAND_HEIGHT = 3;

Floor::~Floor() {
    CC_SAFE_DELETE(_roles);
}

Floor* Floor::create(CCLayer* building, int floorCount, int curFloor, FloorType type, CCPoint pos) {
    Floor* rtn = new Floor();
    if (rtn && rtn->init(building, floorCount, curFloor, type, pos)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool Floor::init(CCLayer* building, int floorCount, int curFloor, FloorType type, CCPoint pos) {
    _type = type;
    
    CCArray* arrRoles = CCArray::create();
    
    if (type == FloorType_Reception) {
        _sptFloor = CCSprite::create("pic/building/floor_reception.png");
        _sptFloor->setPosition(pos);
        building->addChild(_sptFloor);
        
        CCSprite* role = CCSprite::create("pic/building/role_receptionist.png");
        role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
        role->setAnchorPoint(ccp(0.5, 0));
        role->setScale(ROLE_SCALE);
        arrRoles->addObject(role);
    }
    else if (type == FloorType_Office) {
        _sptFloor = CCSprite::create("pic/building/floor_office_1.png");
        _sptFloor->setPosition(pos);
        building->addChild(_sptFloor);
        
        if (curFloor == 2) {
            int count = 1;
            if (floorCount > 5) {
                count = MIN(floorCount - 5, 3);
            }
            if (count == 1) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 1);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
            else {
                for (int i = 1; i <= count; i++) {
                    CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                    CCSprite* role = CCSprite::create(strRole->getCString());
                    role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
                    role->setAnchorPoint(ccp(0.5, 0));
                    role->setScale(ROLE_SCALE);
                    arrRoles->addObject(role);
                }
            }
        }
        else if (curFloor == 3) {
            int count = 1;
            if (floorCount > 6) {
                count = MIN(floorCount - 6, 3);
            }
            if (count == 1) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 2);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
            else {
                for (int i = 1; i <= count; i++) {
                    CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                    CCSprite* role = CCSprite::create(strRole->getCString());
                    role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
                    role->setAnchorPoint(ccp(0.5, 0));
                    role->setScale(ROLE_SCALE);
                    arrRoles->addObject(role);
                }
            }
        }
        else if (curFloor == 4) {
            int count = 1;
            if (floorCount > 7) {
                count = MIN(floorCount - 7, 3);
            }
            if (count == 1) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", 3);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
            else {
                for (int i = 1; i <= count; i++) {
                    CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                    CCSprite* role = CCSprite::create(strRole->getCString());
                    role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
                    role->setAnchorPoint(ccp(0.5, 0));
                    role->setScale(ROLE_SCALE);
                    arrRoles->addObject(role);
                }
            }
        }
//        else if (curFloor == 5) {
//            int count = MIN(floorCount - 6, 3);
//            for (int i = 1; i <= count; i++) {
//                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
//                CCSprite* role = CCSprite::create(strRole->getCString());
//                role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
//                role->setAnchorPoint(ccp(0.5, 0));
//                role->setScale(ROLE_SCALE);
//                arrRoles->addObject(role);
//            }
//        }
        else {
            int count = MIN(floorCount - curFloor - 1, 3);
            for (int i = 1; i <= count; i++) {
                CCString* strRole = CCString::createWithFormat("pic/building/role_staff_%d.png", i);
                CCSprite* role = CCSprite::create(strRole->getCString());
                role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
                role->setAnchorPoint(ccp(0.5, 0));
                role->setScale(ROLE_SCALE);
                arrRoles->addObject(role);
            }
        }
    }
    else if (type == FloorType_Manager_Office) {
        if (floorCount < 5) {
            _sptFloor = CCSprite::create("pic/building/floor_manager_1.png");
            _sptFloor->setPosition(pos);
            building->addChild(_sptFloor);
            
            CCSprite* role = CCSprite::create("pic/building/role_manager.png");
            role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
            role->setAnchorPoint(ccp(0.5, 0));
            role->setScale(ROLE_SCALE);
            arrRoles->addObject(role);
        }
        else {
            _sptFloor = CCSprite::create("pic/building/floor_manager_2.png");
            _sptFloor->setPosition(pos);
            building->addChild(_sptFloor);
            
            CCSprite* role = CCSprite::create("pic/building/role_manager.png");
            role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
            role->setAnchorPoint(ccp(0.5, 0));
            role->setScale(ROLE_SCALE);
            arrRoles->addObject(role);
        }
    }
    else if (type == FloorType_Leisure) {
        _sptFloor = CCSprite::create("pic/building/floor_bar.png");
        _sptFloor->setPosition(pos);
        building->addChild(_sptFloor);
        
        int count = MIN(floorCount - 5, 2);
        for (int i = 1; i <= count; i++) {
            CCString* strRole = CCString::createWithFormat("pic/building/role_leisure_%d.png", i);
            CCSprite* role = CCSprite::create(strRole->getCString());
            role->setAnchorPoint(ccp(0.5, 0));
            role->setPosition(ccp(FLOOR_WIDTH * 0.3, STAND_HEIGHT));
            role->setScale(ROLE_SCALE);
            arrRoles->addObject(role);
        }
    }
    
    _roles = arrRoles;
    _roles->retain();

    return true;
}

#pragma mark - Inner

void Floor::start() {
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
//    else {
//        CCSprite* spt = (CCSprite*)_roles->objectAtIndex(0);
//        CCJumpBy* move = CCJumpBy::create(randomDuration(), randomEdge(), jumpDelta, jumpTimes);
//        spt->setPosition(randomStartPos(true));
//        CCSequence* seq = CCSequence::create(move, CCFlipX::create(true), move->reverse(), CCFlipX::create(false), NULL);
//        spt->runAction(CCRepeatForever::create(seq));
//    }
    /*
    for (int i = 0; i < count; i++) {
        CCSprite* spt = (CCSprite*)_roles->objectAtIndex(i);
        CCJumpBy* move = CCJumpBy::create(randomDuration(), randomEdge(), jumpDelta, jumpTimes);
        if (_type % 2 == 0 && i % 2 == 0) {
            spt->setPosition(randomStartPos(true));
            CCSequence* seq = CCSequence::create(move, CCFlipX::create(true), move->reverse(), CCFlipX::create(false), NULL);
            spt->runAction(CCRepeatForever::create(seq));
        }
        else {
            spt->setPosition(randomStartPos(false));
            CCSequence* seq = CCSequence::create(CCFlipX::create(true), move->reverse(), CCFlipX::create(false), move, NULL);
            spt->runAction(CCRepeatForever::create(seq));
        }
    }
    */
}

CCPoint Floor::randomStartPos(bool left) {
    float widthDelta = 50;
    if (left) {
        float ran = CCRANDOM_MINUS1_1() * widthDelta;
        CCLOG("ran = %f", ran);
        return CCPointMake(FLOOR_WIDTH * 0.2 + ran, STAND_HEIGHT);
    }
    else {
        float ran = CCRANDOM_MINUS1_1() * widthDelta;
        CCLOG("ran = %f", ran);
        return CCPointMake(FLOOR_WIDTH * 0.8 + ran, STAND_HEIGHT);
    }
}

float Floor::randomDuration() {
    return 10 + CCRANDOM_MINUS1_1() * 1.2;
}

CCPoint Floor::randomEdge() {
    float widthDelta = 50;
    return CCPointMake(FLOOR_WIDTH * 0.5 + CCRANDOM_MINUS1_1() * widthDelta, 0);
}





