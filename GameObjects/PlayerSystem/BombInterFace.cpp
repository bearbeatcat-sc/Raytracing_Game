#include "BombInterFace.h"

#include <Device/Raytracing/DXRPipeLine.h>

#include "BombUIObject.h"


void BombInterFace::Generate(int index)
{
	_bombUIObjects[index]->Generate();
}

void BombInterFace::Throw(int index)
{
	_bombUIObjects[index]->Throw();
}

void BombInterFace::UpdateActor()
{

}

void BombInterFace::Init()
{
	auto basePos = SimpleMath::Vector3(0.8f, -38, 2.0f);

	for (int i = 0; i < 3; ++i)
	{
		auto bombUI = new BombUIObject();
		_bombUIObjects.push_back(bombUI);
		SetChild(bombUI);

		bombUI->SetPosition(basePos + SimpleMath::Vector3(1.6f * i, 0, 1));
		bombUI->SetScale(SimpleMath::Vector3::Zero);
		//bombUI->SetScale(SimpleMath::Vector3(0.320f, 6, 0.1f));
	}

}

void BombInterFace::Shutdown()
{
}

void BombInterFace::OnCollsion(Actor* other)
{
}
