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
    
public:
    CCDictionary* strangers();
    CCDictionary* friends() const;
    bool is_friend(const char* other_sid);
    bool has_send_energy(const char* other_sid);
    int energy_could_take();
    
    ShowComp* getSelectedStrangerByIndex(int idx);
    ShowComp* getSelectedFriendByIndex(int idx);
    const char* getSelectedStrangerIDbyIndex(int idx);
    const char* getSelectedFriendIDbyIndex(int idx);
    CC_SYNTHESIZE(int, _selectedStranger, SelectedStranger);
    CC_SYNTHESIZE(int, _selectedFriend, SelectedFriend);
    
private:
    int                 _energy_token;
    CCDictionary*       _strangers;
    CCDictionary*       _friends;
    CCArray*            _arr_friends;
    CCArray*            _energy_send;
    CCArray*            _energy_receive;
};

#endif /* SocialComp_hpp */
