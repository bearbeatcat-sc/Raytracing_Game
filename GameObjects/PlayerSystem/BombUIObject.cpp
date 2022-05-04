#include "BombUIObject.h"

#include <Device/Raytracing/DXRPipeLine.h>
#include <Components/Animations//AnimationQue.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/Vector3AnimationCommand.h>

void BombUIObject::UpdateActor()
{
	if(_isThrow)
	{
		auto mtx = GetWorldMatrix();;
		_bombUIInstance->SetMatrix(SimpleMath::Matrix::Identity);
		_bombThrowUIInstance->SetMatrix(mtx);
	}
	else
	{
		auto mtx = GetWorldMatrix();;
		_bombThrowUIInstance->SetMatrix(SimpleMath::Matrix::Identity);
		_bombUIInstance->SetMatrix(mtx);
	}
}

void BombUIObject::Generate()
{
	_AnimationComponent->PlayAnimation("Generate");

	_isThrow = false;
}

void BombUIObject::Throw()
{
	_isThrow = true;
}

void BombUIObject::Init()
{
	_bombUIInstance = DXRPipeLine::GetInstance().AddInstance("BombUI", 0,1);
	_bombThrowUIInstance = DXRPipeLine::GetInstance().AddInstance("BombThrowUI", 0,1);

	auto mtx = GetWorldMatrix();;
	_bombUIInstance->SetMatrix(mtx);

	_bombUIInstance->CreateRaytracingInstanceDesc();
	_bombThrowUIInstance->CreateRaytracingInstanceDesc();


	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto maxScale = SimpleMath::Vector3(0.6f,8,1);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, maxScale, m_Scale, 8.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->PlayAnimation("Generate");

	_isThrow = false;
}

void BombUIObject::Shutdown()
{
	if (_bombThrowUIInstance != nullptr)
		_bombThrowUIInstance->Destroy();

	if (_bombUIInstance != nullptr)
		_bombUIInstance->Destroy();
}

void BombUIObject::OnCollsion(Actor* other)
{
}
