//
//  MessageTableView.h
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#ifndef __tiegao__MessageTableView__
#define __tiegao__MessageTableView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SliderBar.h"


using namespace cocos2d;
using namespace cocos2d::extension;


class MessageTableView
: public CCLayer
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
	MessageTableView();
	~MessageTableView();
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    static CCScene* scene();
    
    CREATE_FUNC(MessageTableView);
    
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
    
    CCArray* messageArr;
    
    float offsetFloat;
    bool sliderBool;
    int nowIndex;
    SliderBar *slider;
    
    void creat_View();
    void updateTableView();
    void hanle_message_oper(int id, int oper);
    
    void agreedCallBack(CCObject* pSender);
    void deleteCallBack(CCObject* pSender);
    void _805CallBack(CCObject* pSender);
};


#endif /* defined(__tiegao__MessageTableView__) */
