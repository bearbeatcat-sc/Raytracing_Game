#pragma once
#include <memory>
#include <Game_Object/Actor.h>

class GameManager;
class CollisionComponent;
class DXRInstance;

class NineSideCube
	:public Actor
{
public:
	NineSideCube(GameManager* pGameManager);
	~NineSideCube() = default;

private:
	virtual void UpdateActor() override;
	void CreateBody();
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;

	void Move();

private:
	std::shared_ptr<DXRInstance> _instance;
	GameManager* _pGameManager;
	CollisionComponent* m_pCollisionComponent;

	float _cos;
	float _moveSpeed;
	const float _verticalRange ;
};
