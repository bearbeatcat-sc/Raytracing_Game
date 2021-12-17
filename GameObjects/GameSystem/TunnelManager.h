#pragma once

#include <Game_Object/Actor.h>

class GameManager;
class Tunnel;

class TunnelManager
	:public Actor
{
public:
	TunnelManager(GameManager* pGameManager);
	~TunnelManager() = default;
	void AddTask(std::function<Tunnel* (Actor* actor, GameManager* pGameManager)> task);

private:
	
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	void CreateTunnel();
	void DestroyTunnel();

private:
	std::vector<std::function<Tunnel* (Actor* actor, GameManager* pGameManager)>> _createTasks;
	std::vector<Tunnel*> _generateTunnels;
	const int _maxGenerateCount = 4;
	int _generateIndex;

	GameManager* _pGamaManager;
};
