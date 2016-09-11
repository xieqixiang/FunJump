#include "PillarInfo.h"


PillarInfo::PillarInfo():node(nullptr)
{
}


PillarInfo::~PillarInfo()
{
}

void PillarInfo::setPosition(float x, float y)
{
	this->ps.x = x;
	this->ps.y = y;
}

cocos2d::Vec2 PillarInfo::getPosition()
{
	return this->ps;
}

void PillarInfo::setSize(float width, float height)
{
	this->size.x = width;
	this->size.y = height;
}

cocos2d::Vec2 PillarInfo::getSize()
{
	return this->size;
}


void PillarInfo::setPointA(float x, float y)
{
	this->pointA.x = x;
	this->pointA.y = y;
}

void PillarInfo::setPointB(float x, float y)
{
	this->pointB.x = x;
	this->pointB.y = y;
}

void PillarInfo::setPointC(float x, float y)
{
	this->pointC.x = x;
	this->pointC.y = y;
}

void PillarInfo::setPointD(float x, float y)
{
	this->pointD.x = x;
	this->pointD.y = y;
}

cocos2d::Vec2 PillarInfo::getPointA()
{
	return this->pointA;
}

cocos2d::Vec2 PillarInfo::getPointB()
{
	return this->pointB;
}

cocos2d::Vec2 PillarInfo::getPointC()
{
	return this->pointC;
}

cocos2d::Vec2 PillarInfo::getPointD()
{
	return this->pointD;
}

void PillarInfo::setEdge1Normalize(float x, float y)
{
	this->edge1Normalize.x = x;
	this->edge1Normalize.y = y;
}

void PillarInfo::setEdge2Normalize(float x, float y)
{
	this->edge2Normalize.x = x;
	this->edge2Normalize.y = y;
}

void PillarInfo::setEdge3Normalize(float x, float y)
{
	this->edge3Normalize.x = x;
	this->edge3Normalize.y = y;
}

void PillarInfo::setEdge4Normalize(float x, float y)
{
	this->edge4Normalize.x = x;
	this->edge4Normalize.y = y;
}

cocos2d::Vec2 PillarInfo::getEdge1Normalize()
{
	return this->edge1Normalize;
}

cocos2d::Vec2 PillarInfo::getEdge2Normalize()
{
	return this->edge2Normalize;
}

cocos2d::Vec2 PillarInfo::getEdge3Normalize()
{
	return this->edge3Normalize;
}

cocos2d::Vec2 PillarInfo::getEdge4Normalize()
{
	return this->edge4Normalize;
}

void PillarInfo::setActionTag(int actionT)
{
    this->actionTag = actionT;
}

int PillarInfo::getActionTag()
{
    return this->actionTag;
}

void PillarInfo::setNode (cocos2d::Node* node)
{
    this->node = node;
}

cocos2d::Node* PillarInfo::getNode()
{
    return  this->node;
}

