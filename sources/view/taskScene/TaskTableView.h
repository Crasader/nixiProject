//
//  TaskTableView.h
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#ifndef __tiegao__TaskTableView__
#define __tiegao__TaskTableView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define def_TX_Time             0.7
#define def_Page_Index          7.3

typedef enum{
    Tag_Task_Xing1 = 1000,
    Tag_Task_Xing2,
    Tag_Task_Xing3,
    Tag_Task_Xing4,
    Tag_Task_Xing5
}TaskTable_Enum;

class TaskTableView
: public CCLayer
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    TaskTableView();
    ~TaskTableView();
    
    virtual bool init();
    
    
    CREATE_FUNC(TaskTableView);
    
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
    
public:
    
    CCTableView* pTableView;
    
    CCArray* taskMission;
    
    CCDictionary* ratingDic;
    int OpenToWhichOne;
    
    CCNode* sprNode;
    
    int allNumber;
    int selectedIndex;
    int stopIndex;
    
public:
        
    void buttonStatus(int index, CCSprite* button);
    CCString* getTaskName(int index);
    int getTaskIcon(int index);
    int getTaskPhase(int index);
    
    void creatStopStatus();
    void showCellOfIndex(unsigned int index);
    
    void taskTableCallBack(CCObject* pSender);
};

#endif /* defined(__tiegao__TaskTableView__) */
