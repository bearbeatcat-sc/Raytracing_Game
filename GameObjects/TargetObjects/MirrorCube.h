#pragma once

#include <Game_Object/Actor.h>

class DXRInstance;
class CollisionComponent;
class AnimationComponent;
class Vector3AnimationCommand;
class GameManager;

class MirrorCube
	:public Actor
{
public:
	MirrorCube(GameManager* pGameManager,bool controllFlag,float angle = 0.0f);
	~MirrorCube() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;
	void Rotate(float angle);

private:
	CollisionComponent* m_pCollisionComponent;

	GameManager* _pGameManager;
	std::shared_ptr<DXRInstance> _instance;

	float _angle;
	const float _radius;
	const float _rotateSpeed;
	bool _controllFlag;
};
