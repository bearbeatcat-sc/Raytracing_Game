#pragma once

#include <Game_Object/Actor.h>

class Tunnel;

class TunnelManager
	:public Actor
{
public:
	TunnelManager();
	~TunnelManager() = default;
	void AddTask(std::function<Tunnel* (Actor* actor)> task);

private:
	
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	void CreateTunnel();
	void DestroyTunnel();

private:
	std::vector<std::function<Tunnel* (Actor* actor)>> _createTasks;
	std::vector<Tunnel*> _generateTunnels;
	const int _maxGenerateCount = 4;
	int _generateIndex;
};
