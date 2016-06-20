//
//  NotePanel.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/20.
//
//

#ifndef NotePanel_hpp
#define NotePanel_hpp

#include <cocos2d.h>

USING_NS_CC;

class NotePanel: public CCLayer{
public:
    ~NotePanel();
    CREATE_FUNC(NotePanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
//    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
};

#endif /* NotePanel_hpp */
