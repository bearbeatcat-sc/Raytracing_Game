#include "BlenderMonkeyObject.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Components/Animations/AnimationComponent.h>
#include <Utility/Random.h>
#include <Game_Object/ActorManager.h>

#include "../GameSystem/GameManager.h"
#include "../BreakEffect.h"
#include "../PlayerSystem/BombArea.h"

BlenderMonkeyObject::BlenderMonkeyObject(const int maxHP, float destroyTime, BlenderMonkyObjectType blenderMonkyType, GameManager* pGameManager)
	:TargetObject(pGameManager, destroyTime),_maxHP(maxHP), _hp(maxHP),_blenderMonkeyType(blenderMonkyType)
{
	if(blenderMonkyType == BlenderMonkyObjectType_Clear)
	{
		_dxrMeshName = "BlenderMonkeyClear";
		_effectMeshName = "ClearCube";
	}
	else
	{
		_dxrMeshName = "BlenderMonkeyMirror";
		_effectMeshName = "WhiteCube";
	}

	_instance = DXRPipeLine::GetInstance().AddInstance(_dxrMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void BlenderMonkeyObject::UpdateActor()
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
}

void BlenderMonkeyObject::Init()
{
	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "TargetObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	SetTag("Target");

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	_initScale = GetScale();


	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, _initScale, m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto destroyAnimation = std::make_shared<AnimationCommandList>();
	destroyAnimation->AddAnimation(std::make_shared<Vector3AnimationCommand>(GetScale(), SimpleMath::Vector3::Zero, m_Scale, 6.0f));

	auto damageAnimationCommandList0 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand0 = std::make_shared<Vector3AnimationCommand>(m_Scale, m_Scale * 1.2f, m_Scale, 24.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList0->AddAnimation(_damageAnimationCommand0);

	auto damageAnimationCommandList1 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand1 = std::make_shared<Vector3AnimationCommand>(m_Scale, _damageScale, m_Scale, 24.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList1->AddAnimation(_damageAnimationCommand1);

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);

	_AnimationComponent->AddAnimationState(damageAnimationCommandList0, "Damage0", "Damage1");
	_AnimationComponent->AddAnimationState(damageAnimationCommandList1, "Damage1", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->AddAnimationState(destroyAnimation, "Destroy", AnimationQue::AnimationStateType_End);
	_AnimationComponent->PlayAnimation("Generate");

	_bombRadius = GetScale().x;

}

void BlenderMonkeyObject::Shutdown()
{
	_instance->Destroy();
	m_pCollisionComponent->Delete();
}

void BlenderMonkeyObject::OnCollsion(Actor* other)
{
	if (_isDelete)return;

	if (other->IsContainsTag("Bullet"))
	{
		Damage();

		if (IsDeath())
		{
			_AnimationComponent->PlayAnimation("Destroy");
			_isDelete = true;
			_pGameManager->AddScore(1000);

			auto bombArea = new BombArea(2.5f * _initScale.x);
			bombArea->SetPosition(GetPosition());
			ActorManager::GetInstance().AddActor(bombArea);

			for (int i = 0; i < 240; ++i)
			{
				float x = Random::GetRandom(-1.0f, 1.0f);
				float y = Random::GetRandom(-1.0f, 1.0f);
				float z = Random::GetRandom(-1.0f, 1.0f);

				float cos = std::cosf(i * 90.0f);
				float sin = std::sinf(i * 90.0f);

				auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, y, sin) * 10.0f, _effectMeshName);
				breakEffect->SetPosition(GetPosition());
				breakEffect->Destroy(4.0f);
				breakEffect->SetScale(SimpleMath::Vector3(0.1f) * _initScale);
				breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
				ActorManager::GetInstance().AddActor(breakEffect);
			}
			return;

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

			float cos = std::cosf(i * 90.0f);
			float sin = std::sinf(i * 90.0f);
			auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, y, sin) * 10.0f, _effectMeshName);
			breakEffect->SetPosition(GetPosition());
			breakEffect->Destroy(4.0f);
			breakEffect->SetScale(SimpleMath::Vector3(0.1f) * _initScale);
			breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
			ActorManager::GetInstance().AddActor(breakEffect);
		}

		return;
	}


}

bool BlenderMonkeyObject::IsDeath()
{
	return _hp <= 0;
}

void BlenderMonkeyObject::Damage()
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

		auto breakEffect = new BreakEffect(SimpleMath::Vector3(x, y, z) * 10.0f, _effectMeshName);
		breakEffect->SetPosition(GetPosition());
		breakEffect->Destroy(4.0f);
		breakEffect->SetScale(SimpleMath::Vector3(0.04f) * _initScale);
		breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
		ActorManager::GetInstance().AddActor(breakEffect);
	}
}

void BlenderMonkeyObject::ActiveAction()
{

}
