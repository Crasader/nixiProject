//
//  PaperComp.cpp
//  tiegao
//
//  Created by mac on 16/6/29.
//
//

#include "PaperComp.h"

PaperItem::~PaperItem() {
}

bool PaperItem::init() {
    return true;
}

void PaperItem::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("PaperItem::init_with_json() json object error.");
        return;
    }
    
    id = json["id"].asInt();
    sender = json["sender"].asString();
    sender_name = json["sender_name"].asString();
    content = json["content"].asString();
}

void PaperItem::print_self() {
    CCLOG("====== Paper: < %d > ======", id);
    CCLOG("    sender = %s", sender.c_str());
    CCLOG("    content = %s", content.c_str());
}

// --------------------------------------------------

PaperComp::~PaperComp() {
    CC_SAFE_DELETE(_papers);
}

bool PaperComp::init() {
    return true;
}

void PaperComp::init_with_json(Value json) {
    if (!json.isArray()) {
        CCLOG("PaperComp::init_with_json() json object error.");
        return;
    }
    
    CCArray* array = CCArray::create();
    for (CSJson::ValueIterator iterator = json.begin(); iterator != json.end(); iterator++) {
        CSJson::Value value = (CSJson::Value)* iterator;
        PaperItem* item = PaperItem::create();
        item->init_with_json(value);
        array->addObject(item);
    }
    
    CC_SAFE_RELEASE(_papers);
    _papers = array;
    _papers->retain();
    
    this->print_papers();
}

CCArray* PaperComp::papers() const {
    return _papers;
}

void PaperComp::print_papers() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_papers, pObj) {
        PaperItem* item = (PaperItem* )pObj;
        item->print_self();
    }
}


