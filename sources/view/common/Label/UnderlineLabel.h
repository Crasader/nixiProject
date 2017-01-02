//
//  UnderlineLabel.hpp
//  tiegao
//
//  Created by mac on 17/1/2.
//
//

#ifndef UnderlineLabel_hpp
#define UnderlineLabel_hpp

#include "cocos2d.h"

USING_NS_CC;

class UnderlineLabel : public CCLabelTTF
{
public:
    void setUnderlineColor(const ccColor4B& color);
    void setUnderlineEnabled(bool isEnabled);
    void setUnderlineSize(float size);
    void setFrontInterval(float interval);
    void setBehindInterval(float interval);
    void setUnderlineHeight(float height);
    
public:
    ~UnderlineLabel();
    static UnderlineLabel * create(const char *string, const char *fontName, float fontSize);
    bool initWithString(const char *string, const char *fontName, float fontSize);
    
    virtual void draw();
    
private:
    ccColor4B       _underlineColor;
    bool            _isUnderlineEnabled;
    float           _lineSize;
    float           _frontInterval;
    float           _behindInterval;
    float           _lineHeight;
};

#endif /* UnderlineLabel_hpp */
