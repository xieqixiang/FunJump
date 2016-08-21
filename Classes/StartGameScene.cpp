<<<<<<< HEAD
#include "StartGameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace cocos2d::ui;

bool StartGameScene::GAME_OVER = false;
int StartGameScene::SECTION = 1;
int StartGameScene::LEVEL = 1;

//using namespace cocostudio::timeline;

StartGameScene::StartGameScene() :levelList(nullptr), conStartGame(nullptr), frameWidth(0), frameHeight(0), openglScaleX(0), openglScaleY(0)
{
}

Scene* StartGameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = StartGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartGameScene::init()
{
    /**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto openglView = Director::getInstance()->getOpenGLView();
	auto frameSize = openglView->getFrameSize();
	this->frameWidth = frameSize.width;
	this->frameHeight = frameSize.height;

	this->openglScaleX = openglView->getScaleX();
	this->openglScaleY = openglView->getScaleY();
    
    auto rootNode = CSLoader::createNode("StartScene.csb");
	auto mainContainer = rootNode->getChildByName("MainContainer");
	this->levelList = mainContainer->getChildByName("conLevelList_StartScene");
	this->levelList->setScaleX(1 / this->openglScaleX);
	this->levelList->setScaleY(1 / this->openglScaleY);

	this->conStartGame = mainContainer->getChildByName("conStartGame_StartScene");

	conStartGame->setScaleX(1/this->openglScaleX);
	conStartGame->setScaleY(1/this->openglScaleY);
    
    auto conGameOver = mainContainer->getChildByName("conGameOver_StartScene");
    this->conGame = conGameOver;
    conGameOver->setScaleX(1/this->openglScaleX);
    conGameOver->setScaleY(1/this->openglScaleY);
    
    Button* btnRestartGame = dynamic_cast<Button*>(conGameOver->getChildByName("btnRestartGame_StartScene"));
    btnRestartGame->addClickEventListener(CC_CALLBACK_1(StartGameScene::onClickPlayAgain, this));
                                                   
                                                   
    Button* btnSelectLevel = dynamic_cast<Button*>(conGameOver->getChildByName("btnSelectLevel_StartScene"));
    btnSelectLevel->addClickEventListener(CC_CALLBACK_1(StartGameScene::onClickStartGame, this));
                                                   
                                                   
                            
    
    if (GAME_OVER) {
        conStartGame->setVisible(false);
        conGameOver->setVisible(true);
    }

	Button* btnStartGame = dynamic_cast<Button*>(this->conStartGame->getChildByName("btnStartGame_StartScene"));
	btnStartGame->addClickEventListener(CC_CALLBACK_1(StartGameScene::onClickStartGame, this));

	for (int i = 1; i <= 12; i++)
	{
		char str[11] = "btnLevel";
		int index = i;
		char cIndex[3];
        sprintf(cIndex,"%d",index);
		strcat(str, cIndex);

		Button* btnlevel = dynamic_cast<Button*>(this->levelList->getChildByName(str));
		btnlevel->addClickEventListener(CC_CALLBACK_1(StartGameScene::onClickLevel, this));
	}
	
    addChild(rootNode);

    return true;
}

void StartGameScene::onClickStartGame(cocos2d::Ref* pSender)
{
	log("onClickStartGame");
	this->levelList->setVisible(true);
	this->conStartGame->setVisible(false);
    this->conGame->setVisible(false);
}

void StartGameScene::onClickPlayAgain(cocos2d::Ref* pSender)
{
    auto scene = PlayLevelScene::createScene(StartGameScene::SECTION,StartGameScene::LEVEL);
    Director::getInstance()->replaceScene(scene);
}

void StartGameScene::onClickLevel(cocos2d::Ref* pSender)
{
	Node* node = dynamic_cast<Node*>(pSender);
	auto levelIndex = node->getTag();
	levelIndex = levelIndex - 100;
	if (levelIndex <= LEVELCOUNT)
	{
		auto scene = PlayLevelScene::createScene(1,levelIndex);
		Director::getInstance()->replaceScene(scene);
	}
=======
#include "StartGameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace cocos2d::ui;

//using namespace cocostudio::timeline;

StartGameScene::StartGameScene() :levelList(nullptr), conStartGame(nullptr), frameWidth(0), frameHeight(0), openglScaleX(0), openglScaleY(0)
{
}

Scene* StartGameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = StartGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartGameScene::init()
{
    /**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto openglView = Director::getInstance()->getOpenGLView();
	auto frameSize = openglView->getFrameSize();
	this->frameWidth = frameSize.width;
	this->frameHeight = frameSize.height;

	this->openglScaleX = openglView->getScaleX();
	this->openglScaleY = openglView->getScaleY();
    
    auto rootNode = CSLoader::createNode("StartScene.csb");
	auto mainContainer = rootNode->getChildByName("MainContainer");
	this->levelList = mainContainer->getChildByName("conLevelList_StartScene");
	this->levelList->setScaleX(1 / this->openglScaleX);
	this->levelList->setScaleY(1 / this->openglScaleY);

	this->conStartGame = mainContainer->getChildByName("conStartGame_StartScene");

	conStartGame->setScaleX(1/this->openglScaleX);
	conStartGame->setScaleY(1/this->openglScaleY);

	Button* btnStartGame = dynamic_cast<Button*>(this->conStartGame->getChildByName("btnStartGame_StartScene"));
	btnStartGame->addClickEventListener(CC_CALLBACK_1(StartGameScene::onClickStartGame, this));

	for (int i = 1; i <= 12; i++)
	{
		char str[11] = "btnLevel";
		int index = i;
		char cIndex[3];
        sprintf(cIndex,"%d",index);
		strcat(str, cIndex);

		Button* btnlevel = dynamic_cast<Button*>(this->levelList->getChildByName(str));
		btnlevel->addClickEventListener(CC_CALLBACK_1(StartGameScene::onClickLevel, this));
	}
	
    addChild(rootNode);

    return true;
}

void StartGameScene::onClickStartGame(cocos2d::Ref* pSender)
{
	log("onClickStartGame");
	this->levelList->setVisible(true);
	this->conStartGame->setVisible(false);
}

void StartGameScene::onClickLevel(cocos2d::Ref* pSender)
{
	Node* node = dynamic_cast<Node*>(pSender);
	auto levelIndex = node->getTag();
	levelIndex = levelIndex - 100;
	if (levelIndex <= LEVELCOUNT)
	{
		auto scene = PlayLevelScene::createScene(1,levelIndex);
		Director::getInstance()->replaceScene(scene);
	}
>>>>>>> origin/master
}