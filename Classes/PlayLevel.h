#ifndef __PlayLevelScene_SCENE_H__
#define __PlayLevelScene_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PillarInfo.h"
<<<<<<< HEAD
#include "StartGameScene.h"
=======
>>>>>>> origin/master

USING_NS_CC;
using namespace cocos2d::ui;

class PlayLevelScene : public cocos2d::Layer
{
public:
	PlayLevelScene();

<<<<<<< HEAD
    static cocos2d::Scene* createScene(int section,int level);
=======
	//section : 章节
	//level : 关卡
	static cocos2d::Scene* createScene(int section,int level);
>>>>>>> origin/master

	virtual bool init();

	void update(float dt);

	//void updateCustom(float dt);

	CREATE_FUNC(PlayLevelScene);

	void handleTouchEvent(cocos2d::Ref* ref, Widget::TouchEventType touchEventType);

<<<<<<< HEAD
=======
	//执行跳跃动作
>>>>>>> origin/master
	void runJump(float interval);

	Vec2 * findPoint(cocos2d::Node* node, float psx, float psy);

	Vec2 *findPrependicular(cocos2d::Node* node,float psx,float psy);


	bool checkCollision(float nextX,float nextY);
<<<<<<< HEAD
    
    void onClickRestart(cocos2d::Ref* sender);

	void resetValue();
    
    std::vector<PillarInfo> findNearPillarMap(float psx,float psy);
    
    float customMax(float a,float b ,float c,float d);
    float customMin(float a,float b ,float c,float d);
=======

	void resetValue();
>>>>>>> origin/master

	struct MapPillar{
		float minX;
		float maxX;
		std::vector<PillarInfo> vPillar;
	};

private:
	bool bTouch;
	bool bJump;
	bool bFront;
	bool bBack;

	float acceleration ;
	float tempAcceleration ;

	float up ;
	float tempUp ;

	float gravity ;

	bool bCollisionPillar ;
	bool bCollision ;
<<<<<<< HEAD
    
    bool GAME_OVER ;
    
    int touchCount ;
=======
>>>>>>> origin/master

	float conMaxX;

	Layout* pillarContainer;

<<<<<<< HEAD
	PillarInfo curCollisionPillar;
=======
	Layout* curCollisionPillar;
>>>>>>> origin/master
	
	Vec2 startPoint;

	ImageView *hero;

	LoadingBar *loadbar;

	Size heroSize;

	std::vector<MapPillar> vMap;
<<<<<<< HEAD
    
    Vec2 curPoint;
    
    static int curSection ;
    static int curLevel ;
=======
>>>>>>> origin/master
	
};

#endif // __PlayLevelScene_SCENE_H__