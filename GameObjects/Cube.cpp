#include "Cube.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>

#include <Utility/Random.h>
#include <Device/Raytracing/DXRPipeLine.h>


Cube::Cube(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale, const std::string& meshName)
	:_initScale(scale),_DXRMeshName(meshName),Actor()
{
	SetPosition(pos);
	SetScale(scale);


}

void Cube::Delete()
{
	_AnimationComponent->PlayAnimation("Destroy");
}

void Cube::UpdateActor()
{
	if(_AnimationComponent->GetCurrentState() == "End")
	{
		Destroy();
	}

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);


}

void Cube::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance(_DXRMeshName, 0);


	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, _initScale, m_Scale, 2.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto destroyAnimation = std::make_shared<AnimationCommandList>();
	destroyAnimation->AddAnimation(std::make_shared<Vector3AnimationCommand>(GetScale(), SimpleMath::Vector3::Zero, m_Scale, 6.0f));

	_AnimationComponent->AddAnimationState(destroyAnimation, "Destroy", AnimationQue::AnimationStateType_End);
	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->PlayAnimation("Generate");
}

void Cube::Shutdown()
{
	if(_instance)
		_instance->Destroy();
}

void Cube::OnCollsion(Actor* other)
{

}
