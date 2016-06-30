//
//  NoteTableView.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/29.
//
//

#ifndef NoteTableView_hpp
#define NoteTableView_hpp

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SliderBar.h"

USING_NS_CC;

using namespace cocos2d;
using namespace cocos2d::extension;

#define NEI_MAXNUM 30


class NoteTableView
: public CCLayer
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    NoteTableView();
    ~NoteTableView();
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    static CCScene* scene();
    
    CREATE_FUNC(NoteTableView);
    
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
    virtual void tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
    
    //松开的时候，取消高亮状态
    virtual void tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
    
private:
    void valueChanged( CCObject *sender, CCControlEvent controlEvent );
    
private:
    
    CCSprite* diSpr;
    
    int _number;
    int clickIndex;
    int tableFloat;
    
    bool m_bTable;
    bool m_bSlider;
    CCSize winSize;
    
public:
    
    CCTableView *pTableView;
    CCTableViewCell *pCell;
    
    CCArray* paperArr;
    
    float offsetFloat;
    bool sliderBool;
    int nowIndex;
    SliderBar *slider;
    
    void creat_View();
    void updateTableView();
    
    void replyCallBack(CCObject* pSender);
    void deleteCallBack(CCObject* pSender);
    void addCallBack(CCObject* pSender);
    void addfriend_callback_803(CCObject* pSender);
    void delete_callback_811(CCObject* pSender);
};


#endif /* NoteTableView_hpp */
