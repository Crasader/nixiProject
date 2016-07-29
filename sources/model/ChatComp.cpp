//
//  ChatComp.cpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#include "ChatComp.h"

void ChatItem::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("ChatItem::init_with_json() json object error.");
        return;
    }
    
    name = json["name"].asString();
    chat = json["chat"].asString();
}

void ChatItem::print_self() {
    CCLOG("    ChatItem:");
    CCLOG("        name = %s", name.c_str());
    CCLOG("        chat = %s", chat.c_str());
}

#pragma mark -

void ChatComp::addItem(ChatItem* item) {
    if (_items) {
        _items->addObject(item);
    }
}

ChatComp::~ChatComp() {
    CC_SAFE_DELETE(_items);
    CC_SAFE_DELETE(_colors);
}

bool ChatComp::init() {
    this->setItems(CCArray::create());
    this->setColors(CCArray::create());
    this->initColors();
    
    return true;
}

CCColor3bObject* ChatComp::randColor() {
    if (_colors) {
        return (CCColor3bObject*)_colors->randomObject();
    }
    
    CCColor3bObject* color = new CCColor3bObject(ccBLUE);
    color->release();
    return color;
}

void ChatComp::initColors() {
    CCColor3bObject* color1 = new CCColor3bObject(ccc3(95, 246, 255));
    _colors->addObject(color1);
    
    CCColor3bObject* color2 = new CCColor3bObject(ccc3(173, 228, 200));
    _colors->addObject(color2);
    
    CCColor3bObject* color3 = new CCColor3bObject(ccc3(254, 187, 200));
    _colors->addObject(color3);
}
