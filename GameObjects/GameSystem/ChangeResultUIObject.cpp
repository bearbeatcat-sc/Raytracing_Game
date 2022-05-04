#include "ChangeResultUIObject.h"

#include <Components/Animations/AnimationQue.h>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <Device/Raytracing/DXRPipeLine.h>

void ChangeResultUIObject::UpdateActor()
{
	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
}

void ChangeResultUIObject::Init()
{
	if(DirectXInput::GetInstance().IsActiveGamePad())
	{
		_instance = DXRPipeLine::GetInstance().AddInstance("ChangeResultUI_Pad", 0);
	}
	else
	{
		_instance = DXRPipeLine::GetInstance().AddInstance("ChangeResultUI_Key", 0);
	}



	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);

	_instance->CreateRaytracingInstanceDesc();

	//_AnimationComponent = std::make_shared<AnimationComponent>(this);
	//AddComponent(_AnimationComponent);

	//auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	//generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3(4, 2, 1), m_Scale, 4.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));
	//generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(GetPosition(), SimpleMath::Vector3(0, 2, 10), m_Position, 4.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	//_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	//_AnimationComponent->PlayAnimation("Generate");
}

void ChangeResultUIObject::Shutdown()
{
	if(_instance)
	{
		_instance->Destroy();
	}
}

void ChangeResultUIObject::OnCollsion(Actor* other)
{
}
