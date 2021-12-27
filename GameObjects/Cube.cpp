#include "Cube.h"

#include "Utility/Random.h"
#include "Utility/Timer.h"

#include "Components/Collsions/CollisionManager.h"
#include "Components/Collsions/OBBCollisionComponent.h"
#include "Components/Physics/RigidBodyComponent.h"
#include "Device/DirectX/DirectXInput.h"
#include "Device/Raytracing/DXRPipeLine.h"
#include "Utility/LogSystem.h"


Cube::Cube(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale, float destroyTime, const std::string& meshName, bool moveFlag)
	:_initScale(scale), _IsGenerate(false), _DXRMeshName(meshName),Actor()
{
	SetPosition(pos);
	SetScale(scale);

	_GenerateTimer = std::make_shared<Timer>(1.0f);

}

void Cube::UpdateActor()
{


	if (!_IsGenerate)
	{
		Generate();
		return;
	}


	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);


}

void Cube::Generate()
{
	_GenerateTimer->Update();

	if (_GenerateTimer->IsTime())
	{
		_GenerateTimer->Reset();
		_IsGenerate = true;
		return;
	}

	m_Scale = SimpleMath::Vector3::Lerp(SimpleMath::Vector3(0.0f), _initScale, _GenerateTimer->GetRatio() * 1.0f);
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
}

void Cube::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance(_DXRMeshName, 0);


	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();


}

void Cube::Shutdown()
{
	_instance->Destroy();
}

void Cube::OnCollsion(Actor* other)
{

}
