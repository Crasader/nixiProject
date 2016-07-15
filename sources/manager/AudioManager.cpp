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

AudioManager::~AudioManager()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

void AudioManager::init(bool music, bool effect)
{
    // 背景音量
    this->setBackgroundMusicVolume(1.f);
    // 音效音量
    this->setEffectsVolume(1.f);
    
    CCUserDefault* user_default = CCUserDefault::sharedUserDefault();
    this->set_music_on(user_default->getBoolForKey(kUDK_Music_Bool, true));
    this->set_effect_on(user_default->getBoolForKey(kUDK_Effect_Bool, true));
    
    // 开启关闭背景音
    this->set_music_on(music);
    // 开启关闭音效
    this->set_effect_on(effect);
}


AudioManager* AudioManager::Inst()
{
    if (!_instance) {
        _instance = new AudioManager();
        _instance->preload_audios();
    }
    
    return _instance;
}


void AudioManager::preload_audios()
{
/*
    this->preloadBackgroundMusic(kMusic_BG_Lives);
    this->preloadBackgroundMusic(kMusic_BG_Choose);
    this->preloadBackgroundMusic(kMusic_BG_Qingjing);
    //
    // 穿衣服 音效
    this->preloadEffect(kAudio_Button_Clothes);
    // 通用按键 音效
    this->preloadEffect(kAudio_Button_Common);
    // 通用 错误 音效
    this->preloadEffect(kAudio_Button_errorCommon);
    // 通用 按钮 返回声音
    this->preloadEffect(kAudio_Button_Back);
    // 通用 确认 音效
    this->preloadEffect(kAudio_AH_YES);
    // 购买 音效（花钱的声音）
    this->preloadEffect(kAudio_Button_buy);
    // 正确 音效
    this->preloadEffect(kAudio_right);
    // 商城 音效
    this->preloadEffect(kAudio_Button_shop);
    // 升级 音效
    this->preloadEffect(kAudio_levelup);
    // 任务成功 音效
    this->preloadEffect(kAudio_task_success);
    // 剧情 失败 音效
    this->preloadEffect(kAudio_failure);
    // 剧情 成功 音效
    this->preloadEffect(kAudio_success);
    // 任务结算 音效
    this->preloadEffect(kAudio_a);
    this->preloadEffect(kAudio_b);
    this->preloadEffect(kAudio_s);
    this->preloadEffect(kAudio_ss);
    this->preloadEffect(kAudio_sss);
    
    // 正确 音效
    this->preloadEffect(kAudio_game_right);
    // 错误 音效
    this->preloadEffect(kAudio_game_wrong);
    // 保养 音效
    this->preloadEffect(kAudio_game_baoyang_1_4);
    this->preloadEffect(kAudio_game_baoyang_2);
    this->preloadEffect(kAudio_game_baoyang_3);
    // 垃圾 音效
    this->preloadEffect(kAudio_game_housework_1);
    // music 音效
    this->preloadEffect(kAudio_game_music_1);
    this->preloadEffect(kAudio_game_music_2);
    this->preloadEffect(kAudio_game_music_3);
    this->preloadEffect(kAudio_game_music_4);
    // 窗口弹出 音效
    this->preloadEffect(kAudio_audio_AHMessageBox);
    // 睡觉 音效
    this->preloadEffect(kAudio_game_sleep_1);
*/
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
    
    if (!m_is_music_on) {
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

