//
//  MailComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "MailComp.h"
#include "Reward.h"

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
    
    CCArray* array = CCArray::create();
    for (CSJson::ValueIterator iterator = json.begin(); iterator != json.end(); iterator++) {
        CSJson::Value value = (CSJson::Value)* iterator;
        MailItem* item = MailItem::create();
        item->init_with_json(value);
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

