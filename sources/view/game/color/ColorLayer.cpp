//
//  ColorLayer.cpp
//  mm3c
//
//  Created by Sunrise on 15-1-13.
//
//

#include "ColorLayer.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "GameCheckoutPanel.h"
#include "Loading2.h"

#define LOST_LIMIT  5
#define GAME_ID     "2"

void ColorLayer::onEnter(){
    CCLayer::onEnter();
    this->setKeypadEnabled(true);
    
//    MMAudioManager::get_instance()->play_music(kMusic_BG_Lives, true);
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&ColorLayer::nc_commit_game_707), "HTTP_FINISHED_707", NULL);
}

void ColorLayer::onExit(){
    
    m_Colours->release();
    m_allWords->release();
    m_allColors->release();
    
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCLayer::onExit();
}

bool ColorLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->getChildByTag(0x121))
    {
        if (!animationBool) {
            animationBool = true;
//            this->removeChildByTag(0x121, true);
            this->scheduleOnce(SEL_SCHEDULE(&ColorLayer::starAnimation), .5f);
        }
    }
    if(this->getChildByTag(25))
    {
        return true;
    }

    return false;
}
bool ColorLayer::init(){
    
    if (!CCLayer::init()) {
        return false;
    }
    
    m_time_going= false;
    m_stage = 1;
    m_IsConform = false;
    m_speed = 8;
    m_num = 0;
    animationBool = false;
    m_time = 5;
    ciIndex = 0;
    m_wrong = 0;
    buttonBool = false;
    
    // background
    CCSprite * bg = CCSprite::create("res/pic/game/color/color_bg.png");
    bg->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bg);
    CCSprite * bg2 = CCSprite::create("res/pic/mask.png");
    bg2->setPosition(ccp(bg->getContentSize().width* .5f, bg->getContentSize().height* .5f));
    bg->addChild(bg2);
    
    _ManSpr = CCSprite::create();
    _ManSpr->setScale(1.f);
    this->addChild(_ManSpr, 1);
    
    
    this->parseDate(m_stage);
    this->initView();
    this->updateView();
    
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
//    this->setTouchEnabled(true);
    
    this->scheduleOnce(SEL_SCHEDULE(&ColorLayer::openTouch), 1.f);

    return true;
}
void ColorLayer::openTouch(float dt){
    this->setTouchEnabled(true);
}

void ColorLayer::initView(){
//    MZLog("m_time : %d",m_time);
    
    //人物
    this->creat_Man();
    
    //想象框
    tishikuang = CCSprite::create("res/pic/game/color/color_tishiban1.png");
    tishikuang->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    tishikuang->setVisible(false);
    _ManSpr->addChild(tishikuang, 100);
    CCSprite* tishikuang2 = CCSprite::create("res/pic/game/color/color_tishiban2.png");
    tishikuang2->setPosition(ccp(tishikuang->getContentSize().width* .5f, tishikuang->getContentSize().height* .5f));
    tishikuang->addChild(tishikuang2);
    
    m_word = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 100);
    m_word->setPosition(ccp(tishikuang->getContentSize().width* .5f, tishikuang->getContentSize().height* .5f));
    m_word->setVisible(false);
    tishikuang->addChild(m_word);
    
    //正确或者错误
    m_right = CCSprite::create("res/pic/game/color/color_right.png");
    m_right->setPosition(ccp(tishikuang->boundingBox().size.width* .85f, tishikuang->boundingBox().size.height* .2f));
    m_right->setVisible(false);
    tishikuang->addChild(m_right, 1);
    
    m_false = CCSprite::create("res/pic/game/color/color_wrong.png");
    m_false->setPosition(ccp(tishikuang->boundingBox().size.width* .85f, tishikuang->boundingBox().size.height* .2f));
    m_false->setVisible(false);
    tishikuang->addChild(m_false, 1);
    
    CCDictionary* scores = DATA->getHome()->getScores();
    _history = ((CCInteger*)scores->objectForKey(GAME_ID))->getValue();
    
    {
        //时间框
        CCSprite* timeSpr = CCSprite::create("res/pic/game/color/color_scorepanel.png");
        timeSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .85f));
        this->addChild(timeSpr, 1);
        
        //本次得分
        label_num = CCLabelTTF::create("0", DISPLAY->fangzhengFont(), 25);
        label_num->setPosition(ccp(timeSpr->getContentSize().width* .85f ,timeSpr->getContentSize().height* .25f));
        label_num->setColor(ccWHITE);
        timeSpr->addChild(label_num, 20);
        
        CCString* sco_str = CCString::createWithFormat("%d", _history);
        CCLabelTTF* label_score = CCLabelTTF::create(sco_str->getCString(), DISPLAY->fangzhengFont(), 25);
        label_score->setColor(ccWHITE);
        label_score->setPosition(ccp(timeSpr->getContentSize().width* .85f ,timeSpr->getContentSize().height* .72f));
        timeSpr->addChild(label_score, 20);
        
        CCString* wrongStr = CCString::createWithFormat("%d/%d", m_wrong, LOST_LIMIT);
        m_Worng_word = CCLabelTTF::create(wrongStr->getCString(), DISPLAY->fangzhengFont(), 30);
        m_Worng_word->setColor(ccWHITE);
        m_Worng_word->setPosition(ccp(timeSpr->getContentSize().width* .2f ,timeSpr->getContentSize().height* .27f));
        timeSpr->addChild(m_Worng_word, 20);
        
    }
    
    {//初始化想象图片
        //错误
        CCSprite* sprite_1 = CCSprite::create("res/pic/game/color/color_wrong.png");
        CCSprite* sprite_2 = CCSprite::create("res/pic/game/color/color_wrong.png");
        sprite_2->setScale(1.02f);
        
        CCMenuItemSprite* item1 = CCMenuItemSprite::create(sprite_1, sprite_2, this, menu_selector(ColorLayer::menu_callBack));
        item1->setAnchorPoint(ccp(.5f, .5f));
        item1->setPosition(ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .25f));
        item1->setTag(AH_LEFT_TAG + 1000);
        
        //正确
        CCSprite* sprite_3 = CCSprite::create("res/pic/game/color/color_right.png");
        CCSprite* sprite_4 = CCSprite::create("res/pic/game/color/color_right.png");
        sprite_4->setScale(1.02f);
        CCMenuItemSprite* item2 = CCMenuItemSprite::create(sprite_3, sprite_4, this, menu_selector(ColorLayer::menu_callBack));
        item2->setAnchorPoint(ccp(.5f, .5f));
        item2->setPosition(ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .25f));
        item2->setTag(AH_RIGHT_TAG + 1000);
        
        choose_menu = CCMenu::create(item1, item2, NULL);
        choose_menu->setPosition(CCPointZero);
        choose_menu->setEnabled(false);
        this->addChild(choose_menu, 10);
    }
    
    {// 加分的+1
        spr_add = CCSprite::create("res/pic/game/color/color_addone.png");
        spr_add->setPosition(ccp(tishikuang->getContentSize().width* .8f, tishikuang->getContentSize().height* .7f));
        spr_add->setVisible(false);
        tishikuang->addChild(spr_add, 50);
        
        spr_reduce = CCSprite::create("res/pic/game/color/color_reduceone.png");
        spr_reduce->setPosition(ccp(tishikuang->getContentSize().width* .8f, tishikuang->getContentSize().height* .7f));
        spr_reduce->setVisible(false);
        tishikuang->addChild(spr_reduce, 50);
    }

//    this->scheduleOnce(SEL_SCHEDULE(&ColorLayer::starAnimation), 1.5f);
    
//    {// 3 2 1 go + 提示
//        CCActionInterval * delaytime = CCDelayTime::create(0.f);
    CCSprite * tishi = CCSprite::create("res/pic/game/color/guide_prompt_plane.png");
    tishi->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .54f));
    this->addChild(tishi, 20, 0x121);
    CCSprite * tishi1 = CCSprite::create("res/pic/game/color/games_yd_color.png");
    tishi1->setPosition(ccp(tishi->getContentSize().width* .5f, tishi->getContentSize().height* .5f));
    tishi->addChild(tishi1,2);
    
    CCSprite* kSpr = CCSprite::create("res/pic/game/color/guide_kuang2.png");
    kSpr->setPosition(ccp(tishi->getContentSize().width* .65f, 0));
    tishi->addChild(kSpr, 3);
    
    CCLabelTTF* label = CCLabelTTF::create("点击任意处继续", DISPLAY->fangzhengFont(), 35);
    label->setPosition(ccp(kSpr->getContentSize().width* .5f, kSpr->getContentSize().height* .5f));
    label->setColor(ccc3(24, 21, 70));
    kSpr->addChild(label, 4);
        
}

void ColorLayer::starAnimation(){
    
    CCSprite* spr = CCSprite::create();
    spr->setTag(0x99999);
    this->addChild(spr, 100);
    
    CCSprite* renwuSpr1 = CCSprite::create("res/pic/game/color/a_star_s.png");
    renwuSpr1->setScale(.3f);
    renwuSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* .85f));
    spr->addChild(renwuSpr1);
    
    CCSprite* renwuSpr2 = CCSprite::create("res/pic/game/color/a_star_t.png");
    renwuSpr2->setScale(.3f);
    renwuSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* .9f));
    spr->addChild(renwuSpr2, 1);
    
    CCSprite* renwuSpr3 = CCSprite::create("res/pic/game/color/a_star_a.png");
    renwuSpr3->setScale(.3f);
    renwuSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .85f));
    spr->addChild(renwuSpr3, 1);
    
    CCSprite* renwuSpr4 = CCSprite::create("res/pic/game/color/a_star_r.png");
    renwuSpr4->setScale(.3f);
    renwuSpr4->setPosition(ccp(DISPLAY->ScreenWidth()* .6f, DISPLAY->ScreenHeight()* .9f));
    spr->addChild(renwuSpr4);
    
    CCSprite* renwuSpr5 = CCSprite::create("res/pic/game/color/a_star_t.png");
    renwuSpr5->setScale(.3f);
    renwuSpr5->setPosition(ccp(DISPLAY->ScreenWidth()* .7f, DISPLAY->ScreenHeight()* .85f));
    spr->addChild(renwuSpr5);
    
    float heightFolat = .75f;
    
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo3_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo4_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .6f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo5_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .7f, DISPLAY->ScreenHeight()* heightFolat));
    
    CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.f);
    CCScaleTo* scaleTo2 = CCScaleTo::create(.2f, 1.f);
    CCScaleTo* scaleTo3 = CCScaleTo::create(.2f, 1.f);
    CCScaleTo* scaleTo4 = CCScaleTo::create(.2f, 1.f);
    CCScaleTo* scaleTo5 = CCScaleTo::create(.2f, 1.f);
    
    CCSpawn* spawn1 = CCSpawn::create(moveTo1_1, scaleTo1, NULL);
    CCSpawn* spawn2 = CCSpawn::create(moveTo2_1, scaleTo2, NULL);
    CCSpawn* spawn3 = CCSpawn::create(moveTo3_1, scaleTo3, NULL);
    CCSpawn* spawn4 = CCSpawn::create(moveTo4_1, scaleTo4, NULL);
    CCSpawn* spawn5 = CCSpawn::create(moveTo5_1, scaleTo5, NULL);
    
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .45f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo3_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo4_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .55f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo5_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .6f, DISPLAY->ScreenHeight()* heightFolat));
    
    CCJumpTo* jumpTo1_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* heightFolat), 80.f, 1);
    CCJumpTo* jumpTo1_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* heightFolat), 40.f, 2);
    CCJumpTo* jumpTo2_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* heightFolat), 90.f, 2);
    CCJumpTo* jumpTo2_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .35f, DISPLAY->ScreenHeight()* heightFolat), 50.f, 1);
    CCJumpTo* jumpTo3_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* heightFolat), 90.f, 2);
    CCJumpTo* jumpTo3_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* heightFolat), 50.f, 1);
    CCJumpTo* jumpTo4_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .6f, DISPLAY->ScreenHeight()* heightFolat), 80.f, 1);
    CCJumpTo* jumpTo4_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .65f, DISPLAY->ScreenHeight()* heightFolat), 40.f, 2);
    CCJumpTo* jumpTo5_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .7f, DISPLAY->ScreenHeight()* heightFolat), 80.f, 1);
    CCJumpTo* jumpTo5_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* heightFolat), 40.f, 2);
    
    CCFadeOut * out1 = CCFadeOut::create(1.f);
    CCFadeOut * out2 = CCFadeOut::create(1.f);
    CCFadeOut * out3 = CCFadeOut::create(1.f);
    CCFadeOut * out4 = CCFadeOut::create(1.f);
    CCFadeOut * out5 = CCFadeOut::create(1.f);
    CCCallFuncN *remove =  CCCallFuncN::create(this, callfuncN_selector(ColorLayer::funCallback));
    
    CCSequence* seq1 = CCSequence::create(spawn1, CCDelayTime::create(.2f), moveTo1_2, CCDelayTime::create(.1f), jumpTo1_1, jumpTo1_2, out1, NULL);
    CCSequence* seq2 = CCSequence::create(spawn2, CCDelayTime::create(.2f), moveTo2_2, CCDelayTime::create(.1f), jumpTo2_1, jumpTo2_2, out2, NULL);
    CCSequence* seq3 = CCSequence::create(spawn3, CCDelayTime::create(.2f), moveTo3_2, CCDelayTime::create(.1f), jumpTo3_1, jumpTo3_2, out3, NULL);
    CCSequence* seq4 = CCSequence::create(spawn4, CCDelayTime::create(.2f), moveTo4_2, CCDelayTime::create(.1f), jumpTo4_1, jumpTo4_2, out4, NULL);
    CCSequence* seq5 = CCSequence::create(spawn5, CCDelayTime::create(.2f), moveTo5_2, CCDelayTime::create(.1f), jumpTo5_1, jumpTo5_2, out5, remove, NULL);
    
    renwuSpr1->runAction(seq1);
    renwuSpr2->runAction(seq2);
    renwuSpr3->runAction(seq3);
    renwuSpr4->runAction(seq4);
    renwuSpr5->runAction(seq5);
    
    // 播放倒计时
    if (!m_time_going) {
        m_time_going = true;
        //MMAudioManager::get_instance()->play_effect(kAudio_Read_Go, false);
        this->schedule(SEL_SCHEDULE(&ColorLayer::updateView), m_speed);
    }
}

void ColorLayer::creat_Man(){
    this->init_Clothes();
}
void ColorLayer::init_Clothes(){
    
}


void ColorLayer::parseDate(int type)
{
//    MZLog("m_time : %d",m_time);
    switch (type) {
        case 1:
            m_Colours = CCDictionary::createWithContentsOfFile(INFO_FILE1);
            m_Colours->retain();
            break;
        case 2:
            m_Colours = CCDictionary::createWithContentsOfFile(INFO_FILE2);
            m_Colours->retain();
            break;
        case 3:
            m_Colours = CCDictionary::createWithContentsOfFile(INFO_FILE3);
            m_Colours->retain();
            break;
        case 4:
            m_Colours = CCDictionary::createWithContentsOfFile(INFO_FILE4);
            m_Colours->retain();
            break;
        case 5:
            m_Colours = CCDictionary::createWithContentsOfFile(INFO_FILE5);
            m_Colours->retain();
            break;
            
        default:
            break;
    }
    
    
    m_allWords = m_Colours->allKeys();
    m_allWords->retain();
    
    m_allColors = CCArray::create();
    m_allColors->retain();
    
    CCObject * obj = NULL;
    CCARRAY_FOREACH(m_allWords, obj)
    {
        CCString * word = (CCString *)obj;
        CCArray * colur = (CCArray *)m_Colours->objectForKey(word->getCString());
        m_allColors->addObject(colur);
    }
}

void ColorLayer::Model()
{
    srand(time(NULL));
    CCRANDOM_0_1();
    CCRANDOM_0_1();
    CCString* tempWord = (CCString*)(m_allWords->randomObject());
    
    CCArray * CurrColor = (CCArray*)(m_Colours->objectForKey(tempWord->getCString()));
    m_CurrR = ((CCString*)CurrColor->objectAtIndex(0))->intValue();
    m_CurrG = ((CCString*)CurrColor->objectAtIndex(1))->intValue();
    m_CurrB = ((CCString*)CurrColor->objectAtIndex(2))->intValue();
    
    float randnum = CCRANDOM_0_1();
    if( randnum > 0.3){
        m_CurrWord = (CCString*)(m_allWords->randomObject());
    }else{
        m_CurrWord = tempWord;
    }
    
    if (m_CurrWord->isEqual(tempWord)) {
        m_IsConform = true;
    }else{
        m_IsConform = false;
    }
}

void ColorLayer::rule()
{
    // 进度条 时间
    if (m_num != ciIndex) {
        if( m_num <= 2 ){
            ciIndex = m_num;
            m_speed = m_speed / 2;
        }else if( m_num < 4 ){
            ciIndex = m_num;
            m_speed = m_speed - 0.3;
        }else if( m_num < 79 ){
            ciIndex = m_num;
            if( m_num % 20 == 0 )
            {
                m_speed -= 0.2;
            }
        }else if( m_num < 119 ){
            ciIndex = m_num;
            if( m_num % 20 == 0 )
            {
                m_speed -= 0.1;
            }
        }else{
            if (m_num >= 0) {
                ciIndex = m_num;
                if( m_num % 20 == 0 )
                {
                    m_speed -= 0.05;
                }
            }else{
                m_num = .05f;
            }
        }
    }
    
    // 颜色的数量
    if (m_num % 20 == 1)
    {
        parseDate(m_stage++);
    }
    // 30以后字体随机出现粗体
    if(m_num > 30)
    {
        float r = CCRANDOM_0_1();
        if( r > 0.1 ){
            m_word->setFontName(DISPLAY->fangzhengFont());
        }else{
            m_word->setFontName(DISPLAY->fangzhengFont());
        }
    }
}

void ColorLayer::updateView()
{
    this->unschedule(SEL_SCHEDULE(&ColorLayer::updateView));
    if (buttonBool) {
        buttonBool = false;
        
        this->Model();
        // 汉字 颜色
        ccColor3B yanse = {static_cast<GLubyte>(m_CurrR), static_cast<GLubyte>(m_CurrG), static_cast<GLubyte>(m_CurrB)};
        m_word->setString(m_CurrWord->getCString());
        m_word->setColor(yanse);
        
        if (m_time_going) {
            this->rule();
            
            CCProgressTo* progress1 = CCProgressTo::create(m_speed,100);
            m_word->runAction(CCSequence::create(progress1,NULL));
            
            this->schedule(SEL_SCHEDULE(&ColorLayer::updateView), m_speed);
        }
    }else{
        if (m_time_going) {
            m_wrong++;
        }
        
        CCActionInterval* action = CCFadeOut::create(.5f);
        
        CCCallFuncN *display2 =  CCCallFuncN::create(this, callfuncN_selector(ColorLayer::show_addone2));
        CCFiniteTimeAction* _actMove1 = CCMoveBy::create(.3f, ccp(0, 50));
        CCCallFuncN *setPos2 =  CCCallFuncN::create(this, callfuncN_selector(ColorLayer::setPosFunc2));
        CCFiniteTimeAction* _actMove3 = _actMove1->reverse();
        if (m_num > 0) {
            m_num--;
        }else{
            m_num = 0;
        }
        spr_reduce->runAction(CCSequence::create(display2, _actMove1, setPos2, _actMove3, NULL));
        m_false->setVisible(true);
        m_false->runAction(CCSequence::create(action, NULL));
        label_num->setString((CCString::createWithFormat("%d", m_num))->getCString());
        
        if (m_wrong >= LOST_LIMIT) {
            m_wrong = LOST_LIMIT;
            CCString* wrongStr = CCString::createWithFormat("%d/%d", m_wrong, LOST_LIMIT);
            m_Worng_word->setString(wrongStr->getCString());
            
            gameOver();
        }else{
            this->Model();
            // 汉字 颜色
            ccColor3B yanse = {static_cast<GLubyte>(m_CurrR), static_cast<GLubyte>(m_CurrG), static_cast<GLubyte>(m_CurrB)};
            m_word->setString(m_CurrWord->getCString());
            m_word->setColor(yanse);
            
            if (m_time_going) {
                this->rule();
                
                CCProgressTo* progress1 = CCProgressTo::create(m_speed,100);
                m_word->runAction(CCSequence::create(progress1,NULL));
                
                this->schedule(SEL_SCHEDULE(&ColorLayer::updateView), m_speed);
            }
            CCString* wrongStr = CCString::createWithFormat("%d/%d", m_wrong, LOST_LIMIT);
            m_Worng_word->setString(wrongStr->getCString());
        }
    }
}

void ColorLayer::menu_callBack(CCObject* pSender){
    CCActionInterval* action = CCFadeOut::create(.5f);
    
    CCCallFuncN *display1 =  CCCallFuncN::create(this, callfuncN_selector(ColorLayer::show_addone1));
    CCCallFuncN *display2 =  CCCallFuncN::create(this, callfuncN_selector(ColorLayer::show_addone2));
    CCFiniteTimeAction* _actMove1 = CCMoveBy::create(.3f, ccp(0, 50));
    CCCallFuncN *setPos1 =  CCCallFuncN::create(this, callfuncN_selector(ColorLayer::setPosFunc1));
    CCCallFuncN *setPos2 =  CCCallFuncN::create(this, callfuncN_selector(ColorLayer::setPosFunc2));
    CCFiniteTimeAction* _actMove3 = _actMove1->reverse();
    
    buttonBool = true;
    CCMenuItem* item = dynamic_cast<CCMenuItem*>(pSender);
    int tag = item->getTag() - 1000;
    if(tag == AH_LEFT_TAG)
    {
        //加精灵
//        MZLog("错误");
        
        if (m_IsConform == false) {
//            if (MMAudioManager::get_instance()->is_effect_on()) {
//                MMAudioManager::get_instance()->play_effect(kAudio_game_right, false);
//            }
            
            m_num ++;
            spr_add->runAction(CCSequence::create(display1, _actMove1, setPos1, _actMove3, NULL));
            m_right->setVisible(true);
            m_right->runAction(CCSequence::create(action, NULL));
        }else{
//            if (MMAudioManager::get_instance()->is_effect_on()) {
//                MMAudioManager::get_instance()->play_effect(kAudio_game_wrong, false);
//            }
            m_wrong++;
            if (m_num > 0) {
                m_num--;
            }else{
                m_num = 0;
            }
            spr_reduce->runAction(CCSequence::create(display2, _actMove1, setPos2, _actMove3, NULL));
            m_false->setVisible(true);
            m_false->runAction(CCSequence::create(action, NULL));
        }
    }else if(tag == AH_RIGHT_TAG)
    {
//        MZLog("正确");
        if ( m_IsConform ) {
//            if (MMAudioManager::get_instance()->is_effect_on()) {
//                MMAudioManager::get_instance()->play_effect(kAudio_game_right, false);
//            }
            
            m_num ++;
            spr_add->runAction(CCSequence::create(display1, _actMove1, setPos1, _actMove3, NULL));
            m_right->setVisible(true);
            m_right->runAction(CCSequence::create(action, NULL));
        }else{
//            if (MMAudioManager::get_instance()->is_effect_on()) {
//                MMAudioManager::get_instance()->play_effect(kAudio_game_wrong, false);
//            }
            m_wrong++;
            if (m_num > 0) {
                m_num--;
            }else{
                m_num = 0;
            }
            spr_reduce->runAction(CCSequence::create(display2, _actMove1, setPos2, _actMove3, NULL));
            m_false->setVisible(true);
            m_false->runAction(CCSequence::create(action, NULL));
        }
    }
    //tishikuang->removeChildByTag(spr_image->getTag());
    
    if (m_wrong >= LOST_LIMIT) {
        m_wrong = LOST_LIMIT;
        CCString* wrongStr = CCString::createWithFormat("%d/%d", m_wrong, LOST_LIMIT);
        m_Worng_word->setString(wrongStr->getCString());
        
        gameOver();
    }
    else {
        label_num->setString((CCString::createWithFormat("%d", m_num))->getCString());
        
        CCString* wrongStr = CCString::createWithFormat("%d/%d", m_wrong, LOST_LIMIT);
        m_Worng_word->setString(wrongStr->getCString());
        
        updateView();
    }
}

void ColorLayer::show_addone1(){
    spr_add->setVisible(true);
}
void ColorLayer::show_addone2(){
    spr_reduce->setVisible(true);
}
void ColorLayer::setPosFunc1(){
    spr_add->setVisible(false);
}
void ColorLayer::setPosFunc2(){
    spr_reduce->setVisible(false);
}
void ColorLayer::funCallback(CCNode* pSender){
    if(pSender && pSender->getParent()){
        pSender->removeFromParentAndCleanup(true);
    }
    if(this->getChildByTag(0x121))
    {
        this->removeChildByTag(0x121, true);
    }
    
    tishikuang->setVisible(true);
    m_word->setVisible(true);
    
    this->setTouchEnabled(false);
    //初始化想象图片
    choose_menu->setEnabled(true);
    
    this->schedule(schedule_selector(ColorLayer::updateTime), .1f);
}
void ColorLayer::updateTime(float dt){
//    m_time = m_time - .1f;
//    if (m_time == 0)
//    {
//        ciIndex++;
//        if (ciIndex == ) {
//            <#statements#>
//        }
//        this->updateView();
//    }
}

void ColorLayer::gameOver()
{
    this->unschedule(SEL_SCHEDULE(&ColorLayer::updateView));
    choose_menu->setEnabled(false);
    this->unscheduleAllSelectors();
    
    LOADING->show_loading();
    NET->commit_game_707(GAME_ID, m_num);
}

void ColorLayer::nc_commit_game_707(CCObject *pObj) {
    LOADING->remove();
    CCDictionary* first = (CCDictionary*)pObj;
    GameCheckoutPanel::show(this->getScene(), GAME_ID, m_num, _history, first);
}
