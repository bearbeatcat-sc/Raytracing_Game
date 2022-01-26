#include "GoalObject.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>

#include "GameManager.h"

GoalObject::GoalObject(GameManager* pGameManager)
	:Actor(), _pGameManager(pGameManager)
{
}

GoalObject::~GoalObject()
{
}

void GoalObject::UpdateActor()
{
}

void GoalObject::Init()
{
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), GetScale(), "AllHitObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);
	_pCollisionComponent->SetAdjustPos(SimpleMath::Vector3(0, 0, 30));

	//_instance = DXRPipeLine::GetInstance().AddInstance("ClearCube", 0);

	//auto mtx = GetWorldMatrix();;
	//_instance->SetMatrix(mtx);
	//_instance->CreateRaytracingInstanceDesc();

	SetTag("GoalObject");
}

void GoalObject::Shutdown()
{
	//_instance->Destroy();
	_pCollisionComponent->Delete();
}

void GoalObject::OnCollsion(Actor* other)
{
	if(!other->IsContainsTag("Player") || other->IsContainsTag("Bullet"))
	{
		return;
	}

	_pGameManager->ChangeGameState(GameManager::GameStete_Result);

}
