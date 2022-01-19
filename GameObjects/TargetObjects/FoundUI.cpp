#include "FoundUI.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>

#include <Utility/Random.h>
#include <Device/Raytracing/DXRPipeLine.h>

FoundUI::FoundUI()
{
}

void FoundUI::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
}

void FoundUI::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance("FoundUI", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_initScale = GetScale();

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, _initScale, m_Scale, 6.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->PlayAnimation("Generate");
}

void FoundUI::Shutdown()
{
	if (_instance)
		_instance->Destroy();
}

void FoundUI::OnCollsion(Actor* other)
{
}
