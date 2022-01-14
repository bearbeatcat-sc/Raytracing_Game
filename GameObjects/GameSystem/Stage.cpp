#include "Stage.h"

#include <Game_Object/ActorManager.h>
#include <Utility/LogSystem.h>
#include <Utility/Random.h>
#include <Utility/Timer.h>
#include <Utility/Math/MathUtility.h>

#include "../TargetObjects/TargetGroup.h"
#include "../../GameObjects/VerticalMoveBlock.h"
#include "../Cube.h"
#include "../PointLightObject.h"

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

	auto leftWall = new Cube(SimpleMath::Vector3(-100, 0, 0), SimpleMath::Vector3(1, 100, 200), "RoughCube");
	leftWall->SetActorName("leftWall");
	SetChild(leftWall);

	auto rightWall = new Cube(SimpleMath::Vector3(100, 0, 0), SimpleMath::Vector3(1, 100, 200), "RoughCube");
	rightWall->SetActorName("rightWall");
	SetChild(rightWall);

	auto topWall = new Cube(SimpleMath::Vector3(0, 100, 0), SimpleMath::Vector3(100, 1, 200), "RoughCube");
	topWall->SetActorName("topWall");
	SetChild(topWall);

	auto downWall = new Cube(SimpleMath::Vector3(0, -100, 0), SimpleMath::Vector3(100, 1, 200), "RoughCube");
	downWall->SetActorName("downWall");
	SetChild(downWall);

	auto frontWall = new Cube(SimpleMath::Vector3(0, 0, 150), SimpleMath::Vector3(110, 200, 1), "RoughCube");
	frontWall->SetActorName("frontWall");
	SetChild(frontWall);

	auto backWall = new Cube(SimpleMath::Vector3(0, 0, -150), SimpleMath::Vector3(110, 200, 1), "RoughCube");
	backWall->SetActorName("backWall");
	SetChild(backWall);

	//auto greenLight = new PointLightObject(SimpleMath::Vector3(40, 8, 40), SimpleMath::Color(103.0f / 255, 254.0f / 255, 31.0f / 255)  ,100.0f);
	//ActorManager::GetInstance().AddActor(greenLight);

	//auto blueLight = new PointLightObject(SimpleMath::Vector3(-48, 8, 40), SimpleMath::Color(90.0f / 255, 231.0f / 255, 255.0f / 255), 100.0f);
	//ActorManager::GetInstance().AddActor(blueLight);

	//auto pinkLight = new PointLightObject(SimpleMath::Vector3(-48, 8, -50), SimpleMath::Color(248.0f / 255, 104.0f / 255, 172.0f / 255), 100.0f);
	//ActorManager::GetInstance().AddActor(pinkLight);

	//auto yellowLight = new PointLightObject(SimpleMath::Vector3(40, 8, -50), SimpleMath::Color(243.0f / 255, 255.0f / 255, 59.0f / 255), 100.0f);
	//ActorManager::GetInstance().AddActor(yellowLight);

	auto greenLight = new PointLightObject(SimpleMath::Vector3(40, 8, 40), SimpleMath::Color(1.0f,1.0f,1.0f), 100.0f);
	ActorManager::GetInstance().AddActor(greenLight);

	auto blueLight = new PointLightObject(SimpleMath::Vector3(-48, 8, 40), SimpleMath::Color(1.0f, 1.0f, 1.0f), 100.0f);
	ActorManager::GetInstance().AddActor(blueLight);

	auto pinkLight = new PointLightObject(SimpleMath::Vector3(-48, 8, -50), SimpleMath::Color(1.0f, 1.0f, 1.0f), 100.0f);
	ActorManager::GetInstance().AddActor(pinkLight);

	auto yellowLight = new PointLightObject(SimpleMath::Vector3(40, 8, -50), SimpleMath::Color(1.0f, 1.0f, 1.0f), 100.0f);
	ActorManager::GetInstance().AddActor(yellowLight);
}

void Stage::Shutdown()
{
}

void Stage::OnCollsion(Actor* other)
{
}