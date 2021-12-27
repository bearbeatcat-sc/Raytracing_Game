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

	auto leftWall = new Cube(SimpleMath::Vector3(-50.0f, 0.0f, 1900.0f), SimpleMath::Vector3(1, 12, 2000), 0.0f, "WhiteCube");
	SetChild(leftWall);

	auto rightWall = new Cube(SimpleMath::Vector3(50.0f, 0.0f, 1900.0f), SimpleMath::Vector3(1, 12, 2000), 0.0f, "WhiteCube");
	SetChild(rightWall);

	for (int i = 0; i < 30; ++i)
	{
		auto x = Random::GetRandom(-300.0f, 300.0f);
		auto y = Random::GetRandom(-300.0f, 300.0f);
		auto z = Random::GetRandom(100.0f, 1000.0f);

		auto pos = SimpleMath::Vector3(x, y, z);
		auto scale = SimpleMath::Vector3(y * 0.1f);
		auto rotate = SimpleMath::Vector3(x, y, z) * 0.01f;

		auto cube = new VerticalMoveBlock(pos, scale, "WhiteCube");
		SetChild(cube);
		cube->SetRotation(rotate);
	}

	_createFloorCount = 0;
	_floors.clear();
}

void Stage::Shutdown()
{
}

void Stage::OnCollsion(Actor* other)
{
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

			float test = 60.0f / z;

			int flag = (x + z) % 2;

			if (flag == 0)
			{
				continue;
			}

			auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "RedCube", test);
			SetChild(cube);

			_floors.push_back(cube);
		}

		_createFloorCount++;
	}

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
