//
//  AudioManager.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "AudioManager.h"

static AudioManager* _instance = nullptr;

#define kUDK_Music_Bool                 "is_music_on"
#define kUDK_Effect_Bool                "is_effect_on"

#define BGM_Main            "audio/mp3/bgm_main.mp3"
#define BGM_Company         "audio/mp3/bgm_company.mp3"
#define BGM_Story           "audio/mp3/bgm_story.mp3"

AudioManager::~AudioManager()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

void AudioManager::init()
{
    // 背景音量
    this->setBackgroundMusicVolume(1.f);
    // 音效音量
    this->setEffectsVolume(1.f);
    
    CCUserDefault* UD = CCUserDefault::sharedUserDefault();
    this->set_music_on(UD->getBoolForKey(kUDK_Music_Bool, true));
    this->set_effect_on(UD->getBoolForKey(kUDK_Effect_Bool, true));
}


AudioManager* AudioManager::Inst()
{
    if (!_instance) {
        _instance = new AudioManager();
        _instance->init();
        _instance->preload_audios();
    }
    
    return _instance;
}


void AudioManager::preload_audios()
{
    this->preloadBackgroundMusic(BGM_Main);
    this->preloadBackgroundMusic(BGM_Company);
    this->preloadBackgroundMusic(BGM_Story);
    
    // 通用按键 音效
    this->preloadEffect("audio/wav/audio_button_common.wav");
    // 通用 错误 音效
    this->preloadEffect("audio/wav/audio_button_error.wav");
    // 通用 确认 音效
    this->preloadEffect("audio/wav/audio_button_Yes.wav");
    // 通用 按钮 返回声音
    this->preloadEffect("audio/wav/audio_button_back.wav");
    // 穿衣服 音效
    this->preloadEffect("audio/wav/audio_button_clothes.wav");
    // 购买 音效（花钱的声音）
    this->preloadEffect("audio/wav/audio_button_buy.wav");
    // 商城 音效
    this->preloadEffect("audio/wav/audio_button_shop.wav");
    // right 音效
    this->preloadEffect("audio/wav/audio_game_right.wav");
    // wrong 音效
    this->preloadEffect("audio/wav/audio_game_wrong.wav");
    
    // 进入公司播放
    this->preloadEffect("audio/wav/11.wav");
    this->preloadEffect("audio/wav/12.wav");
    this->preloadEffect("audio/wav/13.wav");
    // 进入家播放
    this->preloadEffect("audio/wav/21.wav");
    this->preloadEffect("audio/wav/22.wav");
    this->preloadEffect("audio/wav/23.wav");
    // 小手机出现播放
    this->preloadEffect("audio/wav/31.wav");
    this->preloadEffect("audio/wav/32.wav");
    this->preloadEffect("audio/wav/33.wav");
    this->preloadEffect("audio/wav/34.wav");
    // 进入陌生人播放
    this->preloadEffect("audio/wav/41.wav");
    this->preloadEffect("audio/wav/42.wav");
    this->preloadEffect("audio/wav/43.wav");
    // 高级衣服
    this->preloadEffect("audio/wav/71.wav");
    this->preloadEffect("audio/wav/72.wav");
    this->preloadEffect("audio/wav/73.wav");
    this->preloadEffect("audio/wav/74.wav");
    
    
    // pk
    this->preloadEffect("audio/wav/audio_pk_jiemian.wav");
    this->preloadEffect("audio/wav/audio_pk_shangsheng.wav");
    this->preloadEffect("audio/wav/audio_pk_shengli.wav");
    this->preloadEffect("audio/wav/audio_pk_shibai.wav");
    this->preloadEffect("audio/wav/audio_pk_wenzi.wav");
    
    
}


bool AudioManager::is_music_on()
{
    return m_is_music_on;
}


bool AudioManager::is_effect_on()
{
    return m_is_effect_on;
}


bool AudioManager::is_music_paused()
{
    return m_is_music_paused;
}


bool AudioManager::is_effect_paused()
{
    return m_is_effect_paused;
}


void AudioManager::set_music_on(bool is_on)
{
    m_is_music_on = is_on;
    CCUserDefault::sharedUserDefault()->setBoolForKey(kUDK_Music_Bool, m_is_music_on);
    CCUserDefault::sharedUserDefault()->flush();
    if (m_is_music_on && !m_is_music_paused)
        this->resumeBackgroundMusic();
    else
        this->pauseBackgroundMusic();
}


void AudioManager::set_effect_on(bool is_on)
{
    m_is_effect_on = is_on;
    CCUserDefault::sharedUserDefault()->setBoolForKey(kUDK_Effect_Bool, m_is_effect_on);
    CCUserDefault::sharedUserDefault()->flush();
    if (m_is_effect_on && !m_is_effect_paused)
        this->resumeAllEffects();
    else
        this->stopAllEffects();
}


void AudioManager::purge()
{
    
}


void AudioManager::play_music(const char *pszFilePath, bool bLoop)
{
    this->playBackgroundMusic(pszFilePath, bLoop);
    
    if (! m_is_music_on) {
        this->pauseBackgroundMusic();
    }
}


unsigned int AudioManager::play_effect(const char *pszFilePath, bool bLoop)
{
    if (m_is_effect_on) {
        return this->playEffect(pszFilePath, bLoop);
    }
    
    return 0;
}


void AudioManager::pause_music()
{
    m_is_music_paused = true;
    if (m_is_music_on) {
        this->pauseBackgroundMusic();
    }
}

void AudioManager::resume_music()
{
    m_is_music_paused = false;
    if (m_is_music_on) {
        this->resumeBackgroundMusic();
    }
}


void AudioManager::pause_effects()
{
    m_is_effect_paused = true;
    this->pauseAllEffects();
}

void AudioManager::resume_all_effects()
{
    m_is_effect_paused = false;
    if (m_is_effect_on) {
        this->resumeAllEffects();
    }
}

#pragma - Export API

void AudioManager::first_run_config() {
    this->set_effect_on(true);
    this->set_music_on(true);
}

void AudioManager::play_main_bgm() {
    this->play_music(BGM_Main, true);
    this->setCurBGM(ccs("MAIN"));
}

void AudioManager::play_company_bgm() {
    this->play_music(BGM_Company, true);
    this->setCurBGM(ccs("COMPANY"));
}

void AudioManager::play_story_bgm() {
    this->play_music(BGM_Story, true);
    this->setCurBGM(ccs("STORY"));
}

void AudioManager::common_effect() {
    this->play_effect("audio/wav/audio_button_common.wav", false);
}

void AudioManager::error_effect() {
    this->play_effect("audio/wav/audio_button_error.wav", false);
}

void AudioManager::comfirm_effect() {
    this->play_effect("audio/wav/audio_button_Yes.wav", false);
}

void AudioManager::goback_effect() {
    this->play_effect("audio/wav/audio_button_back.wav", false);
}

void AudioManager::buy_effect() {
    this->play_effect("audio/wav/audio_button_buy.wav", false);
}

void AudioManager::shop_effect() {
    this->play_effect("audio/wav/audio_button_shop.wav", false);
}

void AudioManager::clothes_effect() {
    this->play_effect("audio/wav/audio_button_clothes.wav", false);
}

void AudioManager::right_effect(){
    this->play_effect("audio/wav/audio_game_right.wav", false);
}

void AudioManager::wrong_effect(){
    this->play_effect("audio/wav/audio_game_wrong.wav", false);
}


void AudioManager::pk_jiemian_effect(){
    this->play_effect("audio/wav/audio_pk_jiemian.wav", false);
}
void AudioManager::pk_shangsheng_effect(){
    this->play_effect("audio/wav/audio_pk_shangsheng.wav", false);
}
void AudioManager::pk_shengli_effect(){
    this->play_effect("audio/wav/audio_pk_shengli.wav", false);
}
void AudioManager::pk_shibai_effect(){
    this->play_effect("audio/wav/audio_pk_shibai.wav", false);
}
void AudioManager::pk_wenzi_effect(){
    this->play_effect("audio/wav/audio_pk_wenzi.wav", false);
}


// 进入公司播放
void AudioManager::enterTheTask_effect(){
    int index = 1 + rand()%3;
    if (index == 1) {
        this->play_effect("audio/wav/11.wav", false);
    }else if (index == 2){
        this->play_effect("audio/wav/12.wav", false);
    }else if (index == 3){
        this->play_effect("audio/wav/13.wav", false);
    }
}
// 进入家播放
void AudioManager::enterTheHome_effect(){
    int index = 1 + rand()%3;
    if (index == 1) {
        this->play_effect("audio/wav/21.wav", false);
    }else if (index == 2){
        this->play_effect("audio/wav/22.wav", false);
    }else if (index == 3){
        this->play_effect("audio/wav/23.wav", false);
    }
}
// 小手机出现播放
void AudioManager::enterThePhone_effect(){
    int index = 1 + rand()%4;
    if (index == 1) {
        this->play_effect("audio/wav/31.wav", false);
    }else if (index == 2){
        this->play_effect("audio/wav/32.wav", false);
    }else if (index == 3){
        this->play_effect("audio/wav/33.wav", false);
    }else if (index == 4){
        this->play_effect("audio/wav/34.wav", false);
    }
}
// 进入陌生人播放
void AudioManager::enterTheHaoyou_effect(){
    int index = 1 + rand()%3;
    if (index == 1) {
        this->play_effect("audio/wav/41.wav", false);
    }else if (index == 2){
        this->play_effect("audio/wav/42.wav", false);
    }else if (index == 3){
        this->play_effect("audio/wav/43.wav", false);
    }
}
// 高级衣服
void AudioManager::enterTheClothes_effect(){
    int index = 1 + rand()%4;
    if (index == 1) {
        this->play_effect("audio/wav/71.wav", false);
    }else if (index == 2){
        this->play_effect("audio/wav/72.wav", false);
    }else if (index == 3){
        this->play_effect("audio/wav/73.wav", false);
    }else if (index == 4){
        this->play_effect("audio/wav/74.wav", false);
    }
}













