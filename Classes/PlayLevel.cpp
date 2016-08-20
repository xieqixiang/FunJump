#include "PlayLevel.h"


static int curSection = 1;
static int curLevel = 1;

PlayLevelScene::PlayLevelScene() :bTouch(false), bJump(false), hero(nullptr), loadbar(nullptr), pillarContainer(nullptr)
{
	this->acceleration = 104;
	this->tempAcceleration = 104;

	this->up = 700;
	this->tempUp = 700;

	this->gravity = 8;

	this->bCollisionPillar = false;
	this->bCollision = false;
	this->curCollisionPillar = nullptr;

	this->conMaxX = 0;
}


Scene* PlayLevelScene::createScene(int section, int level)
{
	curSection = section;
	curLevel = level;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PlayLevelScene::create();

	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

bool PlayLevelScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	
	auto rootNode = CSLoader::createNode("MainScene.csb");
	addChild(rootNode);

	scheduleUpdate();
	//schedule(schedule_selector(PlayLevelScene::updateCustom),0.1f,kRepeatForever,0);
	auto contentLayout= rootNode->getChildByName("contentLayout_mainScene");

	char str[22] = "topLayout";
	int index = curLevel;
	char cIndex[3];
	sprintf(cIndex, "%d", index);
	strcat(str, cIndex);

	char str2[11] = "_mainScene";
	strcat(str, str2);

	Layout* levelNode = static_cast<Layout*>(contentLayout->getChildByName(str));
	levelNode->setVisible(true);
	this->pillarContainer = levelNode;
	this->conMaxX = -(levelNode->getContentSize().width-640);
	levelNode->addTouchEventListener(CC_CALLBACK_2(PlayLevelScene::handleTouchEvent, this));

	auto openglView = Director::getInstance()->getOpenGLView();
	auto frameSize = openglView->getFrameSize();

	auto scaleX = openglView->getScaleX();
	auto scaleY = openglView->getScaleY();

	auto imgHero = static_cast<ImageView*>(levelNode->getChildByName("imgHero_mainScene"));
	imgHero->setScaleX(1/scaleX);
	imgHero->setScaleY(1/scaleY);

	this->heroSize.width = imgHero->getContentSize().width*imgHero->getScaleX();
	this->heroSize.height = imgHero->getContentSize().height*imgHero->getScaleY();

	this->hero = imgHero;

	int count = 0;

	std::vector<Widget*> vWidget;
	
	for (int i = 1; i <= 30;i++)
	{
		char str3[20] = "stairs";
	    index = 1;
	    char str4[11] = "_mainScene";
		index = i;
		sprintf(cIndex,"%d",index);
		strcat(str3, cIndex);
		strcat(str3, str4);
		auto childNode = static_cast<Widget*>(levelNode->getChildByName(str3));
		
		if (childNode)
		{
			childNode->setScaleX(1 / scaleX);
			childNode->setScaleY(1 / scaleY);
			vWidget.push_back(childNode);
		}
		else
		{
			break;
		}

		if (i == 1)
		{
			auto psx = childNode->getPositionX();
			auto psy = childNode->getPositionY();
			auto height = childNode->getContentSize().height*childNode->getScaleY();
			auto width = childNode->getContentSize().width * childNode->getScaleX();

			this->hero->setPositionX(psx - width / 2);
			this->hero->setPositionY(psy + height);

			startPoint.x = psx - width / 2;
			startPoint.y = psy + height;
		}
		count = count + 1;
	}

	auto countMap = ceil(count / 4); // 计算把柱子分成几个集合
	auto tempCount = 1;

	for (int i = 1; i <= countMap; i ++)
	{
		MapPillar mapPillar;
		for (int j = 1; j <= 4;j++)
		{
			if (tempCount > count)
			{
				break;
			}
			auto pillarNode = vWidget.at(tempCount-1);
			auto pillarSize = pillarNode->getContentSize();
			auto pillarPS = pillarNode->getPosition();
			pillarSize.width = pillarSize.width*pillarNode->getScaleX();
			pillarSize.height = pillarSize.height*pillarNode->getScaleY();
			if (j == 1)
			{
				mapPillar.minX = pillarPS.x;
			}
			
			if (j == 4)
			{
				mapPillar.maxX = pillarPS.x + pillarSize.width;
			}

			PillarInfo pillarInfo;
			Vec2* vPointInfo = this->findPoint(pillarNode,0,0);//四个坐标的信息
			pillarInfo.setPointA(vPointInfo[0].x, vPointInfo[0].y);
			pillarInfo.setPointB(vPointInfo[1].x, vPointInfo[1].y);
			pillarInfo.setPointC(vPointInfo[2].x, vPointInfo[2].y);
			pillarInfo.setPointD(vPointInfo[3].x, vPointInfo[3].y);

			pillarInfo.setPosition(pillarPS.x, pillarPS.y);
			pillarInfo.setSize(pillarSize.width, pillarSize.height);

			delete []vPointInfo;

			Vec2* vPrependicular = this->findPrependicular(pillarNode, 0, 0); //投影轴
			pillarInfo.setEdge1Normalize(vPrependicular[0].x, vPrependicular[0].y);
			pillarInfo.setEdge1Normalize(vPrependicular[1].x, vPrependicular[1].y);
			pillarInfo.setEdge1Normalize(vPrependicular[2].x, vPrependicular[2].y);
			pillarInfo.setEdge1Normalize(vPrependicular[3].x, vPrependicular[3].y);

			mapPillar.vPillar.push_back(pillarInfo);
			delete []vPrependicular;

			tempCount = tempCount + 1;
		}
		vMap.push_back(mapPillar);
	}


	auto layoutBottom = contentLayout->getChildByName("layoutBottom_mainscene");
	auto loadbIntensity = static_cast<LoadingBar*>(layoutBottom->getChildByName("loadbIntensity_mainScene"));
	loadbIntensity->setScaleX(1/scaleX);
	loadbIntensity->setScaleY(1 / scaleY);
	loadbIntensity->setPercent(0.0);

	auto imageBg= layoutBottom->getChildByName("imageBg_mainscene");
	imageBg->setScaleX(1/scaleX);
	imageBg->setScaleY(1/scaleY);

	this->loadbar = loadbIntensity;

// 	auto listener1 = EventListenerTouchOneByOne::create();
// 	listener1->setSwallowTouches(true);
// 
// 	listener1->onTouchBegan = [&](Touch* touch, Event* event)
// 	{
// 		//获取事件所绑定的target
// 		auto target = static_cast<Layout*>(event->getCurrentTarget());
// 		
// 		//获取当前点击点所在相对按钮的位置坐标
// 		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
// 		Size s = target->getContentSize();
// 		Rect rect = Rect(0,0,s.width,s.height);
// 
// 		if (rect.containsPoint(locationInNode))
// 		{
// 			this->bTouch = true;
// 			return true;
// 		}
// 		return false;
// 	};
// 
// 	listener1->onTouchMoved = [&](Touch* touch, Event* event)
// 	{
// 
// 	};
// 
// 	listener1->onTouchEnded = [&](Touch* touch, Event* event)
// 	{
// 		this->bTouch = false;
// 	};
// 
// 
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, node);
// 
	return true;
}

// void PlayLevelScene::updateCustom(float dt)
// {
// 
// }

void PlayLevelScene::update(float dt)
{
	if (this->bTouch)
	{
		auto percent = this->loadbar->getPercent();
		if (percent == 0)
		{
			this->bFront = true;
			this->bBack = false;
		}
		else if (percent >=100)
		{
			this->bFront = false;
			this->bBack = true;
		}

		if (this->bFront)
		{
			this->loadbar->setPercent(percent + 1);
		}
		else if(this->bBack)
		{
			this->loadbar->setPercent(percent - 1);
		}
	}
	if (this->bJump)
	{
		this->runJump(dt);
	}
}

cocos2d::Vec2 * PlayLevelScene::findPoint(cocos2d::Node* node, float psx, float psy)
{
	if (psx <= 0.0 && psy <= 0.0)
	{
		psx = node->getPositionX();
		psy = node->getPositionY();
	}

	float tempPsx = psx;
	float tempPsy = psy;

	cocos2d::Size nodeSize = node->getContentSize();
	float width = nodeSize.width*node->getScaleX();
	float height = nodeSize.height*node->getScaleY();

	cocos2d::Vec2 *vec2 = new Vec2[4];
	vec2[0].x = tempPsx;
	vec2[0].y = tempPsy;

	vec2[1].x = tempPsx;
	vec2[1].y = tempPsy + height;

	vec2[2].x = tempPsx + width;
	vec2[2].y = tempPsy + height;

	vec2[3].x = tempPsx + width;
	vec2[3].y = tempPsy;

	return vec2;
}

Vec2 * PlayLevelScene::findPrependicular(cocos2d::Node* node, float psx, float psy)
{
	Vec2* fourPoint = this->findPoint(node, psx, psy);

	Vec2* edge1 = new Vec2(fourPoint[1].x-fourPoint[0].x,fourPoint[1].y-fourPoint[0].y);
	Vec2* edge2 = new Vec2(fourPoint[2].x - fourPoint[1].x, fourPoint[2].y - fourPoint[1].y);
	Vec2* edge3 = new Vec2(fourPoint[3].x - fourPoint[2].x, fourPoint[3].y - fourPoint[2].y);
	Vec2* edge4 = new Vec2(fourPoint[0].x - fourPoint[3].x, fourPoint[0].y - fourPoint[3].y);

	delete []fourPoint; //释放指针

	Vec2 prependicularA;
	Vec2 prependicularB;
	Vec2 prependicularC;
	Vec2 prependicularD;

	prependicularA.x = edge1->y;
	prependicularA.y = -edge1->x;

	prependicularB.x = edge2->y;
	prependicularB.y = -edge2->x;

	prependicularC.x = edge3->y;
	prependicularC.y = -edge3->x;

	prependicularD.x = edge4->y;
	prependicularD.y = -edge4->x;

	
	float edge1Magnitude = sqrt(prependicularA.x*prependicularA.x + prependicularA.y*prependicularA.y);
	float edge2Magnitude = sqrt(prependicularB.x*prependicularB.x + prependicularB.y*prependicularB.y);
	float edge3Magnitude = sqrt(prependicularC.x*prependicularC.x + prependicularC.y*prependicularC.y);
	float edge4Magnitude = sqrt(prependicularD.x*prependicularD.x + prependicularD.y*prependicularD.y);

	Vec2 *edgeAxisVector = new Vec2[4];
	edgeAxisVector[0].x = prependicularA.x / edge1Magnitude;
	edgeAxisVector[0].y = prependicularA.y / edge1Magnitude;
	edgeAxisVector[1].x = prependicularB.x / edge2Magnitude;
	edgeAxisVector[1].y = prependicularB.y / edge2Magnitude;
	edgeAxisVector[2].x = prependicularC.x / edge3Magnitude;
	edgeAxisVector[2].y = prependicularC.y / edge3Magnitude;
	edgeAxisVector[3].x = prependicularD.x / edge4Magnitude;
	edgeAxisVector[3].y = prependicularD.y / edge4Magnitude;


	delete edge1;
	delete edge2;
	delete edge3;
	delete edge4;

	return edgeAxisVector;

}

void PlayLevelScene::handleTouchEvent(cocos2d::Ref* ref, Widget::TouchEventType touchEventType)
{
	if (touchEventType == Widget::TouchEventType::BEGAN)
	{
		this->bTouch = true;
	}
	else if (touchEventType == Widget::TouchEventType::CANCELED)
	{
		this->bJump = false;
		this->bTouch = false;
	}
	else if (touchEventType == Widget::TouchEventType::ENDED)
	{
		this->bTouch = false;
		this->bJump = true;
	}
}

void PlayLevelScene::runJump(float interval)
{
	float nextX = this->hero->getPositionX() + interval*this->tempAcceleration;
	float nextY = this->hero->getPositionY() + interval*this->tempUp;

	float containerNextX = abs(this->pillarContainer->getPositionX()+interval*100);


	if (this->tempAcceleration > 0)
	{
		this->bCollision = this->checkCollision(nextX, nextY);
	}

	this->tempUp = this->tempUp - this->gravity;

	if (this->bCollisionPillar) //碰撞到柱子了
	{
		this->tempAcceleration = 0;
		this->pillarContainer->setTouchEnabled(false);
		
		if (nextX > this->curCollisionPillar->getPositionX()-this->heroSize.width)
		{
			nextX = this->curCollisionPillar->getPositionX() - this->heroSize.width;
		}
	}
	else
	{
		if (-containerNextX > this->conMaxX)
		{
			this->pillarContainer->setPositionX(-containerNextX);
		}
	}

	this->hero->setPosition(Vec2(nextX,nextY));


}

bool PlayLevelScene::checkCollision(float nextX, float nextY)
{

}

void PlayLevelScene::resetValue()
{
	this->tempAcceleration = this->acceleration;

	this->tempUp = this->up;

	this->bCollisionPillar = false;
	this->bCollision = false;
}