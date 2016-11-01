//
//  TrystDialogState.hpp
//  tiegao
//
//  Created by mac on 16/11/1.
//
//

#ifndef TrystDialogState_hpp
#define TrystDialogState_hpp

#include <string>

class TrystDialogState
{
public: // Export
    static const char* StateInit;
    static const char* StateIdle;
    static const char* StatePlaying;
    static const char* StateEnd;
    
public: // Export
    ~TrystDialogState();
    TrystDialogState();
    
    std::string const curState();
    void transform(const char* eventName);
    
private:
    bool tryPlaying();
    bool tryIdle();
    bool tryEnd();
    
private:
    std::string         _curState;
};

#endif /* TrystDialogState_hpp */
