#include "BombArea.h"

#include <algorithm>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>
#include <Device/Raytracing/DXRInstance.h>
#include <Device/Raytracing/DXRPipeLine.h>

#include <Utility/Timer.h>

BombArea::BombArea(float maxRadius)
	:_maxRadius(maxRadius), _currentRadius(0.0f), _expandSpeed(10.0f), _isExpand(true)
{
	SetActorName("BombArea");

	// ボムだが、当たり判定を大きい弾として扱う
	SetTag("Bullet");

	_instance = DXRPipeLine::GetInstance().AddInstance("ClearCube", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_expandTimer = std::make_shared<Timer>(1.4f);
}

void BombArea::UpdateActor()
{
	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);

	if(_isExpand)
	{
		Expand();
		return;
	}

	Contraction();
}

void BombArea::Init()
{
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), GetScale(), "PlayerObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);
}

void BombArea::Shutdown()
{
	_pCollisionComponent->Delete();
}

void BombArea::OnCollsion(Actor* other)
{

}

void BombArea::Contraction()
{
	_currentRadius = MathUtility::Lerp(_currentRadius, 0.0f, Time::DeltaTime * _expandSpeed);
	_currentRadius = std::clamp(_currentRadius, 0.0f, _maxRadius);

	auto scale = SimpleMath::Vector3(_currentRadius);
	_pCollisionComponent->SetSize(scale);
	SetScale(scale);

	if (_currentRadius <= 0.0f)
	{
		Destroy();
	}
}

void BombArea::Expand()
{
	_currentRadius = MathUtility::Lerp(_currentRadius, _maxRadius * 1.2f, Time::DeltaTime * _expandSpeed);
	_currentRadius = std::clamp(_currentRadius, 0.0f, _maxRadius);

	auto scale = SimpleMath::Vector3(_currentRadius);
	_pCollisionComponent->SetSize(scale);
	SetScale(scale);

	if (_currentRadius >= _maxRadius)
	{
		_expandTimer->Update();

		if(_expandTimer->IsTime())
		{
			_isExpand = false;
		}
	}
}
