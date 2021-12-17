#include "TargetGroup.h"

TargetGroup::TargetGroup(GameManager* pGameManager,std::function<void(Actor* actor, GameManager* pGameManager)> pCreateEvent)
	:_pCreateEvent(pCreateEvent),_pGameManager(pGameManager)
{
}

void TargetGroup::UpdateActor()
{
}

void TargetGroup::Init()
{
	_pCreateEvent(this, _pGameManager);
}

void TargetGroup::Shutdown()
{
}

void TargetGroup::OnCollsion(Actor* other)
{
}
