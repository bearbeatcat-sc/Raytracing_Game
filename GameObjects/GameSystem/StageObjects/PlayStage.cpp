#include "PlayStage.h"

#include <Game_Object/ActorManager.h>
#include <Utility/Random.h>
#include <Utility/Math/MathUtility.h>

#include "../../../GameObjects/VerticalMoveBlock.h"
#include "../../Cube.h"
#include "../../PointLightObject.h"

PlayStage::PlayStage()
	: _floorCreateTime(0.1f)
{
}

void PlayStage::Delete()
{
}

void PlayStage::UpdateActor()
{
}

void PlayStage::Init()
{
	_floors.clear();

	auto besePos = GetPosition();

	//for (int z = 0; z < 60; ++z)
	//{
	//	for (int x = 0; x < 13; x++)
	//	{
	//		auto pos = SimpleMath::Vector3(x * 3.9f, 0.0f, z * 4.0f) - SimpleMath::Vector3(7.0f * 3.9f, 0.0f, 0.0f);

	//		if ((x >= 6 && x <= 8) /*|| (z >= 13 && z < 16)*/)
	//		{
	//			auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "RedCube");
	//			SetChild(cube);
	//			_floors.push_back(cube);
	//			continue;
	//		}

	//		auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "BlackCube");
	//		SetChild(cube);
	//		_floors.push_back(cube);

	//		//if (z <= 12)
	//		//{



	//		//}

	//		//auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "BlueCube");
	//		//SetChild(cube);
	//		//_floors.push_back(cube);

	//	}
	//}

	auto leftWall = new Cube(SimpleMath::Vector3(-100, 0, 0), SimpleMath::Vector3(1, 100, 1000), "WhiteCube");
	leftWall->SetActorName("leftWall");
	SetChild(leftWall);

	auto rightWall = new Cube(SimpleMath::Vector3(100, 0, 0), SimpleMath::Vector3(1, 100, 1000), "WhiteCube");
	rightWall->SetActorName("rightWall");
	SetChild(rightWall);

	auto topWall = new Cube(SimpleMath::Vector3(0, 100, 0), SimpleMath::Vector3(100, 1, 1000), "WhiteCube");
	topWall->SetActorName("topWall");
	SetChild(topWall);

	auto downWall = new Cube(SimpleMath::Vector3(0, -100, 0), SimpleMath::Vector3(100, 1, 1000), "WhiteCube");
	downWall->SetActorName("downWall");
	SetChild(downWall);



	auto rightUpPillar = new Cube(SimpleMath::Vector3(90, 90, 0), SimpleMath::Vector3(10, 50, 1000), "BlackCube");
	rightUpPillar->SetActorName("rightUpPillar");
	SetChild(rightUpPillar);

	auto leftUpPillar = new Cube(SimpleMath::Vector3(-90, 90, 0), SimpleMath::Vector3(10, 50, 1000), "BlackCube");
	leftUpPillar->SetActorName("leftUpPillar");
	SetChild(leftUpPillar);

	auto leftDownPillar = new Cube(SimpleMath::Vector3(-90, -90, 0), SimpleMath::Vector3(10, 50, 1000), "BlackCube");
	leftDownPillar->SetActorName("leftDownPillar");
	SetChild(leftDownPillar);

	auto rightDownPillar = new Cube(SimpleMath::Vector3(90, -90, 0), SimpleMath::Vector3(10, 50, 1000), "BlackCube");
	rightDownPillar->SetActorName("rightDownPillar");
	SetChild(rightDownPillar);

	//auto frontWall = new Cube(SimpleMath::Vector3(0, 0, 1000), SimpleMath::Vector3(110, 200, 1), "WhiteCube");
	//frontWall->SetActorName("frontWall");
	//SetChild(frontWall);

	for(int i = 0; i < 30; ++i)
	{
		int flag = Random::GetRandom(0, 1);

		if(flag == 0)
		{
			auto pillar = new Cube(SimpleMath::Vector3(-90, 0, i * 30), SimpleMath::Vector3(5, 50, 5), "BlackCube");
			pillar->SetActorName("Pillar");
			SetChild(pillar);
			continue;
		}

		auto pillar = new Cube(SimpleMath::Vector3(100, 0, i * 30), SimpleMath::Vector3(5, 50, 5), "BlackCube");
		pillar->SetActorName("Pillar");
		SetChild(pillar);
	}

	auto backWall = new Cube(SimpleMath::Vector3(0, 0, -1000), SimpleMath::Vector3(110, 200, 1), "WhiteCube");
	backWall->SetActorName("backWall");
	SetChild(backWall);

}

void PlayStage::Shutdown()
{
}

void PlayStage::OnCollsion(Actor* other)
{
}
