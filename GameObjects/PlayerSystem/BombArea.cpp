﻿#include "BombArea.h"

#include <algorithm>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Components/Collsions/SphereCollisionComponent.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>
#include <Device/Raytracing/DXRInstance.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Random.h>
#include <Utility/Timer.h>

#include "../BreakEffect.h"

BombArea::BombArea(float maxRadius)
	:_maxRadius(maxRadius), _currentRadius(0.0f), _expandSpeed(8.0f), _isExpand(true), _contractionSpeed(18.0f)
{
	SetActorName("BombArea");

	// ボムだが、当たり判定を大きい弾として扱う
	SetTag("Bullet");

	//_instance = DXRPipeLine::GetInstance().AddInstance("ClearSphere", 0);

	auto mtx = GetWorldMatrix();;
	//_instance->SetMatrix(mtx);
	//_instance->CreateRaytracingInstanceDesc();

	_expandTimer = std::make_shared<Timer>(0.4f);
}

void BombArea::UpdateActor()
{
	auto mtx = GetWorldMatrix();;
	//_instance->SetMatrix(mtx);

	if(_isExpand)
	{
		Expand();
		return;
	}

	Contraction();
}

void BombArea::Init()
{
	_pCollisionComponent = new SphereCollisionComponent(this, GetScale().x, "AllHitObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);
}

void BombArea::Shutdown()
{
	_pCollisionComponent->Delete();
	//_instance->Destroy();
}

void BombArea::OnCollsion(Actor* other)
{

}

void BombArea::Contraction()
{
	_currentRadius = MathUtility::Lerp(_currentRadius, 0.0f, Time::DeltaTime * _contractionSpeed);
	_currentRadius = std::clamp(_currentRadius, 0.0f, _maxRadius);

	auto scale = SimpleMath::Vector3(_currentRadius);
	_pCollisionComponent->SetRadius(scale.x);
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
	_pCollisionComponent->SetRadius(scale.x);
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
