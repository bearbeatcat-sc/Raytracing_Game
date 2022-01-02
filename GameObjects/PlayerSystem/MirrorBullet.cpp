#include "MirrorBullet.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>
#include <Components/Animations/AnimationComponent.h>
#include <Device/Raytracing/DXRInstance.h>

MirrorBullet::MirrorBullet(const float moveSpeed, const float moveTime, const SimpleMath::Vector3& vec, const SimpleMath::Vector3& maxScale)
	:_moveSpeed(moveSpeed), _moveVec(vec), _maxScale(maxScale)
{
	_moveTimer = std::make_shared<Timer>(moveTime);
}

void MirrorBullet::UpdateActor()
{
	_instance->SetMatrix(GetWorldMatrix());
	const auto currentState = _AnimationComponent->GetCurrentState();

	if (currentState == "None")
	{
		auto pos = GetPosition();
		pos += _moveVec * _moveSpeed * Time::DeltaTime;

		SetPosition(pos);
	}

	if (currentState == "Expand" || currentState == "End")
	{
		return;
	}

	_moveTimer->Update();
	if (_moveTimer->IsTime())
	{
		Expand();
	}

}

void MirrorBullet::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance("WhiteCube", 0);

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	SetTag("MirrorBullet");
	SetActorName("MirrorBullet");

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto expandAnimation = std::make_shared<AnimationCommandList>();
	expandAnimation->AddAnimation(std::make_shared<Vector3AnimationCommand>(GetScale(), _maxScale, m_Scale, 4.0f));

	_AnimationComponent->AddAnimationState(expandAnimation, "Expand", AnimationQue::AnimationStateType_End);
	_AnimationComponent->PlayAnimation("None");
}

void MirrorBullet::Shutdown()
{
	_instance->Destroy();
}

void MirrorBullet::OnCollsion(Actor* other)
{
}

void MirrorBullet::Expand()
{
	_AnimationComponent->PlayAnimation("Expand");

}
