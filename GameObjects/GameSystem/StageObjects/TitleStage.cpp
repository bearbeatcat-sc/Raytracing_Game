#include "TitleStage.h"

#include <Device/DirectX/Core/Sounds/SoundManager.h>
#include <Utility/Random.h>
#include <Utility/Math/MathUtility.h>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/Vector3AnimationCommand.h>

#include "../../../GameObjects/VerticalMoveBlock.h"

TitleStage::TitleStage()
	:_floorCreateTime(0.1f)
{

}

void TitleStage::Delete()
{
	Destroy(10.0f);

	_AnimationComponent->PlayAnimation("Destroy");
}

void TitleStage::UpdateActor()
{
}

void TitleStage::Init()
{
	_floors.clear();

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto besePos = GetPosition();

	for (int z = 0; z < 27; ++z)
	{
		for (int x = 0; x < 27; x++)
		{
			auto pos = SimpleMath::Vector3(x * 3.9f, 0.0f, z * 4.0f) - SimpleMath::Vector3(12.0f * 3.9f, 0.0f, 0.0f);

			auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "BlackCube");
			SetChild(cube);
		}
	}

	auto destroyAnimation = std::make_shared<AnimationCommandList>();
	destroyAnimation->AddAnimation(std::make_shared<Vector3AnimationCommand>(GetScale(), SimpleMath::Vector3::Zero, m_Scale, 8.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));
	_AnimationComponent->AddAnimationState(destroyAnimation, "Destroy", AnimationQue::AnimationStateType_End);

}

void TitleStage::Shutdown()
{
	_floors.clear();
}

void TitleStage::OnCollsion(Actor* other)
{
}
