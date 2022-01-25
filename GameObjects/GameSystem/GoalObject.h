#pragma once

#include <Game_Object/Actor.h>

class OBBCollisionComponent;
class DXRInstance;
class GameManager;

class GoalObject
	:public Actor
{
public:
	GoalObject(GameManager* pGameManager);
	~GoalObject();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	OBBCollisionComponent* _pCollisionComponent;
	GameManager* _pGameManager;
	std::shared_ptr<DXRInstance> _instance;
};
