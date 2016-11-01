//
//  TrystDialogState.cpp
//  tiegao
//
//  Created by mac on 16/11/1.
//
//

#include "TrystDialogState.h"

// Export
const char* TrystDialogState::StateInit = "INIT";
const char* TrystDialogState::StateIdle = "IDLE";
const char* TrystDialogState::StatePlaying = "PLAYING";
const char* TrystDialogState::StateEnd = "END";

TrystDialogState::~TrystDialogState() {

}

TrystDialogState::TrystDialogState()
: _curState(TrystDialogState::StateInit) {

}

std::string const TrystDialogState::curState() {
    return _curState;
}

void TrystDialogState::transform(const char *eventName) {
    if (strcmp(eventName, TrystDialogState::StatePlaying) == 0) {
        if (this->tryPlaying()) {
            _curState = eventName;
        }
    }
    else if (strcmp(eventName, TrystDialogState::StateIdle) == 0) {
        if (this->tryIdle()) {
            _curState = eventName;
        }
    }
    else if (strcmp(eventName, TrystDialogState::StateEnd) == 0) {
        if (this->tryEnd()) {
            _curState = eventName;
        }
    }
    else {
        
    }
}

// Inner

bool TrystDialogState::tryPlaying() {
    return true;
}

bool TrystDialogState::tryIdle() {
    return true;
}

bool TrystDialogState::tryEnd() {
    return true;
}

