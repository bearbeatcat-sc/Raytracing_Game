#include "Stage.h"

#include <Game_Object/ActorManager.h>
#include <Utility/LogSystem.h>
#include <Utility/Random.h>
#include <Utility/Timer.h>

#include "../TargetObjects/TargetGroup.h"
#include "../../GameObjects/VerticalMoveBlock.h"
#include "../Cube.h"

Stage::Stage()
	:_floorCreateTime(0.1f)
{
}

void Stage::UpdateActor()
{
	CreateFloor();
}

void Stage::Init()
{
	_floorCreateTimer = std::make_shared<Timer>(_floorCreateTime);

	_createFloorCount = 0;
	_floors.clear();
}

void Stage::Shutdown()
{
}

void Stage::OnCollsion(Actor* other)
{
}

void Stage::DeleteFloor()
{
	if(_floors.size() >= _maxCreateFloorCount)
	{
		LogSystem::AddLog("Reaching the maximum number of floors");
		const int deleteCount = 10;
		int count = 0;

		for(auto itr = _floors.begin(); itr != _floors.end();)
		{
			(*itr)->Destroy();
			itr = _floors.erase(itr);
			count++;

			if(count >= deleteCount)
			{
				return;
			}
		}
	}
}

void Stage::CreateFloor()
{
	_floorCreateTimer->Update();

	if(_floorCreateTimer->IsTime() && _maxCreateFloorCount >= _floors.size())
	{
		_floorCreateTimer->Reset();

		int z = _createFloorCount;

		for (int x = 0; x < 24; x++)
		{
			auto pos = SimpleMath::Vector3(x * 3.9f, 0.0f, z * 4.0f) - SimpleMath::Vector3(12.0f * 3.9f, 0.0f, 0.0f);

			if((x >= 0 && x < 3) || (x >= 21)
				|| (x >= 11 && x < 14))
			{
				auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "BlackCube");
				SetChild(cube);
				_floors.push_back(cube);
				continue;
			}

			auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "PurpleCube");
			SetChild(cube);
			_floors.push_back(cube);

		}

		_createFloorCount++;
	}

	DeleteFloor();
}
