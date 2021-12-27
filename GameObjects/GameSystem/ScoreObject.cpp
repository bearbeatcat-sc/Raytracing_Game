#include "ScoreObject.h"

#include <Device/Raytracing/DXRPipeLine.h>

#include <Components/Animations//AnimationQue.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/Vector3AnimationCommand.h>

ScoreObject::ScoreObject(const int number)
{
	_meshName = "NumberObject" + std::to_string((number));
}

void ScoreObject::UpdateActor()
{
	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
}

void ScoreObject::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance(_meshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();


	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3(0.25f), m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero,SimpleMath::Vector3(0.0f, 6.0f,0.0f), m_Rotation, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	_AnimationComponent->AddAnimationState(generateAnimationCommandList,"Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->PlayAnimation("Generate");
}

void ScoreObject::Shutdown()
{
	if(_instance != nullptr)
		_instance->Destroy();
}

void ScoreObject::OnCollsion(Actor* other)
{
}
