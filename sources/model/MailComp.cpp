//
//  MailComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "MailComp.h"
#include "Reward.h"
#include "DataManager.h"

MailItem::~MailItem() {
    CC_SAFE_DELETE(reward);
}

bool MailItem::init() {
    return true;
}

void MailItem::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("MailItem::init_with_json() json object error.");
        return;
    }
    
    id = json["id"].asInt();
    send_time = json["send_time"].asInt();
    sender = json["sender"].asString();
    title = json["title"].asString();
    content = json["content"].asString();
    reward = Reward::create();
    reward->retain();
    reward->init_with_json(json["attachment"]);
}

void MailItem::print_self() {
    CCLOG("====== mail: < %d > ======", id);
    CCLOG("    send_time = %d", send_time);
    CCLOG("    sender = %s", sender.c_str());
    CCLOG("    title = %s", title.c_str());
    CCLOG("    content = %s", content.c_str());
    CCLOG("    sender = %s", sender.c_str());
    reward->print_self();
}

// --------------------------------------------------

MailComp::~MailComp() {
    CC_SAFE_DELETE(_mails);
}

bool MailComp::init() {

    return true;
}

void MailComp::init_with_json(Value json) {
    if (!json.isArray()) {
        CCLOG("MailComp::init_with_json() json object error.");
        return;
    }
    
    int size = json.size();
    CCArray* array = CCArray::createWithCapacity(size);
    for (int i = size - 1; i >= 0; i--) {
        MailItem* item = MailItem::create();
        item->init_with_json(json[i]);
        array->addObject(item);

    }
    
    CC_SAFE_RELEASE(_mails);
    _mails = array;
    _mails->retain();
    
    this->print_mails();
}

CCArray* MailComp::mails() const {
    return _mails;
}

void MailComp::print_mails() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_mails, pObj) {
        MailItem* item = (MailItem* )pObj;
        item->print_self();
    }
}

void MailComp::handle_mail_oper(int id, int oper) {
    int size = _mails->count();
    for (int i = 0; i < size; i++) {
        MailItem* item = (MailItem* )_mails->objectAtIndex(i);
        if (item->id == id) {
            
            if (item->reward != NULL) {
                Reward* reward = item->reward;
                if (reward->diam > 0 && item->sender.compare("sys_order") != 0) {
                    CCString* diamStr = CCString::createWithFormat("邮件赠予%d钻石", reward->diam);
                    DATA->onReward(reward->diam, diamStr->getCString());
                }
            }
            
            _mails->removeObjectAtIndex(i);
            break;
        }
    }
}

int MailComp::item_index_by_mail_id(int id) {
    int size = _mails->count();
    for (int i = 0; i < size; i++) {
        MailItem* item = (MailItem* )_mails->objectAtIndex(i);
        if (item->id == id) {
            return i;
        }
    }
    
    return -1;
}




