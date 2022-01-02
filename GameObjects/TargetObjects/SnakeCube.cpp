#include "SnakeCube.h"

#include <algorithm>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

#include "../GameSystem/GameManager.h"

SnakeCube::SnakeCube(const int maxHP, const SimpleMath::Vector3& moveVec, const std::string& dxrMeshName, GameManager* pGameManager)
	:_hp(maxHP), TargetObject(pGameManager), _maxHP(maxHP), _pTarget(nullptr), _moveSpeed(30.0f), _keepDistance(3.0f),
	_moveVec(moveVec)
{
	SetActorName("SnakeCube");

	_instance = DXRPipeLine::GetInstance().AddInstance(dxrMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void SnakeCube::SetTarget(SnakeCube* pParent)
{
	pParent->SetTracker(this);

	_pTarget = pParent;
}

void SnakeCube::SetTracker(SnakeCube* pTracker)
{
	_pTracker = pTracker;
}

void SnakeCube::OnDestroyTarget()
{
	_pTarget = nullptr;
}

void SnakeCube::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	if (_isDelete)
	{
		Destroy();
	}

	Move();
}

bool SnakeCube::IsSetTarget()
{
	return _pTarget != nullptr && !_pTarget->IsDelete();
}

void SnakeCube::Move()
{
	if(IsSetTarget())
	{
		Chase();
		return;
	}

	auto currentPos = GetPosition();
	currentPos += _moveVec * _moveSpeed * Time::DeltaTime;

	SetPosition(currentPos);
}

void SnakeCube::Chase()
{

	auto targetPos = _pTarget->GetPosition();

	if(MathUtility::Abs(SimpleMath::Vector3::Distance(GetPosition(),targetPos)) <= _keepDistance)
	{
		return;
	}

	auto qu = MathUtility::LookAt(GetPosition(), targetPos);
	auto moveVec = SimpleMath::Vector3::Transform(GetBackward(), qu);

	auto currentPos = GetPosition();
	currentPos += moveVec * Time::DeltaTime * _moveSpeed;

	SetPosition(currentPos);
}

void SnakeCube::Init()
{


	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "TargetObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	SetTag("Target");

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3::One, m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto destroyAnimationCommandList = std::make_shared<AnimationCommandList>();
	destroyAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::One, SimpleMath::Vector3(2.0f), m_Scale, 4.0f));
	destroyAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3(2.0f), SimpleMath::Vector3::Zero, m_Scale, 8.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto damageAnimationCommandList0 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand0 = std::make_shared<Vector3AnimationCommand>(m_Scale, m_Scale * 1.6f, m_Scale, 4.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList0->AddAnimation(_damageAnimationCommand0);

	auto damageAnimationCommandList1 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand1 = std::make_shared<Vector3AnimationCommand>(m_Scale, _damageScale, m_Scale, 4.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList1->AddAnimation(_damageAnimationCommand1);

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);

	_AnimationComponent->AddAnimationState(damageAnimationCommandList0, "Damage0", "Damage1");
	_AnimationComponent->AddAnimationState(damageAnimationCommandList1, "Damage1", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->PlayAnimation("Generate");

}

void SnakeCube::Shutdown()
{
	_instance->Destroy();
	m_pCollisionComponent->Delete();
}

void SnakeCube::OnCollsion(Actor* other)
{
	if (_isDelete)return;

	if (other->IsContainsTag("Bullet"))
	{
		Damage();

		if (IsDeath())
		{
			_isDelete = true;
			_pGameManager->AddScore(1000);
			return;
		}
	}
}

bool SnakeCube::IsDeath()
{
	return _hp <= 0;
}

void SnakeCube::Damage()
{
	_hp = std::clamp(_hp - 1, 0, _maxHP);

	_damageScale = (SimpleMath::Vector3::One * ((float)_hp / (float)_maxHP));

	_damageAnimationCommand0->_start = m_Scale;
	_damageAnimationCommand0->_target = m_Scale * 1.6f;

	_damageAnimationCommand1->_start = m_Scale * 1.6f;
	_damageAnimationCommand1->_target = _damageScale;
	_AnimationComponent->PlayAnimation("Damage0");
}
