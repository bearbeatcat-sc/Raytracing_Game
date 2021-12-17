#pragma once

#include <Game_Object/Actor.h>

class GameManager;

class TargetGroup
	:public Actor
{
public:
	TargetGroup(GameManager* pGameManager,std::function<void(Actor* actor, GameManager* pGameManager)> pCreateEvent);
	~TargetGroup() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::function<void(Actor* actor, GameManager* pGameManager)> _pCreateEvent = nullptr;
	GameManager* _pGameManager;
};
