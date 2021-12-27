#include "TrackerShotObject.h"

#include <Game_Object/ActorManager.h>
#include <Utility/Random.h>
#include <Utility/Timer.h>

#include "TrackerBullet.h"

TrackerShotObject::TrackerShotObject(Actor* pTarget,GameManager* pGameManager, const float bulletSpeed)
	:_pTarget(pTarget), _pGameManager(pGameManager),_bulletSpeed(bulletSpeed), _generateCount(0)
{
	_ShootCoolTimer = std::make_shared<Timer>(0.04f);
}

void TrackerShotObject::Shot()
{
	float pos_x = 4.5f - _generateCount * 2.0f;

	auto pos = GetPosition() + SimpleMath::Vector3(pos_x, 6.0f, 0.0f) + SimpleMath::Vector3::Backward * 1.0f;

	auto bullet = new TrackerBullet(_bulletSpeed, _pTarget, _pGameManager, 1.0f, 1.0f);;
	bullet->SetPosition(pos);
	bullet->SetScale(SimpleMath::Vector3(0.4f));
	bullet->SetRotation(SimpleMath::Vector3(0, 0.0f, 2.4f));
	ActorManager::GetInstance().AddActor(bullet);

	bullet->Destroy(10.0f);
}

void TrackerShotObject::UpdateActor()
{
	_ShootCoolTimer->Update();

	if(_ShootCoolTimer->IsTime())
	{
		_ShootCoolTimer->Reset();
		Shot();
		_generateCount++;
	}

	if(_generateCount >= _maxBulletCount)
	{
		Destroy();
	}
}

void TrackerShotObject::Init()
{
	SetActorName("TrackerShotObject");

}

void TrackerShotObject::Shutdown()
{
}

void TrackerShotObject::OnCollsion(Actor* other)
{
}
