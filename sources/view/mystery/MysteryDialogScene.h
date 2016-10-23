//
//  MysteryDialogScene.h
//  tiegao
//
//  Created by mac on 16-7-8.
//
//

#ifndef __tiegao__MysteryDialogScene__
#define __tiegao__MysteryDialogScene__

#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

#define def_man1_oder  20
#define def_man2_oder  19
#define def_biaoqing_oder  30

typedef enum{
    Tag_Task_kuang = 0,
    Tag_Task_bg,
    Tag_Task_man1,
    Tag_Task_head1,
    Tag_Task_man2,
    Tag_Task_head2,
    Tag_Task_mingzi,
    Tag_Task_label,
}Task_Enum;

typedef enum{
    Tag_TS_TouFa = 1,
    Tag_TS_WaiTao,
    Tag_TS_ShangYi,
    Tag_TS_KuZi,
    Tag_TS_WaZi,
    Tag_TS_XieZi,
    Tag_TS_ShiPin,
    Tag_TS_Bao,
    Tag_TS_ZhuangRong,
    Tag_TS_TeXiao,
    Tag_TS_TouFa1 = 30,
    Tag_TS_WaiTao1,
    Tag_TS_ShangYi1,
    Tag_TS_KuZi1,
    Tag_TS_WaZi1,
    Tag_TS_XieZi1,
    Tag_TS_ShiPin1,
    Tag_TS_Bao1,
    Tag_TS_ZhuangRong1,
    Tag_TS_TeXiao1,
    Tag_TS_TouFa2 = 40,
    Tag_TS_WaiTao2,
    Tag_TS_ShangYi2,
    Tag_TS_KuZi2,
    Tag_TS_WaZi2,
    Tag_TS_XieZi2,
    Tag_TS_ShiPin2,
    Tag_TS_Bao2,
    Tag_TS_TouFa3 = 50,
    Tag_TS_WaiTao3,
    Tag_TS_ShangYi3,
    Tag_TS_KuZi3,
    Tag_TS_WaZi3,
    Tag_TS_XieZi3,
    Tag_TS_ShiPin3,
    Tag_TS_Bao3,
    Tag_TS_Bao4 = 60,
    Tag_TS_ShangCheng = 90,
    Tag_TS_BuLiao = 91,
    
    Tag_TS_ShiPin1_1 = 1011,
    Tag_TS_ShiPin1_2,
    Tag_TS_ShiPin1_3,
    Tag_TS_ShiPin1_4,
    Tag_TS_ShiPin1_5,
    Tag_TS_ShiPin1_6,
    Tag_TS_ShiPin1_7,
    Tag_TS_ShiPin1_8,
    Tag_TS_ShiPin1_9,
    Tag_TS_ShiPin2_1 = 2011,
    Tag_TS_ShiPin2_2,
    Tag_TS_ShiPin2_3,
    Tag_TS_ShiPin2_4,
    Tag_TS_ShiPin2_5,
    Tag_TS_ShiPin2_6,
    Tag_TS_ShiPin2_7,
    Tag_TS_ShiPin2_8,
    Tag_TS_ShiPin2_9,
    Tag_TS_ShiPin3_1 = 3011,
    Tag_TS_ShiPin3_2,
    Tag_TS_ShiPin3_3,
    Tag_TS_ShiPin3_4,
    Tag_TS_ShiPin3_5,
    Tag_TS_ShiPin3_6,
    Tag_TS_ShiPin3_7,
    Tag_TS_ShiPin3_8,
    Tag_TS_ShiPin3_9,
}TS_Enum;

class MysteryDialogScene : public BaseScene
{
public:
    static CCScene* scene(const char* category, CCArray* dialogs);
    ~MysteryDialogScene();
    
    static MysteryDialogScene* create(const char* category, CCArray* dialogs);
    virtual bool init(const char* category, CCArray* dialogs);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
public:
    
    void creat_view();
    
    void creat_Man(float _widthFolt, float _heightFloat, float _scaleFloat);
    void creat_Man2(float _widthFolt, float _heightFloat, float _scaleFloat);
    void initClothes();
    
    void backCallBack(CCObject* pSender);
    
    void init_Man();
    void dialogueControl(float dt);
    
    const char* getContent(){return content.c_str();}
    void init(CCDictionary* dic);
    std::string getContentByLength(int length);
    int getContentLength();
    
    void recordLabel(std::string nameStr, std::string saidStr);
    void logic(float dt);
    void openTouch(float dt);
    void getIndex(float dt);
    void emptyLabel();
    void removeMan();
    void addButton();
    
    
    void startCallBack(CCObject* pSender);

    void nc_clothes_info_400(CCObject* pSender);

    
    void huikanCallBack(CCObject* pSender);
    void kuaijinCallBack(CCObject* pSender);
    void _603CallBack(CCObject* pSender);
    void _905CallBack(CCObject* pSender);
    void _800CallBack(CCObject* pObj);
    void _704CallBack(CCObject* pObj);
    void _600CallBack(CCObject* pObj);
    void goTaskScene();
    void LabelColorFhCallBack(CCObject* pSender);
    
    void clothesCallBack(CCObject* pSender);
    
private:
    const char* category;
    CCArray* dialogs;
    CCDictionary* curDialogDic;
    
    CCDictionary* allClothesDic;
    
    int num_child;
    
    bool openStory;
    bool quanBool;
    bool m_bIsKJSelect;
    bool buttonBool;
    bool startBool;
    
    int index;
    int tagIndex;
    int logIndex;
    int nowIndex;
    int contentLength;
    int wordCount;
    int subscriptIndex;
    
    CCMenuItem* backItem;
    CCMenuItem* startItem;
    CCMenuItemToggle* kuaijinToggleItem;
    CCSprite* labelSpr1,* labelSpr2,* labelSpr3;
    
    CCSprite* _dkSpr;
    CCSprite* taskKuang;
    CCSprite* kuangSpr;
    CCSprite* _ManSpr1;
    CCSprite* _ManSpr2;
    CCSprite* _touSpr;
    CCSprite* _tfSpr1; // 头发
    CCSprite* _tfSpr2;
    CCSprite* _tfSpr3;
    CCSprite* _wtSpr1; // 外套
    CCSprite* _wtSpr2;
    CCSprite* _wtSpr3;
    CCSprite* _sySpr1; // 上衣
    CCSprite* _sySpr2;
    CCSprite* _sySpr3;
    CCSprite* _kzSpr1; // 裤子
    CCSprite* _kzSpr2;
    CCSprite* _kzSpr3;
    CCSprite* _wzSpr1; // 袜子
    CCSprite* _wzSpr2;
    CCSprite* _wzSpr3;
    CCSprite* _xzSpr1; // 鞋子
    CCSprite* _xzSpr2;
    CCSprite* _xzSpr3;
    CCSprite* _spSpr1; // 饰品
    CCSprite* _spSpr2;
    CCSprite* _spSpr3;
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
    CCSprite* _zrSpr1; // 妆容
    
    CCSprite* quanSpr;
    CCSprite* nameKuang;
    CCLabelTTF* nameLab;
    CCLabelTTF* saidLabel;
    
    CCArray* taskArr;
    
    int OpenToWhichOne;
    int taskPhase;
    int taskIndex;
    
    std::string content;
    std::string labStr;
    
};
#endif /* defined(__tiegao__MysteryDialogScene__) */
