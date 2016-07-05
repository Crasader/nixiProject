//
//  Building.cpp
//  paperai
//
//  Created by mac on 16/7/3.
//
//

#include "Building.h"
#include "Floor.h"
#include "TaskScene.h"
#include "DisplayManager.h"

Building::~Building() {
    CC_SAFE_RELEASE_NULL(_floors);
}


Building* Building::create(int floorCount) {
    Building* rtn = new Building();
    if (rtn && rtn->init(floorCount)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool Building::init(int floorCount) {
    if (! CCLayer::init()) {
        return false;
    }
    
    _floorCount = floorCount;
    _floors = CCArray::create();
    _floors->retain();
    
    this->setTouchEnabled(true);
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    init_floors();
    
    return true;
}

void Building::onEnter() {
    CCLayer::onEnter();
    
    this->start();
}

void Building::onExit() {
    
    CCLayer::onExit();
}

bool Building::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint pos = pTouch->getLocation();
    CCRect rect = _scroll->boundingBox();
    if (! rect.containsPoint(pos)) {
        this->go_back();
        return true;
    }
    
    return false;
}

//void Building::scrollViewDidScroll(CCScrollView *view) {
//
//}
//
//void Building::scrollViewDidZoom(CCScrollView *view) {
//
//}

#pragma mark - Inner

void Building::init_floors() {
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    _scroll = CCScrollView::create(CCSizeMake(FLOOR_WIDTH, DISPLAY->H() * 0.85));
    _scroll->setPosition(ccp((DISPLAY->W() - FLOOR_WIDTH) * 0.5, DISPLAY->H() * (1 - 0.85) * 0.5));
    _scroll->setDirection(kCCScrollViewDirectionVertical);
//    _scroll->setContentSize(CCSizeMake(FLOOR_WIDTH, FLOOR_HEIGHT * MAX(5, _floorCount) + ROOF_HEIGHT));
    _scroll->setContentSize(CCSizeMake(FLOOR_WIDTH, FLOOR_HEIGHT * MAX(5, _floorCount) + ROOF_HEIGHT));
    this->addChild(_scroll);
    
    Floor* floor = Floor::create(_scroll, _floorCount, 1, FloorType_Reception, posWithFloor(1));
    _floors->addObject(floor);
    
    int officeCount = 0;
    if (_floorCount < 6) {
        officeCount = _floorCount - 2;
    }
    else {
        officeCount = _floorCount - 3;
    }
    //
    for (int i = 1; i <= officeCount; i++) {
        Floor* floor = Floor::create(_scroll, _floorCount, 1 + i, FloorType_Office, posWithFloor(1 + i));
        _floors->addObject(floor);
    }
    //
    if (_floorCount < 6) {
        Floor* floor = Floor::create(_scroll, _floorCount, _floorCount, FloorType_Manager_Office, posWithFloor(2 + officeCount));
        _floors->addObject(floor);
    }
    else {
        Floor* floor1 = Floor::create(_scroll, _floorCount, _floorCount - 1, FloorType_Leisure, posWithFloor(2 + officeCount));
        _floors->addObject(floor1);
        Floor* floor2 = Floor::create(_scroll, _floorCount, _floorCount, FloorType_Manager_Office, posWithFloor(3 + officeCount));
        _floors->addObject(floor2);
    }
    
    // roof
    CCString* strRoof = NULL;
    if (_floorCount == 3) {
        strRoof = CCString::createWithFormat("pic/building/roof_%d.png", 1);
    }
    else if (_floorCount == 4) {
        strRoof = CCString::createWithFormat("pic/building/roof_%d.png", 2);
    }
    else {
        strRoof = CCString::createWithFormat("pic/building/roof_%d.png", 3);
    }
    CCSprite* sptRoof = CCSprite::create(strRoof->getCString());
    sptRoof->setPosition(this->posWithFloor(_floorCount) + ccp(0, ROOF_HEIGHT));
    _scroll->addChild(sptRoof);
    
    CCScale9Sprite* sptPromptBar = CCScale9Sprite::create("pic/clothesScene/gj_dikuang1.png");
    sptPromptBar->setContentSize(CCSizeMake(300, 40));
    sptPromptBar->setPosition(ccp(ROOF_WIDTH * 0.5, 10));
    sptRoof->addChild(sptPromptBar);

    CCLabelTTF* label = CCLabelTTF::create("公司还差8颗星升级", DISPLAY->fangzhengFont(), 24.f);
    label->setColor(DISPLAY->defalutColor());
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setPosition(sptPromptBar->getPosition());
    sptRoof->addChild(label);
//    CCLayer* layer = CCLayer::create();
//    this->addChild(layer);
//    
//    CCSprite* goback1 = CCSprite::create("pic/qingjingScene/qj_fanhui.png");
//    CCSprite* goback2 = CCSprite::create("pic/qingjingScene/qj_fanhui.png");
//    goback2->setScale(1.01f);
//    CCMenuItem* btnGoBack = CCMenuItemSprite::create(goback1, goback2, this, SEL_MenuHandler(&Building::go_back));
//    btnGoBack->setPosition(ccp(50, 50));
//    layer->addChild(btnGoBack);
    CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
    txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.06));
    this->addChild(txt_close);
}

void Building::start() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_floors, pObj) {
        Floor* floor = (Floor*)pObj;
        floor->start();
    }
}

void Building::go_back() {
    this->removeFromParentAndCleanup(true);
}

CCPoint Building::posWithFloor(int floor) {
    return ccp(0, FLOOR_HEIGHT * (floor - 1));
}



