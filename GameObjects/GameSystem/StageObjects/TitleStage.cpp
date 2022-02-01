#include "TitleStage.h"

#include <Device/DirectX/Core/Sounds/SoundManager.h>
#include <Game_Object/ActorManager.h>
#include <Utility/Random.h>
#include <Utility/Math/MathUtility.h>

#include "../../../GameObjects/VerticalMoveBlock.h"
#include "../../Cube.h"
#include "../../PointLightObject.h"

TitleStage::TitleStage()
	:_floorCreateTime(0.1f)
{

}

void TitleStage::Delete()
{
	Destroy(10.0f);


	for(auto cube : _floors)
	{
		cube->Delete();
	}
}

void TitleStage::UpdateActor()
{
}

void TitleStage::Init()
{
	_floors.clear();

	auto besePos = GetPosition();

	//for (int z = 0; z < 27; ++z)
	//{
	//	for (int x = 0; x < 27; x++)
	//	{
	//		auto pos = SimpleMath::Vector3(x * 3.9f, 0.0f, z * 4.0f) - SimpleMath::Vector3(12.0f * 3.9f, 0.0f, 0.0f);

	//		if ((x >= 13 && x < 16) /*|| (z >= 13 && z < 16)*/)
	//		{
	//			auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "BlackCube");
	//			SetChild(cube);
	//			_floors.push_back(cube);
	//			continue;
	//		}

	//		if (z <= 12)
	//		{
	//			auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "RedCube");
	//			SetChild(cube);
	//			_floors.push_back(cube);
	//			continue;
	//		}

	//		auto cube = new VerticalMoveBlock(pos, SimpleMath::Vector3(1.8f), "BlueCube");
	//		SetChild(cube);
	//		_floors.push_back(cube);

	//	}
	//}


}

void TitleStage::Shutdown()
{
}

void TitleStage::OnCollsion(Actor* other)
{
}
