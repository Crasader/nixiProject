//
//  MMMenu.h
//  mm3c
//
//  Created by mac on 15-5-26.
//
//

#ifndef __mm3c__MMMenu__
#define __mm3c__MMMenu__

#include <iostream>
#include "CCBReader/CCMenuItemLoader.h"
#include "layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */
typedef enum
{
    kMMMenuStateWaiting,
    kMMMenuStateTrackingTouch
} tMMMenuState;

enum {
    //* priority used by the menu for the event handler
    kMMMenuHandlerPriority = -128,
};

/** @brief A CCMenu
 *
 * Features and Limitation:
 *  - You can add MenuItem objects in runtime using addChild:
 *  - But the only accepted children are MenuItem objects
 */
class CC_DLL MMMenu : public CCLayer
{
    /** whether or not the menu will receive events */
    bool m_bEnabled;
    
public:
    /**
     *  @js ctor
     */
    MMMenu() : m_pSelectedItem(NULL) {}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~MMMenu(){}
    
    /** creates an empty CCMenu */
    static MMMenu* create();
    
    /** creates a CCMenu with CCMenuItem objects
     * @lua NA
     */
    static MMMenu* create(CCMenuItem* item, ...);
    
    /** creates a CCMenu with a CCArray of CCMenuItem objects
     * @js NA
     */
    static MMMenu* createWithArray(CCArray* pArrayOfItems);
    
    /** creates a CCMenu with it's item, then use addChild() to add
     * other items. It is used for script, it can't init with undetermined
     * number of variables.
     * @js NA
     */
    static MMMenu* createWithItem(CCMenuItem* item);
    
    /** creates a CCMenu with CCMenuItem objects
     * @js NA
     * @lua NA
     */
    static MMMenu* createWithItems(CCMenuItem *firstItem, va_list args);
    
    /** initializes an empty CCMenu */
    bool init();
    
    /** initializes a CCMenu with a NSArray of CCMenuItem objects
     * @lua NA
     */
    bool initWithArray(CCArray* pArrayOfItems);
    
    /** align items vertically */
    void alignItemsVertically();
    /** align items vertically with padding
     @since v0.7.2
     */
    void alignItemsVerticallyWithPadding(float padding);
    
    /** align items horizontally */
    void alignItemsHorizontally();
    /** align items horizontally with padding
     @since v0.7.2
     */
    void alignItemsHorizontallyWithPadding(float padding);
    
    /** align items in rows of columns
     * @code
     * when this function bound to js,the input params are changed
     * js:var alignItemsInColumns(...)
     * @endcode
     * @lua NA
     */
    void alignItemsInColumns(unsigned int columns, ...);
    /**
     * @js NA
     * @lua NA
     */
    void alignItemsInColumns(unsigned int columns, va_list args);
    /**
     * @js NA
     */
    void alignItemsInColumnsWithArray(CCArray* rows);
    
    /** align items in columns of rows
     * @code
     * when this function bound to js,the input params are changed
     * js:var alignItemsInRows(...)
     * @endcode
     * @lua NA
     */
    void alignItemsInRows(unsigned int rows, ...);
    /**
     * @js NA
     * @lua NA
     */
    void alignItemsInRows(unsigned int rows, va_list args);
    /**
     * @js NA
     */
    void alignItemsInRowsWithArray(CCArray* columns);
    
    /** set event handler priority. By default it is: kCCMenuTouchPriority */
    void setHandlerPriority(int newPriority);
    
    //super methods
    virtual void addChild(CCNode * child);
    virtual void addChild(CCNode * child, int zOrder);
    virtual void addChild(CCNode * child, int zOrder, int tag);
    virtual void registerWithTouchDispatcher();
    virtual void removeChild(CCNode* child, bool cleanup);
    
    /**
     @brief For phone event handle functions
     */
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    /**
     @since v0.99.5
     override onExit
     *  @js NA
     *  @lua NA
     */
    virtual void onExit();
    
    virtual bool isEnabled() { return m_bEnabled; }
    virtual void setEnabled(bool value) { m_bEnabled = value; };
    
protected:
    CCMenuItem* itemForTouch(CCTouch * touch);
    tMMMenuState m_eState;
    CCMenuItem *m_pSelectedItem;
    
public:
    
    static bool m_bTouchesBool;
    CCPoint m_touchBeginWorldPos;
    void setTouchesBool(bool isTouchesBool);
    static bool m_bCheckScissor;
    void setCheckScissor(bool isCheckScissor);
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif /* defined(__mm3c__MMMenu__) */
