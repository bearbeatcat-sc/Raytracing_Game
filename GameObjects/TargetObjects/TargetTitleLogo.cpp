#include "TargetTitleLogo.h"

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

TargetTitleLogo::TargetTitleLogo(const int maxHP, GameManager* pGameManager)
	:_hp(maxHP), TargetObject(pGameManager,100000.0f), _maxHP(maxHP), _dxrMeshName("TitleLogo")
{
	_instance = DXRPipeLine::GetInstance().AddInstance("TitleLogo", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void TargetTitleLogo::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	if (_isDelete)
	{
		DestoryOrder();
	}
}

void TargetTitleLogo::Init()
{
	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "TargetObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	SetTag("Target");

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3(12, 4, 1.0f), m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto destroyAnimationCommandList = std::make_shared<AnimationCommandList>();
	destroyAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::One, SimpleMath::Vector3(2.0f), m_Scale, 4.0f));
	destroyAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3(2.0f), SimpleMath::Vector3::Zero, m_Scale, 8.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto damageAnimationCommandList0 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand0 = std::make_shared<Vector3AnimationCommand>(m_Scale, m_Scale * 1.2f, m_Scale, 16.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList0->AddAnimation(_damageAnimationCommand0);

	auto damageAnimationCommandList1 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand1 = std::make_shared<Vector3AnimationCommand>(m_Scale, _damageScale, m_Scale, 16.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList1->AddAnimation(_damageAnimationCommand1);

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);

	_AnimationComponent->AddAnimationState(damageAnimationCommandList0, "Damage0", "Damage1");
	_AnimationComponent->AddAnimationState(damageAnimationCommandList1, "Damage1", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->PlayAnimation("Generate");

	auto cube1 = new Cube(SimpleMath::Vector3(0, -0.29f, 55.0f), SimpleMath::Vector3(1,1,1), "ControllUI");
	SetChild(cube1);

	auto cube2 = new Cube(SimpleMath::Vector3(0,-2.7f,55), SimpleMath::Vector3(1,0.5f,1), "BreakTitleUI");
	SetChild(cube2);

}

void TargetTitleLogo::Shutdown()
{
	_instance->Destroy();
	m_pCollisionComponent->Delete();
}

bool TargetTitleLogo::IsDeath()
{
	return _hp <= 0;
}

void TargetTitleLogo::Damage()
{
	_hp = std::clamp(_hp - 1, 0, _maxHP);

	_damageScale = (SimpleMath::Vector3(12, 4, 1.0f) * ((float)_hp / (float)_maxHP));

	_damageAnimationCommand0->_start = m_Scale;
	_damageAnimationCommand0->_target = m_Scale * 1.2f;

	_damageAnimationCommand1->_start = m_Scale * 1.2f;
	_damageAnimationCommand1->_target = _damageScale;
	_AnimationComponent->PlayAnimation("Damage0");

	for (int i = 0; i < 6; ++i)
	{
		float x = Random::GetRandom(-1.0f, 1.0f);
		float y = Random::GetRandom(-1.0f, 1.0f);
		float z = Random::GetRandom(-1.0f, 1.0f);

		auto breakEffect = new BreakEffect(SimpleMath::Vector3(x, y, z) * 10.0f, "RedClearCube");
		breakEffect->SetPosition(GetPosition());
		breakEffect->Destroy(4.0f);
		breakEffect->SetScale(SimpleMath::Vector3(0.1f));
		breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
		ActorManager::GetInstance().AddActor(breakEffect);
	}

}

void TargetTitleLogo::OnCollsion(Actor* other)
{
	if (_isDelete)return;



	if (other->IsContainsTag("Bullet"))
	{
		Damage();

		if (IsDeath())
		{
			_isDelete = true;
			_pGameManager->ChangeGameState(GameManager::GameStete_GamePlay);

			for (int i = 0; i < 300; ++i)
			{
				float x = Random::GetRandom(-1.0f, 1.0f);
				float y = Random::GetRandom(-1.0f, 1.0f);
				float z = Random::GetRandom(-1.0f, 1.0f);

				float cos = std::cosf(i * 30.0f);
				float sin = std::sinf(i * 30.0f);

				auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, y, sin) * 10.0f, "RedClearCube");
				breakEffect->SetPosition(GetPosition());
				breakEffect->Destroy(4.0f);
				breakEffect->SetScale(SimpleMath::Vector3(0.05f) * m_Scale);
				breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
				ActorManager::GetInstance().AddActor(breakEffect);
			}
		}
		return;
	}


}

void TargetTitleLogo::ActiveAction()
{
}

