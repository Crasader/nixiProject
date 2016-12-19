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
#include "Loading2.h"

#import "IOSIAPUtil.h"

static IOSIAPManager* _instance = nullptr;

#define APPSTORE_RECEIPT_VERIFY_URL "https://buy.itunes.apple.com/verifyReceipt"
#define SANDBOX_RECEIPT_VERIFY_URL  "https://sandbox.itunes.apple.com/verifyReceipt"

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
        
        CCNotificationCenter::sharedNotificationCenter()->addObserver(_instance, SEL_CallFuncO(&IOSIAPManager::nc_verify_iOS_133), "HTTP_FINISHED_133", NULL);
        
        CCStore::sharedStore()->setReceiptVerifyMode(CCStoreReceiptVerifyModeDevice);
        CCStore::sharedStore()->setReceiptVerifyServerUrl(APPSTORE_RECEIPT_VERIFY_URL);
        
//        CCDictionary* conf = DATA->getLogin()->config();
//        CCInteger* formal = (CCInteger*)conf->objectForKey("formal");
//        if (formal->getValue() == 0) {
//            CCStore::sharedStore()->setReceiptVerifyMode(CCStoreReceiptVerifyModeDevice);
//            CCStore::sharedStore()->setReceiptVerifyServerUrl(SANDBOX_RECEIPT_VERIFY_URL);
//        }
//        else {
//            CCStore::sharedStore()->setReceiptVerifyMode(CCStoreReceiptVerifyModeDevice);
//            CCStore::sharedStore()->setReceiptVerifyServerUrl(APPSTORE_RECEIPT_VERIFY_URL);
//        }
        
        CCStore::sharedStore()->postInitWithTransactionObserver(_instance);
        CCLOG("URL:: %s", CCStore::sharedStore()->getReceiptVerifyServerUrl());
    }
    
    return _instance;
}

#pragma mark - =================== CCStoreProductsRequestDelegate ===================


void IOSIAPManager::requestProductsFailed(int errorCode, const char *errorString) {
    LOADING->remove();
    
    CCString* strError = CCString::createWithFormat("requestProductsFailed: %s", errorString);
    CCNative::createAlert("支付中断", "", "OK");
    CCNative::showAlert();
}

void IOSIAPManager::requestProductsCompleted(CCArray *products, CCArray* invalidProductsId) {
    products->retain();
    if (products) {
        this->setProducts(products);
//        this->printProducts();
    }
    products->release();
    
    
    if (products->count() > 0) {
        CCStoreProduct* pro = (CCStoreProduct* )this->getProducts()->objectAtIndex(0);
        CCStore::sharedStore()->purchase(pro->getProductIdentifier().c_str());
    }
    else {
        LOADING->remove();
        
        CCNative::createAlert("支付中断", "没有该商品", "OK");
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
        if (transaction->getReceiptVerifyStatus() == 0) { // 本机验证成功
            std::string orderId = transaction->getTransactionIdentifier();
            std::string productId = transaction->getProductIdentifier();
//            if (productId.compare("tiegao_story") == 0) {
//                CCLOG("购买剧情~");
//                DATA->onChargeRequest(orderId, productId, 0, 0);
//                DATA->onChargeSuccess(orderId);
//                CCNotificationCenter::sharedNotificationCenter()->postNotification("IOS_BUY_FINISHED");
//            }
//            else {
            // 保存待服务器验证的订单
            _transactions->setObject(transaction, orderId);
            // 服务器二次验证
            NET->verify_order_iOS_133(orderId, productId, [IOSIAPUtil getReceiptByProductId:transaction]);
            this->printTransaction(transaction);
            
//            }

        }
        else {
            CCNative::createAlert("支付失败", "验证失败~!", "OK");
            CCNative::showAlert();
        }
    }
}

void IOSIAPManager::transactionFailed(CCStorePaymentTransaction* transaction) {
    CCLOG("IOSIAPManager::transactionFailed~");
    LOADING->remove();
    
    CCStore::sharedStore()->finishTransaction(transaction);
    CCNative::createAlert("支付失败", "", "OK");
    CCNative::showAlert();
}

void IOSIAPManager::transactionRestored(CCStorePaymentTransaction* transaction) {
    CCLOG("IOSIAPManager::transactionRestored~");
    LOADING->remove();
    
    CCStore::sharedStore()->finishTransaction(transaction);
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
        CCNative::createAlert("支付失败", "无网络可用!", "OK");
        CCNative::showAlert();
    }
    else if (!CCStore::sharedStore()->canMakePurchases()) {
        CCNative::createAlert("支付失败", "无法使用IAP!", "OK");
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

void IOSIAPManager::nc_verify_iOS_133(CCObject* pObj) {
    CCString* orderId = (CCString* )pObj;
    CCStorePaymentTransaction* transaction = (CCStorePaymentTransaction* )_transactions->objectForKey(orderId->getCString());
    if (transaction) {
        CCStore::sharedStore()->finishTransaction(transaction);
    }
    //
    LOADING->remove();
    
    string orderId2 = DATA->getLogin()->obtain_UUID();
    DATA->onChargeSuccess(orderId2);
    
    // TalkingData
    DATA->onChargeSuccess(orderId->getCString());
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

void IOSIAPManager::printTransaction(CCStorePaymentTransaction* trans) {
    CCLOG("========== Transaction =========");
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
