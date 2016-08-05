//
//  extend_define.h
//  tiegao
//
//  Created by mac on 16/7/16.
//
//

#ifndef extend_define_h
#define extend_define_h

#define SYNTHESIZE_PASS_BY_REF_READONLY(varType, varName, funName) \
protected: varType varName; \
public: virtual const varType& get##funName(void) const { return varName; } \
private: virtual void set##funName(const varType& var){ varName = var; }


#define SYNTHESIZE_READONLY(varType, varName, funName) \
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
private: virtual void set##funName(varType var){ varName = var; }


#define SYNTHESIZE_RETAIN_READONLY(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
private: virtual void set##funName(varType var)   \
{ \
if (varName != var) \
{ \
CC_SAFE_RETAIN(var); \
CC_SAFE_RELEASE(varName); \
varName = var; \
} \
}

#endif /* extend_define_h */
