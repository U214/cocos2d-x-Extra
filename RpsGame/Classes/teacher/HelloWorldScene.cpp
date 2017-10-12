#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)) )
    {
        return false;
    }
    
    /////////////////////////////

    auto pMenuItem1 = MenuItemFont::create(
        "[ 가위 ]",
        CC_CALLBACK_1(HelloWorld::doClick, this));
    pMenuItem1->setColor(Color3B(0, 0, 0));

    auto pMenuItem2 = MenuItemFont::create(
        "[ 바위 ]",
        CC_CALLBACK_1(HelloWorld::doClick, this));
    pMenuItem2->setColor(Color3B(0, 0, 0));

    auto pMenuItem3 = MenuItemFont::create(
        "[  보  ]",
        CC_CALLBACK_1(HelloWorld::doClick, this));
    pMenuItem3->setColor(Color3B(0, 0, 0));

    auto pMenuItem4 = MenuItemFont::create(
        "[ 다시하기 ]",
        CC_CALLBACK_1(HelloWorld::doReset, this));
    pMenuItem4->setColor(Color3B(0, 0, 0));

    pMenuItem1->setTag(1);
    pMenuItem2->setTag(2);
    pMenuItem3->setTag(3);
    pMenuItem4->setTag(4);

    // 메뉴 생성
    auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, pMenuItem4, nullptr);

    // 메뉴 배치
    pMenu->setPosition(Vec2(0, 0));
    pMenuItem1->setPosition(Vec2(100, 90));
    pMenuItem2->setPosition(Vec2(240, 90));
    pMenuItem3->setPosition(Vec2(380, 90));
    pMenuItem4->setPosition(Vec2(240, 30));

    // 레이어에 메뉴 객체 추가
    this->addChild(pMenu);

    nUserScore = 0;
    nComScore = 0;

    pYourScore = LabelTTF::create("Your Score : 0", "Thonburi", 20);
    pYourScore->setAnchorPoint(Vec2(0, 0));
    pYourScore->setPosition(Vec2(50, 270));
    pYourScore->setColor(Color3B(0, 0, 255));

    this->addChild(pYourScore);


    pComScore = LabelTTF::create("Computer Score : 0", "Thonburi", 20);
    pComScore->setAnchorPoint(Vec2(1, 0));
    pComScore->setPosition(Vec2(430, 270));
    pComScore->setColor(Color3B(0, 0, 255));

    this->addChild(pComScore);

    pResult = LabelTTF::create("", "Thonburi", 34);
    pResult->setPosition(Vec2(240, 250));
    pResult->setColor(Color3B(255, 0, 0));

    this->addChild(pResult, 100);

    srand((int)time(NULL));


    return true;
}

void HelloWorld::doClick(Ref* pSender)
{
    // 기존의 가위바위보 그림 지우기
    auto pTemp1 = (Sprite*)getChildByTag(11);
    if (pTemp1 != NULL) {
        this->removeChild(pTemp1, true);
    }

    auto pTemp2 = (Sprite*)getChildByTag(12);
    if (pTemp2 != NULL) {
        this->removeChild(pTemp2, true);
    }

    // 사용자 가위바위보 표시
    auto tItem = (MenuItem *)pSender;
    int nUserNum = tItem->getTag();
    Sprite* pSprite;

    switch (nUserNum) {
    case 1:
        pSprite = Sprite::create("Images/img1.png");
        pSprite->setPosition(Vec2(120, 190));
        break;

    case 2:
        pSprite = Sprite::create("Images/img2.png");
        pSprite->setPosition(Vec2(120, 190));
        break;

    case 3:
        pSprite = Sprite::create("Images/img3.png");
        pSprite->setPosition(Vec2(120, 190));
        break;

    default:
        break;
    }

    pSprite->setFlippedX(true);
    pSprite->setTag(11);

    this->addChild(pSprite);

    // 컴퓨터 가위바위보 표시
    int nComNum = (rand() % 3) + 1;
    Sprite* pSprite2;
    switch (nComNum) {
    case 1:
        pSprite2 = Sprite::create("Images/img1.png");
        pSprite2->setPosition(Vec2(360, 190));
        break;

    case 2:
        pSprite2 = Sprite::create("Images/img2.png");
        pSprite2->setPosition(Vec2(360, 190));
        break;

    case 3:
        pSprite2 = Sprite::create("Images/img3.png");
        pSprite2->setPosition(Vec2(360, 190));
        break;

    default:
        break;
    }

    pSprite2->setTag(12);

    this->addChild(pSprite2);

    // 승패 표시
    int n = nUserNum - nComNum;

    if (n == 1 || n == -2) {
        // 사용자 이김
        nUserScore = nUserScore + 1;
        pResult->setString("당신이 이겼습니다.");
    }
    else if (n == 0) {
        // 비김
        pResult->setString("비겼습니다.");
    }
    else {
        // 컴퓨터 이김
        nComScore = nComScore + 1;
        pResult->setString("컴퓨터가 이겼습니다.");
    }

    this->doShowScore();
}

void HelloWorld::doShowScore()
{
//    std::stringstream out1;
//    out1 << nUserScore;
//    std::string myNum = "Your Score : " + out1.str();
//    pYourScore->setString(myNum.c_str());

    char myNum[20] = { 0 };
    sprintf(myNum, "Your Score : %d", nUserScore);
    pYourScore->setString(myNum);

    std::stringstream out2;
    out2 << nComScore;
    std::string comNum = "Computer Score : " + out2.str();
    pComScore->setString(comNum.c_str());
}

void HelloWorld::doReset(Ref* pSender)
{
    this->removeChildByTag(11, true);
    this->removeChildByTag(12, true);
    pResult->setString("");

    nUserScore = 0;
    nComScore = 0;
    this->doShowScore();
}
