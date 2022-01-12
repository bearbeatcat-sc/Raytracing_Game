#include "Stage.h"

#include <Game_Object/ActorManager.h>
#include <Utility/LogSystem.h>
#include <Utility/Random.h>
#include <Utility/Timer.h>
#include <Utility/Math/MathUtility.h>

#include "../TargetObjects/TargetGroup.h"
#include "../../GameObjects/VerticalMoveBlock.h"
#include "../Cube.h"

Stage::Stage()
	:_floorCreateTime(0.1f)
{
}

void Stage::UpdateActor()
{
}

void Stage::Init()
{
	_floors.clear();

	auto besePos = GetPosition();

	for (int z = 0; z < 27; ++z)
	{
		for (int x = 0; x < 27; x++)
		{
			auto pos = SimpleMath::Vector3(x * 3.9f, 0.0f, z * 4.0f) - SimpleMath::Vector3(12.0f * 3.9f, 0.0f, 0.0f);

			if ((x >= 13 && x < 16) || (z >= 13 && z < 16))
			{
				auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "BlackCube");
				SetChild(cube);
				_floors.push_back(cube);
				continue;
			}

			auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "RoughCube");
			SetChild(cube);
			_floors.push_back(cube);

		}
	}

	for (int z = 0; z < 27; ++z)
	{
		for (int x = 0; x < 27; x++)
		{
			auto pos = SimpleMath::Vector3(x * 3.9f, 40.0f, z * 4.0f) - SimpleMath::Vector3(12.0f * 3.9f, 0.0f, 0.0f);

			if ((x >= 13 && x < 16) || (z >= 13 && z < 16))
			{
				auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "BlackCube");
				SetChild(cube);
				_floors.push_back(cube);
				continue;
			}

			auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "RoughCube");
			SetChild(cube);
			_floors.push_back(cube);

		}
	}
}

void Stage::Shutdown()
{
}

void Stage::OnCollsion(Actor* other)
{
}