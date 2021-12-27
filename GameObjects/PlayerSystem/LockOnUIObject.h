#pragma once

#include <Game_Object/Actor.h>

class DXRInstance;
class Timer;

class LockOnUIObject
	:public Actor
{
public:
	LockOnUIObject(const float destroyTime,Actor* lockonTarget);
	~LockOnUIObject() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<DXRInstance> _instance;

	std::shared_ptr<Timer> _generateTimer;
	std::shared_ptr<Timer> _destroyTimer;

	Actor* _pTarget;

	bool _isGenerate;
	SimpleMath::Vector3 _initScale;
};
