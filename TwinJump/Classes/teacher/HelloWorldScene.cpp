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

    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("Images/player2.png");
    auto animation = Animation::create();
    
	// canabalt 이미지 : http://adamatomic.com/canabalt/
    // run : 16
    animation->setDelayPerUnit(0.03f);
    for (int i = 0; i<16; i++)
    {
        animation->addSpriteFrameWithTexture(texture, Rect(i * 30, 0, 30, 30));
    }
    
    // jump : 4
    //animation->setDelayPerUnit(0.25f);
    //for (int i=16;i<20;i++)
    //{
    //    animation->addSpriteFrameWithTexture(texture, Rect(i * 30, 0, 30, 30));
    //}
    
    // fall : 7
    //animation->setDelayPerUnit(0.1f);
    //for (int i=0;i<7;i++)
    //{
    //    animation->addSpriteFrameWithTexture(texture, Rect(i * 30, 30, 30, 30));
    //}
    
    // stumble : 11
    //animation->setDelayPerUnit(0.08f);
    //for (int i=8;i<19;i++)
    //{
    //    animation->addSpriteFrameWithTexture(texture, Rect(i * 30, 30, 30, 30));
    //}
    
    // 스프라이트 생성 및 초기화
    pMan = Sprite::createWithTexture(texture, Rect(0, 0, 30, 30));
    pMan->setPosition(Vec2(100, 100));
    pMan->setScale(1.7f);
    this->addChild(pMan);
    
    auto animate = Animate::create(animation);
    auto rep = RepeatForever::create(animate);
    pMan->runAction(rep);
    
    yValue = 0;
    nJumpCount = 0;
	bDown = false;
    this->schedule(schedule_selector(HelloWorld::myTick), 1.0f / 60);
    
    
    return true;
}

void HelloWorld::onEnter()
{
    Layer::onEnter();
    
    // 싱글터치모드로 터치리스너 등록
    auto listener = EventListenerTouchOneByOne::create();
    // Swallow touches only available in OneByOne mode.
    // 핸들링된 터치 이벤트를 터치 이벤트 array에서 지우겠다는 의미이다.
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    
    // The priority of the touch listener is based on the draw order of sprite
    // 터치리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
    _eventDispatcher->removeAllEventListeners();
    
    Layer::onExit();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	if (nJumpCount < 2) {
		if (bDown == false) {
			this->doJump();
		}
		else {
			log("내려가는 중이에요~");
		}
	}
	else {
		log("3단 점프는 안되요~");
	}
    
    return true;
}

void HelloWorld::doJump()
{
    yValue = 10;
    nJumpCount = nJumpCount + 1;
}

void HelloWorld::myTick(float f)
{
    if (nJumpCount == 0)
    {
        return;
    }
    
    pMan->setPosition(Vec2(pMan->getPosition().x, pMan->getPosition().y + yValue));
    
    if (pMan->getPosition().y > 100) 
    {
        // y축이 100보다 높을때 yValue값을 0.5씩 뺀다.
        yValue = yValue - 0.5f;
        if (yValue <= 0)
        {
			bDown = true;
            //log("3. %f", pMan->getPosition().y);
        }
    }
    else
    {
        // y축이 100밑으로 떨어지지 않게 보정
        yValue = 0;
        nJumpCount = 0;
		bDown = false;
        pMan->setPosition(Vec2(pMan->getPosition().x, 100));
    }
}
