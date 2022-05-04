#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

using namespace DirectX;

class DXRInstance;
class AnimationComponent;

class AimCursorObject
	:public Actor
{
public:
	AimCursorObject() = default;
	~AimCursorObject() = default;

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<DXRInstance> _instance;
};
