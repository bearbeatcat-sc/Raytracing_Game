#include "BombBullet.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Device/Raytracing/DXRInstance.h>
#include <Utility/Random.h>
#include <Utility/Time.h>

#include <Components/Animations/AnimationQue.h>
#include <Device/DirectX/DirectXInput.h>
#include <Device/DirectX/Core/Sounds/SoundManager.h>
#include <Game_Object/ActorManager.h>
#include <Utility/Timer.h>

#include "BombArea.h"
#include "TraceEffect.h"
#include "../BreakEffect.h"
#include "../Cube.h"


BombBullet::BombBullet(const float moveSpeed, const SimpleMath::Vector3& vec)
:_moveSpeed(120.0f), _addmoveSpeed(moveSpeed), _moveVec(vec), _moveTime(0.0f), _rotate(0.0f), _isExploding(false)
{
	_traceEffectTimer = std::make_shared<Timer>(0.01f);
	_explodeTimer = std::make_shared<Timer>(_explodeTime);
}

bool BombBullet::IsDelete()
{
	return _isDeleteFlag;
}

void BombBullet::GenerateTraceEffect()
{
	_traceEffectTimer->Update();
	if(_traceEffectTimer->IsTime())
	{
		_traceEffectTimer->Reset();
		float x = Random::GetRandom(-1.0f, 1.0f);

		auto traceEffect = new TraceEffect("ClearCube");
		traceEffect->SetPosition(GetPosition());
		traceEffect->Destroy(0.8f);
		traceEffect->SetScale(SimpleMath::Vector3(0.2f));
		traceEffect->SetRotation(GetRotation());

		//auto cube = new Cube(GetPosition(), SimpleMath::Vector3::One , "ClearCube");
		//cube->Destroy(4.0f);

		ActorManager::GetInstance().AddActor(traceEffect);
	}
}

void BombBullet::GenerateRedTraceEffect()
{
	_traceEffectTimer->Update();
	if (_traceEffectTimer->IsTime())
	{
		_traceEffectTimer->Reset();
		float x = Random::GetRandom(-1.0f, 1.0f);

		auto traceEffect = new TraceEffect("RedClearCube");
		traceEffect->SetPosition(GetPosition());
		traceEffect->Destroy(0.8f);
		traceEffect->SetScale(SimpleMath::Vector3(0.2f));
		traceEffect->SetRotation(GetRotation());

		//auto cube = new Cube(GetPosition(), SimpleMath::Vector3::One , "ClearCube");
		//cube->Destroy(4.0f);

		ActorManager::GetInstance().AddActor(traceEffect);
	}
}

void BombBullet::UpdateActor()
{
	if (_isDeleteFlag)return;

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	_addRotate = std::clamp(_addRotate + 16.0f * Time::DeltaTime, 0.0f, 16.0f);
	_rotate += Time::DeltaTime * _addRotate;

	auto pos = GetPosition();

	pos += _moveVec * Time::DeltaTime * _moveSpeed;
	_moveVec += SimpleMath::Vector3(0, -0.2f, 0) * Time::DeltaTime;

	SetRotation(GetEulerRotation() + SimpleMath::Vector3(0, 0, _rotate));
	SetPosition(pos);


	if(_isExploding)
	{
		GenerateRedTraceEffect();
		Exploding();
		return;
	}

	GenerateTraceEffect();
	_explodeTimer->Update();
	if(_explodeTimer->IsTime())
	{
		_explodeTimer->Reset();

		Explode();
		return;
	}




}

void BombBullet:: Explode()
{
	_isDeleteFlag = true;

	auto bombArea = new BombArea(8.0f);
	bombArea->SetPosition(GetPosition());
	ActorManager::GetInstance().AddActor(bombArea);

	SoundManager::GetInstance().OneShot("./Resources/Sound/CrashGlass.sound",0.6f);
	SoundManager::GetInstance().OneShot("./Resources/Sound/Explo.sound", 0.6f);


	for (int i = 0; i < 120; ++i)
	{
		float x = Random::GetRandom(-1.0f, 1.0f);
		//float y = Random::GetRandom(-1.0f, 1.0f);
		float z = Random::GetRandom(-1.0f, 1.0f);

		float cos = std::cosf(i * 1.0f);
		float sin = std::sinf(i * 1.0f);

		auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, 0.0f, sin) * 40.0f, "ClearCube");
		breakEffect->SetPosition(GetPosition());
		breakEffect->Destroy(4.0f);
		breakEffect->SetScale(SimpleMath::Vector3(0.3f));
		breakEffect->SetRotation(SimpleMath::Vector3(x, 0, z));
		ActorManager::GetInstance().AddActor(breakEffect);
	}

	for (int i = 0; i < 120; ++i)
	{
		float x = Random::GetRandom(-1.0f, 1.0f);
		float y = Random::GetRandom(-1.0f, 1.0f);
		float z = Random::GetRandom(-1.0f, 1.0f);



		auto breakEffect = new BreakEffect(SimpleMath::Vector3(x, y, z) * 40.0f, "ClearCube");
		breakEffect->SetPosition(GetPosition());
		breakEffect->Destroy(4.0f);
		breakEffect->SetScale(SimpleMath::Vector3(0.8f) * std::abs(x));
		breakEffect->SetRotation(SimpleMath::Vector3(x, 0, z));
		ActorManager::GetInstance().AddActor(breakEffect);
	}
}

void BombBullet::Init()
{
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), SimpleMath::Vector3(1.4f), "PlayerObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);

	_instance = DXRPipeLine::GetInstance().AddInstance("ClearSphere", 0);

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	SetTag("PlayerBombBullet");

}

void BombBullet::Shutdown()
{
	_instance->Destroy();
	_pCollisionComponent->Delete();
}

void BombBullet::Exploding()
{
	_explodingTimer->Update();
	if(_explodingTimer->IsTime())
	{
		Explode();
	}
}

void BombBullet::OnCollsion(Actor* other)
{
	//if (other->IsContainsTag("Player") || other->IsContainsTag("LockOnArea"))return;

	if(other->IsContainsTag("Bullet"))
	{
		_isExploding = true;
		_explodingTimer = std::make_shared<Timer>(1.0f);

		_moveSpeed *= 1.5f;
		return;
	}

	Explode();
}
