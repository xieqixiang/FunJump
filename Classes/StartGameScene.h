#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PlayLevel.h"

const static int LEVELCOUNT = 12;

class StartGameScene : public cocos2d::Layer
{
public:
	StartGameScene();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void onClickStartGame(cocos2d::Ref* pSender);
	void onClickLevel(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
	CREATE_FUNC(StartGameScene);
private:
	cocos2d::Node *levelList;
	cocos2d::Node *conStartGame;

	float frameWidth;
	float frameHeight;

	float openglScaleX;
	float openglScaleY;

	
};

#endif // __HELLOWORLD_SCENE_H__
