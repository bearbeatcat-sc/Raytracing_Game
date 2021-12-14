#include "LockOnSystem.h"

#include <Game_Object/ActorManager.h>

#include "LockOnArea.h"
#include "LockOnUIObject.h"

LockOnSystem::LockOnSystem(Actor* pUser)
	:Component(pUser)
{
	_pLockOnArea = new LockOnArea(pUser);
	_pLockOnArea->SetScale(SimpleMath::Vector3(30, 30, 300));
	ActorManager::GetInstance().AddActor(_pLockOnArea);
	_pLockOnArea->SetActorName("LockOnArea");
}

LockOnSystem::~LockOnSystem()
{
	_pLockOnArea->Destroy();
}

void LockOnSystem::LockOn()
{
	auto targets = _pLockOnArea->GetLockOnTargets();

	// 後でロックオンマークをセットする。
	// 攻撃はどうする？

	for(int i = 0; i < targets.size(); ++i)
	{
		auto pos = targets[i]->GetPosition();
		auto lockOnUI = new LockOnUIObject();

		lockOnUI->SetPosition(pos);
		lockOnUI->SetScale(lockOnUI->GetScale() + SimpleMath::Vector3(3.0f));
		lockOnUI->Destroy(1.0f);
		ActorManager::GetInstance().AddActor(lockOnUI);
	}
}

void LockOnSystem::Update()
{
	
}

void LockOnSystem::DrawProperties()
{
}
