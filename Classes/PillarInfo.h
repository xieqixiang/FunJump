<<<<<<< HEAD
#ifndef __PillarInfo_H__
#define __PillarInfo_H__

#include "cocos2d.h"
//保存柱子的信息
class PillarInfo
{
public:
	PillarInfo();
	~PillarInfo();

	void setPosition(float x,float y);
	cocos2d::Vec2 getPosition();

	void setSize(float width,float height);
	cocos2d::Vec2 getSize();

	void setPointA(float x,float y);
	void setPointB(float x, float y);
	void setPointC(float x, float y);
	void setPointD(float x, float y);

	cocos2d::Vec2 getPointA();
	cocos2d::Vec2 getPointB();
	cocos2d::Vec2 getPointC();
	cocos2d::Vec2 getPointD();

	void setEdge1Normalize(float x,float y);
	void setEdge2Normalize(float x, float y);
	void setEdge3Normalize(float x, float y);
	void setEdge4Normalize(float x, float y);

	cocos2d::Vec2 getEdge1Normalize();
	cocos2d::Vec2 getEdge2Normalize();
	cocos2d::Vec2 getEdge3Normalize();
	cocos2d::Vec2 getEdge4Normalize();

private:
	cocos2d::Vec2 pointA;
	cocos2d::Vec2 pointB;
	cocos2d::Vec2 pointC;
	cocos2d::Vec2 pointD;

	cocos2d::Vec2 edge1Normalize;
	cocos2d::Vec2 edge2Normalize;
	cocos2d::Vec2 edge3Normalize;
	cocos2d::Vec2 edge4Normalize;

	cocos2d::Vec2 ps;
	cocos2d::Vec2 size;

};

=======
#ifndef __PillarInfo_H__
#define __PillarInfo_H__

#include "cocos2d.h"
//保存柱子的信息
class PillarInfo
{
public:
	PillarInfo();
	~PillarInfo();

	void setPosition(float x,float y);
	cocos2d::Vec2 getPosition();

	void setSize(float width,float height);
	cocos2d::Vec2 getSize();

	void setPointA(float x,float y);
	void setPointB(float x, float y);
	void setPointC(float x, float y);
	void setPointD(float x, float y);

	cocos2d::Vec2 getPointA();
	cocos2d::Vec2 getPointB();
	cocos2d::Vec2 getPointC();
	cocos2d::Vec2 getPointD();

	void setEdge1Normalize(float x,float y);
	void setEdge2Normalize(float x, float y);
	void setEdge3Normalize(float x, float y);
	void setEdge4Normalize(float x, float y);

	cocos2d::Vec2 getEdge1Normalize();
	cocos2d::Vec2 getEdge2Normalize();
	cocos2d::Vec2 getEdge3Normalize();
	cocos2d::Vec2 getEdge4Normalize();

private:
	cocos2d::Vec2 pointA;
	cocos2d::Vec2 pointB;
	cocos2d::Vec2 pointC;
	cocos2d::Vec2 pointD;

	cocos2d::Vec2 edge1Normalize;
	cocos2d::Vec2 edge2Normalize;
	cocos2d::Vec2 edge3Normalize;
	cocos2d::Vec2 edge4Normalize;

	cocos2d::Vec2 ps;
	cocos2d::Vec2 size;

};

>>>>>>> origin/master
#endif