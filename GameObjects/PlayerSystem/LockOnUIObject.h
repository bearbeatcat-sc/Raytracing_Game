#pragma once

#include <Game_Object/Actor.h>

class DXRInstance;

class LockOnUIObject
	:public Actor
{
public:
	LockOnUIObject() = default;
	~LockOnUIObject() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<DXRInstance> _instance;

};
