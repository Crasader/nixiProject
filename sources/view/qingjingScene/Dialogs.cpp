//
//  Dialogs.cpp
//  manman_client
//
//  Created by lakkey on 14-1-13.
//
//

#include "Dialogs.h"
#include "DialogItem.h"
#include "MZResourceLoader.h"


Dialogs::~Dialogs()
{
    //    CC_SAFE_RELEASE_NULL(_dialogs);
    //    CC_SAFE_RELEASE_NULL(_cluesDialogs);
}


bool Dialogs::init()
{
    return true;
}


void Dialogs::config_with_file(const char *file)
{
    MZResourceLoader* loader = MZResourceLoader::get_instance();
//    char* file_data = loader->reader_file(file);
    char* file_data = loader->reader_xxtea_file(file);
    
    CSJson::Reader reader;
    CSJson::Value root;
    
    if (!reader.parse(file_data, root, false)) {
        CCLog("CSJson reader error~~~");
        return;
    }
    
    if (root.isArray()) {
        int count = root.size();
//        MZLog("count ======== %d", count);
        this->setDialogs(CCArray::create());
        for (int i = 0; i < count; i++) {
            CSJson::Value value = root[i];
            DialogItem* item = DialogItem::create();
            item->config_static_with_json(value);
            _dialogs->addObject(item);
        }
    }
}


void Dialogs::clues_config_with_file(const char* file){
    MZResourceLoader* loader = MZResourceLoader::get_instance();
    
//    char* file_data = loader->reader_file(file);
    char* file_data = loader->reader_xxtea_file(file);
    
    CSJson::Reader reader;
    CSJson::Value root;
    
    if (!reader.parse(file_data, root, false)) {
        CCLog("CSJson reader error~~~");
        return;
    }
    
    if (root.isArray()) {
        int count = root.size();
//        MZLog("count ========= %d", count);
        this->setCluesDialogs(CCArray::create());
        for (int i = 0; i < count; i++) {
            CSJson::Value value = root[i];
            DialogItem* item = DialogItem::create();
            item->clues_config_static_with_json(value);
            _cluesDialogs->addObject(item);
        }
    }
}


void Dialogs::story_figure_config_with_file(const char* file, int PetId, int index){
    
    MZResourceLoader* loader = MZResourceLoader::get_instance();
    CSJson::Value root = loader->get_json_data_with_file(file);
//    MZLog("===== %d", PetId);
    CCString * storyPetId = CCString::createWithFormat("%d",PetId);
    CSJson::Value value = root[storyPetId->getCString()];
    int randIndex = root.size();
    CCLog("randIndex === %d", randIndex);
    
    _zishi = value["zishi"][rand()%2].asInt();
}








