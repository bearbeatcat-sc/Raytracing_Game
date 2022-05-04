#include "VerticalMoveBlock.h"

#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>
#include <Utility/Random.h>

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>

#include <Device/Raytracing/DXRPipeLine.h>


VerticalMoveBlock::VerticalMoveBlock(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale,const std::string& meshName,float angle)
	:_initScale(scale),  _DXRMeshName(meshName), Actor(), _moveSpeed(3.0f),_angle(angle),_initPosition(pos)

{
	SetPosition(pos);
	SetScale(scale);

}

void VerticalMoveBlock::Delete()
{

	_AnimationComponent->PlayAnimation("Destroy");
}

void VerticalMoveBlock::UpdateActor()
{
	if (_AnimationComponent->GetCurrentState() == "End")
	{
		Destroy();
	}

	Move();
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);


}

void VerticalMoveBlock::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance(_DXRMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, _initScale, m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto destroyAnimation = std::make_shared<AnimationCommandList>();
	destroyAnimation->AddAnimation(std::make_shared<Vector3AnimationCommand>(_initScale, SimpleMath::Vector3::Zero, m_Scale, 8.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	_AnimationComponent->AddAnimationState(destroyAnimation, "Destroy", AnimationQue::AnimationStateType_End);

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->PlayAnimation("Generate");
}


void VerticalMoveBlock::Shutdown()
{
	_instance->Destroy();
}

void VerticalMoveBlock::OnCollsion(Actor* other)
{
}

void VerticalMoveBlock::Move()
{
	float y = MathUtility::Sin(_angle);
	_angle += Time::DeltaTime * _moveSpeed;

	auto pos = _initPosition + SimpleMath::Vector3(0, y, 0);
	SetPosition(pos);
}
