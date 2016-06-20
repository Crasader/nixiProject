//
//  NotePanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/20.
//
//

#include "NotePanel.h"
#include "DisplayManager.h"

NotePanel::~NotePanel(){
    
}

bool NotePanel::init(){
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        CCSprite* note_panel = CCSprite::create("res/pic/haoyoupaihang/note_panel.png");
        note_panel->setPosition(DISPLAY->center());
        this->addChild(note_panel);
        
        CCString* title_str = CCString::createWithFormat("发给 %s 的纸条", "女总裁");
        CCLabelTTF* title = CCLabelTTF::create(title_str->getCString(), DISPLAY->font(), 33);
        title->setPosition(ccp(note_panel->getContentSize().width/2, note_panel->getContentSize().height*.9));
        note_panel->addChild(title);
        
        return true;
    }else{
        return false;
    }
}

void NotePanel::onEnter(){
    CCLayer::onEnter();
}

void NotePanel::onExit(){
    CCLayer::onExit();
}
