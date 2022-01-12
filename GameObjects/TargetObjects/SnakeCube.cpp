#include "SnakeCube.h"

#include <algorithm>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Game_Object/ActorManager.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

#include "../GameSystem/GameManager.h"
#include "../BreakEffect.h"

SnakeCube::SnakeCube(const int maxHP, const SimpleMath::Vector3& moveVec, const std::string& dxrMeshName, GameManager* pGameManager)
	:_hp(maxHP), TargetObject(pGameManager), _maxHP(maxHP), _pTarget(nullptr), _moveSpeed(40.0f), _keepDistance(3.0f),
	_moveVec(moveVec), _rotateSpeed(30.0f),_roll(0.0f)
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
		DestoryOrder();
		if (_pTracker)
		{
			_pTracker->OnDestroyTarget();
		}
	}


	Move();

	_roll += Time::DeltaTime * _rotateSpeed;
	SetRotation(SimpleMath::Vector3(0, 0, _roll));
}

bool SnakeCube::IsSetTarget()
{
	return _pTarget != nullptr && !_pTarget->IsDelete();
}

void SnakeCube::KnockBack()
{
	_knockBackAnimationCommand->_start = GetPosition();
	_knockBackAnimationCommand->_target = GetPosition() + GetBackward() * _moveSpeed * 2.0f;
	_AnimationComponent->PlayAnimation("KnockBack");
}

void SnakeCube::Move()
{
	auto currentPos = GetPosition();

	// 遠くに行き過ぎた場合に消滅させる
	if(currentPos.z > 500 || currentPos.z < -500)
	{
		DestoryOrder();
		if (_pTracker)
		{
			_pTracker->OnDestroyTarget();
		}

		return;
	}

	if(IsSetTarget())
	{
		Chase();
		return;
	}

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

	auto pos = SimpleMath::Vector3::Lerp(GetPosition(), targetPos, Time::DeltaTime * 6.0f);

	//auto qu = MathUtility::LookAt(GetPosition(), targetPos);
	//auto moveVec = SimpleMath::Vector3::Transform(GetBackward(), qu);

	//auto currentPos = GetPosition();
	//currentPos += moveVec * Time::DeltaTime * _moveSpeed;

	SetPosition(pos);
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

	auto knockBackAnimationCommandList = std::make_shared<AnimationCommandList>();
	_knockBackAnimationCommand = std::make_shared<Vector3AnimationCommand>(GetPosition(), GetPosition() + GetBackward() * 2.0f, m_Position, 16.0f);
	knockBackAnimationCommandList->AddAnimation(_knockBackAnimationCommand);


	auto damageAnimationCommandList0 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand0 = std::make_shared<Vector3AnimationCommand>(m_Scale, m_Scale * 1.6f, m_Scale, 4.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList0->AddAnimation(_damageAnimationCommand0);

	auto damageAnimationCommandList1 = std::make_shared<AnimationCommandList>();
	_damageAnimationCommand1 = std::make_shared<Vector3AnimationCommand>(m_Scale, _damageScale, m_Scale, 4.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic);
	damageAnimationCommandList1->AddAnimation(_damageAnimationCommand1);

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);

	_AnimationComponent->AddAnimationState(damageAnimationCommandList0, "Damage0", "Damage1");
	_AnimationComponent->AddAnimationState(damageAnimationCommandList1, "Damage1", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->AddAnimationState(knockBackAnimationCommandList, "KnockBack", AnimationQue::StandardAnimationStateType::AnimationStateType_None);

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



	if(other->IsContainsTag("Mirror"))
	{
		//TODO:後で実装
		 KnockBack();
		 return;
	}

	if (other->IsContainsTag("Bullet"))
	{
		Damage();

		if (IsDeath())
		{
			_isDelete = true;

			for(int i = 0; i < 6; ++i)
			{
				float x = Random::GetRandom(-1.0f, 1.0f);
				float y = Random::GetRandom(-1.0f, 1.0f);
				float z = Random::GetRandom(-1.0f, 1.0f);

				float cos = std::cosf(i * 30.0f);
				float sin = std::sinf(i * 30.0f);

				auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, y, sin) * 10.0f, "RedCube");
				breakEffect->SetPosition(GetPosition());
				breakEffect->Destroy(4.0f);
				breakEffect->SetScale(SimpleMath::Vector3(0.1f) * m_Scale);
				breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
				ActorManager::GetInstance().AddActor(breakEffect);
			}

			_pGameManager->AddScore(100);
			return;
		}

		return;
	}

	if (other->IsContainsTag("Player"))
	{
		_isDelete = true;

		for (int i = 0; i < 6; ++i)
		{
			float x = Random::GetRandom(-1.0f, 1.0f);
			float y = Random::GetRandom(-1.0f, 1.0f);
			float z = Random::GetRandom(-1.0f, 1.0f);

			float cos = std::cosf(i * 30.0f);
			float sin = std::sinf(i * 30.0f);

			auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, y, sin) * 10.0f, "RedCube");
			breakEffect->SetPosition(GetPosition());
			breakEffect->Destroy(4.0f);
			breakEffect->SetScale(SimpleMath::Vector3(0.1f) * m_Scale);
			breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
			ActorManager::GetInstance().AddActor(breakEffect);
		}

		return;
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

	for (int i = 0; i < 6; ++i)
	{
		float x = Random::GetRandom(-1.0f, 1.0f);
		float y = Random::GetRandom(-1.0f, 1.0f);
		float z = Random::GetRandom(-1.0f, 1.0f);

		auto breakEffect = new BreakEffect(SimpleMath::Vector3(x, y, z) * 10.0f, "RedCube");
		breakEffect->SetPosition(GetPosition());
		breakEffect->Destroy(4.0f);
		breakEffect->SetScale(SimpleMath::Vector3(0.1f));
		breakEffect->SetRotation(SimpleMath::Vector3(x, y, z));
		ActorManager::GetInstance().AddActor(breakEffect);
	}
}
