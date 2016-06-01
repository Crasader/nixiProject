//
//  DisplayManager.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "DisplayManager.h"

static DisplayManager* _instance = nullptr;

DisplayManager::~DisplayManager() {
    
}

DisplayManager* DisplayManager::Inst() {
    if (_instance == nullptr) {
        _instance = new DisplayManager();
        _instance->init();
    }
    
    return _instance;
}

void DisplayManager::init() {
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    _width = winsize.width;
    _height = winsize.height;
}

const char* DisplayManager::font() {
    return "TRENDS.ttf";
}

CCPoint DisplayManager::center() {
    return CCPointMake(_width * 0.5, _height * 0.5);
}

float DisplayManager::ScreenWidth(){
    return _width;
}
float DisplayManager::ScreenHeight(){
    return _height;
}
float DisplayManager::OriginWidth(){
    return (_width - 640) * 0.5f;
}



CCString* DisplayManager::GetOffTheNumber(int index){
    
    CCString* str;
    if (index == 1){
        str = CCString::create("第一章");
    }else if (index == 2){
        str = CCString::create("第二章");
    }else if (index == 3){
        str = CCString::create("第三章");
    }else if (index == 4){
        str = CCString::create("第四章");
    }else if (index == 5){
        str = CCString::create("第五章");
    }else if (index == 6){
        str = CCString::create("第六章");
    }else if (index == 7){
        str = CCString::create("第七章");
    }else if (index == 8){
        str = CCString::create("第八章");
    }else if (index == 9){
        str = CCString::create("第九章");
    }else if (index == 10){
        str = CCString::create("第十章");
    }else if (index == 11){
        str = CCString::create("第十一章");
    }else if (index == 12){
        str = CCString::create("第十二章");
    }else if (index == 13){
        str = CCString::create("第十三章");
    }else if (index == 14){
        str = CCString::create("第十四章");
    }else if (index == 15){
        str = CCString::create("第十五章");
    }else if (index == 16){
        str = CCString::create("第十六章");
    }else if (index == 17){
        str = CCString::create("第十七章");
    }else if (index == 18){
        str = CCString::create("第十八章");
    }else if (index == 19){
        str = CCString::create("第十九章");
    }else if (index == 20){
        str = CCString::create("第二十章");
    }else if (index == 21){
        str = CCString::create("第二十一章");
    }else if (index == 22){
        str = CCString::create("第二十二章");
    }else if (index == 23){
        str = CCString::create("第二十三章");
    }else if (index == 24){
        str = CCString::create("第二十四章");
    }else if (index == 25){
        str = CCString::create("第二十五章");
    }else if (index == 26){
        str = CCString::create("第二十六章");
    }else if (index == 27){
        str = CCString::create("第二十七章");
    }else if (index == 28){
        str = CCString::create("第二十八章");
    }else if (index == 29){
        str = CCString::create("第二十九章");
    }else if (index == 30){
        str = CCString::create("第三十章");
    }else if (index == 31){
        str = CCString::create("第三十一章");
    }else if (index == 32){
        str = CCString::create("第三十二章");
    }else if (index == 33){
        str = CCString::create("第三十三章");
    }else if (index == 34){
        str = CCString::create("第三十四章");
    }else if (index == 35){
        str = CCString::create("第三十五章");
    }else if (index == 36){
        str = CCString::create("第三十六章");
    }else if (index == 37){
        str = CCString::create("第三十七章");
    }else if (index == 38){
        str = CCString::create("第三十八章");
    }else if (index == 39){
        str = CCString::create("第三十九章");
    }else if (index == 40){
        str = CCString::create("第四十章");
    }else if (index == 41){
        str = CCString::create("第四十一章");
    }else if (index == 42){
        str = CCString::create("第四十二章");
    }else if (index == 43){
        str = CCString::create("第四十三章");
    }else if (index == 44){
        str = CCString::create("第四十四章");
    }else if (index == 45){
        str = CCString::create("第四十五章");
    }else if (index == 46){
        str = CCString::create("第四十六章");
    }else if (index == 47){
        str = CCString::create("第四十七章");
    }else if (index == 48){
        str = CCString::create("第四十八章");
    }else if (index == 49){
        str = CCString::create("第四十九章");
    }else if (index == 50){
        str = CCString::create("第五十章");
    }else if (index == 51){
        str = CCString::create("第五十一章");
    }else if (index == 52){
        str = CCString::create("第五十二章");
    }else if (index == 53){
        str = CCString::create("第五十三章");
    }else if (index == 54){
        str = CCString::create("第五十四章");
    }else if (index == 55){
        str = CCString::create("第五十五章");
    }else if (index == 56){
        str = CCString::create("第五十六章");
    }else if (index == 57){
        str = CCString::create("第五十七章");
    }else if (index == 58){
        str = CCString::create("第五十八章");
    }else if (index == 59){
        str = CCString::create("第五十九章");
    }else if (index == 60){
        str = CCString::create("第六十章");
    }else if (index == 61){
        str = CCString::create("第六十一章");
    }else if (index == 62){
        str = CCString::create("第六十二章");
    }else if (index == 63){
        str = CCString::create("第六十三章");
    }else if (index == 64){
        str = CCString::create("第六十四章");
    }else if (index == 65){
        str = CCString::create("第六十五章");
    }else if (index == 66){
        str = CCString::create("第六十六章");
    }else if (index == 67){
        str = CCString::create("第六十七章");
    }else if (index == 68){
        str = CCString::create("第六十八章");
    }else if (index == 69){
        str = CCString::create("第六十九章");
    }else if (index == 70){
        str = CCString::create("第七十章");
    }else if (index == 71){
        str = CCString::create("第七十一章");
    }else if (index == 72){
        str = CCString::create("第七十二章");
    }else if (index == 73){
        str = CCString::create("第七十三章");
    }else if (index == 74){
        str = CCString::create("第七十四章");
    }else if (index == 75){
        str = CCString::create("第七十五章");
    }else if (index == 76){
        str = CCString::create("第七十六章");
    }else if (index == 77){
        str = CCString::create("第七十七章");
    }else if (index == 78){
        str = CCString::create("第七十八章");
    }else if (index == 79){
        str = CCString::create("第七十九章");
    }else if (index == 80){
        str = CCString::create("第八十章");
    }else if (index == 81){
        str = CCString::create("第八十一章");
    }else if (index == 82){
        str = CCString::create("第八十二章");
    }else if (index == 83){
        str = CCString::create("第八十三章");
    }else if (index == 84){
        str = CCString::create("第八十四章");
    }else if (index == 85){
        str = CCString::create("第八十五章");
    }else if (index == 86){
        str = CCString::create("第八十六章");
    }else if (index == 87){
        str = CCString::create("第八十七章");
    }else if (index == 88){
        str = CCString::create("第八十八章");
    }else if (index == 89){
        str = CCString::create("第八十九章");
    }else if (index == 90){
        str = CCString::create("第九十章");
    }else if (index == 91){
        str = CCString::create("第九十一章");
    }else if (index == 92){
        str = CCString::create("第九十二章");
    }else if (index == 93){
        str = CCString::create("第九十三章");
    }else if (index == 94){
        str = CCString::create("第九十四章");
    }else if (index == 95){
        str = CCString::create("第九十五章");
    }else if (index == 96){
        str = CCString::create("第九十六章");
    }else if (index == 97){
        str = CCString::create("第九十七章");
    }else if (index == 98){
        str = CCString::create("第九十八章");
    }else if (index == 99){
        str = CCString::create("第九十九章");
    }
    
    return str;
}

float DisplayManager::W() {
    return _width;
}

float DisplayManager::H() {
    return _height;
}

float DisplayManager::halfW() {
    return _width * 0.5;
}

float DisplayManager::halfH() {
    return _height * 0.5;
}

float DisplayManager::btn_scale() {
    return 1.02f;
}

