#include "LockOnArea.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>

#include <Device/Raytracing/DXRPipeLine.h>

#include "../TargetObjects/TargetObject.h"

class TargetObject;

LockOnArea::LockOnArea(Actor* pActor)
	:_pActor(pActor)
{
}

const std::vector<TargetObject*>& LockOnArea::GetLockOnTargets()
{
	return _lockOnTargets;
}

void LockOnArea::UpdateActor()
{
	// 毎フレームクリアは効率は悪いかもしれない。
	_lockOnTargets.clear();

	SetPosition(_pActor->GetPosition());
}

void LockOnArea::Init()
{
	_obbCollisionComponent = new OBBCollisionComponent(this, GetPosition(), GetScale(), "PlayerObject");
	CollisionManager::GetInstance().AddComponent(_obbCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_obbCollisionComponent);

	SetTag("LockOnArea");

}

void LockOnArea::Shutdown()
{
	if (_obbCollisionComponent != nullptr)
		_obbCollisionComponent->Delete();

}

void LockOnArea::AddTarget(TargetObject* target)
{
	_lockOnTargets.push_back(target);
}

void LockOnArea::OnCollsion(Actor* other)
{

	if(other->IsContainsTag("Target"))
	{
		auto hitList = DXRPipeLine::GetInstance().GetHitResult();

		auto instanceID = static_cast<TargetObject*>(other)->GetInstanceID();

		if (instanceID == -1)return;

		if(hitList.at(instanceID)._isHit == 0)
		{
			AddTarget(static_cast<TargetObject*>(other));
		}

	}

}
	
