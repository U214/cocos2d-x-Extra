#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

bool HelloWorld::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);

	/////////////////////////////////
	// 배경 이미지 추가
	auto pBackgroud = Sprite::create("Images/BB_Source_TITLE/Bckground/Background.png");
	pBackgroud->setPosition(Vec2(320, 480));
	this->addChild(pBackgroud);

	// 타이틀로고 이미지 추가 (z-order : 6)
	auto pLogoLight = Sprite::create("Images/BB_Source_TITLE/Flash_Effect/Logo_Light.png");
	pLogoLight->setPosition(Vec2(335, 685));
	this->addChild(pLogoLight, 6);

	auto pTitleLogo = Sprite::create("Images/BB_Source_TITLE/Game_Logo/Bubble_Logo.png");
	pTitleLogo->setPosition(Vec2(
		pLogoLight->getContentSize().width / 2 - 30,
		pLogoLight->getContentSize().height / 2));
	pLogoLight->addChild(pTitleLogo);

	auto actionUpDown = Sequence::create(
		MoveBy::create(2.0, Vec2(0, 25)),
		MoveBy::create(2.0, Vec2(0, -25)),
		NULL);

	pTitleLogo->runAction(RepeatForever::create(actionUpDown));
	pLogoLight->runAction(RepeatForever::create(actionFadeInOut(1.0)));

	// 시작 버튼 이미지 
	auto pPlayBtn = MenuItemImage::create(
		"Images/BB_Source_TITLE/Bubble_Button/Play_Button/Play_Button_01.png",
		"Images/BB_Source_TITLE/Bubble_Button/Play_Button/Play_Button_02.png",
		CC_CALLBACK_1(HelloWorld::doClick, this));
	pPlayBtn->setPosition(Vec2(320, 285));
	pPlayBtn->setGlobalZOrder(3);

	// 진동 버튼 이미지
	auto pVibarBtn = MenuItemImage::create(
		"Images/BB_Source_TITLE/Bubble_Button/Setting_Button/Vibration_ON.png", 
		"Images/BB_Source_TITLE/Bubble_Button/Setting_Button/Vibration_OFF.png",
		CC_CALLBACK_1(HelloWorld::doClick, this));
	pVibarBtn->setPosition(Vec2(450, 160));
	pVibarBtn->setGlobalZOrder(4);
	pVibarBtn->setTag(1);

	//  시작 & 진동 버튼 주변 라이트 추가 (z-order : 2)
	auto pPlayLight = Sprite::create("Images/BB_Source_TITLE/Bubble_Button/Base.png");
	pPlayLight->setPosition(Vec2(347, 260));
	this->addChild(pPlayLight, 6);

	auto repBtnFade = RepeatForever::create(actionFadeInOut(0.85));
	pPlayLight->runAction(repBtnFade);

	// 정보 버튼 이미지
	auto pInfoBtn = MenuItemImage::create(
		"Images/BB_Source_TITLE/Bubble_Button/Create_Button/Create_Button_01.png",
		"Images/BB_Source_TITLE/Bubble_Button/Create_Button/Create_Button_02.png",
		CC_CALLBACK_1(HelloWorld::doClick, this));
	pInfoBtn->setPosition(Vec2(600, 45));

	// 버튼 메뉴 추가 (z-order : 5)
	auto pMenu = Menu::create(pPlayBtn, pVibarBtn, pInfoBtn, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 5);

	// 정보 버튼 주변 라이트 추가
	auto pInfoLight = Sprite::create("Images/BB_Source_TITLE/Bubble_Button/Create_Button/Create_base.png");
	pInfoLight->setPosition(Vec2(600, 45));
	this->addChild(pInfoLight, 6);

	auto repInfoRotate = RepeatForever::create(RotateBy::create(1, 360));
	pInfoLight->runAction(repInfoRotate);

	// 별 이미지 추가
	auto pBlueStar = Sprite::create("Images/BB_Source_TITLE/Background_Effect/Blue_Star.png");
	auto pGoldStar = Sprite::create("Images/BB_Source_TITLE/Background_Effect/Gold_Star.png");
	auto pPurpleStar = Sprite::create("Images/BB_Source_TITLE/Background_Effect/Purple_Star.png");
	auto pStarLight = Sprite::create("Images/BB_Source_TITLE/Flash_Effect/Lighting.png");

	pBlueStar->setPosition(Vec2(100, 150));
	pGoldStar->setPosition(Vec2(350, 50));
	pPurpleStar->setPosition(Vec2(600, 90));
	pStarLight->setPosition(Vec2(370, 85));

	this->addChild(pBlueStar);
	this->addChild(pGoldStar);
	this->addChild(pPurpleStar);
	this->addChild(pStarLight);

	pBlueStar->runAction(RepeatForever::create(RotateBy::create(1, 180)));
	pGoldStar->runAction(RepeatForever::create(RotateBy::create(1, 360)));
	pPurpleStar->runAction(RepeatForever::create(RotateBy::create(1, 360)));
	pStarLight->runAction(RepeatForever::create(actionFadeInOut(1.0)));

	// 생선 이미지 추가 (z-order : 1)
	auto pLeftGoFish1 = Sprite::create("Images/BB_Source_TITLE/Fish_Effect/Fish_002.png");
	auto pLeftGoFish2 = Sprite::create("Images/BB_Source_TITLE/Fish_Effect/Fish_002.png");
	auto pRightGoFish1 = Sprite::create("Images/BB_Source_TITLE/Fish_Effect/Fish_001.png");
	auto pRightGoFish2 = Sprite::create("Images/BB_Source_TITLE/Fish_Effect/Fish_001.png");

	pLeftGoFish1->setOpacity(100);
	pLeftGoFish2->setOpacity(100);
	pRightGoFish1->setOpacity(100);
	pRightGoFish2->setOpacity(100);

	pLeftGoFish1->setPosition(Vec2(700, 480));
	pLeftGoFish2->setPosition(Vec2(700, 300));
	pRightGoFish1->setPosition(Vec2(0, 450));
	pRightGoFish2->setPosition(Vec2(0, 200));

	this->addChild(pLeftGoFish1, 1);
	this->addChild(pLeftGoFish2, 1);
	this->addChild(pRightGoFish1, 1);
	this->addChild(pRightGoFish2, 1);

	pLeftGoFish1->runAction(
		RepeatForever::create(actionMovePlace(8, Vec2(-800, 0), pLeftGoFish1->getPosition())));
	pLeftGoFish2->runAction(
		RepeatForever::create(actionMovePlace(6, Vec2(-800, 0), pLeftGoFish2->getPosition())));
	pRightGoFish1->runAction(
		RepeatForever::create(actionMovePlace(12, Vec2(740, 0), pRightGoFish1->getPosition())));
	pRightGoFish2->runAction(
		RepeatForever::create(actionMovePlace(5, Vec2(740, 0), pRightGoFish2->getPosition())));

	// 햇빛 추가
	auto pLineLight1 = Sprite::create("Images/BB_Source_TITLE/Flash_Effect/Flash_005.png");
	auto pLineLight2 = Sprite::create("Images/BB_Source_TITLE/Flash_Effect/Flash_003.png");
	auto pLineLight3 = Sprite::create("Images/BB_Source_TITLE/Flash_Effect/Flash_004.png");

	auto pTopLight = Sprite::create("Images/BB_Source_TITLE/Bubble_Effect/Lighting_Background.png");
	auto pMidLight1 = Sprite::create("Images/BB_Source_TITLE/Flash_Effect/Flash_001.png");
	auto pMidLight2 = Sprite::create("Images/BB_Source_TITLE/Flash_Effect/Flash_004.png");

	pMidLight1->setOpacity(100);
	pMidLight2->setOpacity(100);
	pLineLight3->setOpacity(120);

	pLineLight1->setPosition(Vec2(400, 900));
	pLineLight2->setPosition(Vec2(530, 800));
	pLineLight3->setPosition(Vec2(420, 700));

	pTopLight->setPosition(Vec2(320, 830));
	pMidLight1->setPosition(Vec2(300, 290));
	pMidLight2->setPosition(Vec2(150, 400));

	this->addChild(pLineLight1);
	this->addChild(pLineLight2);
	this->addChild(pLineLight3);
	this->addChild(pTopLight);
	this->addChild(pMidLight1);
	this->addChild(pMidLight2);

	pLineLight1->runAction(RepeatForever::create(actionFadeInOut(0.9)));
	pLineLight2->runAction(RepeatForever::create(actionFadeInOut(0.9)));
	pLineLight3->runAction(RepeatForever::create(actionFadeInOut(5.0, 120)));
	pTopLight->runAction(RepeatForever::create(actionFadeInOut(1.7)));
	pMidLight1->runAction(RepeatForever::create(actionFadeInOut(5.0, 100)));
	pMidLight2->runAction(RepeatForever::create(actionFadeInOut(5.0, 100)));

	// 방울 추가
	auto pBubble1 = Sprite::create("Images/BB_Source_TITLE/Bubble_Effect/Title_Bubble_001.png");
	auto pBubble2 = Sprite::create("Images/BB_Source_TITLE/Bubble_Effect/Title_Bubble_002.png");
	auto pBubble3 = Sprite::create("Images/BB_Source_TITLE/Bubble_Effect/Title_Bubble_003.png");

	pBubble1->setPosition(Vec2(535, -500));
	pBubble2->setPosition(Vec2(420, -500));
	pBubble3->setPosition(Vec2(440, -500));

	this->addChild(pBubble1);
	this->addChild(pBubble2);
	this->addChild(pBubble3);

	pBubble1->runAction(
		RepeatForever::create(actionMovePlace(5, Vec2(0, 2000), pBubble1->getPosition())));
	pBubble2->runAction(
		RepeatForever::create(actionMovePlace(2, Vec2(0, 2000), pBubble2->getPosition())));
	pBubble3->runAction(
		RepeatForever::create(actionMovePlace(8, Vec2(0, 2000), pBubble3->getPosition())));

	return true;
}

void HelloWorld::doClick(Ref* pSender) {
	auto tItem = (MenuItemImage *)pSender;
	int tagNum = tItem->getTag();

	if (tagNum == 1) { // 진동 버튼 ON <-> OFF 전환
		if (vibarFlag == true) {
			tItem->setNormalImage(Sprite::create("Images/BB_Source_TITLE/Bubble_Button/Setting_Button/Vibration_OFF.png"));
			tItem->setSelectedImage(Sprite::create("Images/BB_Source_TITLE/Bubble_Button/Setting_Button/Vibration_ON.png"));
			vibarFlag = false;

			return;
		}
		else {
			tItem->setNormalImage(Sprite::create("Images/BB_Source_TITLE/Bubble_Button/Setting_Button/Vibration_ON.png"));
			tItem->setSelectedImage(Sprite::create("Images/BB_Source_TITLE/Bubble_Button/Setting_Button/Vibration_OFF.png"));
			vibarFlag = true;

			return;
		}
	}
}

/*
원하는 시간으로 FadeInOut 액션 만들어 주는 함수
int opacity : 원하는 투명도 (기본값 -1)
*/
Sequence* HelloWorld::actionFadeInOut(float time, int opacity) {
	if ((opacity > -1) && (opacity < 256)) {
		return Sequence::create(
			FadeOut::create(time),
			FadeTo::create(time, opacity),
			NULL);
	}
	else {
		return Sequence::create(
			FadeOut::create(time),
			FadeIn::create(time),
			NULL);
	}
}

/*
원하는 시간 동안 이동하고 다시 처음 위치로 가는 액션 만들어 주는 함수
cocos2d::Vec2 movePos : MoveBy에서 위치 값
cocos2d::Vec2 placePos : Place에서 위치 값
*/
Sequence* HelloWorld::actionMovePlace(float time, cocos2d::Vec2 movePos, cocos2d::Vec2 placePos) {
	return Sequence::create(
		MoveBy::create(time, movePos),
		Place::create(placePos),
		NULL);
}