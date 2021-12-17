#include "ScoreObject.h"

#include <Device/Raytracing/DXRPipeLine.h>

#include <Components/AnimationQue.h>
#include <Components/AnimationComponent.h>
#include <Components/Vector3AnimationCommand.h>

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

	_GenerateAnimationQue = std::make_shared<AnimationQue>();
	_GenerateAnimationQue->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3(0.25f), m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));
	_GenerateAnimationQue->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero,SimpleMath::Vector3(0.0f, 6.0f,0.0f), m_Rotation, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));
	_AnimationComponent->AddAnimationQue("Generate", _GenerateAnimationQue);
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
