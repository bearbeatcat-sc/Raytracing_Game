#pragma once

#include <Game_Object/Actor.h>

class Stage;
class GameManager;
class Tunnel;

class TunnelManager
	:public Actor
{
public:
	TunnelManager(GameManager* pGameManager);
	~TunnelManager() = default;

private:
	
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;


private:
	Stage* _pStage;
	GameManager* _pGamaManager;
};
