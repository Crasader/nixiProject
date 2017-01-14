//
//  MessageComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "MessageComp.h"

MessageItem::~MessageItem() {
}

bool MessageItem::init() {
    return true;
}

void MessageItem::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("MessageItem::init_with_json() json object error.");
        return;
    }
    
    id = json["id"].asUInt64();
    type = json["type"].asInt();
    sender = json["sender"].asString();
}

void MessageItem::print_self() {
    CCLOG("====== message: < %lld > ======", id);
    CCLOG("    type = %d", type);
    CCLOG("    sender = %s", sender.c_str());
}

// --------------------------------------------------

MessageComp::~MessageComp() {
    CC_SAFE_DELETE(_messages);
}

bool MessageComp::init() {
    return true;
}

void MessageComp::init_with_json(Value json) {
    if (!json.isArray()) {
        CCLOG("MessageComp::init_with_json() json object error.");
        return;
    }
    
    CCArray* array = CCArray::create();
    for (CSJson::ValueIterator iterator = json.begin(); iterator != json.end(); iterator++) {
        CSJson::Value value = (CSJson::Value)* iterator;
        MessageItem* item = MessageItem::create();
        item->init_with_json(value);
        array->addObject(item);
    }
    
    CC_SAFE_RELEASE(_messages);
    _messages = array;
    _messages->retain();
    
//    this->print_messages();
}

CCArray* MessageComp::messages() const {
    return _messages;
}

void MessageComp::print_messages() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_messages, pObj) {
        MessageItem* item = (MessageItem* )pObj;
        item->print_self();
    }
}


