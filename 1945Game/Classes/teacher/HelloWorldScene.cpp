#include "HelloWorldScene.h"


Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init())
    {
        return false;
    }
    
	playerLife = 3;
	score = 0;
	stopGame = false;

	// 음악 프리 로딩
	simpleAudio = SimpleAudioEngine::getInstance();
	simpleAudio->preloadBackgroundMusic("Music/mainMainMusic.mp3");
    simpleAudio->setBackgroundMusicVolume(0.3f);
    simpleAudio->preloadEffect("Music/explodeEffect.mp3");
	simpleAudio->preloadEffect("Music/fireEffect.mp3");
	simpleAudio->preloadEffect("Music/shipDestroyEffect.mp3");

	// 배경음악 플레이
//	simpleAudio->playBackgroundMusic("Music/mainMainMusic.mp3", true);

	size_ = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto pCloseItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	pCloseItem->setPosition(Vec2(size_.width - 20, 20));

	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 1);

	LabelTTF* pLabel = LabelTTF::create("1945", "Thonburi", 34);
	pLabel->setColor(Color3B::RED);
	pLabel->setPosition(Vec2(size_.width / 2, size_.height - 20));
	this->addChild(pLabel, 1);

	Sprite* pSprite = Sprite::create("bg.png");
	pSprite->setPosition(Vec2(size_.width / 2, size_.height / 2));
	this->addChild(pSprite, 0);

	player_ = Sprite::create("airplain_01.png");
	player_->setPosition(Vec2(size_.width / 2, 50));
	this->addChild(player_, 1);

	schedule(schedule_selector(HelloWorld::Update));
	schedule(schedule_selector(HelloWorld::RegenEnemy), 1.f);
	schedule(schedule_selector(HelloWorld::Shooting), 0.2f);

	// 점수용 글자
	
	scoreLabel = LabelTTF::create("0", "Thonburi", 20);
	scoreLabel->setColor(Color3B::RED);
	scoreLabel->setPosition(Vec2(size_.width / 2, 20));
	this->addChild(scoreLabel, 1);

	for (int i = playerLife; i >0; i--)
	{
		Sprite* Life = Sprite::create("airplain_01.png");
		Life->setScale(0.7);
		Life->setAnchorPoint(Vec2(0.0f, 1.0f));
		Life->setPosition(Vec2((i-1)*Life->getContentSize().width / 2, size_.height));
		Life->setTag(4);
		this->addChild(Life);
	}

    return true;
}

void HelloWorld::AddEnemy(const Vec2& position)
{
	Size size = Director::getInstance()->getWinSize();

	//적비행기 하나를 Scene에 추가
	Sprite *enemy = Sprite::create("enemy_01.png");
	enemy->setPosition(position);
	addChild(enemy);

	//화면 아래로 내려가는 액션 등록
	ActionInterval *move = MoveBy::create(3.f, Vec2(0, -size.height));
	enemy->runAction(move);

	//배열에 방금 생성된 적비행기 스프라이트 포인터를 배열 요소로 추가
	enemy_.pushBack(enemy);
}

void HelloWorld::Update(float time)
{
	// 적과 미사일을 지울 백터를 만든다.
	Vector<Sprite*> eraseEnemy;
	Vector<Sprite*> eraseMissile;
	for (auto enemy :enemy_)
	{
		//적 비행기가 화면 아래로 사라졌을 경우 메모리에서 삭제합니다
		if (enemy->getPosition().y <= 0)
		{
			// 적을 지울 백터에 넣는다.
			eraseEnemy.pushBack(enemy);
		}

		//모든 미사일 백터를 반복하여 충돌검사
		for (int a = 0; a < MAX_MISSILE; a++)
		{
			
			for (auto missile :missile_)
			{
				if (missile->getPosition().y > size_.height)
				{
					// 미사일을 지울 백터에 넣는다.
					eraseMissile.pushBack(missile);
					continue;
				}
				if (missile->boundingBox().intersectsRect(enemy->boundingBox()))
				{
					//미사일과 적 비행기가 부딪혔을 때
					PutCrashEffect(enemy->getPosition());

                    //사운드 출력
                    //simpleAudio->playEffect("Music/explodeEffect.mp3");
                    this->scheduleOnce(schedule_selector(HelloWorld::explosionSound), 0.01);

					eraseEnemy.pushBack(enemy);
					
				
					eraseMissile.pushBack(missile);
					// 점수 증가
					score = score + 100;
					scoreLabel->setString(StringUtils::format("%d", score));
				}
			}
		}
		if (player_)
		{
			//intersectsRect함수 리턴값이 충돌하면 true를 리턴
			if (player_->boundingBox().intersectsRect(enemy->boundingBox()))
			{
				//충돌했을때 처리

				//1.폭파 이펙트 & 사운드 출력
				PutCrashEffect(player_->getPosition());
				PutCrashEffect(enemy->getPosition());
				//simpleAudio->playEffect("Music/shipDestroyEffect.mp3");
                this->scheduleOnce(schedule_selector(HelloWorld::shipDestroySound), 0.01);

				//2.적 비행기를 지울 백터에 넣는다.
				
				eraseEnemy.pushBack(enemy);

				removeChild(player_, false);
				player_ = NULL;

				regenCheckTime_ = 0.f;
				//몫숨을 1개 줄인다.
//				playerLife -= 1;
//				this->removeChildByTag(4);
				stopGame = true;
				
			}
		}
	}
	
	// 적 비행기와 미사일을 지워준다.
	for (auto eraseAllEnemy : eraseEnemy)
	{
		removeChild(eraseAllEnemy, false);
		enemy_.eraseObject(eraseAllEnemy);
	}
	for (auto eraseAllMissile : eraseMissile)
	{
		removeChild(eraseAllMissile, false);
		missile_.eraseObject(eraseAllMissile);
	}

	eraseEnemy.clear();
	eraseMissile.clear();

	//유저 비행기가 죽었을때만 체크하여 부활처리
	if (!player_ &&
		regenCheckTime_ != -1.f && playerLife>0)
	{

		//시간을 누적시켜서 3초 이상 경과되었을 때만 부활시킨다
		regenCheckTime_ += time;
		if (regenCheckTime_ >= 3.f)
		{
			//유저 비행기 부활처리
			stopGame = false;
			player_ = Sprite::create("airplain_01.png");
			player_->setPosition(Vec2(size_.width / 2, 50));
			this->addChild(player_, 1);
		}
	}


	//현재 배열의 크기 = 적 비행기 갯수
	//log("%d", enemy_.size());
}

void HelloWorld::Shooting(float time)
{
	if (!player_)
		return;
	// 양쪽에 2개의 미사일 발싸
	for (int a = 0; a < MAX_MISSILE; ++a)
	{
		Sprite *missile = Sprite::create("missile.png");

		if (a == 0)
		{
			missile->setPosition(Vec2(player_->getPosition().x - 16.f, player_->getPosition().y));
		}
		else if (a == 1)
		{
			missile->setPosition(Vec2(player_->getPosition().x + 16.f, player_->getPosition().y));
		}

		addChild(missile);

		Size size = Director::getInstance()->getWinSize();
		ActionInterval *move = MoveBy::create(0.75f, Vec2(0, size.height));
		missile->runAction(move);

		missile_.pushBack(missile);
	}
	
    //simpleAudio->playEffect("Music/fireEffect.mp3");
    this->scheduleOnce(schedule_selector(HelloWorld::fireSound), 0.01);
}

void HelloWorld::RegenEnemy(float time)
{
	if (!stopGame)
	{
		for (int a = 0; a < random(1, 5); ++a) {
			Size size = Director::getInstance()->getWinSize();
			AddEnemy(Vec2(random(0, (int)size.width), size.height));
		}
	}
}

void HelloWorld::PutCrashEffect(const cocos2d::Vec2 &pos)
{
	//맨첫번째 스프라이트가 로딩되어 베이스가 된다
	Sprite *expBase = Sprite::create("exp_01.png");
	expBase->setPosition(pos);

	//애니메이션 스프라이트 5프레임을 차례로 등록해줌
	Animation *animation = Animation::create();
	animation->addSpriteFrameWithFileName("exp_01.png");
	animation->addSpriteFrameWithFileName("exp_02.png");
	animation->addSpriteFrameWithFileName("exp_03.png");
	animation->addSpriteFrameWithFileName("exp_04.png");
	animation->addSpriteFrameWithFileName("exp_05.png");

	//0.1초 간격으로 애니메이션 처리 설정
	animation->setDelayPerUnit(0.1f);

	//애니메이션이 끝나고 시작 프레임(1번)으로 돌아간다
	animation->setRestoreOriginalFrame(true);

	//에니메이션 생성
	Animate *animate = Animate::create(animation);

	//애니메이션이 끝나면 removeChild를 자동으로 호출하여 메모리 삭제.. 이것도 일종의 액션이다
	CallFunc *removeAction = CallFunc::create(expBase, CC_CALLFUNC_SELECTOR(Node::removeFromParent));
	
	//애니메이션과 리무브액션 2개의 액션을 하나의 시퀀스로 등록후 runAction 실행
	FiniteTimeAction *seq = Sequence::create(animate, removeAction, NULL);
	this->addChild(expBase);//씬에 이펙트(베이스 스프라이트)를 추가
	expBase->runAction(seq);//베이스 스프라이트는 애니메이션 시퀀스를 실행한다

	//simpleAudio->playEffect("Music/explodeEffect.mp3");
    this->scheduleOnce(schedule_selector(HelloWorld::explosionSound), 0.01);
}

void HelloWorld::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	if (!player_)
	{
		return true;
	}
	Vec2 touchPoint = touch->getLocationInView();
	Vec2 touchGlPoint = Director::getInstance()->convertToGL(touchPoint);

	Vec2 playerPos = player_->getPosition();
	distance_ = Vec2(playerPos.x - touchGlPoint.x, playerPos.y - touchGlPoint.y);

	return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
	if (!player_)
		return;

	Vec2 touchPoint = touch->getLocationInView();
	Vec2 touchGlPoint = Director::getInstance()->convertToGL(touchPoint);

	Vec2 pos = Vec2(touchGlPoint.x + distance_.x, touchGlPoint.y + distance_.y);

	if (pos.x < 0.f)
	{
		pos.x = 0.f;
		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
	}
	else if (pos.x > size_.width)
	{
		pos.x = size_.width;
		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
	}

	if (pos.y < 0.f)
	{
		pos.y = 0.f;
		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
	}
	else if (pos.y > size_.height)
	{
		pos.y = size_.height;
		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
	}

	player_->setPosition(pos);
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::explosionSound(float time)
{
    simpleAudio->playEffect("Music/explodeEffect.mp3");
}

void HelloWorld::shipDestroySound(float time)
{
    simpleAudio->playEffect("Music/shipDestroyEffect.mp3");
}

void HelloWorld::fireSound(float time)
{
    simpleAudio->playEffect("Music/fireEffect.mp3");
}
