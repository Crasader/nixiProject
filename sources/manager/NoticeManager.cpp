//
//  NoticeManager.cpp
//  tiegao
//
//  Created by mac on 16/7/22.
//
//

#include "NoticeManager.h"
#include "DataManager.h"
#include "ConfigManager.h"

static NoticeManager* _instance = nullptr;

#pragma mark - Export API

Notice* NoticeManager::fetch_notice() {
    Notice* rtn = NULL;
    
    rtn = this->fetch_prior_notice();
    if (rtn) {
        return rtn;
    }
    
    rtn = this->fetch_second_notice();
    if (rtn) {
        return rtn;
    }
    
    return rtn;
}

#pragma mark - Family&Life API

NoticeManager::~NoticeManager() {
    CC_SAFE_DELETE(_candidates);
}

NoticeManager* NoticeManager::Inst() {
    if (_instance == nullptr) {
        _instance = new NoticeManager();
        _instance->init();
    }
    
    return _instance;
}

void NoticeManager::init() {
    _candidates = new CCArray();
    _candidates->addObject(ccs("social"));
    _candidates->addObject(ccs("mission"));
//    _candidates->addObject(ccs("story"));
}

#pragma mark - Inner API

Notice* NoticeManager::fetch_prior_notice() {
    Notice* rtn = NULL;
    
    NewsComp* news = DATA->getNews();
    if (news->signin7 > 0) {
        rtn = Notice::create();
        rtn->setName("signin");
        rtn->setDesc("小秘书提醒您：今天的签到还么有签，钱钱钱~!");
    }
    
    return rtn;
}

Notice* NoticeManager::fetch_second_notice() {
    Notice* rtn = NULL;
    
    CCArray* temp = CCArray::createWithArray(_candidates);
    while (temp->count() > 0) {
        CCString* choosed = (CCString*)temp->randomObject();
        if (choosed->compare("social") == 0) {
            NewsComp* news = DATA->getNews();
            if (news->paper + news->message > 0) {
                rtn = Notice::create();
                rtn->setName(choosed->getCString());
                rtn->setDesc("小秘书提醒您：咱们是不是去和朋友聚聚~!");
                break;
            }
        }
        else if (choosed->compare("mission") == 0) {
//            CCDictionary* completedMissions = DATA->getMission()->cmpleted_mission();
//            int completed = completedMissions->count();
//            CCArray* allMissionTemplates = CONFIG->mission();
//            int missionCountTotal = allMissionTemplates->count();
//            if (completed < missionCountTotal) {
                rtn = Notice::create();
                rtn->setName(choosed->getCString());
                rtn->setDesc("小秘书提醒您：公司还有工作需要您去处理~!");
                break;
//            }
        }
        else if (choosed->compare("story") == 0) {
//            CCDictionary* completedStory = DATA->getStory()->cmpleted_story();
//            int completed = completedStory->count();
//            CCArray* allStoryTemplates = CONFIG->mission();
//            int missionCountTotal = allStoryTemplates->count();
//            if (completed < missionCountTotal) {
                rtn = Notice::create();
                rtn->setName(choosed->getCString());
                rtn->setDesc("小秘书提醒您：段飞找您有事，请尽快前往啊~!");
                break;
//            }
        }
    }
    
    return rtn;
}




