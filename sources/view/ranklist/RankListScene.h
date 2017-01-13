//
//  TotalTotalScene.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#ifndef RankListScene_hpp
#define RankListScene_hpp

#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class ShowerView;
class ShowComp;
class CompetitionItem;
class RankListView;
class NotePanel;

class RankListScene: public BaseScene
{
public:
    CC_SYNTHESIZE_PASS_BY_REF(string, _comeFrom, ComeFrom);
    
public:
    ~RankListScene();
    CREATE_FUNC(RankListScene);
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
private: // old
    void btn_share_callback(CCObject* pSender);
    void shareStatus(float dt);
    void btn_note_callback(CCObject* pSender);
    void btn_back_callback(CCObject* pSender);

private:
    void createShower();
    void createUI();
    void initDefaultRL();
    void createCompetitionRL();
    void createCollectionRL();
    
    void switchRankList();
    void changeCompetition(CompetitionItem* shower);
    void changeShower(ShowComp* shower);
    void gotoPkScene(CCMenuItem* btn);
    
    void afterHttp300(CCObject* pObj);
    void afterHttp321(CCObject* pObj);
    
    void testPK();
    
private:
    ShowerView*         _shower;
    RankListView*       _rlv;
    NotePanel*          _note;
    
    int                 num_child;
};

#endif /* RankListScene_hpp */
