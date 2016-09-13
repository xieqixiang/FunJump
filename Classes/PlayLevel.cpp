#include "PlayLevel.h"
#include "math.h"

int PlayLevelScene::curSection = 1;

int  PlayLevelScene::curLevel = 1;

PlayLevelScene::PlayLevelScene() :bTouch(false),skeletonNode(nullptr), bJump(false), hero(nullptr), loadbar(nullptr), pillarContainer(nullptr)
{
	this->acceleration = 104;
	this->tempAcceleration = 104;

	this->up = 700;
	this->tempUp = 700;

	this->gravity = 8;

	this->bCollisionPillar = false;
	this->bCollision = false;

	this->conMaxX = 0;
    
    this->touchCount = 0;
    
    this->previousY = 0;
	this->previousX = 0;
    this->psxxx = 0.0;
    this->pssxDis = 0.0;
    this->standActionPillar = false;
}

PlayLevelScene::~PlayLevelScene()
{
	if (skeletonNode != nullptr)
	{
		delete skeletonNode;
	}
}


Scene* PlayLevelScene::createScene(int section, int level)
{
    PlayLevelScene::curSection = section;
    PlayLevelScene::curLevel = level;
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

	cocostudio::timeline::ActionTimeline * action = CSLoader::createTimeline("MainScene.csb");
    
    cocos2d::Vector<cocostudio::timeline::Timeline*> timelines = action->getTimelines();
    
    for (cocos2d::Vector<cocostudio::timeline::Timeline*>::const_iterator it = timelines.begin() ; it < timelines.end(); it++) {
        int actionTag = (*it)->getActionTag();
        this->actionTags.push_back(actionTag);
    }
    
	auto rootNode = CSLoader::createNode("MainScene.csb");
	addChild(rootNode);
    
    rootNode->runAction(action);

    action->gotoFrameAndPlay(0,true);
    
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

	float minscale = scaleX < scaleY ? scaleX : scaleY;
    
    auto heroCon =static_cast<Widget*>(contentLayout->getChildByName("heroCon_mainScene"));
    heroCon->setContentSize(levelNode->getContentSize());
    heroCon->setLocalZOrder(1000);
    heroCon->setTouchEnabled(false);
	auto conHero = heroCon->getChildByName("conHero_mainScene");
    

	conHero->setScaleX(minscale / scaleX);
	conHero->setScaleY(minscale / scaleY);
	//imgHero->setVisible(false);

	this->heroSize.width = conHero->getContentSize().width*conHero->getScaleX();
	this->heroSize.height = conHero->getContentSize().height*conHero->getScaleY();


	//skeletonNode = new SkeletonAnimation("armatures/pet/pet_0001.json","armatures/pet/pet_0001.atlas");
	//skeletonNode->setAnimation(0, "standby",true);
	//skeletonNode->setAnchorPoint(Vec2(0,0));
	//skeletonNode->setScaleX(minscale / scaleX*0.6);
	//skeletonNode->setScaleY(minscale / scaleY*0.6);

	//levelNode->addChild(skeletonNode);

	this->hero = conHero;

	float count = 0.0;

	std::vector<Widget*> vWidget;

    Vector<Node*>nodes =  levelNode->getChildren();
    int childSize = nodes.size();
	
	for (int i =0 ; i < childSize;i++)
	{
		
		auto childNode = static_cast<Widget*>(nodes.at(i));
		childNode->setScaleX(minscale / scaleX);
		childNode->setScaleY(minscale / scaleY);
		auto childNode2 = dynamic_cast<Layout*>(childNode);
		if (childNode2 && childNode != nullptr)
		{
			vWidget.push_back(childNode);
			count = count + 1;
		}
		else if (childNode2 == nullptr && childNode != nullptr)
		{
			this->startS.push_back(childNode);
		}
		else
		{
			break;
		}

		if (i == 0)
		{
			auto psx = childNode->getPositionX();
			auto psy = childNode->getPositionY();
			auto height = childNode->getContentSize().height*childNode->getScaleY();
			auto width = childNode->getContentSize().width * childNode->getScaleX();

			this->hero->setPositionX(psx - width / 2);
			this->hero->setPositionY(psy + height);

			//skeletonNode->setPositionX(psx - width / 2 + 40);
			//skeletonNode->setPositionY(psy + height-10);

			this->startPoint.x = psx - width / 2;
			this->startPoint.y = psy + height;
            
            this->curPoint = this->startPoint;
		}
		
	}
    
    float temppp = count / 4.0;
    
	auto countMap = ceil(temppp);
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
            pillarInfo.setNode(pillarNode);
            pillarInfo.setActionTag(pillarNode->getActionTag());
			Vec2* vPointInfo = this->findPoint(pillarSize,pillarPS.x,pillarPS.y,pillarNode->getScaleX(),pillarNode->getScaleY());
			pillarInfo.setPointA(vPointInfo[0].x, vPointInfo[0].y);
			pillarInfo.setPointB(vPointInfo[1].x, vPointInfo[1].y);
			pillarInfo.setPointC(vPointInfo[2].x, vPointInfo[2].y);
			pillarInfo.setPointD(vPointInfo[3].x, vPointInfo[3].y);

			pillarInfo.setPosition(pillarPS.x, pillarPS.y);
			pillarInfo.setSize(pillarSize.width, pillarSize.height);

			delete []vPointInfo;

			Vec2* vPrependicular = this->findPrependicular(pillarSize,pillarPS.x,pillarPS.y,pillarNode->getScaleX(),pillarNode->getScaleY());
			pillarInfo.setEdge1Normalize(vPrependicular[0].x, vPrependicular[0].y);
			pillarInfo.setEdge1Normalize(vPrependicular[1].x, vPrependicular[1].y);
			pillarInfo.setEdge1Normalize(vPrependicular[2].x, vPrependicular[2].y);
			pillarInfo.setEdge1Normalize(vPrependicular[3].x, vPrependicular[3].y);

			mapPillar.vPillar.push_back(pillarInfo);
			delete []vPrependicular;

			tempCount = tempCount + 1;
            if (i == 1 && j == 1) {
                this->frontPillar = pillarInfo;
            }
		}
		vMap.push_back(mapPillar);
	}


	auto layoutBottom = contentLayout->getChildByName("layoutBottom_mainscene");
	auto loadbIntensity = static_cast<LoadingBar*>(layoutBottom->getChildByName("loadbIntensity_mainScene"));
	loadbIntensity->setScaleX(minscale / scaleX);
	loadbIntensity->setScaleY(minscale / scaleY);
	loadbIntensity->setPercent(0.0);
    
    auto btnReset = static_cast<Button*>(layoutBottom->getChildByName("btnReset_mainScene"));
	btnReset->setScaleX(minscale / scaleX);
	btnReset->setScaleY(minscale / scaleY);
    btnReset->addClickEventListener(CC_CALLBACK_1(PlayLevelScene::onClickRestart,this));

	auto imageBg= layoutBottom->getChildByName("imageBg_mainscene");
	imageBg->setScaleX(minscale / scaleX);
	imageBg->setScaleY(minscale / scaleY);
    
    auto btnSelectLevel = static_cast<Button*>(layoutBottom->getChildByName("btnSelectLevel_mainScene"));
    btnSelectLevel->setScaleX(minscale / scaleX);
    btnSelectLevel->setScaleY(minscale / scaleY);
    btnSelectLevel->addClickEventListener(CC_CALLBACK_1(PlayLevelScene::onClickSelectLevel,this));
    
	this->loadbar = loadbIntensity;

// 	auto listener1 = EventListenerTouchOneByOne::create();
// 	listener1->setSwallowTouches(true);
// 
// 	listener1->onTouchBegan = [&](Touch* touch, Event* event)
// 	{
// 		//ªÒ»° ¬º˛À˘∞Û∂®µƒtarget
// 		auto target = static_cast<Layout*>(event->getCurrentTarget());
// 		
// 		//ªÒ»°µ±«∞µ„ª˜µ„À˘‘⁄œ‡∂‘∞¥≈•µƒŒª÷√◊¯±Í
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

void PlayLevelScene::onClickSelectLevel(cocos2d::Ref* sender)
{
    StartGameScene::SELECT_LEVEL = true;
    auto scene = StartGameScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void PlayLevelScene::onClickRestart(cocos2d::Ref* sender)
{
    this->pillarContainer->setPositionX(0);
    this->pillarContainer->setPositionY(0);
    
    this->hero->getParent()->setPositionX(0);
    this->hero->getParent()->setPositionY(0);
    
    this->hero->setPosition(this->startPoint);
    this->curPoint = this->startPoint;
    
    this->standActionPillar = false;
    this->frontPillar.setNode(nullptr);

	int sizes = this->startS.size();
	for (int i = 0; i < sizes; i++)
	{
		this->startS.at(i)->setVisible(true);
	}
}

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
    
	if (this->standActionPillar == true && this->bJump == false) {
        Node* node = this->curCollisionPillar.getNode();
        float psy = node->getPositionY();
        float psx = node->getPositionX();
        float dis = psx-this->psxxx;
        
        float nodeHeight = node->getContentSize().height;
        
        this->hero->setPositionY(psy+nodeHeight);
        float nextPSX =this->hero->getPositionX()+dis+this->pssxDis;
        this->hero->setPositionX(nextPSX);
        this->psxxx = nextPSX;
    }
    
	if (this->bJump)
	{
		this->runJump(dt);
	}
}

cocos2d::Vec2 * PlayLevelScene::findPoint(cocos2d::Size nodeSize, float psx, float psy,float scaleX,float scaleY)
{
	if (psx <= 0.0 && psy <= 0.0)
	{
		psx = psx;
		psy = psy;
	}

	float tempPsx = psx;
	float tempPsy = psy;

	float width = nodeSize.width*scaleX;
	float height = nodeSize.height*scaleY;

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

Vec2 * PlayLevelScene::findPrependicular(cocos2d::Size nodeSize, float psx, float psy,float scaleX,float scaleY)
{
	Vec2* fourPoint = this->findPoint(nodeSize, psx, psy, scaleX, scaleY);

	Vec2* edge1 = new Vec2(fourPoint[1].x-fourPoint[0].x,fourPoint[1].y-fourPoint[0].y);
	Vec2* edge2 = new Vec2(fourPoint[2].x - fourPoint[1].x, fourPoint[2].y - fourPoint[1].y);
	Vec2* edge3 = new Vec2(fourPoint[3].x - fourPoint[2].x, fourPoint[3].y - fourPoint[2].y);
	Vec2* edge4 = new Vec2(fourPoint[0].x - fourPoint[3].x, fourPoint[0].y - fourPoint[3].y);

	delete []fourPoint;

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
    if (this->acceleration == 0 || this->touchCount > 1) {
        return;
    }
    
	if (touchEventType == Widget::TouchEventType::BEGAN)
	{
        this->bJump = false;
		this->bTouch = true;
	}
	else if (touchEventType == Widget::TouchEventType::CANCELED)
	{
		this->bJump = false;
		this->bTouch = false;
	}
	else if (touchEventType == Widget::TouchEventType::ENDED)
	{
        float percent = this->loadbar->getPercent();
        
        this->tempUp = this->up;
        this->tempUp = this->tempUp * (percent/100);
        
		this->bTouch = false;
		this->bJump = true;
        this->touchCount = this->touchCount + 1;
        this->loadbar->setPercent(0);
        this->pillarName = "";
      
	}
}

void PlayLevelScene::runJump(float interval)
{
    

	float nextX = this->hero->getPositionX() + interval*this->tempAcceleration;
	float nextY = this->hero->getPositionY() + interval*this->tempUp;

    float containerNextX = std::abs(this->pillarContainer->getPositionX())+interval*100;


	if (this->tempAcceleration > 0)
	{
		this->bCollision = this->checkCollision(nextX, nextY);
	}

	this->tempUp = this->tempUp - this->gravity;

	if (this->bCollisionPillar) 
	{
		this->tempAcceleration = 0;
		this->pillarContainer->setTouchEnabled(false);
		
		if (this->curCollisionPillar2.getSize().x > 0 && nextX > this->curCollisionPillar2.getPosition().x - this->heroSize.width)
		{
			nextX = this->curCollisionPillar2.getPosition().x - this->heroSize.width;
		}
	}
	else
	{
		if (-containerNextX > this->conMaxX)
		{
			this->pillarContainer->setPositionX(-containerNextX);
            this->hero->getParent()->setPositionX(-containerNextX);
		}
	}

	this->hero->setPosition(Vec2(nextX,nextY));
    
    if (this->bCollision) {
        if (this->curCollisionPillar.getSize().x > 0) {
            Vec2 size= this->curCollisionPillar.getSize();
            float psy = this->curCollisionPillar.getNode()->getPositionY();
            this->hero->setPosition(Vec2(nextX,size.y+psy));
            this->curPoint.x = nextX;
            this->psxxx = nextX;
            this->pssxDis = this->psxxx - this->curCollisionPillar.getNode()->getPositionX();
            this->frontPillar = this->curCollisionPillar;
        }
        
        auto maxCount = this->vMap.size();
        MapPillar mp = this->vMap.at(maxCount-1);
        
        maxCount = mp.vPillar.size();
        
        PillarInfo pInfo = mp.vPillar.at(maxCount-1);
        
        if (this->curCollisionPillar.getSize().x > 0 && this->curCollisionPillar.getPosition().x == pInfo.getPosition().x) {
            StartGameScene::GAME_OVER = true;
            StartGameScene::LEVEL = this->curLevel;
            StartGameScene::SECTION = this->curSection;
            auto scene = StartGameScene::createScene();
            Director::getInstance()->replaceScene(scene);
            return ;
        }
        this->resetValue();
    }
    
    if (nextY <= -50) {
        this->resetValue();
        this->hero->setPositionX(this->curPoint.x);
        if (this->frontPillar.getNode() != nullptr) {
            this->hero->setPositionY(this->frontPillar.getNode()->getPositionY()+this->frontPillar.getSize().y);
        }
        else
        {
            this->hero->setPositionY(this->curPoint.y);
        }
        
        if (-(this->curPoint.x - 60) > this->conMaxX) {
            this->pillarContainer->setPositionX(-(this->curPoint.x-60));
            this->hero->getParent()->setPositionX(-(this->curPoint.x-60));
        }
        else
        {
            this->pillarContainer->setPositionX(this->conMaxX);
            this->hero->getParent()->setPositionX(this->conMaxX);
        }
    }
    
}

bool PlayLevelScene::checkCollision(float nextX, float nextY)
{
	this->checkStart(nextX, nextY);
    std::vector<PillarInfo> vPillar = this->findNearPillarMap(nextX,nextY);
    if (vPillar.size() <= 0) {
        vPillar = this->vMap.at(this->vMap.size()-1).vPillar;
    }
    
    if (this->previousY <= 0) {
		this->previousY = nextY;
		this->previousX = nextX + (this->heroSize.width * hero->getScaleX());
    }
    
    Vec2* vPoints = this->findPoint(this->heroSize,nextX,nextY,this->hero->getScaleX(),this->hero->getScaleY());
    
    Vec2* vPrependicular = this->findPrependicular(this->heroSize,nextX,nextY,this->hero->getScaleX(),this->hero->getScaleY());
    
    int size = vPillar.size();
    for (int i = 0 ; i < size ; i++) {
        PillarInfo pInfo = vPillar.at(i);
        
        float height =pInfo.getNode()->getPositionY()+ pInfo.getSize().y;
        
        float width = pInfo.getSize().x;
		float pillarPSX = pInfo.getNode()->getPositionX();
        if (nextX > pillarPSX + width || nextX + this->heroSize.width  < pillarPSX) {
            continue;
        }
        
        Vec2 pillarPointA = pInfo.getPointA();
        Vec2 pillarPointB = pInfo.getPointB();
        Vec2 pillarPointC = pInfo.getPointC();
        Vec2 pillarPointD = pInfo.getPointD();
        
        Vec2 pillarEdge1Normalize = pInfo.getEdge1Normalize();
        Vec2 pillarEdge2Normalize = pInfo.getEdge2Normalize();
        Vec2 pillarEdge3Normalize = pInfo.getEdge3Normalize();
        Vec2 pillarEdge4Normalize = pInfo.getEdge4Normalize();
        
        int actionTag = pInfo.getActionTag();
        bool bPlayAction  = this->bPlayingAction(actionTag);
        bool bCollision = true;
        std::string nodeName = pInfo.getNode()->getName();
        int index = nodeName.find("dead");
        if (index >= 0) {
            bCollision = false;
        }
        if (bPlayAction) {
            cocos2d::Size tempSize;
            tempSize.width = pInfo.getSize().x;
            tempSize.height = pInfo.getSize().y;
            cocos2d::Vec2 tempP = pInfo.getNode()->getPosition();
            
            float scaleXX = pInfo.getNode()->getScaleX();
            float scaleYY = pInfo.getNode()->getScaleY();
            
             Vec2* temp = this->findPrependicular(tempSize,tempP.x,tempP.y,scaleXX,scaleYY);
             pillarEdge1Normalize = temp[0];
             pillarEdge2Normalize = temp[1];
             pillarEdge3Normalize = temp[2];
             pillarEdge4Normalize = temp[3];
            
            
            Vec2* tempPoint = this->findPoint(tempSize,tempP.x,tempP.y,scaleXX,scaleYY);
            pillarPointA  = tempPoint[0];
            pillarPointB  = tempPoint[1];
            pillarPointC  = tempPoint[2];
            pillarPointD  = tempPoint[3];
           
        }
        
        bool sign1 = false;
        
        float pillarPointAPSByNodeEdge1 = pillarPointA.x * pillarEdge1Normalize.x + pillarPointA.y * pillarEdge1Normalize.y;
        
        float pillarPointBPSByNodeEdge1 = pillarPointB.x * pillarEdge1Normalize.x + pillarPointB.y * pillarEdge1Normalize.y;
        
        float pillarPointCPSByNodeEdge1 = pillarPointC.x * pillarEdge1Normalize.x + pillarPointC.y * pillarEdge1Normalize.y;
        
        float pillarPointDPSByNodeEdge1 = pillarPointD.x * pillarEdge1Normalize.x + pillarPointD.y * pillarEdge1Normalize.y;
        
        float pillarNodeEdge1Max = this->customMax(pillarPointAPSByNodeEdge1,pillarPointBPSByNodeEdge1,pillarPointCPSByNodeEdge1,pillarPointDPSByNodeEdge1);
        
        float pillarNodeEdge1Min = this->customMin(pillarPointAPSByNodeEdge1,pillarPointBPSByNodeEdge1,pillarPointCPSByNodeEdge1,pillarPointDPSByNodeEdge1);
        

        float heroPointAPSByNodeEdge1 = vPoints[0].x * pillarEdge1Normalize.x + vPoints[0].y * pillarEdge1Normalize.y;
        float heroPointBPSByNodeEdge1 = vPoints[1].x * pillarEdge1Normalize.x + vPoints[1].y * pillarEdge1Normalize.y;
        float heroPointCPSByNodeEdge1 = vPoints[2].x * pillarEdge1Normalize.x + vPoints[2].y * pillarEdge1Normalize.y;
        float heroPointDPSByNodeEdge1 = vPoints[3].x * pillarEdge1Normalize.x + vPoints[3].y * pillarEdge1Normalize.y;
        float heroNodeEdge1Max = this->customMax(heroPointAPSByNodeEdge1,heroPointBPSByNodeEdge1,heroPointCPSByNodeEdge1,heroPointDPSByNodeEdge1);
        float heroNodeEdge1Min = this->customMin(heroPointAPSByNodeEdge1,heroPointBPSByNodeEdge1,heroPointCPSByNodeEdge1,heroPointDPSByNodeEdge1);
        
        float distance1 = -1;
        
        if (heroNodeEdge1Min <= pillarNodeEdge1Max && heroNodeEdge1Max >= pillarNodeEdge1Min) {
            distance1 = pillarNodeEdge1Min - heroNodeEdge1Min;
            sign1 = true;
        }
        
        bool sign2 = false;
        float pillarPointAPSByNodeEdge2 = pillarPointA.x * pillarEdge2Normalize.x + pillarPointA.y * pillarEdge2Normalize.y ;
        float pillarPointBPSByNodeEdge2 = pillarPointB.x * pillarEdge2Normalize.x + pillarPointB.y * pillarEdge2Normalize.y;
        float pillarPointCPSByNodeEdge2 = pillarPointC.x * pillarEdge2Normalize.x + pillarPointC.y * pillarEdge2Normalize.y;
        float pillarPointDPSByNodeEdge2 = pillarPointD.x * pillarEdge2Normalize.x + pillarPointD.y * pillarEdge2Normalize.y;
        float pillarNodeEdge2Max = this->customMax(pillarPointAPSByNodeEdge2,pillarPointBPSByNodeEdge2,pillarPointCPSByNodeEdge2,pillarPointDPSByNodeEdge2);
        float pillarNodeEdge2Min = this->customMin(pillarPointAPSByNodeEdge2,pillarPointBPSByNodeEdge2,pillarPointCPSByNodeEdge2,pillarPointDPSByNodeEdge2);
        
        float heroPointAPSByNodeEdge2 = vPoints[0].x * pillarEdge2Normalize.x + vPoints[0].y * pillarEdge2Normalize.y;
        float heroPointBPSByNodeEdge2 = vPoints[1].x * pillarEdge2Normalize.x + vPoints[1].y * pillarEdge2Normalize.y;
        float heroPointCPSByNodeEdge2 = vPoints[2].x * pillarEdge2Normalize.x + vPoints[2].y * pillarEdge2Normalize.y;
        float heroPointDPSByNodeEdge2 = vPoints[3].x * pillarEdge2Normalize.x + vPoints[3].y * pillarEdge2Normalize.y;
        float heroNodeEdge2Max = this->customMax(heroPointAPSByNodeEdge2,heroPointBPSByNodeEdge2,heroPointCPSByNodeEdge2,heroPointDPSByNodeEdge2);
        float heroNodeEdge2Min = this->customMin(heroPointAPSByNodeEdge2,heroPointBPSByNodeEdge2,heroPointCPSByNodeEdge2,heroPointDPSByNodeEdge2);
        
        float distance2 = -1;
        if ( heroNodeEdge2Min <= pillarNodeEdge2Max && heroNodeEdge2Max >= pillarNodeEdge2Min)
        {
             distance2 = pillarNodeEdge2Min - heroNodeEdge2Min;
            sign2 = true;
        }
        
        
        float pillarPointAPSByNodeEdge3 = pillarPointA.x * pillarEdge3Normalize.x + pillarPointA.y * pillarEdge3Normalize.y;
        float pillarPointBPSByNodeEdge3 = pillarPointB.x * pillarEdge3Normalize.x + pillarPointB.y * pillarEdge3Normalize.y;
        float pillarPointCPSByNodeEdge3 = pillarPointC.x * pillarEdge3Normalize.x + pillarPointC.y * pillarEdge3Normalize.y;
        float pillarPointDPSByNodeEdge3 = pillarPointD.x * pillarEdge3Normalize.x + pillarPointD.y * pillarEdge3Normalize.y;
        float pillarNodeEdge3Max = this->customMax(pillarPointAPSByNodeEdge3,pillarPointBPSByNodeEdge3,pillarPointCPSByNodeEdge3,pillarPointDPSByNodeEdge3);
        float pillarNodeEdge3Min = this->customMin(pillarPointAPSByNodeEdge3,pillarPointBPSByNodeEdge3,pillarPointCPSByNodeEdge3,pillarPointDPSByNodeEdge3);
        
        
        float heroPointAPSByNodeEdge3 = vPoints[0].x * pillarEdge3Normalize.x + vPoints[0].y * pillarEdge3Normalize.y;
        float heroPointBPSByNodeEdge3 = vPoints[1].x * pillarEdge3Normalize.x + vPoints[1].y * pillarEdge3Normalize.y;
        float heroPointCPSByNodeEdge3 = vPoints[2].x * pillarEdge3Normalize.x + vPoints[2].y * pillarEdge3Normalize.y;
        float heroPointDPSByNodeEdge3 = vPoints[3].x * pillarEdge3Normalize.x + vPoints[3].y * pillarEdge3Normalize.y;
        float heroNodeEdge3Max = this->customMax(heroPointAPSByNodeEdge3,heroPointBPSByNodeEdge3,heroPointCPSByNodeEdge3,heroPointDPSByNodeEdge3);
        float heroNodeEdge3Min = this->customMin(heroPointAPSByNodeEdge3,heroPointBPSByNodeEdge3,heroPointCPSByNodeEdge3,heroPointDPSByNodeEdge3);
        
        float distance3 = -1;
        bool sign3 = false;
        if (heroNodeEdge3Min <= pillarNodeEdge3Max && heroNodeEdge3Max >= pillarNodeEdge3Min)
        {
             distance3 = pillarNodeEdge3Min - heroNodeEdge3Min;
            sign3 = true;
        }
        
        
        float pillarPointAPSByNodeEdge4 = pillarPointA.x * pillarEdge4Normalize.x + pillarPointA.y * pillarEdge4Normalize.y;
        float pillarPointBPSByNodeEdge4 = pillarPointB.x * pillarEdge4Normalize.x + pillarPointB.y * pillarEdge4Normalize.y;
        float pillarPointCPSByNodeEdge4 = pillarPointC.x * pillarEdge4Normalize.x + pillarPointC.y * pillarEdge4Normalize.y;
        float pillarPointDPSByNodeEdge4 = pillarPointD.x * pillarEdge4Normalize.x + pillarPointD.y * pillarEdge4Normalize.y;
        float pillarNodeEdge4Max = this->customMax(pillarPointAPSByNodeEdge4,pillarPointBPSByNodeEdge4,pillarPointCPSByNodeEdge4,pillarPointDPSByNodeEdge4);
        float pillarNodeEdge4Min = this->customMin(pillarPointAPSByNodeEdge4,pillarPointBPSByNodeEdge4,pillarPointCPSByNodeEdge4,pillarPointDPSByNodeEdge4);
        
        float heroPointAPSByNodeEdge4 = vPoints[0].x * pillarEdge4Normalize.x + vPoints[0].y * pillarEdge4Normalize.y;
        float heroPointBPSByNodeEdge4 = vPoints[1].x * pillarEdge4Normalize.x + vPoints[1].y * pillarEdge4Normalize.y;
        float heroPointCPSByNodeEdge4 = vPoints[2].x * pillarEdge4Normalize.x + vPoints[2].y * pillarEdge4Normalize.y;
        float heroPointDPSByNodeEdge4 = vPoints[3].x * pillarEdge4Normalize.x + vPoints[3].y * pillarEdge4Normalize.y;
        float heroNodeEdge4Max = this->customMax(heroPointAPSByNodeEdge4,heroPointBPSByNodeEdge4,heroPointCPSByNodeEdge4,heroPointDPSByNodeEdge4);
        float heroNodeEdge4Min = this->customMin(heroPointAPSByNodeEdge4,heroPointBPSByNodeEdge4,heroPointCPSByNodeEdge4,heroPointDPSByNodeEdge4);
        
        float distance4 = -1;
        bool sign4 = false;
        if (heroNodeEdge4Min <= pillarNodeEdge4Max && heroNodeEdge4Max >= pillarNodeEdge4Min)
        {
            distance4 = pillarNodeEdge4Min - heroNodeEdge4Min;
            sign4 = true;
        }
        
        
        float AAA1 = pillarPointA.x * vPrependicular[0].x + pillarPointA.y * vPrependicular[0].y;
        float AAB1 = pillarPointB.x * vPrependicular[0].x + pillarPointB.y * vPrependicular[0].y;
        float AAC1 = pillarPointC.x * vPrependicular[0].x + pillarPointC.y * vPrependicular[0].y;
        float AAD1 = pillarPointD.x * vPrependicular[0].x + pillarPointD.y * vPrependicular[0].y;
        float ACMax = this->customMax(AAA1,AAB1,AAC1,AAD1);
        float ACMin = this->customMin(AAA1,AAB1,AAC1,AAD1);

        float ABA1 = vPoints[0].x * vPrependicular[0].x + vPoints[0].y * vPrependicular[0].y;
        float ABB1 = vPoints[2].x * vPrependicular[0].x + vPoints[1].y * vPrependicular[0].y;
        float ABC1 = vPoints[3].x * vPrependicular[0].x + vPoints[2].y * vPrependicular[0].y;
        float ABD1 = vPoints[4].x * vPrependicular[0].x + vPoints[3].y * vPrependicular[0].y;
        float ADMax = this->customMax(ABA1,ABB1,ABC1,ABD1);
        float ADMin = this->customMin(ABA1,ABB1,ABC1,ABD1);
        
        bool sign5 = false;
        float distance5 = -1;
        if (ADMin <= ACMax && ADMax >= ACMin)
        {
            distance5 = ACMin - ADMin;
            sign5 = true;
        };
        
        float BAA1 = pillarPointA.x * vPrependicular[1].x + pillarPointA.y * vPrependicular[1].y;
        float BAB1 = pillarPointB.x * vPrependicular[1].x + pillarPointB.y * vPrependicular[1].y;
        float BAC1 = pillarPointC.x * vPrependicular[1].x + pillarPointC.y * vPrependicular[1].y;
        float BAD1 = pillarPointD.x * vPrependicular[1].x + pillarPointD.y * vPrependicular[1].y;
        float BCMax = this->customMax(BAA1,BAB1,BAC1,BAD1);
        float BCMin = this->customMin(BAA1,BAB1,BAC1,BAD1);
        
        float BBA1 = vPoints[0].x * vPrependicular[1].x + vPoints[0].y * vPrependicular[1].y;
        float BBB1 = vPoints[1].x * vPrependicular[1].x + vPoints[1].y * vPrependicular[1].y;
        float BBC1 = vPoints[2].x * vPrependicular[1].x + vPoints[2].y * vPrependicular[1].y;
        float BBD1 = vPoints[3].x * vPrependicular[1].x + vPoints[3].y * vPrependicular[1].y;
        float BDMax = this->customMax(BBA1,BBB1,BBC1,BBD1);
        float BDMin = this->customMin(BBA1,BBB1,BBC1,BBD1);
        
        bool sign6 = false;
        float distance6 = -1;
        if (BDMin <= BCMax && BDMax >= BCMin)
        {
            distance6 = BCMin - BDMin;
            sign6 = true;
        }
        
        float CAA1 = pillarPointA.x * vPrependicular[2].x + pillarPointA.y * vPrependicular[2].y;
        float CAB1 = pillarPointB.x * vPrependicular[2].x + pillarPointB.y * vPrependicular[2].y;
        float CAC1 = pillarPointC.x * vPrependicular[2].x + pillarPointC.y * vPrependicular[2].y;
        float CAD1 = pillarPointD.x * vPrependicular[2].x + pillarPointD.y * vPrependicular[2].y;
        float CCMax = this->customMax(CAA1,CAB1,CAC1,CAD1);
        float CCMin = this->customMin(CAA1,CAB1,CAC1,CAD1);
        
        float CBA1 = vPoints[0].x * vPrependicular[2].x + vPoints[0].y * vPrependicular[2].y;
        float CBB1 = vPoints[1].x * vPrependicular[2].x + vPoints[1].y * vPrependicular[2].y;
        float CBC1 = vPoints[2].x * vPrependicular[2].x + vPoints[2].y * vPrependicular[2].y;
        float CBD1 = vPoints[3].x * vPrependicular[2].x + vPoints[3].y * vPrependicular[2].y;
        float CDMax = this->customMax(CBA1,CBB1,CBC1,CBD1);
        float CDMin =this->customMin(CBA1,CBB1,CBC1,CBD1);

        bool sign7 = false;
        float distance7 = -1;
        if (CDMin <= CCMax && CDMax >= CCMin)
        {
            distance7 = CCMin - CDMin;
            sign7 = true;
        }
        
        
        float DAA1 = pillarPointA.x * vPrependicular[3].x + pillarPointA.y * vPrependicular[3].y;
        float DAB1 = pillarPointB.x * vPrependicular[3].x + pillarPointB.y * vPrependicular[3].y;
        float DAC1 = pillarPointC.x * vPrependicular[3].x + pillarPointC.y * vPrependicular[3].y;
        float DAD1 = pillarPointD.x * vPrependicular[3].x + pillarPointD.y * vPrependicular[3].y;
        float DCMax = this->customMax(DAA1,DAB1,DAC1,DAD1);
        float DCMin = this->customMin(DAA1,DAB1,DAC1,DAD1);
        
        float DBA1 = vPoints[0].x * vPrependicular[3].x + vPoints[0].y * vPrependicular[3].y;
        float DBB1 = vPoints[1].x * vPrependicular[3].x + vPoints[1].y * vPrependicular[3].y;
        float DBC1 = vPoints[2].x * vPrependicular[3].x + vPoints[2].y * vPrependicular[3].y;
        float DBD1 = vPoints[3].x * vPrependicular[3].x + vPoints[3].y * vPrependicular[3].y;
        float DDMax = this->customMax(DBA1,DBB1,DBC1,DBD1);
        float DDMin = this->customMin(DBA1,DBB1,DBC1,DBD1);
        
        
        
        bool sign8 = false;
        float distance8 = -1;
        if (DDMin <= DCMax && DDMax >= DCMin)
        {
            distance8 = DCMin - DDMin;
            sign8 = true;
        }
        
        if (sign1 && sign2 && sign3 && sign4 && sign5 && sign6 && sign7 && sign8) {
            
			if (this->previousX >  pillarPSX && this->previousX - this->heroSize.width <= pillarPSX + width && bCollision == true && bPlayAction == false) {
                this->bCollisionPillar = false;
                this->curCollisionPillar = pInfo;
                delete []vPoints;
                delete []vPrependicular;
				this->previousY = nextY;
				this->previousX = nextX + (this->heroSize.width * hero->getScaleX());
				this->standActionPillar = false;
                return true;
            }
			else if (bPlayAction && bCollision == true && this->previousX > pillarPSX && this->previousX - this->heroSize.width <= pillarPSX + width)
			{
					this->bCollisionPillar = false;
					this->curCollisionPillar = pInfo;
					delete[]vPoints;
					delete[]vPrependicular;
					this->standActionPillar = true;
					this->previousY = nextY;
					this->previousX = nextX + (this->heroSize.width * hero->getScaleX());
					return true;
			}
            else
            {
                if (nextY < height || bCollision == true) {
					this->curCollisionPillar2 = pInfo;
                    this->bCollisionPillar = true;
                }
                delete []vPoints;
                delete []vPrependicular;
				this->previousY = nextY;
				this->previousX = nextX + (this->heroSize.width * hero->getScaleX());
                return false;
            }
        }
    }
    
    this->previousY  = nextY;
	this->previousX = nextX + (this->heroSize.width * hero->getScaleX());
    delete []vPoints;
	delete[]vPrependicular;
    this->bCollisionPillar = false;
    return false;
}

bool PlayLevelScene::checkStart(float nextX, float nextY)
{
	int size = this->startS.size();
	for (int i = 0; i < size; i++)
	{
		Node* start = this->startS.at(i);
		if (start->isVisible())
		{
			float psx = start->getPositionX();
			float psy = start->getPositionY();
			float widthS = start->getContentSize().width*start->getScaleX();
			float heightS = start->getContentSize().height*start->getScaleY();
			
			if (nextX + this->heroSize.width > psx && nextX + this->heroSize.width <= psx + widthS && nextY + this->heroSize.height > psy && nextY + this->heroSize.height <= psy + heightS)
			{
				start->setVisible(false);
			}
		}
		
	}
	return false;
}

float PlayLevelScene::customMax(float a,float b ,float c,float d)
{
    float temp = b;
    if (a > b) {
        temp = a;
    }
    
    if (c > temp) {
        temp = c;
    }
    
    if (d > temp) {
        temp = d;
    }
    return temp;
}

float PlayLevelScene::customMin(float a,float b ,float c,float d)
{
    float temp = b;
    if (a < b) {
        temp = a;
    }
    
    if (c < temp) {
        temp = c;
    }
    
    if (d < temp) {
        temp = d;
    }
    return temp;
}

std::vector<PillarInfo> PlayLevelScene::findNearPillarMap(float psx,float psy)
{
    std::vector<PillarInfo> temp ;
    int size = this->vMap.size();
    for (int i = 0; i < size ; i++) {
        MapPillar mp = this->vMap.at(i);
        auto maxX = mp.maxX;
        if (psx <= maxX) {
            temp = mp.vPillar;
            break;
        }
    }
    return temp;
}

void PlayLevelScene::resetValue()
{
	this->tempAcceleration = this->acceleration;
    this->pillarContainer->setTouchEnabled(true);
	this->tempUp = this->up;
    this->touchCount = 0;
    this->bJump = false;

	this->bCollisionPillar = false;
	this->bCollision = false;
    
}

bool PlayLevelScene::bPlayingAction(int actionTag)
{
    int size = this->actionTags.size();
    for (int i = 0; i < size ; i++) {
        if (actionTag == this->actionTags[i]) {
            return true;
        }
    }
    return  false;
}