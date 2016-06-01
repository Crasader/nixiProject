//
//  IOSIAPManager.cpp
//  mm3c
//
//  Created by lakkey on 14-9-25.
//
//

#include "IOSIAPManager.h"

#include "native/CCNative.h"
#include "network/CCNetwork.h"
#include "store/CCStoreProduct.h"

#include "NetManager.h"
#include "DataManager.h"

#import "IOSIAPUtil.h"

static IOSIAPManager* _instance = nullptr;

IOSIAPManager::~IOSIAPManager() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    if (_products) {
        CC_SAFE_RELEASE_NULL(_products);
    }
    if (_transactions) {
        CC_SAFE_DELETE(_transactions);
    }
    //
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

IOSIAPManager* IOSIAPManager::Inst() {
    if (_instance == nullptr) {
        _instance = new IOSIAPManager();
        _instance->_transactions = CCDictionary::create();
        _instance->_transactions->retain();
        CCStore::sharedStore()->postInitWithTransactionObserver(_instance);
    }
    
    return _instance;
}

#pragma mark - =================== CCStoreProductsRequestDelegate ===================


void IOSIAPManager::requestProductsFailed(int errorCode, const char *errorString) {
    CCString* strError = CCString::createWithFormat("requestProductsFailed: %s", errorString);
    CCNative::createAlert("IAP Error", strError->getCString(), "OK");
    CCNative::showAlert();
}

void IOSIAPManager::requestProductsCompleted(CCArray *products, CCArray* invalidProductsId) {
    products->retain();
    if (products) {
        this->setProducts(products);
        this->printProducts();
    }
    products->release();
    
    if (products->count() > 0) {
        CCStoreProduct* pro = (CCStoreProduct* )this->getProducts()->objectAtIndex(0);
        CCStore::sharedStore()->purchase(pro->getProductIdentifier().c_str());
    }
    else {
        CCNative::createAlert("IAP Error", "没有该商品", "OK");
        CCNative::showAlert();
    }
}


#pragma mark - =================== CCStoreTransactionObserver ===================

void IOSIAPManager::transactionCompleted(CCStorePaymentTransaction* transaction) {
    if (!transaction) {
        CCLOG("IOSIAPManager::transactionCompleted but error: transactin is null");
        return;
    }
    
    CCLOG("IOSIAPManager::transactionCompleted() state ======== %d", transaction->getTransactionState());
    if (transaction->getErrorCode() != 0) {
        CCLOG("IOSIAPManager::transactionCompleted error code:%d, reason:%s", transaction->getErrorCode(), transaction->getErrorDescription().c_str());
        return;
    }
    
    if (transaction->getTransactionState() == 2) {
        this->verifyTransaction(transaction->getProductIdentifier().c_str(),  transaction->getTransactionIdentifier().c_str(), [IOSIAPUtil getReceiptByProductId:transaction]);
    }
    
    this->printTransactions();
}

void IOSIAPManager::transactionFailed(CCStorePaymentTransaction* transaction) {
    CCLOG("IOSIAPManager::transactionFailed~");
    CCStore::sharedStore()->finishTransaction(transaction);
    CCNative::createAlert("IAP Error", transaction->getErrorDescription().c_str(), "OK");
    CCNative::showAlert();
}

void IOSIAPManager::transactionRestored(CCStorePaymentTransaction* transaction) {
    CCLOG("IOSIAPManager::transactionRestored~");
    CCStore::sharedStore()->finishTransaction(transaction);
}


#pragma mark - =================== http notif ===================

void IOSIAPManager::notification_http_error(cocos2d::CCObject *pObj) {
//    AHLoading::stop();
//    long err = (long)pObj;
//    CCString* error_message = MZAppUtils::http_error_message_with_status_code((HTTP_ERROR)err);
//    AHMessageBox* mb = AHMessageBox::create_with_message(error_message->getCString(), NULL, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YES, false);
//    mb->setPosition(ccp(ScreenWidth * 0.5f, ScreenHeight * 0.5f));
//    CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 3000);
}

void IOSIAPManager::notification_http_1001(cocos2d::CCObject *pObj) {
    CCString* goods_id = (CCString*)pObj;
    CCStorePaymentTransaction* transaction = (CCStorePaymentTransaction*)_transactions->objectForKey(goods_id->getCString());
    if (transaction) {
        _transactions->removeObjectForKey(goods_id->getCString());
        CCStore::sharedStore()->finishTransaction(transaction);
    }
    
//    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_IOS_IAP_BUYPRODUCT_FINISHED);
}

#pragma mark - =================== other ===================

bool IOSIAPManager::canMakePurchases() {
    return CCStore::sharedStore()->canMakePurchases();
}

void IOSIAPManager::buyProductByIndex(int productIndex) {
    CCStoreProduct* pro = (CCStoreProduct*)_products->objectAtIndex(productIndex);
    this->buyProduct(pro->getProductIdentifier().c_str());
}

// 修改为每次购买只load对应商品的信息，load成功发起支付
void IOSIAPManager::buyProduct(const char* productId) {
    if (kCCNetworkStatusNotReachable == CCNetwork::getInternetConnectionStatus()) {
        CCNative::createAlert("IAP Error", "无网络可用!", "OK");
        CCNative::showAlert();
    }
    else if (!CCStore::sharedStore()->canMakePurchases()) {
        CCNative::createAlert("IAP Error", "无法使用IAP!", "OK");
        CCNative::showAlert();
    }
    else {
        this->loadProducts(CCArray::create(ccs(productId), NULL));
    }
}

void IOSIAPManager::loadProducts(CCArray* product_ids) {
    CCStore::sharedStore()->loadProducts(product_ids, this);
}

bool IOSIAPManager::isProductLoaded(const char *productId) {
    return CCStore::sharedStore()->isProductLoaded(productId);
}

void IOSIAPManager::verifyTransaction(const char* proID,
                                           const char* transcationID,
                                           const char* receipt) {
//    MMNetManager::get_instance()->http_1001_check_iOS_order(transcationID, proID, receipt);
}

void IOSIAPManager::printProducts() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_products, pObj) {
        CCStoreProduct* pro = (CCStoreProduct*)pObj;
        CCLOG("========== product =========");
        CCLOG("ProductIdentifier: %s", pro->getProductIdentifier().c_str());
        CCLOG("price: %f", pro->getPrice());
        CCLOG("PriceLocale: %s", pro->getPriceLocale().c_str());
        CCLOG("LocalizedTitle: %s", pro->getLocalizedTitle().c_str());
        CCLOG("LocalizedDescription: %s", pro->getLocalizedDescription().c_str());
        CCLOG("---------- product ---------");
    }
}

void IOSIAPManager::printTransactions() {
    CCDictElement* pElem = NULL;
    CCDICT_FOREACH(_transactions, pElem) {
        CCStorePaymentTransaction* trans = (CCStorePaymentTransaction* )pElem->getObject();
        CCLOG("========== Transaction <prod_id: %s> =========", pElem->getStrKey());
        CCLOG("state: %d", trans->getTransactionState());
        CCLOG("transactionIdentifier: %s", trans->getTransactionIdentifier().c_str());
        CCLOG("quantity: %d", trans->getQuantity());
        CCLOG("date: %f", trans->getDateTime());
        CCLOG("errorCode: %d", trans->getErrorCode());
        CCLOG("errorString: %s", trans->getErrorDescription().c_str());
        CCLOG("receiptVerifyMode: %d", trans->getReceiptVerifyMode());
        CCLOG("receiptVerifyStatus: %d", trans->getReceiptVerifyStatus());
        CCLOG("---------- Transaction ---------");
    }
}
