//
//  ColorLayer.h
//  mm3c
//
//  Created by Sunrise on 15-1-13.
//
//

#ifndef __mm3c__ColorLayer__
#define __mm3c__ColorLayer__

#include "cocos2d.h"

USING_NS_CC;

#define INFO_FILE1      "res/pic/game/color/colour1.plist"
#define INFO_FILE2      "res/pic/game/color/colour2.plist"
#define INFO_FILE3      "res/pic/game/color/colour3.plist"
#define INFO_FILE4      "res/pic/game/color/colour4.plist"
#define INFO_FILE5      "res/pic/game/color/colour5.plist"

typedef enum
{
    AH_LEFT_TAG  = 1,
    AH_RIGHT_TAG,
}AH_MENU_TAG;

typedef enum{
    Tag_COLOR_TouFa = 1,
    Tag_COLOR_WaiTao,
    Tag_COLOR_ShangYi,
    Tag_COLOR_KuZi,
    Tag_COLOR_WaZi,
    Tag_COLOR_XieZi,
    Tag_COLOR_ShiPin
}Color_Enum;

class ColorLayer
: public CCLayer
{
public:
    CREATE_FUNC(ColorLayer);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
//    virtual void registerWithTouchDispatcher();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
public:
    void openTouch(float dt);
    
    void initView();
    bool isHaveSaveFile();
    void menu_callBack(CCObject* pSender);
    void funCallback(CCNode* pSender);
    void updateTime(float dt);
    void parseDate(int stage);
    void Model();
    void updateView();
    void rule();
    void gameOver();
    void show_addone1();
    void show_addone2();
    void setPosFunc1();
    void setPosFunc2();
    
    void creat_Man();
    void init_Clothes();
    
    bool animationBool;
    void starAnimation();
    void nc_commit_game_707(CCObject* pObj);
    
public:
    int ciIndex;
    
    int             m_stage;
    CCDictionary * m_Colours;
    CCArray      * m_allWords;
    CCArray      * m_allColors;
    CCString * m_CurrWord;
    CCLabelTTF* m_word;
    CCLabelTTF* m_Worng_word;
    int             _history;
    
    int        m_CurrR;
    int        m_CurrG;
    int        m_CurrB;
    bool       m_IsConform;
    float      m_speed;
    int        m_num;
    int        m_wrong;
    bool       buttonBool;
    
    CCSprite*       spr_image;
    CCLabelTTF*     label_num;
    CCSprite*       m_right;
    CCSprite*       m_false;
    float           m_time;
    bool            m_time_going;
    CCMenu*         choose_menu;
    CCSprite*       tishikuang;
    CCSprite*       spr_add;
    CCSprite*       spr_reduce;
    
    CCSprite*       _ManSpr;
    CCSprite*       _wtSpr1; // 外套
    CCSprite*       _wtSpr2;
    CCSprite*       _wtSpr3;
    CCSprite*       _sySpr1; // 上衣
    CCSprite*       _sySpr2;
    CCSprite*       _sySpr3;
    CCSprite*       _kzSpr1; // 裤子
    CCSprite*       _kzSpr2;
    CCSprite*       _kzSpr3;
    CCSprite*       _wzSpr1; // 袜子
    CCSprite*       _wzSpr2;
    CCSprite*       _wzSpr3;
    CCSprite*       _xzSpr1; // 鞋子
    CCSprite*       _xzSpr2;
    CCSprite*       _xzSpr3;
    CCSprite*       _spSpr1; // 饰品
    CCSprite*       _spSpr2;
    CCSprite*       _spSpr3;
    
};
#endif /* defined(__mm3c__ColorLayer__) */
