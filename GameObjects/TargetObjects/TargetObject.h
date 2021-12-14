#pragma once

#include <Game_Object/Actor.h>
#include <Device/Raytracing/DXRInstance.h>

class TargetObject
	:public Actor
{
public:
	TargetObject() = default;
	virtual ~TargetObject() {}

	const int GetInstanceID()
	{
		return _instance->_instnaceID;
	}

private:
	virtual void UpdateActor() = 0;
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void OnCollsion(Actor* other) = 0;

protected:
	std::shared_ptr<DXRInstance> _instance;


};
