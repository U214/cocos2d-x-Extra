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
    if ( !Layer::init() )
    {
        return false;
    }

    /////////////////////////////

    // 윈도우 사이즈 구하기
    winSize = Director::getInstance()->getWinSize();
    
    srand((unsigned)time(nullptr));

    // 벡터 초기화
    cards.clear();
    cardspoint.clear();

    // 게임화면 초기화
    this->initGame();

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
    _eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

    Layer::onExit();
}

void HelloWorld::initGame()
{
    // 게임에 사용될 스프라이트 로딩
    pTexture = Sprite::create("Images/sample.jpg")->getTexture();
    
    int width = 320 / NUM_HORIZONTAL_PIECES;
    int height = 320 / NUM_VERTICAL_PIECES;

    // 위에서부터 아래로 그림을 분할한다.
    int i = 0;
    for (int y=0; y<NUM_VERTICAL_PIECES; y++) {
        for (int x=0; x<NUM_HORIZONTAL_PIECES; x++) {
        
            Rect imgRect = Rect(x * width + SPACE_PIECES,
                                y * height + SPACE_PIECES,
                                width - SPACE_PIECES*2,
                                height - SPACE_PIECES*2);

            auto tTile1 = Sprite::createWithTexture(pTexture, imgRect);
            tTile1->setPosition( Vec2(x * width + width/2, 320 - y * height - height/2) );
            tTile1->setTag(i);

            auto tTile2 = Sprite::createWithTexture(pTexture, imgRect);
            tTile2->setPosition( Vec2(x * width + width/2, 320 - y * height - height/2) );
            tTile2->setTag(i);

            i++;
            
            // 배열에 추가. 위에서 분리를 안하면 주소값이 들어와 두개의 배열값이 같아 진다.
            cards.pushBack(tTile1);
            cardspoint.pushBack(tTile2);

        }
    }
    
    // 카드를 화면에 배치
    i = 0;
    for (int y=0; y<NUM_VERTICAL_PIECES; y++) {
        for (int x=0; x<NUM_HORIZONTAL_PIECES; x++) {
        
            Sprite* tTile = (Sprite *)(cards.at(i));

            tTile->setPosition( Vec2(x*width + width/2, 320 - y*height - height/2) );
            
            i++;
            
            // 화면에 추가
            if (i < NUM_HORIZONTAL_PIECES*NUM_VERTICAL_PIECES) {
                this->addChild(tTile);
            } else {
                blankPoint = tTile->getPosition();
                //cards.eraseObject(tTile, true);
                //cardspoint.eraseObject(tTile, true);
            }
            
        }
    }

    // 변수값 초기화
    bTouch = false;

    // 카드 섞기. 직접 이동시킨다.
	//for (int i = 0; i<NUM_HORIZONTAL_PIECES*NUM_VERTICAL_PIECES * 10; i++)
	for (int i =0; i<5; i++)
    {
        int n = rand() % 4;
        Vec2 p;
        switch (n)
        {
        case 0:
            // 위로 이동
            p = Vec2(blankPoint.x, blankPoint.y + height);
            this->moveCard(p);
            //log("111");
            break;
        case 1:
            // 아래로 이동
            p = Vec2(blankPoint.x, blankPoint.y - height);
            this->moveCard(p);
            //log("222");
            break;
        case 2:
            // 좌로 이동
            p = Vec2(blankPoint.x - width, blankPoint.y);
            this->moveCard(p);
            //log("333");
            break;
        case 3:
            // 우로 이동
            p = Vec2(blankPoint.x + width, blankPoint.y);
            this->moveCard(p);
            //log("444");
            break;

        default:
            break;
        }

    }
    
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    if (bTouch) {
        return false;
    }
    bTouch = true;

    auto touchPoint = touch->getLocation();
    //log("touch .. %d .. %d",(int)touchPoint.x, (int)touchPoint.y);

    this->moveCard(touchPoint);

    return true;
}

void HelloWorld::moveCard(Vec2 touchPoint)
{
    int width = 320 / NUM_HORIZONTAL_PIECES;
    int height = 320 / NUM_VERTICAL_PIECES;

    for (int x = 0; x<cards.size(); x++) {
        Sprite* tTile = (Sprite *)cards.at(x);
        
        Rect rect = tTile->getBoundingBox();
        
        // 카드가 터치되었음을 체크.
        if (rect.containsPoint(touchPoint)) {
            
            float xmove = tTile->getPosition().x - blankPoint.x;
            float ymove = tTile->getPosition().y - blankPoint.y;
            
            Vec2 tmp = tTile->getPosition();
            
            if (fabs(xmove) == width && ymove == 0) {

                // 가로로 이동
                tTile->setPosition(blankPoint);
                blankPoint = tmp;
                
                // 기존 배열의 값 이동한 위치의 값으로 변경
                cards.swap(cards.at(x), tTile);
                
                // 다 맞추었는지 검사
                if (bTouch) {
                    this->checkComplete();
                }
            } else {
                if (fabs(ymove) == height && xmove == 0) {

                    // 세로로 이동
                    tTile->setPosition(blankPoint);
                    blankPoint = tmp;
                    
                    // 기존 배열의 값 이동한 위치의 값으로 변경
                    cards.swap(cards.at(x), tTile);

                    // 다 맞추었는지 검사
                    if (bTouch) {
                        this->checkComplete();
                    }
                }
            }
            
            break;
            
        } // containsPoint
    } // for

    bTouch = false;
}

void HelloWorld::checkComplete()
{
    bool bSame = true;

    for (int x = 0; x<cards.size(); x++) {
        Sprite* tTile1 = (Sprite *)cards.at(x);
        Sprite* tTile2 = (Sprite *)cardspoint.at(x);
        
        if (tTile1->getPosition().x == tTile2->getPosition().x &&
            tTile1->getPosition().y == tTile2->getPosition().y )
        {

        } else {
            bSame = false;
            break;
        }
    }

    if ( bSame )
    {
        log("All clear...");
        
        for (int x = 0; x<cards.size(); x++) {
            Sprite* tTile1 = (Sprite *)cards.at(x);
            Sprite* tTile2 = (Sprite *)cardspoint.at(x);

            this->removeChild(tTile1, true);
            this->removeChild(tTile2, true);
        }
        
        auto tTile = Sprite::createWithTexture(pTexture);
        tTile->setPosition( Vec2(320 / 2, 320 / 2) );
        tTile->setOpacity(0);
        
        this->addChild(tTile);

        auto myAction = FadeIn::create(1.0f);
        tTile->runAction(myAction);
    }
}
