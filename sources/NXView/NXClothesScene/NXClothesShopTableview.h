//
//  NXClothesShopTableview.h
//  tiegao
//
//  Created by mac on 17-7-8.
//
//

#ifndef __tiegao__NXClothesShopTableview__
#define __tiegao__NXClothesShopTableview__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define def_Shop_Time           0.7
#define def_Shop_Index          2.5
#define def_Shop_Index2          4

class NXClothesShopTableview
: public CCLayer
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    NXClothesShopTableview();
    ~NXClothesShopTableview();
    
    virtual bool init();
    
    
    CREATE_FUNC(NXClothesShopTableview);
    
public:
    
    //CCTableViewDelegate继承自CCScrollViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){}
    
    //点击哪个cell
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    //每个cell的size
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    
    //生成cell
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
    //cell的数量
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    //按下去的时候，就是高亮显示，这里可以设置高亮状态
    virtual void tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    //松开的时候，取消高亮状态
    virtual void tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    void onEnter();
    
    void onExit();
    
    void updateTableView(int type);
    int updataClothes(int type);
    void updateTableCell();
    void clothesUpdateTableCell();
    float sliderV;
    
    void clothesButtonCallBack(CCObject* pSender);
    
    CCTableView* pTableView;
    
    float offsetFloat;
    
    int clothesType;
    int clothesPage;
    int clothes_selected;
    
    CCSprite* kuangSpr;
    
public:
    
    void creat_kuang(CCSprite* dikuangSpr, CCArray* arr, int index, int i);
    
};
#endif /* defined(__tiegao__NXClothesShopTableview__) */
