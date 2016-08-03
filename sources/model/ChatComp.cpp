//
//  ChatComp.cpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#include "ChatComp.h"

bool ChatItem::init_with_json(Value json) {
    bool btn = true;
    if (json.type() == nullValue || json.type() != objectValue) {
        CCLOG("ChatItem::init_with_json() json object error.");
        btn = false;
    }
    else  {
        Value name = json["name"];
        if (name.type() != nullValue) {
            this->name = name.asString();
        }
        else {
            btn = false;
        }
        
        Value chat = json["chat"];
        if (name.type() != nullValue) {
            this->chat = chat.asString();
        }
        else {
            btn = false;
        }
    }

    return btn;
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
    
    CCColor3bObject* color4 = new CCColor3bObject(ccc3(254, 187, 252));
    _colors->addObject(color4);
    
    CCColor3bObject* color5 = new CCColor3bObject(ccc3(255, 254, 254));
    _colors->addObject(color5);
    
    CCColor3bObject* color6 = new CCColor3bObject(ccc3(187, 254, 247));
    _colors->addObject(color6);
    
    CCColor3bObject* color7 = new CCColor3bObject(ccc3(168, 205, 254));
    _colors->addObject(color7);
    
    CCColor3bObject* color8 = new CCColor3bObject(ccc3(98, 203, 150));
    _colors->addObject(color8);
    
    CCColor3bObject* color9 = new CCColor3bObject(ccc3(203, 45, 74));
    _colors->addObject(color9);
    
    CCColor3bObject* color10 = new CCColor3bObject(ccc3(235, 212, 107));
    _colors->addObject(color10);
    
    CCColor3bObject* color_name = new CCColor3bObject(ccc3(248, 83, 18));
    _colors->addObject(color_name);
    
    CCColor3bObject* color_selfname = new CCColor3bObject(ccc3(84, 118, 245));
    _colors->addObject(color_selfname);
    
    CCColor3bObject* color_11 = new CCColor3bObject(ccc3(240, 88, 116));
    _colors->addObject(color_11);
    
    CCColor3bObject* color_12 = new CCColor3bObject(ccc3(171, 107, 119));
    _colors->addObject(color_12);
}
