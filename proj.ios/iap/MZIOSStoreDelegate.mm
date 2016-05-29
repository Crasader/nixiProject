//
//  MZIOSStoreDelegate.cpp
//  mm3c
//
//  Created by lakkey on 14-9-25.
//
//

#include "MZIOSStoreDelegate.h"

#include "native/CCNative.h"
#include "network/CCNetwork.h"
#include "store/CCStoreProduct.h"

#include "NetManager.h"
#include "DataManager.h"

#import "MZIOSStoreUtil.h"


MZIOSStoreDelegate::~MZIOSStoreDelegate() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    if (_products) {
        CC_SAFE_RELEASE_NULL(_products);
    }
    if (_strOrder) {
        CC_SAFE_RELEASE_NULL(_strOrder);
    }
    if (_transactions) {
        CC_SAFE_DELETE(_transactions);
    }
    //
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool MZIOSStoreDelegate::init() {
    _transactions = CCDictionary::create();
    _transactions->retain();
    //
    CCStore::sharedStore()->postInitWithTransactionObserver(this);
    //
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&MZIOSStoreDelegate::notification_http_error), NOTIFICATION_HTTP_ERROR, NULL);
//    nc->addObserver(this, SEL_CallFuncO(&MZIOSStoreDelegate::notification_http_1001), NOTIFICATION_1001_DONE, NULL);
    
    return true;
}


#pragma mark - =================== CCStoreProductsRequestDelegate ===================


void MZIOSStoreDelegate::requestProductsFailed(int errorCode, const char *errorString) {
    CCString* strError = CCString::createWithFormat("requestProductsFailed: %s", errorString);
    CCNative::createAlert("IAP Error", strError->getCString(), "OK");
    CCNative::showAlert();
}

void MZIOSStoreDelegate::requestProductsCompleted(CCArray *products, CCArray* invalidProductsId) {
    products->retain();
    if (products) {
        this->setProducts(products);
        this->printProducts();
    }
    products->release();

    CCStoreProduct* pro = (CCStoreProduct* )this->getProducts()->objectAtIndex(0);
    CCStore::sharedStore()->purchase(pro->getProductIdentifier().c_str());
}


#pragma mark - =================== CCStoreTransactionObserver ===================

void MZIOSStoreDelegate::transactionCompleted(CCStorePaymentTransaction* transaction) {
    if (!transaction) {
        CCLOG("MZIOSStoreDelegate::transactionCompleted but error: transactin is null");
        return;
    }
    
    if (transaction->getErrorCode() != 0) {
        CCLOG("MZIOSStoreDelegate::transactionCompleted error code:%d, reason:%s", transaction->getErrorCode(), transaction->getErrorDescription().c_str());
        return;
    }
    
    CCLOG("Transaction state ======== %d", transaction->getTransactionState());
    if (transaction->getTransactionState() == 2) {
        // 保存到字典，等待Verify
        _transactions->setObject(transaction, transaction->getTransactionIdentifier());
        this->verifyTransaction(transaction->getProductIdentifier().c_str(),  transaction->getTransactionIdentifier().c_str(), [MZIOSStoreUtil getReceiptByProductId:transaction]);
    }
}

void MZIOSStoreDelegate::transactionFailed(CCStorePaymentTransaction* transaction) {
    CCLOG("MZIOSStoreDelegate::transactionFailed~");
    CCStore::sharedStore()->finishTransaction(transaction);
    CCNative::createAlert("IAP Error", transaction->getErrorDescription().c_str(), "OK");
    CCNative::showAlert();
}

void MZIOSStoreDelegate::transactionRestored(CCStorePaymentTransaction* transaction) {
    CCLOG("MZIOSStoreDelegate::transactionRestored~");
    CCStore::sharedStore()->finishTransaction(transaction);
}


#pragma mark - =================== http notif ===================

void MZIOSStoreDelegate::notification_http_error(cocos2d::CCObject *pObj) {
//    AHLoading::stop();
//    long err = (long)pObj;
//    CCString* error_message = MZAppUtils::http_error_message_with_status_code((HTTP_ERROR)err);
//    AHMessageBox* mb = AHMessageBox::create_with_message(error_message->getCString(), NULL, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YES, false);
//    mb->setPosition(ccp(ScreenWidth * 0.5f, ScreenHeight * 0.5f));
//    CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 3000);
}

void MZIOSStoreDelegate::notification_http_1001(cocos2d::CCObject *pObj) {
    CCString* goods_id = (CCString*)pObj;
    CCStorePaymentTransaction* transaction = (CCStorePaymentTransaction*)_transactions->objectForKey(goods_id->getCString());
    if (transaction) {
        _transactions->removeObjectForKey(goods_id->getCString());
        CCStore::sharedStore()->finishTransaction(transaction);
    }
    
//    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_IOS_IAP_BUYPRODUCT_FINISHED);
}

#pragma mark - =================== other ===================

bool MZIOSStoreDelegate::canMakePurchases() {
    return CCStore::sharedStore()->canMakePurchases();
}

void MZIOSStoreDelegate::loadProducts(CCArray* product_ids) {
    if (kCCNetworkStatusNotReachable == CCNetwork::getInternetConnectionStatus()) {
        CCNative::createAlert("IAP Error", "无网络可用!", "OK");
        CCNative::showAlert();
    }
    else if (!CCStore::sharedStore()->canMakePurchases()) {
        CCNative::createAlert("IAP Error", "无法使用IAP!", "OK");
        CCNative::showAlert();
    }
    else {
//        AHLoading::showLoading();
        CCStore::sharedStore()->loadProducts(product_ids, this);
    }
}

bool MZIOSStoreDelegate::isProductLoaded(const char *productId) {
    return CCStore::sharedStore()->isProductLoaded(productId);
}

void MZIOSStoreDelegate::buyProductByIndex(int productIndex) {
    CCStoreProduct* pro = (CCStoreProduct*)_products->objectAtIndex(productIndex);
    if (CCStore::sharedStore()->isProductLoaded(pro->getProductIdentifier().c_str())) {
        this->buyProduct(pro->getProductIdentifier().c_str());
    }
    else {
        CCNative::createAlert("IAP Error", "没有该商品", "OK");
        CCNative::showAlert();
    }
}

// 修改为每次购买只load对应商品的信息，load成功发起支付
void MZIOSStoreDelegate::buyProduct(const char* productId) {
    this->loadProducts(CCArray::create(ccs(productId), NULL));
}

void MZIOSStoreDelegate::verifyTransaction(const char* proID,
                                           const char* transcationID,
                                           const char* receipt) {
//    MMNetManager::get_instance()->http_1001_check_iOS_order(transcationID, proID, receipt);
}

void MZIOSStoreDelegate::printProducts() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_products, pObj) {
        CCStoreProduct* pro = (CCStoreProduct*)pObj;
        CCLOG("========== product =========");
        CCLOG("ProductIdentifier: %s", pro->getProductIdentifier().c_str());
        CCLOG("price: %f", pro->getPrice());
        CCLOG("PriceLocale: %s", pro->getPriceLocale().c_str());
        CCLOG("LocalizedTitle: %s", pro->getLocalizedTitle().c_str());
        CCLOG("LocalizedDescription: %s", pro->getLocalizedDescription().c_str());
    }
    CCLOG("---------- product ---------");
}

