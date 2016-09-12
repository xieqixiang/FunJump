#ifndef __PlayLevelScene_SCENE_H__
#define __PlayLevelScene_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PillarInfo.h"
#include "StartGameScene.h"

#include <spine/spine-cocos2dx.h>
#include "spine/spine.h"

using namespace spine;

USING_NS_CC;
using namespace cocos2d::ui;

class PlayLevelScene : public cocos2d::Layer
{
public:
	PlayLevelScene();
	~PlayLevelScene();

    static cocos2d::Scene* createScene(int section,int level);

	virtual bool init();

	void update(float dt);

	//void updateCustom(float dt);

	CREATE_FUNC(PlayLevelScene);

	void handleTouchEvent(cocos2d::Ref* ref, Widget::TouchEventType touchEventType);

	void runJump(float interval);

	Vec2 * findPoint(cocos2d::Size, float psx, float psy,float scaleX,float scaleY);

	Vec2 *findPrependicular(cocos2d::Size,float psx,float psy,float scaleX,float scaleY);


	bool checkCollision(float nextX,float nextY);
    
    
    void onClickRestart(cocos2d::Ref* sender);
    void onClickSelectLevel(cocos2d::Ref* sender);

	bool checkStart(float nextX,float nextY);

	void resetValue();
    
    std::vector<PillarInfo> findNearPillarMap(float psx,float psy);
    
    float customMax(float a,float b ,float c,float d);
    float customMin(float a,float b ,float c,float d);

	struct MapPillar{
		float minX;
		float maxX;
		std::vector<PillarInfo> vPillar;
	};
    
    bool bPlayingAction(int actionTag);

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
    
    bool GAME_OVER ;
    
    int touchCount ;

	float conMaxX;

	Layout* pillarContainer;

	PillarInfo curCollisionPillar;

	PillarInfo curCollisionPillar2;
    
    PillarInfo frontPillar;
	
	Vec2 startPoint;

	Node *hero;

	SkeletonAnimation* skeletonNode;

	LoadingBar *loadbar;

	Size heroSize;

	std::vector<MapPillar> vMap;
	std::vector<Node*> startS;
    
    std::vector<int> actionTags;
    
    Vec2 curPoint;
    
    static int curSection ;
    static int curLevel ;
    
    float previousY;
	float previousX;
    
    std::string pillarName;
    
    bool standActionPillar;
	
};

#endif // __PlayLevelScene_SCENE_H__