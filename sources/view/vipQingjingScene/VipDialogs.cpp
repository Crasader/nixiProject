//
//  VipDialogs.cpp
//  manman_client
//
//  Created by lakkey on 14-1-13.
//
//

#include "VipDialogs.h"
#include "VipDialogItem.h"
#include "MZResourceLoader.h"


VipDialogs::~VipDialogs()
{
    //    CC_SAFE_RELEASE_NULL(_VipDialogs);
    //    CC_SAFE_RELEASE_NULL(_cluesVipDialogs);
}


bool VipDialogs::init()
{
    return true;
}


void VipDialogs::config_with_file(const char *file)
{
    MZResourceLoader* loader = MZResourceLoader::get_instance();
    char* file_data = loader->reader_file(file);
//    char* file_data = loader->reader_xxtea_file(file);
    
    CSJson::Reader reader;
    CSJson::Value root;
    
    if (!reader.parse(file_data, root, false)) {
        CCLog("CSJson reader error~~~");
        return;
    }
    
    if (root.isArray()) {
        int count = root.size();
//        MZLog("count ======== %d", count);
        this->setVipDialogs(CCArray::create());
        for (int i = 0; i < count; i++) {
            CSJson::Value value = root[i];
            VipDialogItem* item = VipDialogItem::create();
            item->config_static_with_json(value);
            _VipDialogs->addObject(item);
        }
    }
}


void VipDialogs::clues_config_with_file(const char* file){
    MZResourceLoader* loader = MZResourceLoader::get_instance();
    
    char* file_data = loader->reader_file(file);
//    char* file_data = loader->reader_xxtea_file(file);
    
    CSJson::Reader reader;
    CSJson::Value root;
    
    if (!reader.parse(file_data, root, false)) {
        CCLog("CSJson reader error~~~");
        return;
    }
    
    if (root.isArray()) {
        int count = root.size();
//        MZLog("count ========= %d", count);
        this->setCluesVipDialogs(CCArray::create());
        for (int i = 0; i < count; i++) {
            CSJson::Value value = root[i];
            VipDialogItem* item = VipDialogItem::create();
            item->clues_config_static_with_json(value);
            _cluesVipDialogs->addObject(item);
        }
    }
}


void VipDialogs::story_figure_config_with_file(const char* file, int PetId, int index){
    
    MZResourceLoader* loader = MZResourceLoader::get_instance();
    CSJson::Value root = loader->get_json_data_with_file(file);
//    MZLog("===== %d", PetId);
    CCString * storyPetId = CCString::createWithFormat("%d",PetId);
    CSJson::Value value = root[storyPetId->getCString()];
    int randIndex = root.size();
    CCLog("randIndex === %d", randIndex);
    
    _zishi = value["zishi"][rand()%2].asInt();
}








