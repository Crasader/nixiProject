//
//  SocialComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef SocialComp_hpp
#define SocialComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class ShowComp;

typedef enum {
    e_Msg_Friend_Ask = 1,   // 加好友消息的类型
    e_Msg_Send_Energy,      // 送体力消息的类型
    e_Msg_Friend_Deletem,   // 当删除好友时消息的类型
} MesssageType;

class SocialComp : public CCObject
{
public:
    ~SocialComp();
    CREATE_FUNC(SocialComp);
    bool init();
    void init_with_json(Value json);
    void update_strangers(Value json);
    void init_friends(Value json);
    
    CC_SYNTHESIZE(bool, _hasInitFriends, HasInitFriends);
    
public:
    CCDictionary* strangers();
    CCDictionary* friends() const;      // 包括自己的Show在内
    CCArray* sortedFriends();           // 按收集度从高到低排序
    bool is_friend(const char* other_sid);
    bool is_requested(const char* other_sid);   // 是否申请好友
    bool has_send_energy(const char* other_sid);
    int energy_could_take();            // 可以领取的体力数量
    
    ShowComp* getSelectedStrangerByIndex(int idx);
    ShowComp* getSelectedFriendByIndex(int idx);
    ShowComp* getSelectedRankerByIndex(int idx);
    const char* getSelectedStrangerIDbyIndex(int idx);
    const char* getSelectedFriendIDbyIndex(int idx);
    const char* getSelectedRankerIDbyIndex(int idx);
    CC_SYNTHESIZE(int, _selectedStranger, SelectedStranger);
    CC_SYNTHESIZE(int, _selectedFriend, SelectedFriend);
    CC_SYNTHESIZE(int, _selectedRanker, SelectedRanker);
    
private:
    CCArray* sort_friends_by_collected(CCDictionary* dic);
    
private:
    int                 _energy_token;
    CCDictionary*       _strangers;
    CCDictionary*       _friends;
    CCArray*            _sortedFriends;
    CCArray*            _arr_friends;
    CCArray*            _energy_send;
    CCArray*            _energy_receive;
    CCArray*            _requested;
};

#endif /* SocialComp_hpp */
