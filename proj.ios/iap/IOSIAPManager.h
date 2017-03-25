//
//  IOSIAPManager.h
//  mm3c
//
//  Created by lakkey on 14-9-25.
//
//

#ifndef __mm3c__IOSIAPManager__
#define __mm3c__IOSIAPManager__

#include "cocos2d.h"
#include "store/CCStore.h"


USING_NS_CC;
USING_NS_CC_EXTRA;


class IOSIAPManager
    : public CCObject
    , public CCStoreProductsRequestDelegate
    , public CCStoreTransactionObserver
{
public:
    ~IOSIAPManager();
    static IOSIAPManager* Inst();
    
    bool canMakePurchases();
    
    virtual void requestProductsCompleted(cocos2d::CCArray* products,
                                          cocos2d::CCArray* invalidProductsId = NULL);
    virtual void requestProductsFailed(int errorCode, const char* errorString);
    
    virtual void transactionCompleted(CCStorePaymentTransaction* transaction);
    virtual void transactionFailed(CCStorePaymentTransaction* transaction);
    virtual void transactionRestored(CCStorePaymentTransaction* transaction);
    
public:
    void loadProducts(CCArray* product_ids);
    bool isProductLoaded(const char *productId);

    void buyProductByIndex(int productIndex);
    void buyProduct(const char* productId);
//    void verifyTransaction(const char* proID, const char* transcationID, const char* receipt);
    void printProducts();
    void printTransaction(CCStorePaymentTransaction* transaction);

private:
    void nc_verify_iOS_133(CCObject* pObj);
    void nc_verify_iOS_card2_155(CCObject* pObj);
    
private:
    CC_SYNTHESIZE_RETAIN(CCArray*, _products, Products);
    CCDictionary*           _transactions; // 保存待Verify的交易
};

#endif /* defined(__mm3c__IOSIAPManager__) */
