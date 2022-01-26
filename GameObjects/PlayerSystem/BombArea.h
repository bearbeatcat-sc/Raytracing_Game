#pragma once

#include <Game_Object/Actor.h>

class SphereCollisionComponent;
class DXRInstance;

class BombArea
	:public Actor
{
public:
	BombArea(float maxRadius);
	~BombArea() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;
	void Contraction();

	void Expand();

private:
	float _maxRadius;
	float _currentRadius;
	const float _expandSpeed;

	SphereCollisionComponent* _pCollisionComponent;
	std::shared_ptr<DXRInstance> _instance;

	bool _isExpand;

	std::shared_ptr<Timer> _expandTimer;
	const float _contractionSpeed;
};
