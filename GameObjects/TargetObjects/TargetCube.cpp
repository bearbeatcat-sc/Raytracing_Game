#include "TargetCube.h"

#include <algorithm>
#include <Components/Animations/AnimationCommand.h>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/AnimationQue.h>
#include <Components/Animations/Vector3AnimationCommand.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Game_Object/ActorManager.h>
#include <Utility/Random.h>

#include "../GameSystem/GameManager.h"
#include "../BreakEffect.h"
#include "../Cube.h"

TargetCube::TargetCube(const int maxHP, float destroyTime, const std::string& dxrMeshName, GameManager* pGameManager)
	:_hp(maxHP), TargetObject(pGameManager, destroyTime), _maxHP(maxHP),_dxrMeshName(dxrMeshName)
{
	_instance = DXRPipeLine::GetInstance().AddInstance(dxrMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void TargetCube::ActiveAction()
{
	if(_findUI == nullptr)
	{
		_findUI = new Cube(SimpleMath::Vector3(0, 2.0f, 0.0f), SimpleMath::Vector3(2, 1, 1), "FoundUI");
		SetChild(_findUI);
	}

}

void TargetCube::UpdateActor()
{

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	if (_AnimationComponent->GetCurrentState() == "End")
	{
		DestoryOrder();
		return;
	}

	_deleteTimer->Update();
	if (_deleteTimer->IsTime() && _AnimationComponent->GetCurrentState() != "Destroy")
	{
		_AnimationComponent->PlayAnimation("Destroy");
		return;
	}

	if(IsExitActive())
	{
		if(_findUI)
		{
			_findUI->Destroy();
			_findUI = nullptr;
		}
	}

	ActiveUpdate();
	SetRotation(m_EulerRotation);
}

void TargetCube::Init()
{
	_initScale = GetScale();

	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "TargetObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	SetTag("Target");

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, _initScale, m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto destroyAnimation = std::make_shared<AnimationCommandList>();
	destroyAnimation->AddAnimation(std::make_shared<Vector3AnimationCommand>(GetScale(), SimpleMath::Vector3::Zero, m_Scale,6.0f));

	auto damageAnimationCommandList0 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand0 = std::make_shared<Vector3AnimationCommand>(m_Scale, m_Scale * 1.6f, m_Scale, 16.0f,AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList0->AddAnimation(_damageAnimationCommand0);

	auto damageAnimationCommandList1 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand1 = std::make_shared<Vector3AnimationCommand>(m_Scale, _damageScale, m_Scale, 16.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList1->AddAnimation(_damageAnimationCommand1);



	_AnimationComponent->AddAnimationState(generateAnimationCommandList,"Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);

	_AnimationComponent->AddAnimationState(damageAnimationCommandList0,"Damage0", "Damage1");
	_AnimationComponent->AddAnimationState(damageAnimationCommandList1,"Damage1", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->AddAnimationState(destroyAnimation,"Destroy",AnimationQue::AnimationStateType_End);
	_AnimationComponent->PlayAnimation("Generate");

}

void TargetCube::Shutdown()
{
	_instance->Destroy();
	m_pCollisionComponent->Delete();
}

bool TargetCube::IsDeath()
{
	return _hp <= 0;
}

void TargetCube::Damage()
{
	_hp = std::clamp(_hp - 1, 0, _maxHP);


	_damageAnimationCommand0->_start = _initScale;
	_damageAnimationCommand0->_target = _initScale * 1.6f;

	_damageAnimationCommand1->_start = _initScale * 1.6f;
	_damageAnimationCommand1->_target = _initScale;
	_AnimationComponent->PlayAnimation("Damage0");

	for (int i = 0; i < 6; ++i)
	{
		float x = Random::GetRandom(-1.0f, 1.0f);
		float y = Random::GetRandom(-1.0f, 1.0f);
		float z = Random::GetRandom(-1.0f, 1.0f);

		auto breakEffect = new BreakEffect(SimpleMath::Vector3(x, y, z) * 10.0f, _dxrMeshName);
		breakEffect->SetPosition(GetPosition());
		breakEffect->Destroy(4.0f);
		breakEffect->SetScale(SimpleMath::Vector3(0.1f));
		breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
		ActorManager::GetInstance().AddActor(breakEffect);
	}

}

void TargetCube::OnCollsion(Actor* other)
{
	if (_isDelete)return;



	if (other->IsContainsTag("Bullet"))
	{
		Damage();

		if(IsDeath())
		{
			_AnimationComponent->PlayAnimation("Destroy");
			_isDelete = true;

			_pGameManager->AddScore(100);

			for (int i = 0; i < 6; ++i)
			{
				float x = Random::GetRandom(-1.0f, 1.0f);
				float y = Random::GetRandom(-1.0f, 1.0f);
				float z = Random::GetRandom(-1.0f, 1.0f);

				float cos = std::cosf(i * 30.0f);
				float sin = std::sinf(i * 30.0f);

				auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, y, sin) * 10.0f, _dxrMeshName);
				breakEffect->SetPosition(GetPosition());
				breakEffect->Destroy(4.0f);
				breakEffect->SetScale(SimpleMath::Vector3(0.1f) * m_Scale);
				breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
				ActorManager::GetInstance().AddActor(breakEffect);
			}
		}
		return;
	}

	if (other->IsContainsTag("Player"))
	{
		_AnimationComponent->PlayAnimation("Destroy");
		_isDelete = true;

		for (int i = 0; i < 6; ++i)
		{
			float x = Random::GetRandom(-1.0f, 1.0f);
			float y = Random::GetRandom(-1.0f, 1.0f);
			float z = Random::GetRandom(-1.0f, 1.0f);

			float cos = std::cosf(i * 30.0f);
			float sin = std::sinf(i * 30.0f);

			auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, y, sin) * 10.0f, _dxrMeshName);
			breakEffect->SetPosition(GetPosition());
			breakEffect->Destroy(4.0f);
			breakEffect->SetScale(SimpleMath::Vector3(0.1f) * m_Scale);
			breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
			ActorManager::GetInstance().AddActor(breakEffect);
		}

		return;
	}
}
