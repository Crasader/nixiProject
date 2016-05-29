//
//  MZIOSStoreDelegate.h
//  mm3c
//
//  Created by lakkey on 14-9-25.
//
//

#ifndef __mm3c__MZIOSStoreDelegate__
#define __mm3c__MZIOSStoreDelegate__

#include "cocos2d.h"
#include "store/CCStore.h"


USING_NS_CC;
USING_NS_CC_EXTRA;


class MZIOSStoreDelegate
    : public CCObject
    , public CCStoreProductsRequestDelegate
    , public CCStoreTransactionObserver
{
public:
    CREATE_FUNC(MZIOSStoreDelegate);
    ~MZIOSStoreDelegate();
    virtual bool init();
    
    bool canMakePurchases();
    
    virtual void requestProductsCompleted(cocos2d::CCArray* products,
                                          cocos2d::CCArray* invalidProductsId = NULL);
    virtual void requestProductsFailed(int errorCode, const char* errorString);
    
    virtual void transactionCompleted(CCStorePaymentTransaction* transaction);
    virtual void transactionFailed(CCStorePaymentTransaction* transaction);
    virtual void transactionRestored(CCStorePaymentTransaction* transaction);
    
    void loadProducts(CCArray* product_ids);
    bool isProductLoaded(const char *productId);

    void buyProductByIndex(int productIndex);
    void buyProduct(const char* productId);
    void verifyTransaction(const char* proID, const char* transcationID, const char* receipt);

    //Special Mothed
    void buyYiyuangou();
    //
    void printProducts();
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _products, Products)
    CC_SYNTHESIZE_RETAIN(CCString*, _strOrder, StrOrder);

protected:
    void notification_http_error(CCObject* pObj);
    void notification_http_1001(CCObject* pObj);
    
    CCDictionary* _transactions; // 保存待Verify的交易
private:
};

#endif /* defined(__mm3c__MZIOSStoreDelegate__) */
