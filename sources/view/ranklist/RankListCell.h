//
//  RankListCell.hpp
//  tiegao
//
//  Created by mac on 16/12/24.
//
//

#ifndef RankListCell_hpp
#define RankListCell_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CompetitionItem;
class ShowComp;

class RankListCell : public CCSprite
{
public:
    static CCSprite* getNumSprite(int num);
    void selected();
    void unselected();
    void showRewardAction(const char* type, int num);

public:
    ~RankListCell();
    static RankListCell* create(const char *pszFileName);
    bool init(const char *pszFileName);
    
    void configCompetition(int idx, CompetitionItem *item, float cellWidth, float cellHeight);
    void configShower(int idx, ShowComp* show, float cellWidth, float cellHeight);
        
private:
    void toBeFriend(CCMenuItem* btn);

private:
    int             _idx;
};

#endif /* RankListCell_hpp */
