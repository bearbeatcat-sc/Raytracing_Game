#include "TunnelManager.h"

#include <Game_Object/ActorManager.h>

#include "GameManager.h"
#include "Stage.h"
#include "../PlayerSystem/Player.h"


TunnelManager::TunnelManager(GameManager* pGameManager)
	:_pGamaManager(pGameManager)
{
}

void TunnelManager::UpdateActor()
{
}

void TunnelManager::Init()
{
	 _pStage = new Stage();
	ActorManager::GetInstance().AddActor(_pStage);


}

void TunnelManager::Shutdown()
{

}

void TunnelManager::OnCollsion(Actor* other)
{
}

