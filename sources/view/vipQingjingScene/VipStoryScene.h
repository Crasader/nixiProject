//
//  VipStoryScene.h
//  mm3c
//
//  Created by mac on 14-12-20.
//
//

#ifndef __mm3c__VipStoryScene__
#define __mm3c__VipStoryScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
#include "VipDialogs.h"
#include "VipDialogItem.h"
#include "BaseScene.h"


USING_NS_CC;

#define inTime .3f

typedef enum{
    Tag_GJ_kuang = 0,
    Tag_GJ_bg,
    Tag_GJ_man1,
    Tag_GJ_head1,
    Tag_GJ_man2,
    Tag_GJ_head2,
    Tag_GJ_mingzi,
    Tag_GJ_label,
}GJ_Enum;

typedef enum{
    Tag_Menu_0 = 10,
    Tag_Menu_1,
    Tag_Menu_2,
    Tag_Label_0 = 20,
    Tag_Label_1,
    Tag_Label_2,
    Tag_Item_0 = 30,
    Tag_Item_1,
    Tag_Item_2,
    Tag_kuaijin = 40,
    Tag_zidong
}Story_Enum;

class VipStoryScene
: public BaseScene
{
public:
    VipStoryScene();
    ~VipStoryScene();
    
    virtual bool init();
    
    static CCScene* scene();
    static VipStoryScene* create_with_story_id(int _index);
    void init_with_story_id(int _index);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    void didAccelerate( CCAcceleration* pAccelerationValue);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    CREATE_FUNC(VipStoryScene);
    
public:
    
    int m_current_story_index_id;
    int index;
    int tagIndex;
    int logIndex;
    int nowIndex;
    int contentLength;
    int wordCount;
    int storyIndex;
    
    std::string manString1;
    std::string manString2;
    
    bool dikuangBool;
    
    bool oneBool;
    bool twoBool;
    
    bool openStory;
    bool buttonBool;
    bool buttonBool1;
    bool recordBool1;
    bool buttonBool2;
    bool recordBool2;
    bool buttonBool3;
    bool recordBool3;
    bool openButtonBool;
    bool m_bIsKJSelect;
    bool m_bIsZDSelect;
    bool _dikuangBool;
    
    bool manBool;
    bool passersbyBool1;
    bool passersbyBool2;
    bool quanBool;
    
    std::string labStr;
    std::string content;
    CCString* endingStr;
    std::string chengjiuStr;    
    
    CCSprite* _dkSpr;
    CCSprite* bgSpr;
    CCSprite* _bSpr;
    CCSprite* kuangSpr;
    
    CCSprite* manSpr1;
    CCSprite* headSpr1;
    CCSprite* manSpr2;
    CCSprite* headSpr2;
    bool passersbyBool;
    CCSprite* passersbyManSpr1;
    CCSprite* passersbyHeadSpr1;
    CCSprite* passersbyManSpr2;
    CCSprite* passersbyHeadSpr2;
    CCSprite* passersbyManSpr3;
    CCSprite* passersbyHeadSpr3;
    CCSprite* passersbyManSpr4;
    CCSprite* passersbyHeadSpr4;
    CCSprite* quanSpr;
    
    CCPoint playerVelocity;
    CCSpriteFrameCache* cache;
    
    
    VipDialogs* dialog;
    VipDialogItem* dialogItem;
    VipDialogItem* storyItem;
    
    CCSprite* nameKuang;
    CCLabelTTF* nameLab;
    CCLabelTTF* saidLabel;
    
    
    CCMenuItemToggle* kuaijinToggleItem;
    CCMenuItemToggle* zidongToggleItem;
    
        
public:
    
    void creatManEyesAnimation();
    void creatManBlinkEyes();
    void creatPassersbyEyesAnimation(int nameIndex);
    void toPassersbyEyesAnimation();
    void toPassersbyEyesAnimation1();
    void toPassersbyEyesAnimation2();
    void creatPassersbyBlinkEyes();
    void closeEyesAnimation();
    
    void creatBg();
    void creatMan(float dt);
    void gengxin(float dt);
    void addKuang(float dt);
    void addComplete();
    void outKuang();
    void logic(float dt);
    void emptyLabel();
    void getIndex(float dt);
    void openTouch(float dt);
    void addButton();
    void initButton();
    void callBack1();
    void callBack2();
    void callBack3();
    void buttonOut(float dt);
    void buttonAction(float dt);
    void removeButton();
    void removeMan();
    void recordLabel(VipDialogItem* dialItem);
    void creatPassersbyMan(float dt);
    void creatButton(int dex);
    void initLabel(int dex);
    void menuEffect(int dex);
    
    
    const char* getContent(){return content.c_str();}
    void init(VipDialogItem* item);
    std::string getContentByLength(int length);
    int getContentLength();
    
    
    void dialogueControl(VipDialogItem* dialItem);
    
    
    void fhCallBack(CCObject* pSender);
    void button1CallBack(CCObject* pSender);
    void button2CallBack(CCObject* pSender);
    void button3CallBack(CCObject* pSender);
    void goCallBackMethods();
    void callBackMethods(CCObject* pSender);
    void LabelColorFhCallBack(CCObject* pSender);
    
    void _503CallBack(CCObject* pSender);
};

#endif /* defined(__mm3c__VipStoryScene__) */
