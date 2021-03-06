//
//  PrivacyPolicyView.cpp
//  mm3c
//
//  Created by mac on 16/4/11.
//
//

#include "PrivacyPolicyView.h"
#include "DisplayManager.h"

PrivacyPolicyView::PrivacyPolicyView(){
    
}
PrivacyPolicyView::~PrivacyPolicyView(){
    
}

void PrivacyPolicyView::onEnter(){
    CCLayer::onEnter();
    this->init_view();
}

void PrivacyPolicyView::onExit(){
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeAllObservers(this);
    CCLayer::onExit();
}

bool PrivacyPolicyView::init(){
    if (!CCLayerColor::initWithColor(ccc4(0,0,0,0))) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->setPosition(this->getPosition() + ccp(0, 50));
    
    return true;
}

void PrivacyPolicyView::init_view() {
    dkSpr = CCSprite::create("pic/loginScene/pp_kuang.png");
    dkSpr->setPosition(DISPLAY->center());
    dkSpr->setScaleY(1.3f);
    this->addChild(dkSpr);
    
    CCSize dkSize = CCSizeMake(dkSpr->getContentSize().width * dkSpr->getScaleX(), dkSpr->getContentSize().height * dkSpr->getScaleY());
    
    CCSize scrollSize = CCSizeMake(dkSize.width * 0.94f, dkSize.height * 0.8f);
    CCScrollView* scroll = CCScrollView::create(scrollSize);
    scroll->setPosition(DISPLAY->center() - ccp(scrollSize.width * 0.5, scrollSize.height * 0.56));
    scroll->setDirection(kCCScrollViewDirectionVertical);
    this->addChild(scroll);
    
    CCSprite* wenziSpr = CCSprite::create("pic/loginScene/titile.png");
    wenziSpr->setPosition(DISPLAY->center() + ccp(0, dkSize.height * 0.47));
    this->addChild(wenziSpr);
    
    float fontSize = 24.f;
    long letterNum = 1840;
    float fontHeightConsideRatio = 0.46f;
    CCSize letterSize = CCSizeMake(scrollSize.width, letterNum / 17 * (fontSize * fontHeightConsideRatio));
    scroll->setContentSize(CCSizeMake(letterSize.width, letterSize.height + 20));
    
    const char* content = "隐私政策\n    本应用尊重并保护所有使用服务用户的个人隐私权。为了给您提供更准确、更有个性化的服务，本应用会按照本隐私权政策的规定使用和披露您的个人信息。但本应用将以高度的勤勉、审慎对待这些信息。除本隐私权政策另有规定外，在未征得您事先许可的情况下，本应用不会将这些信息对外披露或向第三方提供。本应用会不时更新本隐私权政策。\n    您在同意本应用服务使用协议之时，即视为您已经同意本隐私权政策全部内容。本隐私权政策属于本应用服务使用协议不可分割的一部分。\n1. 适用范围\na) 在您注册本应用帐号时，您根据本应用要求提供的个人注册信息；\nb) 在您使用本应用网络服务，或访问本应用平台网页时，本应用自动接收并记录的您的浏览器和计算机上的信息，包括但不限于您的IP地址、浏览器的类型、使用的语言、访问日期和时间、软硬件特征信息及您需求的网页记录等数据；\nc) 本应用通过合法途径从商业伙伴处取得的用户个人数据。\n您了解并同意，以下信息不适用本隐私权政策：\na) 您在使用本应用平台提供的搜索服务时输入的关键字信息；\nb) 本应用收集到的您在本应用发布的有关信息数据，包括但不限于参与活动、成交信息及评价详情；\nc) 违反法律规定或违反本应用规则行为及本应用已对您采取的措施。\n2. 信息使用\na)您的许可，或该第三方和本应用（含本应用关联公司）单独或共同为您提供服务，且在该服务结束后，其将被禁止访问包括其以前能够访问的所有这些资料。\nb) 本应用亦不允许任何第三方以任何手段收集、编辑、出售或者无偿传播您的个人信息。任何本应用平台用户如从事上述活动，一经发现，本应用有权立即终止与该用户的服务协议。\nc) 为服务用户的目的，本应用可能通过使用您的个人信息，向您提供您感兴趣的信息，包括但不限于向您发出产品和服务信息，或者与本应用合作伙伴共享信息以便他们向您发送有关其产品和服务的信息（后者需要您的事先同意）。\n3. 信息披露\n在如下情况下，本应用将依据您的个人意愿或法律的规定全部或部分的披露您的个人信息：\na) 经您事先同意，向第三方披露；\nb)为提供您所要求的产品和服务，而必须和第三方分享您的个人信息；\nc)根据法律的有关规定，或者行政或司法机构的要求，向第三方或者行政、司法机构披露；\nd) 如您出现违反中国有关法律、法规或者本应用服务协议或相关规则的情况，需要向第三方披露；\ne) 如您是适格的知识产权投诉人并已提起投诉，应被投诉人要求，向被投诉人披露，以便双方处理可能的权利纠纷；\nf) 在本应用平台上创建的某一交易中，如交易任何一方履行或部分履行了交易义务并提出信息披露请求的，本应用有权决定向该用户提供其交易对方的联络方式等必要信息，以促成交易的完成或纠纷的解决。\ng) 其它本应用根据法律、法规或者网站政策认为合适的披露。\n4. 信息存储和交换\n本应用收集的有关您的信息和资料将保存在本应用及（或）其关联公司的服务器上，这些信息和资料可能传送至您所在国家、地区或本应用收集信息和资料所在地的境外并在境外被访问、存储和展示。\n5. Cookie的使用\na) 在您未拒绝接受cookies的情况下，本应用会在您的计算机上设定或取用cookies，以便您能登录或使用依赖于cookies的本应用平台服务或功能。本应用使用cookies可为您提供更加周到的个性化服务，包括推广服务。\nb) 您有权选择接受或拒绝接受cookies。您可以通过修改浏览器设置的方式拒绝接受cookies。但如果您选择拒绝接受cookies，则您可能无法登录或使用依赖于cookies的本应用网络服务或功能。\nc) 通过本应用所设cookies所取得的有关信息，将适用本政策。\n6. 信息安全\na) 本应用帐号均有安全保护功能，请妥善保管您的用户名及密码信息。本应用将通过对用户密码进行加密等安全措施确保您的信息不丢失，不被滥用和变造。尽管有前述安全措施，但同时也请您注意在信息网络上不存在“完善的安全措施”。\nb) 在使用本应用网络服务进行网上交易时，您不可避免的要向交易对方或潜在的交易对方披露自己的个人信息，如联络方式或者邮政地址。请您妥善保护自己的个人信息，仅在必要的情形下向他人提供。如您发现自己的个人信息泄密，尤其是本应用用户名及密码发生泄露，请您立即联络本应用客服，以便本应用采取相应措施。";

    CCLabelTTF* lblContent = CCLabelTTF::create(content, DISPLAY->fangzhengFont(), fontSize, letterSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    lblContent->setColor(DISPLAY->defalutColor());
    scroll->setContainer(lblContent);
    scroll->setContentOffset(scroll->minContainerOffset());
    
//    CCLabelTTF* label = CCLabelTTF::create("点击空白处关闭", DefaultFont, 25);
//    label->setPosition(ccp(dkSpr->getContentSize().width* .8f, -10));
//    label->setColor(WhiteColor);
//    dkSpr->addChild(label);
    
    CCSprite* cancel1 = CCSprite::create("pic/common/btn_cancel.png");
    CCSprite* cancel2 = CCSprite::create("pic/common/btn_cancel.png");
    cancel2->setScale(DISPLAY->btn_scale());
    CCMenuItem* btnCancel = CCMenuItemSprite::create(cancel1, cancel2, this, SEL_MenuHandler(&PrivacyPolicyView::cancelCallback));
    
    CCSprite* agree1 = CCSprite::create("pic/common/btn_confirm.png");
    CCSprite* agree2 = CCSprite::create("pic/common/btn_confirm.png");
    agree2->setScale(DISPLAY->btn_scale());
    CCMenuItem* btnAgree = CCMenuItemSprite::create(agree1, agree2, this, SEL_MenuHandler(&PrivacyPolicyView::agreeCallback));
    
    CCMenu* menu = CCMenu::create(btnCancel, btnAgree, NULL);
    menu->alignItemsHorizontallyWithPadding(160);
    menu->setPosition(DISPLAY->halfW(), DISPLAY->H() * 0.81);
    this->addChild(menu);
}

bool PrivacyPolicyView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void PrivacyPolicyView::agreeCallback(CCObject* pSender){
    this->removeFromParentAndCleanup(true);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("Agree_PP");
}

void PrivacyPolicyView::cancelCallback(CCObject* pSender){
    this->removeFromParentAndCleanup(true);
}

