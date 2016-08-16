#ifndef __PlayLevelScene_SCENE_H__
#define __PlayLevelScene_SCENE_H__

#include "cocos2d.h"

class PlayLevelScene : public cocos2d::Layer
{
public:
	PlayLevelScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(PlayLevelScene);
};

#endif // __PlayLevelScene_SCENE_H__